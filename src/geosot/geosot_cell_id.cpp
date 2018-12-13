//
// Created by 濮国梁 on 2018/12/13.
//
#include "geosot/geosot_cell_id.h"

GeosotCellId::GeosotCellId(const S2Point p) {

}
GeosotCellId::GeosotCellId(const S2LatLng ll) {

}


S2Point GeosotCellId::ToPoint() const {
    return S2Point();
}

bool GeosotCellId::FromPoint(S2Point point) const {
    return false;
}

bool GeosotCellId::FromPoint(S2Point point, unsigned int level) const {
    return false;
}


S2LatLng GeosotCellId::ToLatLng() const {
    return S2LatLng();
}

bool GeosotCellId::FromLatLng(S2LatLng latLng) const {
    return false;
}

bool GeosotCellId::FromLatLng(S2LatLng latLng, unsigned int level) const {
    return false;
}

bool GeosotCellId::is_valid() const {
    return false;
}

uint64 GeosotCellId::pos() const {
    return 0;
}

int GeosotCellId::level() const {
    return 0;
}

bool GeosotCellId::is_leaf() const {
    return false;
}

int GeosotCellId::child_position() const {
    return 0;
}

int GeosotCellId::child_position(int level) const {
    return 0;
}

uint64 GeosotCellId::range_min() const {
    return 0;
}

uint64 GeosotCellId::range_max() const {
    return 0;
}

bool GeosotCellId::contains(const uint64 other) const {
    return false;
}

bool GeosotCellId::contains(const GeosotCellId other) const {
    return false;
}

bool GeosotCellId::intersects(const uint64 other) const {
    return false;
}

bool GeosotCellId::intersects(const GeosotCellId other) const {
    return false;
}

uint64 GeosotCellId::parentID() const {
    return 0;
}

uint64 GeosotCellId::parentID(int level) const {
    return 0;
}

uint64 GeosotCellId::child(int position) const {
    return 0;
}

uint64 GeosotCellId::child_begin() const {
    return 0;
}

uint64 GeosotCellId::child_end() const {
    return 0;
}

uint64 GeosotCellId::child_begin(int level) const {
    return 0;
}

uint64 GeosotCellId::child_end(int level) const {
    return 0;
}

uint64 GeosotCellId::next() const {
    return 0;
}

uint64 GeosotCellId::prev() const {
    return 0;
}

uint64 GeosotCellId::advance(int64 steps) const {
    return 0;
}

int64 GeosotCellId::distance_from_begin() const {
    return 0;
}

uint64 GeosotCellId::maximum_tile(uint64 limit) const {
    return 0;
}

int GeosotCellId::GetCommonAncestorLevel(uint64 other) const {
    return 0;
}

uint64 GeosotCellId::Begin(int level) {
    return 0;
}

uint64 GeosotCellId::End(int level) {
    return 0;
}

void GeosotCellId::GetFourNeighbors(uint64 *neighbors) const {

}

void GeosotCellId::GetEightNeighbors(uint64 *neighbors) const {

}

void GeosotCellId::AppendVertexNeighbors(int level, std::vector<uint64> *output) const {

}

void GeosotCellId::AppendAllNeighbors(int nbr_level, std::vector<CellId> *output) const {

}

string GeosotCellId::ToToken() const {
    return std::__cxx11::string();
}

uint64 GeosotCellId::FromToken(const char *token, size_t length) {
    return 0;
}

uint64 GeosotCellId::FromToken(const string &token) {
    return 0;
}

void GeosotCellId::Encode(Encoder *const encoder) const {

}

bool GeosotCellId::Decode(Decoder *const decoder) {
    return false;
}

string GeosotCellId::ToString() const {
    return std::__cxx11::string();
}

int GeosotCellId::GetCommonAncestorLevel(GeosotCellId other) const {
    return 0;
}




