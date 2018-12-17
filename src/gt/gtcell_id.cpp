//
// Created by 濮国梁 on 2018/12/13.
//
#include "gt/gtcell_id.h"
#include "gt/gtcoords.h"

GTCellId::GTCellId(const S2Point p) {
    //默认为叶子网格
    uint32 I,J;
    GT::XYZtoIJ(p,&I,&J);
    GT::IJtoCellID(I,J,&id_);
}
GTCellId::GTCellId(const S2LatLng ll) {
    //默认为叶子网格
    uint32 I,J;
    GT::LLtoIJ(ll,&I,&J);
    GT::IJtoCellID(I,J,&id_);
}

S2Point GTCellId::ToPoint() const {
    S2Point pnt;

    GT::CellIDtoXYZ(id_,&pnt);

    return pnt;
}

bool GTCellId::FromPoint(S2Point point) const {

    GT::XYZtoCellID(point,&id_);

    return true;
}

bool GTCellId::FromPoint(S2Point point, unsigned int level) const {
    uint32 I,J,mask;

    S2_DCHECK_LE(level,31);

    GT::XYZtoIJ(point,&I,&J);

    mask = 0XFFFFFFFF << (kMaxLevel - level + 1);

    I = I & mask;
    J = J & mask;

    GT::IJtoCellID(I,J,&id_);

    return true;
}


S2LatLng GTCellId::ToLatLng() const {
    double lng,lat;

    GT::CellIDtoLL(id_,&lng,&lat);

    return S2LatLng().FromDegrees(lat,lng);
}

bool GTCellId::FromLatLng(S2LatLng latLng) const {
    GT::LLtoCellID(latLng.lng().degrees(),latLng.lat().degrees(),&id_);
    return true;
}

bool GTCellId::FromLatLng(S2LatLng latLng, unsigned int level) const {
    uint32 I,J,mask;

    S2_DCHECK_LE(level,31);

    GT::LLtoIJ(latLng.lng().degrees(),latLng.lat().degrees(),&I,&J);

    mask = 0XFFFFFFFF << (kMaxLevel - level + 1);

    I = I & mask;
    J = J & mask;

    GT::IJtoCellID(I,J,&id_);

    return true;
}

inline uint64 GTCellId::lsb() const {
    return id_ & (~id_ + 2);  //如果是非法编码的话，标识位在奇数位上
}

inline bool GTCellId::is_valid() const {
    // 采用末尾补100..0方式表示层级时，至少有一个偶数位bit必须为1
    return (lsb() & 0x2AAAAAAAAAAAAAAAULL);
}

inline GTCellId GTCellId::range_min_cell() const {
    return GTCellId(id_ - (lsb() - 1));
}

inline GTCellId GTCellId::range_max_cell() const {
    return GTCellId(id_ + (lsb() - 1));
}

uint64 GTCellId::range_min() const {
    return range_min_cell().id();
}

uint64 GTCellId::range_max() const {
    return range_max_cell().id();
}

inline int GTCellId::level() const {
    // We can't just S2_DCHECK(is_valid()) because we want level() to be
    // defined for end-iterators, i.e. S2CellId::End(kLevel).  However there is
    // no good way to define S2CellId::None().level(), so we do prohibit that.
    S2_DCHECK(id_ != 0);

    // A special case for leaf cells is not worthwhile.
    return kMaxLevel - (Bits::FindLSBSetNonZero64(id_) >> 1);
}

int GTCellId::child_position() const {
    return 0;
}

int GTCellId::child_position(int level) const {
    return 0;
}

bool GTCellId::contains(const uint64 other) const {
    return false;
}

bool GTCellId::contains(const GTCellId other) const {
    S2_DCHECK(is_valid());
    S2_DCHECK(other.is_valid());
    return other >= range_min_cell() && other <= range_max_cell();
}

bool GTCellId::isChildOf(const uint64 other) const {
    return false;
}

bool GTCellId::isChildOf(const GTCellId other) const {
    return false;
}

bool GTCellId::intersects(const uint64 other) const {
    return false;
}

bool GTCellId::intersects(const GTCellId other) const {
    return false;
}

uint64 GTCellId::parentID() const {
    return 0;
}

uint64 GTCellId::parentID(int level) const {
    return 0;
}

uint64 GTCellId::child(int position) const {
    return 0;
}

uint64 GTCellId::child_begin() const {
    return 0;
}

uint64 GTCellId::child_end() const {
    return 0;
}

uint64 GTCellId::child_begin(int level) const {
    return 0;
}

uint64 GTCellId::child_end(int level) const {
    return 0;
}

uint64 GTCellId::next() const {
    return 0;
}

uint64 GTCellId::prev() const {
    return 0;
}

uint64 GTCellId::advance(int64 steps) const {
    return 0;
}

int64 GTCellId::distance_from_begin() const {
    return 0;
}

uint64 GTCellId::maximum_tile(uint64 limit) const {
    return 0;
}

int GTCellId::GetCommonAncestorLevel(uint64 other) const {
    return 0;
}

uint64 GTCellId::Begin(int level) {
    return 0;
}

uint64 GTCellId::End(int level) {
    return 0;
}

void GTCellId::GetFourNeighbors(uint64 *neighbors) const {

}

void GTCellId::GetEightNeighbors(uint64 *neighbors) const {

}

void GTCellId::AppendVertexNeighbors(int level, std::vector<uint64> *output) const {

}

void GTCellId::AppendAllNeighbors(int nbr_level, std::vector<CellId> *output) const {

}

string GTCellId::ToToken() const {
    return std::string();
}

uint64 GTCellId::FromToken(const char *token, size_t length) {
    return 0;
}

uint64 GTCellId::FromToken(const string &token) {
    return 0;
}

void GTCellId::Encode(Encoder *const encoder) const {

}

bool GTCellId::Decode(Decoder *const decoder) {
    return false;
}

string GTCellId::ToString() const {
    return std::string();
}

int GTCellId::GetCommonAncestorLevel(GTCellId other) const {
    return 0;
}





