//
// Created by 濮国梁 on 2018/12/13.
//
#include "core/gt_cell_id.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstring>
#include <iosfwd>
#include <mutex>
#include <vector>

using std::min;
using std::max;

#include "base/integral_types.h"
#include "util/bits/bits.h"
#include "third_party/absl/base/casts.h"
#include "third_party/absl/strings/str_cat.h"

#include "core/gt_cell_id.h"
#include "core/gt_coords.h"
#include "s2/r2rect.h"
#include "s2/s2latlng.h"

GTCellId::GTCellId (const S2Point p) {
    //默认为叶子网格
    GT::XYZtoCellID(p, &id_);
}

GTCellId::GTCellId (const S2LatLng ll) {
    //默认为叶子网格
    GT::LLtoCellID(ll.lng().degrees(), ll.lat().degrees(), &id_);
}

GTCellId::GTCellId (const S2Point p, int level) {
    S2_DCHECK_LE(level, kMaxLevel);
    GT::XYZtoCellID(p, &id_, level);
}

GTCellId::GTCellId (const S2LatLng ll, int level) {
    S2_DCHECK_LE(level, kMaxLevel);
    GT::LLtoCellID(ll.lng().degrees(), ll.lat().degrees(), &id_, level);
}

S2Point GTCellId::ToPoint () const {
    S2Point pnt;
    S2LatLng ll;

    //获取中心经纬度
    ll = ToLatLng();

    //将中心经纬度转换为球面坐标
    GT::LLtoXYZ(ll.lng().degrees(),ll.lat().degrees(), &pnt);

    return pnt;
}

GTCellId GTCellId::FromPoint (S2Point point) {
    uint64 id;

    GT::XYZtoCellID(point, &id);
    return GTCellId(id);
}

GTCellId GTCellId::FromPoint (S2Point point, unsigned int level) {
    S2_DCHECK_LE(level, kMaxLevel);
    uint64 id;
    GT::XYZtoCellID(point, &id, level);
    return GTCellId(id);
}

//获得网格中心点的经纬度
S2LatLng GTCellId::ToLatLng () const {
    double lng, lat;
    uint32 I, J,mask;
    int level;

    GT::CellIDtoIJ(id(), &I, &J, &level);

    //获得该网格的第4个子网格的<I,J>坐标（Z序）
    mask = 1 << kMaxLevel - level;
    I = I | mask;
    J = J | mask;

    //求该网格的第4个子网格的定位点经纬度（Z序）
    GT::IJtoLL(I, J, &lng, &lat, level + 1);

    return S2LatLng().FromDegrees(lat, lng);
}

GTCellId GTCellId::FromLatLng (S2LatLng latLng) {
    uint64 id;

    GT::LLtoCellID(latLng.lng().degrees(), latLng.lat().degrees(), &id);

    return GTCellId(id);
}

GTCellId GTCellId::FromLatLng (S2LatLng latLng, unsigned int level) {
    uint64 id;
    S2_DCHECK_LE(level, kMaxLevel);
    GT::LLtoCellID(latLng.lng().degrees(), latLng.lat().degrees(), &id, level);
    return GTCellId(id);
}


//判断是否包含网格other，即判断other是否为其子网格
bool GTCellId::contains (const GTCellId other) const {
    S2_DCHECK(is_valid());
    S2_DCHECK(other.is_valid());
    return other >= range_min() && other <= range_max();
}

//判断是否被网格other包含，即判断other是否为其父网格
bool GTCellId::isChildOf (const GTCellId other) const {
    S2_DCHECK(is_valid());
    S2_DCHECK(other.is_valid());

    return (*this) <= other.range_max() && (*this) >= other.range_min();
}

//判断两个网格是否相交，即判断本网格与other之间是否有相互包含关系
bool GTCellId::intersects (const GTCellId other) const {
    S2_DCHECK(is_valid());
    S2_DCHECK(other.is_valid());
    return other.range_min() <= range_max() && other.range_max() >= range_min();
}


//子网格的第一个ID
GTCellId GTCellId::child_begin () const {
    S2_DCHECK(is_valid());
    S2_DCHECK(!is_leaf());
    uint64 old_lsb = lsb();
    return GTCellId(id_ - old_lsb + (old_lsb >> 2));
}

//子网格的最后一个ID
GTCellId GTCellId::child_end () const {
    S2_DCHECK(is_valid());
    S2_DCHECK(!is_leaf());
    uint64 old_lsb = lsb();
    return GTCellId(id_ + old_lsb + (old_lsb >> 2));
}

//第level级子网格的起始ID
GTCellId GTCellId::child_begin (int level) const {
    S2_DCHECK(is_valid());
    S2_DCHECK_GE(level, this->level());
    S2_DCHECK_LE(level, kMaxLevel);
    return GTCellId(id_ - lsb() + lsb_for_level(level));
}

//第level级子网格的结束ID
GTCellId GTCellId::child_end (int level) const {
            S2_DCHECK(is_valid());
    S2_DCHECK_GE(level, this->level());
    S2_DCHECK_LE(level, kMaxLevel);
    return GTCellId(id_ + lsb() + lsb_for_level(level));
}

//本级曲线上的下一个ID
GTCellId GTCellId::next () const {
    //+1
    return GTCellId(id_ + (lsb() << 1));
}

//本级曲线上的上一个ID
GTCellId GTCellId::prev () const {
    //-1
    return GTCellId(id_ - (lsb() << 1));
}

//待测试
GTCellId GTCellId::advance (int64 steps) const {
    if (steps == 0) return *this;

    // We clamp the number of steps if necessary to ensure that we do not
    // advance past the End() or before the Begin() of this level.  Note that
    // min_steps and max_steps always fit in a signed 64-bit integer.

    int step_shift = 2 * (kMaxLevel - level()) + 1;
    if (steps < 0) {
        int64 min_steps = -static_cast<int64>(id_ >> step_shift);
        if (steps < min_steps) steps = min_steps;
    } else {
        int64 max_steps = (kWrapOffset + lsb() - id_) >> step_shift;
        if (steps > max_steps) steps = max_steps;
    }
    // If steps is negative, then shifting it left has undefined behavior.
    // Cast to uint64 for a 2's complement answer.
    return GTCellId(id_ + (static_cast<uint64>(steps) << step_shift));
}

int64 GTCellId::distance_from_begin () const {
    const int step_shift = 2 * (kMaxLevel - level()) + 1;
    return id_ >> step_shift;
}

GTCellId GTCellId::maximum_tile (GTCellId limit) const {
    GTCellId id = *this;
    GTCellId start = id.range_min();
    if (start >= limit.range_min()) return limit;

    if (id.range_max() >= limit) {
        // The cell is too large.  Shrink it.  Note that when generating coverings
        // of GTCellId ranges, this loop usually executes only once.  Also because
        // id.range_min() < limit.range_min(), we will always exit the loop by the
        // time we reach a leaf cell.
        do { id = id.child(0); } while (id.range_max() >= limit);
        return id;
    }
    // The cell may be too small.  Grow it if necessary.  Note that generally
    // this loop only iterates once.
    while (!id.is_face()) {
        GTCellId parent = id.parent();
        if (parent.range_min() != start || parent.range_max() >= limit) break;
        id = parent;
    }
    return id;
}

int GTCellId::GetCommonAncestorLevel (GTCellId other) const {
    // Basically we find the first bit position at which the two GTCellIds
    // differ and convert that to a level.  The max() below is necessary for the
    // case where one GTCellId is a descendant of the other.
    uint64 bits = max(id() ^ other.id(), max(lsb(), other.lsb()));
    S2_DCHECK_NE(bits, 0);  // Because lsb() is non-zero.

    // Compute the position of the most significant bit, and then map the bit
    // position as follows:
    // {0} -> 30, {1,2} -> 29, {3,4} -> 28, ... , {59,60} -> 0, {61,62,63} -> -1.
    return max(60 - Bits::FindMSBSetNonZero64(bits), -1) >> 1;
}

//level级曲线的起始ID
GTCellId GTCellId::Begin (int level) {
    return FromFace(0).child_begin(level);
}

//level级曲线的结束ID
GTCellId GTCellId::End (int level) {
    return FromFace(3).child_end(level);
}

//获得四个同级相邻网格的ID，四领域
void GTCellId::GetFourNeighbors (GTCellId *neighbors) const {

}

//获得八个同级相邻网格的ID，八领域
void GTCellId::GetEightNeighbors (GTCellId *neighbors) const {

}

//TODO
//获得第level层级上最邻近格点的邻居
void GTCellId::AppendVertexNeighbors (int level, std::vector<GTCellId> *output) const {

}

//TODO
//获得第level层级上的所有邻居
void GTCellId::AppendAllNeighbors (int nbr_level, std::vector<GTCellId> *output) const {

}

// Print the num_digits low order hex digits.
static string HexFormatString(uint64 val, size_t num_digits) {
    string result(num_digits, ' ');
    for (; num_digits--; val >>= 4)
        result[num_digits] = "0123456789abcdef"[val & 0xF];
    return result;
}


//TODO
// 转换为变长字符串
string GTCellId::ToToken () const {
    // Simple implementation: print the id in hex without trailing zeros.
    // Using hex has the advantage that the tokens are case-insensitive, all
    // characters are alphanumeric, no characters require any special escaping
    // in queries for most indexing systems, and it's easy to compare cell
    // tokens against the feature ids of the corresponding features.
    //
    // Using base 64 would produce slightly shorter tokens, but for typical cell
    // sizes used during indexing (up to level 15 or so) the average savings
    // would be less than 2 bytes per cell which doesn't seem worth it.

    // "0" with trailing 0s stripped is the empty string, which is not a
    // reasonable token.  Encode as "X".
    if (id_ == 0) return "X";
    const size_t num_zero_digits = Bits::FindLSBSetNonZero64(id_) / 4;
    return HexFormatString(id_ >> (4 * num_zero_digits), 16 - num_zero_digits);
}

//TODO
GTCellId GTCellId::FromToken (const char *token, size_t length) {
    return GTCellId();
}
//TODO
GTCellId GTCellId::FromToken (const string &token) {
    return GTCellId();
}
//TODO
void GTCellId::Encode (Encoder *const encoder) const {

}
//TODO
bool GTCellId::Decode (Decoder *const decoder) {
    return false;
}

//以四进制字符串形式输出编码，编码形式: "/AAAAAAAAA....",A的值域为['0'..'3']，表示在某个层级中的子网格位置
string GTCellId::ToString4 () const {
    if (!is_valid()) {
        return StrCat("Invalid: ", absl::Hex(id(), absl::kZeroPad16));
    }
    string out = "/";
    for (int current_level = 1; current_level <= level(); ++current_level) {
        // Avoid dependencies of SimpleItoA, and slowness of StrAppend &
        // std::to_string.
        out += "0123"[child_position(current_level)];
    }
    return out;
}

//以16进制字符串形式输出编码，编码形式: "/AAAAAAAAA....",A的值域为['0'..'3']，表示在某个层级中的子网格位置
// Print the num_digits low order hex digits.
static string ToString16 () {
    uint64 val;
    size_t num_digits;

    string result(num_digits, ' ');
    for (; num_digits--; val >>= 4)
        result[num_digits] = "0123456789abcdef"[val & 0xF];
    return result;
}
// Print the num_digits low order 32 digits.
//以32进制字符串形式输出编码，编码形式: "/AAAAAAAAA...."，A的值域如下：
// 0-9，
// A-10/B-11/C-12/D-13/E-14/F-15
// G-16/H-17/J-18/K-19/L-20/M-21
// N-22/P-23/Q-24/R-25/T-26/U-27
// V-28/W-29/X-30/Y-31

static string ToString32 () {
    uint64 val;
    size_t num_digits;

    string result(num_digits, ' ');
    for (; num_digits--; val >>= 5)
        result[num_digits] = "0123456789abcdefghjklmnpqrtuvwxy"[val & 0x1F];
    return result;
}







