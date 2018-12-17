// Copyright 2012 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS-IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// Author: ericv@google.com (Eric Veach)

#include "s2/r2rect.h"

#include <iosfwd>

#include "base/logging.h"
#include "s2/r1interval.h"
#include "s2/r2.h"


inline R2Rect::R2Rect(const R2Point& lo, const R2Point& hi) {
  bounds_[0] = R1Interval(lo.x(), hi.x());
  bounds_[1] = R1Interval(lo.y(), hi.y());
          S2_DCHECK(is_valid());
}

inline R2Rect::R2Rect(const R1Interval& x, const R1Interval& y) {
  bounds_[0] = x;
  bounds_[1] = y;
          S2_DCHECK(is_valid());
}

inline R2Rect::R2Rect() {
  // The default R1Interval constructor creates an empty interval.
          S2_DCHECK(is_valid());
}

inline R2Rect R2Rect::Empty() {
  return R2Rect(R1Interval::Empty(), R1Interval::Empty());
}

inline bool R2Rect::is_valid() const {
  // The x/y ranges must either be both empty or both non-empty.
  return x().is_empty() == y().is_empty();
}

inline bool R2Rect::is_empty() const {
  return x().is_empty();
}

inline R2Rect R2Rect::FromPoint(const R2Point& p) {
  return R2Rect(p, p);
}

inline R2Point R2Rect::GetVertex(int k) const {
  // Twiddle bits to return the points in CCW order (lower left, lower right,
  // upper right, upper left).
  int j = (k >> 1) & 1;
  return GetVertex(j ^ (k & 1), j);
}

inline R2Point R2Rect::GetVertex(int i, int j) const {
  return R2Point(bounds_[0][i], bounds_[1][j]);
}

inline R2Point R2Rect::GetCenter() const {
  return R2Point(x().GetCenter(), y().GetCenter());
}

inline R2Point R2Rect::GetSize() const {
  return R2Point(x().GetLength(), y().GetLength());
}

inline bool R2Rect::Contains(const R2Point& p) const {
  return x().Contains(p.x()) && y().Contains(p.y());
}

inline bool R2Rect::InteriorContains(const R2Point& p) const {
  return x().InteriorContains(p.x()) && y().InteriorContains(p.y());
}

inline R2Rect R2Rect::Expanded(double margin) const {
  return Expanded(R2Point(margin, margin));
}

inline bool R2Rect::operator==(const R2Rect& other) const {
  return x() == other.x() && y() == other.y();
}

R2Rect R2Rect::FromCenterSize(const R2Point& center, const R2Point& size) {
  return R2Rect(R1Interval(center.x() - 0.5 * size.x(),
                           center.x() + 0.5 * size.x()),
                R1Interval(center.y() - 0.5 * size.y(),
                           center.y() + 0.5 * size.y()));
}

R2Rect R2Rect::FromPointPair(const R2Point& p1, const R2Point& p2) {
  return R2Rect(R1Interval::FromPointPair(p1.x(), p2.x()),
                R1Interval::FromPointPair(p1.y(), p2.y()));
}

bool R2Rect::Contains(const R2Rect& other) const {
  return x().Contains(other.x()) && y().Contains(other.y());
}

bool R2Rect::InteriorContains(const R2Rect& other) const {
  return x().InteriorContains(other.x()) && y().InteriorContains(other.y());
}

bool R2Rect::Intersects(const R2Rect& other) const {
  return x().Intersects(other.x()) && y().Intersects(other.y());
}

bool R2Rect::InteriorIntersects(const R2Rect& other) const {
  return x().InteriorIntersects(other.x()) && y().InteriorIntersects(other.y());
}

void R2Rect::AddPoint(const R2Point& p) {
  bounds_[0].AddPoint(p[0]);
  bounds_[1].AddPoint(p[1]);
}

void R2Rect::AddRect(const R2Rect& other) {
  bounds_[0].AddInterval(other[0]);
  bounds_[1].AddInterval(other[1]);
}

R2Point R2Rect::Project(const R2Point& p) const {
  return R2Point(x().Project(p.x()), y().Project(p.y()));
}

R2Rect R2Rect::Expanded(const R2Point& margin) const {
  R1Interval xx = x().Expanded(margin.x());
  R1Interval yy = y().Expanded(margin.y());
  if (xx.is_empty() || yy.is_empty()) return Empty();
  return R2Rect(xx, yy);
}

R2Rect R2Rect::Union(const R2Rect& other) const {
  return R2Rect(x().Union(other.x()), y().Union(other.y()));
}

R2Rect R2Rect::Intersection(const R2Rect& other) const {
  R1Interval xx = x().Intersection(other.x());
  R1Interval yy = y().Intersection(other.y());
  if (xx.is_empty() || yy.is_empty()) return Empty();
  return R2Rect(xx, yy);
}

bool R2Rect::ApproxEquals(const R2Rect& other, double max_error) const {
  return (x().ApproxEquals(other.x(), max_error) &&
          y().ApproxEquals(other.y(), max_error));
}

std::ostream& operator<<(std::ostream& os, const R2Rect& r) {
  return os << "[Lo" << r.lo() << ", Hi" << r.hi() << "]";
}
