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

#include "gtcell_union.h"

#include <algorithm>
#include <vector>

#include "base/integral_types.h"
#include "base/logging.h"
#include "util/coding/coder.h"

#include "s2/s1angle.h"
#include "../../src/tmp/s2metrics.h"

#include "core/latlng_rect.h"
#include "core/cap.h"

#include "core/gtcell.h"
#include "core/gtcell_id.h"


using std::is_sorted;
using std::max;
using std::min;
using std::vector;

DEFINE_int32(s2cell_union_decode_max_num_cells, 1000000,
             "The maximum number of cells allowed by GTCellUnion::Decode");

static const unsigned char kCurrentLosslessEncodingVersionNumber = 1;

vector<GTCellId> GTCellUnion::ToGTCellIds(const vector<uint64>& ids) {
  vector<GTCellId> cell_ids;
  cell_ids.reserve(ids.size());
  for (auto id : ids) cell_ids.push_back(GTCellId(id));
  return cell_ids;
}

GTCellUnion::GTCellUnion(const vector<uint64>& cell_ids)
    : cell_ids_(ToGTCellIds(cell_ids)) {
  Normalize();
}

GTCellUnion GTCellUnion::FromMinMax(GTCellId min_id, GTCellId max_id) {
  GTCellUnion result;
  result.InitFromMinMax(min_id, max_id);
  return result;
}

GTCellUnion GTCellUnion::FromBeginEnd(GTCellId begin, GTCellId end) {
  GTCellUnion result;
  result.InitFromBeginEnd(begin, end);
  return result;
}

void GTCellUnion::Init(const vector<uint64>& cell_ids) {
  cell_ids_ = ToGTCellIds(cell_ids);
  Normalize();
}

void GTCellUnion::InitFromMinMax(GTCellId min_id, GTCellId max_id) {
  S2_DCHECK(max_id.is_valid());
  InitFromBeginEnd(min_id, max_id.next());
}

void GTCellUnion::InitFromBeginEnd(GTCellId begin, GTCellId end) {
  S2_DCHECK(begin.is_leaf());
  S2_DCHECK(end.is_leaf());
  S2_DCHECK_LE(begin, end);

  // We repeatedly add the largest cell we can.
  cell_ids_.clear();
  for (GTCellId id = begin.maximum_tile(end.id());
       id != end; id = id.next().maximum_tile(end.id())) {
    cell_ids_.push_back(id);
  }
  // The output is already normalized.
  S2_DCHECK(IsNormalized());
}

void GTCellUnion::Pack(int excess) {
  if (cell_ids_.capacity() - cell_ids_.size() > excess) {
    cell_ids_.shrink_to_fit();
  }
}

GTCellUnion* GTCellUnion::Clone() const {
  return new GTCellUnion(cell_ids_, VERBATIM);
}

// Returns true if the given four cells have a common parent.
// REQUIRES: The four cells are distinct.
inline static bool AreSiblings(GTCellId a, GTCellId b, GTCellId c, GTCellId d) {
  // A necessary (but not sufficient) condition is that the XOR of the
  // four cells must be zero.  This is also very fast to test.
  if ((a.id() ^ b.id() ^ c.id()) != d.id()) return false;

  // Now we do a slightly more expensive but exact test.  First, compute a
  // mask that blocks out the two bits that encode the child position of
  // "id" with respect to its parent, then check that the other three
  // children all agree with "mask".
  uint64 mask = d.lsb() << 1;
  mask = ~(mask + (mask << 1));
  uint64 id_masked = (d.id() & mask);
  return ((a.id() & mask) == id_masked &&
          (b.id() & mask) == id_masked &&
          (c.id() & mask) == id_masked &&
          !d.is_face());
}

bool GTCellUnion::IsValid() const {
  if (num_cells() > 0 && !cell_id(0).is_valid()) return false;
  for (int i = 1; i < num_cells(); ++i) {
    if (!cell_id(i).is_valid()) return false;
    if (cell_id(i - 1).range_max() >= cell_id(i).range_min()) return false;
  }
  return true;
}

bool GTCellUnion::IsNormalized() const {
  if (num_cells() > 0 && !cell_id(0).is_valid()) return false;
  for (int i = 1; i < num_cells(); ++i) {
    if (!cell_id(i).is_valid()) return false;
    if (cell_id(i - 1).range_max() >= cell_id(i).range_min()) return false;
    if (i >= 3 && AreSiblings(cell_id(i - 3), cell_id(i - 2),
                              cell_id(i - 1), cell_id(i))) {
      return false;
    }
  }
  return true;
}

bool GTCellUnion::Normalize() {
  return Normalize(&cell_ids_);
}

/*static*/ bool GTCellUnion::Normalize(vector<GTCellId>* ids) {
  // Optimize the representation by discarding cells contained by other cells,
  // and looking for cases where all subcells of a parent cell are present.

  std::sort(ids->begin(), ids->end());
  int out = 0;
  for (GTCellId id : *ids) {
    // Check whether this cell is contained by the previous cell.
    if (out > 0 && (*ids)[out-1].contains(id)) continue;

    // Discard any previous cells contained by this cell.
    while (out > 0 && id.contains((*ids)[out-1])) --out;

    // Check whether the last 3 elements plus "id" can be collapsed into a
    // single parent cell.
    while (out >= 3 && AreSiblings((*ids)[out - 3], (*ids)[out - 2],
                                   (*ids)[out - 1], id)) {
      // Replace four children by their parent cell.
      id = id.parent();
      out -= 3;
    }
    (*ids)[out++] = id;
  }
  if (ids->size() == out) return false;
  ids->resize(out);
  return true;
}

void GTCellUnion::Denormalize(int min_level, int level_mod,
                              vector<GTCellId>* out) const {
  Denormalize(cell_ids_, min_level, level_mod, out);
}

void GTCellUnion::Denormalize(const vector<GTCellId>& in,
                              int min_level, int level_mod,
                              vector<GTCellId>* out) {
  S2_DCHECK_GE(min_level, 0);
  S2_DCHECK_LE(min_level, GTCellId::kMaxLevel);
  S2_DCHECK_GE(level_mod, 1);
  S2_DCHECK_LE(level_mod, 3);
  S2_DCHECK_NE(out, &in);

  out->clear();
  out->reserve(in.size());
  for (GTCellId id : in) {
    int level = id.level();
    int new_level = max(min_level, level);
    if (level_mod > 1) {
      // Round up so that (new_level - min_level) is a multiple of level_mod.
      // (Note that GTCellId::kMaxLevel is a multiple of 1, 2, and 3.)
      new_level += (GTCellId::kMaxLevel - (new_level - min_level)) % level_mod;
      new_level = min(GTCellId::kMaxLevel, new_level);
    }
    if (new_level == level) {
      out->push_back(id);
    } else {
      GTCellId end = id.child_end(new_level);
      for (id = id.child_begin(new_level); id != end; id = id.next()) {
        out->push_back(id);
      }
    }
  }
}

GTCap GTCellUnion::GetCapBound() const {
  // Compute the approximate centroid of the region.  This won't produce the
  // bounding cap of minimal area, but it should be close enough.
  if (cell_ids_.empty()) return GTCap::Empty();
  GTPoint centroid(0, 0, 0);
  for (GTCellId id : *this) {
    double area = GTCell::AverageArea(id.level());
    centroid += area * id.ToPoint();
  }
  if (centroid == GTPoint(0, 0, 0)) {
    centroid = GTPoint(1, 0, 0);
  } else {
    centroid = centroid.Normalize();
  }

  // Use the centroid as the cap axis, and expand the cap angle so that it
  // contains the bounding caps of all the individual cells.  Note that it is
  // *not* sufficient to just bound all the cell vertices because the bounding
  // cap may be concave (i.e. cover more than one hemisphere).
  GTCap cap = GTCap::FromPoint(centroid);
  for (GTCellId id : *this) {
    cap.AddCap(GTCell(id).GetCapBound());
  }
  return cap;
}

GTLatLngRect GTCellUnion::GetRectBound() const {
  GTLatLngRect bound = GTLatLngRect::Empty();
  for (GTCellId id : *this) {
    bound = bound.Union(GTCell(id).GetRectBound());
  }
  return bound;
}

bool GTCellUnion::Contains(GTCellId id) const {
  // This is an exact test.  Each cell occupies a linear span of the GT
  // space-filling curve, and the cell id is simply the position at the center
  // of this span.  The cell union ids are sorted in increasing order along
  // the space-filling curve.  So we simply find the pair of cell ids that
  // surround the given cell id (using binary search).  There is containment
  // if and only if one of these two cell ids contains this cell.

  vector<GTCellId>::const_iterator i =
      std::lower_bound(cell_ids_.begin(), cell_ids_.end(), id);
  if (i != cell_ids_.end() && i->range_min() <= id) return true;
  return i != cell_ids_.begin() && (--i)->range_max() >= id;
}

bool GTCellUnion::Intersects(GTCellId id) const {
  // This is an exact test; see the comments for Contains() above.

  vector<GTCellId>::const_iterator i =
      std::lower_bound(cell_ids_.begin(), cell_ids_.end(), id);
  if (i != cell_ids_.end() && i->range_min() <= id.range_max()) return true;
  return i != cell_ids_.begin() && (--i)->range_max() >= id.range_min();
}

bool GTCellUnion::Contains(const GTCellUnion& y) const {
  // TODO(ericv): A divide-and-conquer or alternating-skip-search
  // approach may be sigificantly faster in both the average and worst case.

  for (GTCellId y_id : y) {
    if (!Contains(y_id)) return false;
  }
  return true;
}

bool GTCellUnion::Intersects(const GTCellUnion& y) const {
  // TODO(ericv): A divide-and-conquer or alternating-skip-search
  // approach may be sigificantly faster in both the average and worst case.

  for (GTCellId y_id : y) {
    if (Intersects(y_id)) return true;
  }
  return false;
}

GTCellUnion GTCellUnion::Union(const GTCellUnion& y) const {
  vector<GTCellId> cell_ids;
  cell_ids.reserve(num_cells() + y.num_cells());
  cell_ids = cell_ids_;
  cell_ids.insert(cell_ids.end(), y.cell_ids_.begin(), y.cell_ids_.end());
  return GTCellUnion(std::move(cell_ids));
}

GTCellUnion GTCellUnion::Intersection(GTCellId id) const {
  GTCellUnion result;
  if (Contains(id)) {
    result.cell_ids_.push_back(id);
  } else {
    vector<GTCellId>::const_iterator i = std::lower_bound(
        cell_ids_.begin(), cell_ids_.end(), id.range_min());
    GTCellId id_max = id.range_max();
    while (i != cell_ids_.end() && *i <= id_max)
      result.cell_ids_.push_back(*i++);
  }
  S2_DCHECK(result.IsNormalized() || !IsNormalized());
  return result;
}

GTCellUnion GTCellUnion::Intersection(const GTCellUnion& y) const {
  GTCellUnion result;
  GetIntersection(cell_ids_, y.cell_ids_, &result.cell_ids_);
  // The output is normalized as long as at least one input is normalized.
  S2_DCHECK(result.IsNormalized() || (!IsNormalized() && !y.IsNormalized()));
  return result;
}

/*static*/ void GTCellUnion::GetIntersection(const vector<GTCellId>& x,
                                             const vector<GTCellId>& y,
                                             vector<GTCellId>* out) {
  S2_DCHECK_NE(out, &x);
  S2_DCHECK_NE(out, &y);
  S2_DCHECK(is_sorted(x.begin(), x.end()));
  S2_DCHECK(is_sorted(y.begin(), y.end()));

  // This is a fairly efficient calculation that uses binary search to skip
  // over sections of both input vectors.  It takes logarithmic time if all the
  // cells of "x" come before or after all the cells of "y" in GTCellId order.

  out->clear();
  vector<GTCellId>::const_iterator i = x.begin();
  vector<GTCellId>::const_iterator j = y.begin();
  while (i != x.end() && j != y.end()) {
    GTCellId imin = i->range_min();
    GTCellId jmin = j->range_min();
    if (imin > jmin) {
      // Either j->contains(*i) or the two cells are disjoint.
      if (*i <= j->range_max()) {
        out->push_back(*i++);
      } else {
        // Advance "j" to the first cell possibly contained by *i.
        j = std::lower_bound(j + 1, y.end(), imin);
        // The previous cell *(j-1) may now contain *i.
        if (*i <= (j - 1)->range_max()) --j;
      }
    } else if (jmin > imin) {
      // Identical to the code above with "i" and "j" reversed.
      if (*j <= i->range_max()) {
        out->push_back(*j++);
      } else {
        i = std::lower_bound(i + 1, x.end(), jmin);
        if (*j <= (i - 1)->range_max()) --i;
      }
    } else {
      // "i" and "j" have the same range_min(), so one contains the other.
      if (*i < *j)
        out->push_back(*i++);
      else
        out->push_back(*j++);
    }
  }
  // The output is generated in sorted order.
  S2_DCHECK(is_sorted(out->begin(), out->end()));
}

static void GetDifferenceInternal(GTCellId cell,
                                  const GTCellUnion& y,
                                  vector<GTCellId>* cell_ids) {
  // Add the difference between cell and y to cell_ids.
  // If they intersect but the difference is non-empty, divide and conquer.
  if (!y.Intersects(cell)) {
    cell_ids->push_back(cell);
  } else if (!y.Contains(cell)) {
    GTCellId child = cell.child_begin();
    for (int i = 0; ; ++i) {
      GetDifferenceInternal(child, y, cell_ids);
      if (i == 3) break;  // Avoid unnecessary next() computation.
      child = child.next();
    }
  }
}

GTCellUnion GTCellUnion::Difference(const GTCellUnion& y) const {
  // TODO(ericv): this is approximately O(N*log(N)), but could probably
  // use similar techniques as GetIntersection() to be more efficient.

  GTCellUnion result;
  for (GTCellId id : *this) {
    GetDifferenceInternal(id, y, &result.cell_ids_);
  }
  // The output is normalized as long as the first argument is normalized.
  S2_DCHECK(result.IsNormalized() || !IsNormalized());
  return result;
}

void GTCellUnion::Expand(int expand_level) {
  vector<GTCellId> output;
  uint64 level_lsb = GTCellId::lsb_for_level(expand_level);
  for (int i = num_cells(); --i >= 0; ) {
    GTCellId id = cell_id(i);
    if (id.lsb() < level_lsb) {
      id = id.parent(expand_level);
      // Optimization: skip over any cells contained by this one.  This is
      // especially important when very small regions are being expanded.
      while (i > 0 && id.contains(cell_id(i - 1))) --i;
    }
    output.push_back(id);
    id.AppendAllNeighbors(expand_level, &output);
  }
  Init(std::move(output));
}

void GTCellUnion::Expand(S1Angle min_radius, int max_level_diff) {
  int min_level = GTCellId::kMaxLevel;
  for (GTCellId id : *this) {
    min_level = min(min_level, id.level());
  }
  // Find the maximum level such that all cells are at least "min_radius" wide.
  int radius_level = GT::kMinWidth.GetLevelForMinValue(min_radius.radians());
  if (radius_level == 0 && min_radius.radians() > GT::kMinWidth.GetValue(0)) {
    // The requested expansion is greater than the width of a face cell.
    // The easiest way to handle this is to expand twice.
    Expand(0);
  }
  Expand(min(min_level + max_level_diff, radius_level));
}

uint64 GTCellUnion::LeafCellsCovered() const {
  uint64 num_leaves = 0;
  for (GTCellId id : *this) {
    const int inverted_level = GTCellId::kMaxLevel - id.level();
    num_leaves += (1ULL << (inverted_level << 1));
  }
  return num_leaves;
}

double GTCellUnion::AverageBasedArea() const {
  return GTCell::AverageArea(GTCellId::kMaxLevel) * LeafCellsCovered();
}

double GTCellUnion::ApproxArea() const {
  double area = 0;
  for (GTCellId id : *this) {
    area += GTCell(id).ApproxArea();
  }
  return area;
}

double GTCellUnion::ExactArea() const {
  double area = 0;
  for (GTCellId id : *this) {
    area += GTCell(id).ExactArea();
  }
  return area;
}

bool operator==(const GTCellUnion& x, const GTCellUnion& y) {
  return x.cell_ids() == y.cell_ids();
}

bool operator!=(const GTCellUnion& x, const GTCellUnion& y) {
  return x.cell_ids() != y.cell_ids();
}

bool GTCellUnion::Contains(const Cell& cell) const {
  return Contains(cell.id());
}

bool GTCellUnion::MayIntersect(const Cell& cell) const {
  return Intersects(cell.id());
}

void GTCellUnion::Encode(Encoder* const encoder) const {
  // Unsigned char for version number, and N+1 uint64's for N cell_ids
  // (1 for vector length, N for the ids).
  encoder->Ensure(sizeof(unsigned char) +
                  sizeof(uint64) * (1 + cell_ids_.size()));

  encoder->put8(kCurrentLosslessEncodingVersionNumber);
  encoder->put64(uint64{cell_ids_.size()});
  for (const GTCellId& cell_id : cell_ids_) {
    cell_id.Encode(encoder);
  }
}

bool GTCellUnion::Decode(Decoder* const decoder) {
  // Should contain at least version and vector length.
  if (decoder->avail() < sizeof(unsigned char) + sizeof(uint64)) return false;
  unsigned char version = decoder->get8();
  if (version > kCurrentLosslessEncodingVersionNumber) return false;

  uint64 num_cells = decoder->get64();
  if (num_cells > FLAGS_s2cell_union_decode_max_num_cells) {
    return false;
  }

  vector<GTCellId> temp_cell_ids(num_cells);
  for (int i = 0; i < num_cells; ++i) {
    if (!temp_cell_ids[i].Decode(decoder)) return false;
  }
  cell_ids_.swap(temp_cell_ids);
  return true;
}

bool GTCellUnion::Contains(const S2Point& p) const {
  return Contains(GTCellId(p));
}