//
// Created by 濮国梁 on 2018/12/13.
//
#include "gt/gt_polyline.h"

GTPolyline::GTPolyline () {

}

GTPolyline::GTPolyline (const std::vector<S2Point> &vertices) {

}

GTPolyline::GTPolyline (const std::vector<S2LatLng> &vertices) {

}

void GTPolyline::Init (const std::vector<S2Point> &vertices) {

}

void GTPolyline::Init (const std::vector<S2LatLng> &vertices) {

}

GTPolyline::~GTPolyline () {

}

bool GTPolyline::IsValid () const {
    return false;
}

bool GTPolyline::FindValidationError (S2Error *error) const {
    return false;
}

S1Angle GTPolyline::GetLength () const {
    return S1Angle();
}

S2Point GTPolyline::GetCentroid () const {
    return S2Point();
}

S2Point GTPolyline::Interpolate (double fraction) const {
    return S2Point();
}

S2Point GTPolyline::GetSuffix (double fraction, int *next_vertex) const {
    return S2Point();
}

double GTPolyline::UnInterpolate (const S2Point &point, int next_vertex) const {
    return 0;
}

S2Point GTPolyline::Project (const S2Point &point, int *next_vertex) const {
    return S2Point();
}

bool GTPolyline::IsOnRight (const S2Point &point) const {
    return false;
}

bool GTPolyline::Intersects (const GTPolyline *line) const {
    return false;
}

void GTPolyline::Reverse () {

}

void GTPolyline::SubsampleVertices (S1Angle tolerance, std::vector<int> *indices) const {

}

bool GTPolyline::Equals (const GTPolyline *b) const {
    return false;
}

bool GTPolyline::ApproxEquals (const GTPolyline &b, S1Angle max_error) const {
    return false;
}

bool GTPolyline::NearlyCovers (const GTPolyline &covered, S1Angle max_error) const {
    return false;
}

size_t GTPolyline::SpaceUsed () const {
    return 0;
}

GTPolyline *GTPolyline::Clone () const {
    return nullptr;
}

Cap GTPolyline::GetCapBound () const {
    return Cap();
}

LatLngRect GTPolyline::GetRectBound () const {
    return LatLngRect();
}

bool GTPolyline::Contains (const Cell &cell) const {
    return false;
}

bool GTPolyline::MayIntersect (const Cell &cell) const {
    return false;
}

bool GTPolyline::Contains (const S2Point &p) const {
    return false;
}

void GTPolyline::Encode (Encoder *const encoder) const {

}

bool GTPolyline::Decode (Decoder *const decoder) {
    return false;
}
