//
// Created by 濮国梁 on 2018/12/13.
//
#include <gt/gtcell_id.h>

#include "gt/gtcell_id.h"

GTCellId::GTCellId(const S2Point p) {

}
GTCellId::GTCellId(const S2LatLng ll) {

}


S2Point GTCellId::ToPoint() const {
    return S2Point();
}

bool GTCellId::FromPoint(S2Point point) const {
    return false;
}

bool GTCellId::FromPoint(S2Point point, unsigned int level) const {
    return false;
}


S2LatLng GTCellId::ToLatLng() const {
    return S2LatLng();
}

bool GTCellId::FromLatLng(S2LatLng latLng) const {
    return false;
}

bool GTCellId::FromLatLng(S2LatLng latLng, unsigned int level) const {
    return false;
}


////////////////////////////////////////////////////////
//  网格ID编码的属性获取函数
/////////////////////////////////////////////////////////
inline uint64 GTCellId::lsb() const {
    return id_ & (~id_ + 2);  //如果是非法编码的话
}

inline bool GTCellId::is_valid() const {
    // 采用末尾补100..0方式表示层级时，至少有一个偶数位bit必须为1
    return (lsb() & 0x2AAAAAAAAAAAAAAAULL);
}

inline int GTCellId::level() const {
    // We can't just S2_DCHECK(is_valid()) because we want level() to be
    // defined for end-iterators, i.e. S2CellId::End(kLevel).  However there is
    // no good way to define S2CellId::None().level(), so we do prohibit that.
            S2_DCHECK(id_ != 0);

    // A special case for leaf cells is not worthwhile.
    return kMaxLevel - (Bits::FindLSBSetNonZero64(id_) >> 1);
}

inline uint64 GTCellId::pos() const {
    return 0;
}

inline bool GTCellId::is_leaf() const {
    return (id_ & 0x02ULL);
}

inline GTCellId GTCellId::range_min_cell() const {
    return GTCellId(id_ - (lsb() - 0x2));
}

inline GTCellId GTCellId::range_max_cell() const {
    return GTCellId(id_ + (lsb() - 0x2));
}

uint64 GTCellId::range_min() const {
    return (id_ - (lsb() - 0x2));
}

uint64 GTCellId::range_max() const {
    return (id_ + (lsb() - 0x2));
}


inline R2Point GTCellId::GetCenterBL() const {
    return R2Point();
}

inline double GTCellId::GetSizeBL() const {
    return 0;
}

inline double GTCellId::GetSizeBL(int level) {
    return 0;
}

inline int GTCellId::GetSizeIJ() const {
    return 0;
}

inline int GTCellId::GetSizeIJ(int level) {
    return 0;
}

inline int GTCellId::GetCenterSiTi(int *psi, int *pti) const {
    return 0;
}


////////////////////////////////////////////////////////
//  网格编码的属性获取
/////////////////////////////////////////////////////////






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
    return std::__cxx11::string();
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
    return std::__cxx11::string();
}

int GTCellId::GetCommonAncestorLevel(GTCellId other) const {
    return 0;
}





