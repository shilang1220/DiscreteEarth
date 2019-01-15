//
// Created by 濮国梁 on 2018/12/13.
//

#include "core/gt_cap.h"

#include <cfloat>
#include <cmath>
#include <iosfwd>
#include <vector>

#include "base/integral_types.h"
#include "base/logging.h"
#include "s2/r1interval.h"
#include "s2/s1interval.h"
#include "tools/gt_debug.h"
#include "s2/s2latlng.h"

#include "s2/s2pointutil.h"
#include "util/math/vector.h"

#include "core/gt_cell.h"
#include "core/gt_latlng_rect.h"
//#include "s2/s2edge_distances.h"

using std::fabs;
using std::max;
using std::min;
using std::vector;


//S = 2 * PI * R * Height
double GTCap::GetArea() const {
    return 2 * M_PI * max(0.0, height());
}

S2Point GTCap::GetCentroid() const {
    // From symmetry, the centroid of the cap must be somewhere on the line
    // from the origin to the center of the cap on the surface of the sphere.
    // When a sphere is divided into slices of constant thickness by a set of
    // parallel planes, all slices have the same surface area. This implies
    // that the radial component of the centroid is simply the midpoint of the
    // range of radial distances spanned by the cap. That is easily computed
    // from the cap height.
    if (is_empty()) return S2Point();
    double r = 1.0 - 0.5 * height();
    return r * GetArea() * center_;
}

GTCap GTCap::Complement() const {
    // The complement of a full cap is an empty cap, not a singleton.
    // Also make sure that the complement of an empty cap is full.
    if (is_full()) return Empty();
    if (is_empty()) return Full();
    return GTCap(-center_, S1ChordAngle::FromLength2(4 - radius_.length2()));
}

bool GTCap::Contains(const GTCap& other) const {
    if (is_full() || other.is_empty()) return true;
    return radius_ >= S1ChordAngle(center_, other.center_) + other.radius_;
}

bool GTCap::Intersects(const GTCap& other) const {
    if (is_empty() || other.is_empty()) return false;
    return radius_ + other.radius_ >= S1ChordAngle(center_, other.center_);
}

bool GTCap::InteriorIntersects(const GTCap& other) const {
    // Make sure this cap has an interior and the other cap is non-empty.
    if (radius_.length2() <= 0 || other.is_empty()) return false;
    return radius_ + other.radius_ > S1ChordAngle(center_, other.center_);
}

void GTCap::AddPoint(const S2Point& p) {
    // Compute the squared chord length, then convert it into a height.
    S2_DCHECK(S2::IsUnitLength(p));
    if (is_empty()) {
        center_ = p;
        radius_ = S1ChordAngle::Zero();
    } else {
        // After calling cap.AddPoint(p), cap.Contains(p) must be true.  However
        // we don't need to do anything special to achieve this because Contains()
        // does exactly the same distance calculation that we do here.
        radius_ = max(radius_, S1ChordAngle(center_, p));
    }
}

void GTCap::AddCap(const GTCap& other) {
    if (is_empty()) {
        *this = other;
    } else {
        // We round up the distance to ensure that the cap is actually contained.
        // TODO(ericv): Do some error analysis in order to guarantee this.
        S1ChordAngle dist = S1ChordAngle(center_, other.center_) + other.radius_;
        radius_ = max(radius_, dist.PlusError(DBL_EPSILON * dist.length2()));
    }
}

GTCap GTCap::Expanded(S1Angle distance) const {
    S2_DCHECK_GE(distance.radians(), 0);
    if (is_empty()) return Empty();
    return GTCap(center_, radius_ + S1ChordAngle(distance));
}

GTCap GTCap::Union(const GTCap& other) const {
    if (radius_ < other.radius_) {
        return other.Union(*this);
    }
    if (is_full() || other.is_empty()) {
        return *this;
    }
    // This calculation would be more efficient using S1ChordAngles.
    S1Angle this_radius = GetRadius();
    S1Angle other_radius = other.GetRadius();
    S1Angle distance(center(), other.center());
    if (this_radius >= distance + other_radius) {
        return *this;
    } else {
        S1Angle result_radius = 0.5 * (distance + this_radius + other_radius);
        S2Point result_center;
//        S2Point result_center = s2::InterpolateAtDistance(
//                0.5 * (distance - this_radius + other_radius),
//                center(),
//                other.center());
        return GTCap(result_center, result_radius);
    }
}

GTCap* GTCap::Clone() const {
    return new GTCap(*this);
}

GTCap GTCap::GetCapBound() const {
    return *this;
}

GTLatLngRect GTCap::GetRectBound() const {
    if (is_empty()) return GTLatLngRect::Empty();

    // Convert the center to a (lat,lng) pair, and compute the cap angle.
    S2LatLng center_ll(center_);
    double cap_angle = GetRadius().radians();

    bool all_longitudes = false;
    double lat[2], lng[2];
    lng[0] = -M_PI;
    lng[1] = M_PI;

    // Check whether cap includes the south pole.
    lat[0] = center_ll.lat().radians() - cap_angle;
    if (lat[0] <= -M_PI_2) {
        lat[0] = -M_PI_2;
        all_longitudes = true;
    }
    // Check whether cap includes the north pole.
    lat[1] = center_ll.lat().radians() + cap_angle;
    if (lat[1] >= M_PI_2) {
        lat[1] = M_PI_2;
        all_longitudes = true;
    }
    if (!all_longitudes) {
        // Compute the range of longitudes covered by the cap.  We use the law
        // of sines for spherical triangles.  Consider the triangle ABC where
        // A is the north pole, B is the center of the cap, and C is the point
        // of tangency between the cap boundary and a line of longitude.  Then
        // C is a right angle, and letting a,b,c denote the sides opposite A,B,C,
        // we have sin(a)/sin(A) = sin(c)/sin(C), or sin(A) = sin(a)/sin(c).
        // Here "a" is the cap angle, and "c" is the colatitude (90 degrees
        // minus the latitude).  This formula also works for negative latitudes.
        //
        // The formula for sin(a) follows from the relationship h = 1 - cos(a).

        double sin_a = sin(radius_);
        double sin_c = cos(center_ll.lat().radians());
        if (sin_a <= sin_c) {
            double angle_A = asin(sin_a / sin_c);
            lng[0] = remainder(center_ll.lng().radians() - angle_A, 2 * M_PI);
            lng[1] = remainder(center_ll.lng().radians() + angle_A, 2 * M_PI);
        }
    }
    return GTLatLngRect(R1Interval(lat[0], lat[1]),
                        S1Interval(lng[0], lng[1]));
}


bool GTCap::Intersects(const GTCell& cell, const S2Point* vertices) const {
    // Return true if this cap intersects any point of 'cell' excluding its
    // vertices (which are assumed to already have been checked).

    // If the cap is a hemisphere or larger, the cell and the complement of the
    // cap are both convex.  Therefore since no vertex of the cell is contained,
    // no other interior point of the cell is contained either.
    if (radius_ >= S1ChordAngle::Right()) return false;

    // We need to check for empty caps due to the center check just below.
    if (is_empty()) return false;

    // Optimization: return true if the cell contains the cap center.  (This
    // allows half of the edge checks below to be skipped.)
    if (cell.Contains(center_)) return true;

    // At this point we know that the cell does not contain the cap center,
    // and the cap does not contain any cell vertex.  The only way that they
    // can intersect is if the cap intersects the interior of some edge.

    double sin2_angle = sin2(radius_);
    for (int k = 0; k < 4; ++k) {
        S2Point edge = cell.GetEdge(k);
        double dot = center_.DotProd(edge);
        if (dot > 0) {
            // The center is in the interior half-space defined by the edge.  We don't
            // need to consider these edges, since if the cap intersects this edge
            // then it also intersects the edge on the opposite side of the cell
            // (because we know the center is not contained with the cell).
            continue;
        }
        // The Norm2() factor is necessary because "edge" is not normalized.
        if (dot * dot > sin2_angle * edge.Norm2()) {
            return false;  // Entire cap is on the exterior side of this edge.
        }
        // Otherwise, the great circle containing this edge intersects
        // the interior of the cap.  We just need to check whether the point
        // of closest approach occurs between the two edge endpoints.
        Vector3_d dir = edge.CrossProd(center_);
        if (dir.DotProd(vertices[k]) < 0 && dir.DotProd(vertices[(k+1)&3]) > 0)
            return true;
    }
    return false;
}

bool GTCap::Contains(const GTCell& cell) const {
    // If the cap does not contain all cell vertices, return false.
    // We check the vertices before taking the Complement() because we can't
    // accurately represent the complement of a very small cap (a height
    // of 2-epsilon is rounded off to 2).
    S2Point vertices[4];
    for (int k = 0; k < 4; ++k) {
        vertices[k] = cell.GetVertex(k);
        if (!Contains(vertices[k])) return false;
    }
    // Otherwise, return true if the complement of the cap does not intersect
    // the cell.  (This test is slightly conservative, because technically we
    // want Complement().InteriorIntersects() here.)
    return !Complement().Intersects(cell, vertices);
}

bool GTCap::MayIntersect(const GTCell& cell) const {
    // If the cap contains any cell vertex, return true.
    S2Point vertices[4];
    for (int k = 0; k < 4; ++k) {
        vertices[k] = cell.GetVertex(k);
        if (Contains(vertices[k])) return true;
    }
    return Intersects(cell, vertices);
}

bool GTCap::Contains(const S2Point& p) const {
            S2_DCHECK(S2::IsUnitLength(p));
    return S1ChordAngle(center_, p) <= radius_;
}

bool GTCap::InteriorContains(const S2Point& p) const {
            S2_DCHECK(S2::IsUnitLength(p));
    return is_full() || S1ChordAngle(center_, p) < radius_;
}

bool GTCap::operator==(const GTCap& other) const {
    return (center_ == other.center_ && radius_ == other.radius_) ||
           (is_empty() && other.is_empty()) ||
           (is_full() && other.is_full());
}

bool GTCap::ApproxEquals(const GTCap& other, S1Angle max_error_angle) const {
    const double max_error = max_error_angle.radians();
    const double r2 = radius_.length2();
    const double other_r2 = other.radius_.length2();
    return (S2::ApproxEquals(center_, other.center_, max_error_angle) &&
            fabs(r2 - other_r2) <= max_error) ||
           (is_empty() && other_r2 <= max_error) ||
           (other.is_empty() && r2 <= max_error) ||
           (is_full() && other_r2 >= 2 - max_error) ||
           (other.is_full() && r2 >= 2 - max_error);
}

std::ostream& operator<<(std::ostream& os, const GTCap& cap) {
    return os << "[Center=" << cap.center()
              << ", Radius=" << cap.GetRadius() << "]";
}

void GTCap::Encode(Encoder* encoder) const {
    encoder->Ensure(4 * sizeof(double));

    encoder->putdouble(center_.x());
    encoder->putdouble(center_.y());
    encoder->putdouble(center_.z());
    encoder->putdouble(radius_.length2());

    S2_DCHECK_GE(encoder->avail(), 0);
}

bool GTCap::Decode(Decoder* decoder) {
    if (decoder->avail() < 4 * sizeof(double)) return false;

    double x = decoder->getdouble();
    double y = decoder->getdouble();
    double z = decoder->getdouble();
    center_ = S2Point(x, y, z);
    radius_ = S1ChordAngle::FromLength2(decoder->getdouble());

    if (FLAGS_s2debug) {
        S2_CHECK(is_valid()) << "Invalid GTCap: " << *this;
    }
    return true;
}
