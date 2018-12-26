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

GTCellId::GTCellId(const S2Point p) {
    //默认为叶子网格
    GT::XYZtoCellID(p,&id_);
}
GTCellId::GTCellId(const S2LatLng ll) {
    //默认为叶子网格
    GT::LLtoCellID(ll.lng().degrees(),ll.lat().degrees(),&id_);
}
GTCellId::GTCellId(const S2Point p,int level) {
    S2_DCHECK_LE(level,kMaxLevel);
    GT::XYZtoCellID(p,&id_,level);
}
GTCellId::GTCellId(const S2LatLng ll,int level) {
    S2_DCHECK_LE(level,kMaxLevel);
    GT::LLtoCellID(ll.lng().degrees(),ll.lat().degrees(),&id_,level);
}
S2Point GTCellId::ToPoint() const {
    S2Point pnt;

    GT::CellIDtoXYZ(id_,&pnt);

    return pnt;
}

GTCellId GTCellId::FromPoint(S2Point point) {
    uint64 id;

    GT::XYZtoCellID(point,&id);
    return GTCellId(id);
}

GTCellId GTCellId::FromPoint(S2Point point, unsigned int level)  {
    S2_DCHECK_LE(level,kMaxLevel);
    uint64 id;
    GT::XYZtoCellID(point,&id,level);
    return GTCellId(id);
}


S2LatLng GTCellId::ToLatLng() const {
    double lng,lat;

    GT::CellIDtoLL(id_,&lng,&lat);

    return S2LatLng().FromDegrees(lat,lng);
}

GTCellId GTCellId::FromLatLng(S2LatLng latLng)  {
    uint64 id;

    GT::LLtoCellID(latLng.lng().degrees(),latLng.lat().degrees(),&id);
    return GTCellId(id);
}

GTCellId  GTCellId::FromLatLng(S2LatLng latLng, unsigned int level)  {
    uint64 id;
    S2_DCHECK_LE(level,kMaxLevel);
    GT::LLtoCellID(latLng.lng().degrees(),latLng.lat().degrees(),&id,level);
    return GTCellId(id);
}


//判断是否包含网格other，即判断other是否为其子网格
bool GTCellId::contains(const GTCellId other) const {
    S2_DCHECK(is_valid());
    S2_DCHECK(other.is_valid());
    return other >= range_min() && other <= range_max();
}

//判断是否被网格other包含，即判断other是否为其父网格
bool GTCellId::isChildOf(const GTCellId other) const {
    S2_DCHECK(is_valid());
    S2_DCHECK(other.is_valid());

    return (*this)<=other.range_max() && (*this)>=other.range_min();
}

//判断两个网格是否相交，即判断本网格与other之间是否有相互包含关系
bool GTCellId::intersects(const GTCellId other) const {
    S2_DCHECK(is_valid());
    S2_DCHECK(other.is_valid());
    return other.range_min() <= range_max() && other.range_max() >= range_min();
}





//子网格的第一个ID
GTCellId GTCellId::child_begin() const {
    S2_DCHECK(is_valid());
    S2_DCHECK(!is_leaf());
    uint64 old_lsb = lsb();
    return GTCellId(id_ - old_lsb + (old_lsb >> 2));
}

//子网格的最后一个ID
GTCellId GTCellId::child_end() const {
    S2_DCHECK(is_valid());
    S2_DCHECK(!is_leaf());
    uint64 old_lsb = lsb();
    return GTCellId(id_ + old_lsb + (old_lsb >> 2));
}

//第level级子网格的起始ID
GTCellId GTCellId::child_begin(int level) const {
    S2_DCHECK(is_valid());
    S2_DCHECK_GE(level, this->level());
    S2_DCHECK_LE(level, kMaxLevel);
    return GTCellId(id_ - lsb() + lsb_for_level(level));
}

//第level级子网格的结束ID
GTCellId GTCellId::child_end(int level) const {
    S2_DCHECK(is_valid());
    S2_DCHECK_GE(level, this->level());
    S2_DCHECK_LE(level, kMaxLevel);
    return GTCellId(id_ + lsb() + lsb_for_level(level));    return GTCellId();
}

//本级曲线上的下一个ID
GTCellId GTCellId::next() const {
    return GTCellId(id_ + (lsb() << 1));
}

//本级曲线上的上一个ID
GTCellId GTCellId::prev() const {
    return GTCellId(id_ - (lsb() << 1));
}

GTCellId GTCellId::advance(int64 steps) const {
    return GTCellId();
}

int64 GTCellId::distance_from_begin() const {
    return 0;
}

GTCellId GTCellId::maximum_tile(GTCellId limit) const {
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

int GTCellId::GetCommonAncestorLevel(GTCellId other) const {
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
GTCellId GTCellId::Begin(int level) {
    return GTCellId();
}
//level级曲线的结束ID
GTCellId GTCellId::End(int level) {
    return GTCellId();
}

//获得四个同级相邻网格的ID，四领域
void GTCellId::GetFourNeighbors(GTCellId *neighbors) const {

}
//获得八个同级相邻网格的ID，八领域
void GTCellId::GetEightNeighbors(GTCellId *neighbors) const {

}

//TODO
//获得第level层级上最邻近格点的邻居
void GTCellId::AppendVertexNeighbors(int level, std::vector<GTCellId> *output) const {

}
//TODO
//获得第level层级上的所有邻居
void GTCellId::AppendAllNeighbors(int nbr_level, std::vector<GTCellId> *output) const {

}
//TODO
string GTCellId::ToToken() const {
    return std::string();
}
//TODO
GTCellId GTCellId::FromToken(const char *token, size_t length) {
    return GTCellId();
}

GTCellId GTCellId::FromToken(const string &token) {
    return GTCellId();
}

void GTCellId::Encode(Encoder *const encoder) const {

}

bool GTCellId::Decode(Decoder *const decoder) {
    return false;
}

//以四进制字符串形式输出编码，编码形式: "/AAAAAAAAA....",A的值域为['0'..'3']，表示在某个层级中的子网格位置
string GTCellId::ToString4() const {
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

//以四进制字符串形式输出编码，编码形式: "/AAAAAAAAA....",A的值域为['0'..'3']，表示在某个层级中的子网格位置
string GTCellId::ToString16() const {
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

//以四进制字符串形式输出编码，编码形式: "/AAAAAAAAA....",A的值域为['0'..'3']，表示在某个层级中的子网格位置
string GTCellId::ToString32() const {
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

// Print the num_digits low order hex digits.
static string HexFormatString(uint64 val, size_t num_digits) {
    string result(num_digits, ' ');
    for (; num_digits--; val >>= 4)
        result[num_digits] = "0123456789abcdef"[val & 0xF];
    return result;
}







