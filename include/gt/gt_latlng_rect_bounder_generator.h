// Copyright 2005 Google Inc. All Rights Reserved.
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

#ifndef GT_LATLNG_RECT_BOUNDER_GENERATOR_H
#define GT_LATLNG_RECT_BOUNDER_GENERATOR_H

#include "_fp_contract_off.h"
#include "exports.h"

#include "s2/s2point.h"
#include "s2/s2latlng.h"
#include "core/gt_latlng_rect.h"

// 用于外包矩形计算器类

// This class computes a bounding rectangle that contains all edges defined
// by a vertex chain v0, v1, v2, ...  All vertices must be unit length.
// Note that the bounding rectangle of an edge can be larger than the
// bounding rectangle of its endpoints, e.g. consider an edge that passes
// through the north pole.
//
// The bounds are calculated conservatively to account for numerical errors
// when S2Points are converted to S2LatLngs.  More precisely, this class
// guarantees the following.  Let L be a closed edge chain (loop) such that
// the interior of the loop does not contain either pole.  Now if P is any
// point such that L.Contains(P), then RectBound(L).Contains(S2LatLng(P)).
 class GT_API GTLatLngRectBounderGenerator {
 public:
  GTLatLngRectBounderGenerator() : bound_(GTLatLngRect::Empty()) {}

  // This method is called to add a vertex to the chain when the vertex is
  // represented as an S2Point.  Requires that 'b' has unit length.  Repeated
  // vertices are ignored.
  void AddPoint(const S2Point& b);

  // This method is called to add a vertex to the chain when the vertex is
  // represented as an S2LatLng.  Repeated vertices are ignored.
  void AddLatLng(const S2LatLng& b_latlng);

  // Returns the bounding rectangle of the edge chain that connects the
  // vertices defined so far.  This bound satisfies the guarantee made
  // above, i.e. if the edge chain defines a loop, then the bound contains
  // the S2LatLng coordinates of all S2Points contained by the loop.
  GTLatLngRect GetBound() const;

  // Expands a bound returned by GetBound() so that it is guaranteed to
  // contain the bounds of any subregion whose bounds are computed using
  // this class.  For example, consider a loop L that defines a square.
  // GetBound() ensures that if a point P is contained by this square, then
  // S2LatLng(P) is contained by the bound.  But now consider a diamond
  // shaped loop S contained by L.  It is possible that GetBound() returns a
  // *larger* bound for S than it does for L, due to rounding errors.  This
  // method expands the bound for L so that it is guaranteed to contain the
  // bounds of any subregion S.
  //
  // More precisely, if L is a loop that does not contain either pole, and S
  // is a loop such that L.Contains(S), then
  //
  //   ExpandForSubregions(RectBound(L)).Contains(RectBound(S)).
  static GTLatLngRect ExpandForSubregions(const GTLatLngRect& bound);

  // Returns the maximum error in GetBound() provided that the result does
  // not include either pole.  It is only to be used for testing purposes
  // (e.g., by passing it to LatLngRect::ApproxEquals).
  static S2LatLng MaxErrorForTests();

 private:
  // Common back end for AddPoint() and AddLatLng().  b and b_latlng
  // must refer to the same vertex.
  void AddInternal(const S2Point& b, const S2LatLng& b_latlng);

  S2Point a_;             // The previous vertex in the chain.
  S2LatLng a_latlng_;     // The corresponding latitude-longitude.
  GTLatLngRect bound_;    // The current bounding rectangle.

  GTLatLngRectBounderGenerator(const GTLatLngRectBounderGenerator&) = delete;
  void operator=(const GTLatLngRectBounderGenerator&) = delete;
};

#endif  // GT_LATLNG_RECT_BOUNDER_GENERATOR_H
