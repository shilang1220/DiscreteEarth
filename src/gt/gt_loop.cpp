//
// Created by 濮国梁 on 2018/12/13.
//

#include <gt/gt_loop.h>

#include "gt/gt_loop.h"

GTLoop::GTLoop() {

}

GTLoop::GTLoop(const std::vector<S2Point> &vertices) {

}

GTLoop::GTLoop(const std::vector<S2LatLng> &vertices) {

}

void GTLoop::Init(const std::vector<S2Point> &vertices) {

}

void GTLoop::Init(const std::vector<S2LatLng> &vertices) {

}

GTLoop::~GTLoop() {

}

bool GTLoop::IsValid() const {
    return false;
}

bool GTLoop::FindValidationError(S2Error *error) const {
    return false;
}

S1Angle GTLoop::GetLength() const {
    return S1Angle();
}

S2Point GTLoop::GetCentroid() const {
    return S2Point();
}

S2Point GTLoop::Interpolate(double fraction) const {
    return S2Point();
}

S2Point GTLoop::GetSuffix(double fraction, int *next_vertex) const {
    return S2Point();
}

double GTLoop::UnInterpolate(const S2Point &point, int next_vertex) const {
    return 0;
}

S2Point GTLoop::Project(const S2Point &point, int *next_vertex) const {
    return S2Point();
}

bool GTLoop::IsOnRight(const S2Point &point) const {
    return false;
}

bool GTLoop::Intersects(const GTLoop *line) const {
    return false;
}

void GTLoop::Reverse() {

}

void GTLoop::SubsampleVertices(S1Angle tolerance, std::vector<int> *indices) const {

}

bool GTLoop::Equals(const GTLoop *b) const {
    return false;
}

bool GTLoop::ApproxEquals(const GTLoop &b, S1Angle max_error) const {
    return false;
}

bool GTLoop::NearlyCovers(const GTLoop &covered, S1Angle max_error) const {
    return false;
}

size_t GTLoop::SpaceUsed() const {
    return 0;
}

GTLoop *GTLoop::Clone() const {
    return nullptr;
}

GTCap GTLoop::GetCapBound() const {
    return GTCap();
}

GTLatLngRect GTLoop::GetRectBound() const {
    return GTLatLngRect();
}

bool GTLoop::Contains(const GTCell &cell) const {
    return false;
}

bool GTLoop::MayIntersect(const GTCell &cell) const {
    return false;
}

bool GTLoop::Contains(const S2Point &p) const {
    return false;
}

void GTLoop::Encode(Encoder *const encoder) const {

}

bool GTLoop::Decode(Decoder *const decoder) {
    return false;
}
