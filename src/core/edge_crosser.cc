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

#include "core/edge_crosser.h"

#include "base/logging.h"
#include "s2/s2pointutil.h"
#include "s2/s2predicates.h"

//////////////////   Implementation details follow   ////////////////////


inline S2EdgeCrosser::S2EdgeCrosser(const S2Point* a, const S2Point* b)
        : a_(a), b_(b), a_cross_b_(a_->CrossProd(*b_)), have_tangents_(false),
          c_(nullptr) {
          S2_DCHECK(S2::IsUnitLength(*a));
          S2_DCHECK(S2::IsUnitLength(*b));
}

inline void S2EdgeCrosser::Init(const S2Point* a, const S2Point* b) {
  a_ = a;
  b_ = b;
  a_cross_b_ = a->CrossProd(*b_);
  have_tangents_ = false;
  c_ = nullptr;
}

inline int S2EdgeCrosser::CrossingSign(const S2Point* c, const S2Point* d) {
  if (c != c_) RestartAt(c);
  return CrossingSign(d);
}

inline bool S2EdgeCrosser::EdgeOrVertexCrossing(const S2Point* c,
                                                const S2Point* d) {
  if (c != c_) RestartAt(c);
  return EdgeOrVertexCrossing(d);
}

inline S2EdgeCrosser::S2EdgeCrosser(
        const S2Point* a, const S2Point* b, const S2Point* c)
        : a_(a), b_(b), a_cross_b_(a_->CrossProd(*b_)), have_tangents_(false) {
          S2_DCHECK(S2::IsUnitLength(*a));
          S2_DCHECK(S2::IsUnitLength(*b));
  RestartAt(c);
}

inline void S2EdgeCrosser::RestartAt(const S2Point* c) {
          S2_DCHECK(S2::IsUnitLength(*c));
  c_ = c;
  acb_ = -s2pred::TriageSign(*a_, *b_, *c_, a_cross_b_);
}

inline int S2EdgeCrosser::CrossingSign(const S2Point* d) {
          S2_DCHECK(S2::IsUnitLength(*d));
  // For there to be an edge crossing, the triangles ACB, CBD, BDA, DAC must
  // all be oriented the same way (CW or CCW).  We keep the orientation of ACB
  // as part of our state.  When each new point D arrives, we compute the
  // orientation of BDA and check whether it matches ACB.  This checks whether
  // the points C and D are on opposite sides of the great circle through AB.

  // Recall that TriageSign is invariant with respect to rotating its
  // arguments, i.e. ABD has the same orientation as BDA.
  int bda = s2pred::TriageSign(*a_, *b_, *d, a_cross_b_);
  if (acb_ == -bda && bda != 0) {
    // The most common case -- triangles have opposite orientations.  Save the
    // current vertex D as the next vertex C, and also save the orientation of
    // the new triangle ACB (which is opposite to the current triangle BDA).
    c_ = d;
    acb_ = -bda;
    return -1;
  }
  bda_ = bda;
  return CrossingSignInternal(d);
}

inline bool S2EdgeCrosser::EdgeOrVertexCrossing(const S2Point* d) {
  // We need to copy c_ since it is clobbered by CrossingSign().
  const S2Point* c = c_;
  int crossing = CrossingSign(d);
  if (crossing < 0) return false;
  if (crossing > 0) return true;
  return S2::VertexCrossing(*a_, *b_, *c, *d);
}

inline S2CopyingEdgeCrosser::S2CopyingEdgeCrosser(const S2Point& a,
                                                  const S2Point& b)
        : a_(a), b_(b), c_(S2Point()), crosser_(&a_, &b_) {
}

inline void S2CopyingEdgeCrosser::Init(const S2Point& a, const S2Point& b) {
  a_ = a;
  b_ = b;
  c_ = S2Point();
  crosser_.Init(&a_, &b_);
}

inline int S2CopyingEdgeCrosser::CrossingSign(const S2Point& c,
                                              const S2Point& d) {
  if (c != c_ || crosser_.c_ == nullptr) RestartAt(c);
  return CrossingSign(d);
}

inline bool S2CopyingEdgeCrosser::EdgeOrVertexCrossing(
        const S2Point& c, const S2Point& d) {
  if (c != c_ || crosser_.c_ == nullptr) RestartAt(c);
  return EdgeOrVertexCrossing(d);
}

inline S2CopyingEdgeCrosser::S2CopyingEdgeCrosser(
        const S2Point& a, const S2Point& b, const S2Point& c)
        : a_(a), b_(b), c_(c), crosser_(&a_, &b_, &c) {
}

inline void S2CopyingEdgeCrosser::RestartAt(const S2Point& c) {
  c_ = c;
  crosser_.RestartAt(&c_);
}

inline int S2CopyingEdgeCrosser::CrossingSign(const S2Point& d) {
  int result = crosser_.CrossingSign(&d);
  c_ = d;
  crosser_.set_c(&c_);
  return result;
}

inline bool S2CopyingEdgeCrosser::EdgeOrVertexCrossing(const S2Point& d) {
  bool result = crosser_.EdgeOrVertexCrossing(&d);
  c_ = d;
  crosser_.set_c(&c_);
  return result;
}

int S2EdgeCrosser::CrossingSignInternal(const S2Point* d) {
  // Compute the actual result, and then save the current vertex D as the next
  // vertex C, and save the orientation of the next triangle ACB (which is
  // opposite to the current triangle BDA).
  int result = CrossingSignInternal2(*d);
  c_ = d;
  acb_ = -bda_;
  return result;
}

inline int S2EdgeCrosser::CrossingSignInternal2(const S2Point& d) {
  // At this point, a very common situation is that A,B,C,D are four points on
  // a line such that AB does not overlap CD.  (For example, this happens when
  // a line or curve is sampled finely, or when geometry is constructed by
  // computing the union of S2CellIds.)  Most of the time, we can determine
  // that AB and CD do not intersect by computing the two outward-facing
  // tangents at A and B (parallel to AB) and testing whether AB and CD are on
  // opposite sides of the plane perpendicular to one of these tangents.  This
  // is moderately expensive but still much cheaper than s2pred::ExpensiveSign.
  if (!have_tangents_) {
    S2Point norm = S2::RobustCrossProd(*a_, *b_).Normalize();
    a_tangent_ = a_->CrossProd(norm);
    b_tangent_ = norm.CrossProd(*b_);
    have_tangents_ = true;
  }
  // The error in RobustCrossProd() is insignificant.  The maximum error in
  // the call to CrossProd() (i.e., the maximum norm of the error vector) is
  // (0.5 + 1/sqrt(3)) * DBL_EPSILON.  The maximum error in each call to
  // DotProd() below is DBL_EPSILON.  (There is also a small relative error
  // term that is insignificant because we are comparing the result against a
  // constant that is very close to zero.)
  static const double kError = (1.5 + 1/sqrt(3)) * DBL_EPSILON;
  if ((c_->DotProd(a_tangent_) > kError && d.DotProd(a_tangent_) > kError) ||
      (c_->DotProd(b_tangent_) > kError && d.DotProd(b_tangent_) > kError)) {
    return -1;
  }

  // Otherwise, eliminate the cases where two vertices from different edges
  // are equal.  (These cases could be handled in the code below, but we would
  // rather avoid calling ExpensiveSign whenever possible.)
  if (*a_ == *c_ || *a_ == d || *b_ == *c_ || *b_ == d) return 0;

  // Eliminate cases where an input edge is degenerate.  (Note that in most
  // cases, if CD is degenerate then this method is not even called because
  // acb_ and bda have different signs.)
  if (*a_ == *b_ || *c_ == d) return -1;

  // Otherwise it's time to break out the big guns.
  if (acb_ == 0) acb_ = -s2pred::ExpensiveSign(*a_, *b_, *c_);
  S2_DCHECK_NE(acb_, 0);
  if (bda_ == 0) bda_ = s2pred::ExpensiveSign(*a_, *b_, d);
  S2_DCHECK_NE(bda_, 0);
  if (bda_ != acb_) return -1;

  Vector3_d c_cross_d = c_->CrossProd(d);
  int cbd = -s2pred::Sign(*c_, d, *b_, c_cross_d);
  S2_DCHECK_NE(cbd, 0);
  if (cbd != acb_) return -1;
  int dac = s2pred::Sign(*c_, d, *a_, c_cross_d);
  S2_DCHECK_NE(dac, 0);
  return (dac != acb_) ? -1 : 1;
}
