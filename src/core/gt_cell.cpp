//
// Created by 濮国梁 on 2018/12/13.
//

#include <core/gt_cell.h>

#include "core/gt_cell.h"


S2Point GTCell::GetVertex (int k) const {
    return S2Point();
}

S2Point GTCell::GetEdge (int k) const {
    return S2Point();
}

S2Point GTCell::GetCenter () const {
    return S2Point();
}

double GTCell::AverageArea (int level) {
    return 0;
}

double GTCell::AverageArea () const {
    return 0;
}

double GTCell::ApproxArea () const {
    return 0;
}

double GTCell::ExactArea () const {
    return 0;
}

S1ChordAngle GTCell::GetDistance (const S2Point &target) const {
    return S1ChordAngle();
}

S1ChordAngle GTCell::GetBoundaryDistance (const S2Point &target) const {
    return S1ChordAngle();
}

S1ChordAngle GTCell::GetMaxDistance (const S2Point &target) const {
    return S1ChordAngle();
}

S1ChordAngle GTCell::GetDistance (const S2Point &a, const S2Point &b) const {
    return S1ChordAngle();
}

S1ChordAngle GTCell::GetMaxDistance (const S2Point &a, const S2Point &b) const {
    return S1ChordAngle();
}

S1ChordAngle GTCell::GetDistance (const GTCell &target) const {
    return S1ChordAngle();
}

S1ChordAngle GTCell::GetMaxDistance (const GTCell &target) const {
    return S1ChordAngle();
}

bool GTCell::Subdivide (GTCell *pChildren) const {
    return false;
}

GTCell *GTCell::Clone () const {
    return nullptr;
}

GTCap GTCell::GetCapBound () const {
    GTCap cap;

    return cap;
}

GTLatLngRect GTCell::GetRectBound () const {
    GTLatLngRect rect;

    return rect;
}

bool GTCell::Contains (const GTCell &cell) const {
    return false;
}

bool GTCell::MayIntersect (const GTCell &cell) const {
    return false;
}

void GTCell::Encode (Encoder *const encoder) const {

}

bool GTCell::Decode (Decoder *const decoder) {
    return false;
}

bool GTCell::Contains(const S2Point &p) const {
    return false;
}


