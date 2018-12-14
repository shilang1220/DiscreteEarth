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



uint64 GTCellId::pos() const {
    return 0;
}

int GTCellId::level() const {
    return 0;
}

bool GTCellId::is_leaf() const {
    return false;
}

int GTCellId::child_position() const {
    return 0;
}

int GTCellId::child_position(int level) const {
    return 0;
}

uint64 GTCellId::range_min() const {
    return 0;
}

uint64 GTCellId::range_max() const {
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





