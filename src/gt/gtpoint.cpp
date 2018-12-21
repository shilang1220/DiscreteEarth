//
// Created by Administrator on 2018/12/22 0022.
//

#include "gt/gtpoint.h"


#include "base/logging.h"
#include "util/coding/coder.h"
#include "s2/s2latlng.h"
#include "s2/s2pointutil.h"
#include "core/cap.h"
#include "core/cell.h"
#include "core/latlng_rect.h"

GTPointRegion::~GTPointRegion() {
}

GTPointRegion* GTPointRegion::Clone() const {
  return new GTPointRegion(point_);
}

Cap GTPointRegion::GetCapBound() const {
  return Cap::FromPoint(point_);
}

LatLngRect GTPointRegion::GetRectBound() const {
  S2LatLng ll(point_);
  return LatLngRect(ll, ll);
}

bool GTPointRegion::MayIntersect(const Cell& cell) const {
  return cell.Contains(point_);
}

void GTPointRegion::Encode(Encoder* encoder) const {

  S2_DCHECK_GE(encoder->avail(), 0);
}

bool GTPointRegion::Decode(Decoder* decoder) {

  return true;
}
