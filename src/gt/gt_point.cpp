//
// Created by Administrator on 2018/12/22 0022.
//

#include "gt/gt_point.h"


#include "base/logging.h"
#include "util/coding/coder.h"
#include "s2/s2latlng.h"
#include "s2/s2pointutil.h"
#include "core/gt_cap.h"
#include "core/gt_cell.h"
#include "core/gt_latlng_rect.h"

GTPoint::~GTPoint() {
}

GTPoint* GTPoint::Clone() const {
  return new GTPoint(point_);
}

GTCap GTPoint::GetCapBound() const {
  return GTCap::FromPoint(point_);
}

GTLatLngRect GTPoint::GetRectBound() const {
  S2LatLng ll(point_);
  return GTLatLngRect(ll, ll);
}

bool GTPoint::MayIntersect(const GTCell& cell) const {
  return cell.Contains(point_);
}

void GTPoint::Encode(Encoder* encoder) const {

  S2_DCHECK_GE(encoder->avail(), 0);
}

bool GTPoint::Decode(Decoder* decoder) {

  return true;
}
