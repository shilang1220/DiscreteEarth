//
// Created by 濮国梁 on 2018/12/13.
//
#include <core/gt_cell_id.h>

#include "util/bits/bits.h"
#include "core/gt_cell_id.h"
#include "core/gt_coords.h"
#include "s2/r2rect.h"
#include "s2/s2latlng.h"

GTCellId::GTCellId(const S2Point p) {
    //默认为叶子网格
    FromPoint(p);
}
GTCellId::GTCellId(const S2LatLng ll) {
    //默认为叶子网格
    FromLatLng(ll);
}
GTCellId::GTCellId(const S2Point p,int level) {
    //默认为叶子网格
    FromPoint(p,level);
}
GTCellId::GTCellId(const S2LatLng ll,int level) {
    //默认为叶子网格
    FromLatLng(ll,level);
}
S2Point GTCellId::ToPoint() const {
    S2Point pnt;

    GT::CellIDtoXYZ(id_,&pnt);

    return pnt;
}

bool GTCellId::FromPoint(S2Point point) {

    uint64 id;

    GT::XYZtoCellID(point,&id);
    id_ = id;
    level_= kMaxLevel;

    return true;
}

bool GTCellId::FromPoint(S2Point point, unsigned int level)  {
    uint32 I,J,mask;

    S2_DCHECK_LE(level,31);

    GT::XYZtoCellID(point,&id_,level);
    level_ = level;

    return true;
}


S2LatLng GTCellId::ToLatLng() const {
    double lng,lat;

    GT::CellIDtoLL(id_,&lng,&lat);

    return S2LatLng().FromDegrees(lat,lng);
}

bool GTCellId::FromLatLng(S2LatLng latLng)  {
    GT::LLtoCellID(latLng.lng().degrees(),latLng.lat().degrees(),&id_);
    level_= kMaxLevel;
    return true;
}

bool GTCellId::FromLatLng(S2LatLng latLng, unsigned int level)  {
    uint32 I,J,mask;
    
    S2_DCHECK_LE(level,31);

    GT::LLtoCellID(latLng.lng().degrees(),latLng.lat().degrees(),&id_,level);
    level_ = level;

    return true;
}

int GTCellId::child_position() const {

    return child_position(level());
}

int GTCellId::child_position(int level) const {
    S2_DCHECK(is_valid());
    S2_DCHECK_GE(level, 1);
    S2_DCHECK_LE(level, this->level());
    return static_cast<int>(id_ >> (2 * (kMaxLevel - level +1 ))) & 0x3;
}

bool GTCellId::contains(const GTCellId other) const {
    S2_DCHECK(is_valid());
    S2_DCHECK(other.is_valid());
    return other >= range_min() && other <= range_max();
}

bool GTCellId::isChildOf(const GTCellId other) const {
    S2_DCHECK(is_valid());
    S2_DCHECK(other.is_valid());

    return (*this)<=other.range_max() && (*this)>=other.range_min();
}

bool GTCellId::intersects(const GTCellId other) const {
    S2_DCHECK(is_valid());
    S2_DCHECK(other.is_valid());
    return other.range_min() <= range_max() && other.range_max() >= range_min();
}

GTCellId GTCellId::parent() const {
    S2_DCHECK(is_valid());
    S2_DCHECK_GE(level, 0);
    S2_DCHECK_LE(level, this->level());

    uint64 new_lsb = lsb_for_level(level);
    return GTCellId((id_ & (~new_lsb + 1)) | new_lsb);
}

GTCellId GTCellId::parent(int level) const {
    return GTCellId();
}

GTCellId GTCellId::child(int position) const {
    return GTCellId();
}

GTCellId GTCellId::child_begin() const {
    return GTCellId();
}

GTCellId GTCellId::child_end() const {
    return GTCellId();
}

GTCellId GTCellId::child_begin(int level) const {
    return GTCellId();
}

GTCellId GTCellId::child_end(int level) const {
    return GTCellId();
}

GTCellId GTCellId::next() const {
    return GTCellId();
}

GTCellId GTCellId::prev() const {
    return GTCellId();
}

GTCellId GTCellId::advance(int64 steps) const {
    return GTCellId();
}

int64 GTCellId::distance_from_begin() const {
    return 0;
}

GTCellId GTCellId::maximum_tile(GTCellId limit) const {
    return GTCellId();
}

int GTCellId::GetCommonAncestorLevel(GTCellId other) const {
    return 0;
}

GTCellId GTCellId::Begin(int level) {
    return GTCellId();
}

GTCellId GTCellId::End(int level) {
    return GTCellId();
}

void GTCellId::GetFourNeighbors(GTCellId *neighbors) const {

}

void GTCellId::GetEightNeighbors(GTCellId *neighbors) const {

}

void GTCellId::AppendVertexNeighbors(int level, std::vector<GTCellId> *output) const {

}

void GTCellId::AppendAllNeighbors(int nbr_level, std::vector<GTCellId> *output) const {

}

string GTCellId::ToToken() const {
    return std::__cxx11::string();
}

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

string GTCellId::ToString() const {
    return std::string();
}

uint64 GTCellId::pos() const {
    return 0;
}

bool GTCellId::is_leaf() const {
    return false;
}


uint64 GTCellId::id () const {
    return id_;
}







