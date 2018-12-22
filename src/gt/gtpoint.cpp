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

GTPoint::~GTPoint() {
}

GTPoint* GTPoint::Clone() const {
  return new GTPoint(point_);
}

Cap GTPoint::GetCapBound() const {
  return Cap::FromPoint(point_);
}

LatLngRect GTPoint::GetRectBound() const {
  S2LatLng ll(point_);
  return LatLngRect(ll, ll);
}

bool GTPoint::MayIntersect(const Cell& cell) const {
  return cell.Contains(point_);
}

void GTPoint::Encode(Encoder* encoder) const {

  S2_DCHECK_GE(encoder->avail(), 0);
}

bool GTPoint::Decode(Decoder* decoder) {

  return true;
}
