//
// Created by Administrator on 2018/12/22 0022.
//

#ifndef DISCRETEEARTH_GTPOINTREGION_H
#define DISCRETEEARTH_GTPOINTREGION_H

#include "base/logging.h"
#include "_fp_contract_off.h"
#include "exports.h"

#include "s2/s1angle.h"
#include "s2/s2pointutil.h"
#include "core/region.h"
#include "third_party/absl/base/macros.h"

class Decoder;
class Encoder;
class Cap;
class Cell;
class LatLngRect;


class GT_API GTPointRegion final: public Region {
 public:
  // Create a region containing the given point, which must be unit length.
  explicit GTPointRegion(const S2Point& point);

  ~GTPointRegion() override;

  const S2Point& point() const { return point_; }

  ////////////////////////////////////////////////////////////////////////
  // S2Region interface (see s2region.h for details):

  GTPointRegion* Clone() const override;
  Cap GetCapBound() const override;
  LatLngRect GetRectBound() const override;
  bool Contains(const Cell& cell) const override { return false; }
  bool MayIntersect(const Cell& cell) const override;
  bool Contains(const S2Point& p) const override { return (point_ == p); }

  // Appends a serialized representation of the S2Point to "encoder".
  //
  // REQUIRES: "encoder" uses the default constructor, so that its buffer
  //           can be enlarged as necessary by calling Ensure(int).
  void Encode(Encoder* const encoder) const;

  // Decodes an S2Point encoded with Encode().  Returns true on success.
  // (Returns false if the encoded point is not unit length.)
  bool Decode(Decoder* const decoder);

 private:
  S2Point point_;
};

inline GTPointRegion::GTPointRegion(const S2Point& point) : point_(point) {
  S2_DCHECK(S2::IsUnitLength(point));
}

#endif //DISCRETEEARTH_GTPOINTREGION_H
