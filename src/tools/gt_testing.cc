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

#include "tools/gt_testing.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <utility>
#include <vector>
#include <random>

#include "base/commandlineflags.h"
#include "base/integral_types.h"
#include "base/logging.h"
#include "strings/serialize.h"
#include "third_party/absl/memory/memory.h"
#include "third_party/absl/strings/str_split.h"
#include "util/math/matrix3x3.h"

#include "s2/r1interval.h"
#include "s2/s1angle.h"
#include "s2/s1interval.h"
#include "s2/s2latlng.h"
#include "s2/s2pointutil.h"
#include "core/region.h"
#include "core/gt_cap.h"
#include "core/gt_cell.h"
#include "core/gt_cell_union.h"
#include "core/gt_latlng_rect.h"
#include "gt/gt_loop.h"
#include "gt/gt_polygon.h"
#include "gt/gt_polyline.h"

//#include "encode/s2text_format.h"

using absl::make_unique;
using std::max;
using std::unique_ptr;
using std::vector;


DEFINE_int32(s2_random_seed, 1,
             "Seed value that can be passed to GTTesting::rnd.Reset()");

const double GTTesting::kEarthRadiusKm = 6371.01;

GTTesting::Random::Random() {
  // Unfortunately we can't use FLAGS_s2_random_seed here, because the default
  // GTTesting::Random instance is initialized before command-line flags have
  // been parsed.
   srand(1);
}

void GTTesting::Random::Reset(int seed) {
  srand(seed);
}

// Return a 64-bit unsigned integer whose lowest "num_bits" are random, and
// whose other bits are zero.
inline uint64 GetBits(int num_bits) {
  S2_DCHECK_GE(num_bits, 0);
  S2_DCHECK_LE(num_bits, 64);

  // This code uses random(), which returns an integer in the range
  // from 0 to (2^31)-1 inclusive (i.e. all of the lower 31 bits are
  // in play, and the 32nd bit and higher are 0) regardless of whether
  // its return type (long) is larger than 32 bits.  See
  //
  // www.gnu.org/software/libc/manual/html_node/BSD-Random.html#BSD-Random
  //
  // Note that at some point the manual page in linux claimed that the range
  // is 0 to RAND_MAX as defined in stdlib.h.  RAND_MAX however is part only
  // of the ISO rand() interface.  At least as of glibc-2.21, rand() is
  // simply an alias for random().  On other systems, rand() may differ,
  // but random() should always adhere to the behavior specified in BSD.
  static const int RAND_BITS = 31;
  static std::uniform_int_distribution<uint32> values;
  static std::default_random_engine rng; // Create random number generator

  uint64 result = 0;
  for (int bits = 0; bits < num_bits; bits += RAND_BITS) {
    uint32 rn{values(rng)};
    result = (result << RAND_BITS) + rn;
  }
  if (num_bits < 64) {  // Not legal to shift by full bitwidth of type
    result &= ((1ULL << num_bits) - 1);
  }
  return result;
}

uint64 GTTesting::Random::Rand64() {
    static const int RAND_BITS = 31;
    static std::uniform_int_distribution<uint64> values;
    static std::default_random_engine rng; // Create random number generator

    return uint64(values(rng));
}

uint32 GTTesting::Random::Rand32() {
    static const int RAND_BITS = 31;
    static std::uniform_int_distribution<uint32> values;
    static std::default_random_engine rng; // Create random number generator

    return uint32(values(rng));
}

double GTTesting::Random::RandDouble() {
  static std::uniform_real_distribution<double> values {0.0, 1.0};
//  std::random_device rd; // Non-de terrains tic seed source
  static std::default_random_engine rng; // Create random number generator for(size_t i {}; i<8; ++i)

  return(double{values(rng)});
//
//  const int NUM_BITS = 53;
//  return ldexp(GetBits(NUM_BITS), -NUM_BITS);
}

int32 GTTesting::Random::Uniform(int32 n) {
  S2_DCHECK_GT(n, 0);
  double dbl = RandDouble();

  return static_cast<uint32>(dbl * n);
}

double GTTesting::Random::UniformDouble(double min, double limit) {
  S2_DCHECK_LT(min, limit);
  return min + RandDouble() * (limit - min);
}

bool GTTesting::Random::OneIn(int32 n) {
  return Uniform(n) == 0;
}

int32 GTTesting::Random::Skewed(int max_log) {
  S2_DCHECK_GE(max_log, 0);
  S2_DCHECK_LE(max_log, 31);
  int32 base = Uniform(max_log + 1);
  return GetBits(31) & ((1U << base) - 1);
}

GTTesting::Random GTTesting::rnd;

void GTTesting::AppendLoopVertices(const GTLoop& loop,
                                   vector<S2Point>* vertices) {
//  int n = loop.num_vertices();
//  const S2Point* base = &loop.vertex(0);
//  S2_DCHECK_EQ(&loop.vertex(n - 1), base + n - 1);
//  vertices->insert(vertices->end(), base, base + n);
}

vector<S2Point> GTTesting::MakeRegularPoints(const S2Point& center,
                                             S1Angle radius,
                                             int num_vertices) {
//  unique_ptr<GTLoop> loop(
//      GTLoop::MakeRegularLoop(center, radius, num_vertices));
  vector<S2Point> points;
//  for (int i = 0; i < loop->num_vertices(); i++) {
//    points.push_back(loop->vertex(i));
//  }
  return points;
}

S1Angle GTTesting::MetersToAngle(double meters) {
  return KmToAngle(0.001 * meters);
}

S1Angle GTTesting::KmToAngle(double km) {
  return S1Angle::Radians(km / kEarthRadiusKm);
}

double GTTesting::AreaToMeters2(double steradians) {
  return 1e6 * AreaToKm2(steradians);
}

double GTTesting::AreaToKm2(double steradians) {
  return steradians * kEarthRadiusKm * kEarthRadiusKm;
}

// The overloaded Dump() function is for use within a debugger.
void Dump(const S2Point& p) {
//  std::cout << "S2Point: " << s2textformat::ToString(p) << std::endl;
}

void Dump(const GTLoop& loop) {
//  std::cout << "GTPolygon: " << s2textformat::ToString(loop) << std::endl;
}

void Dump(const GTPolyline& polyline) {
//  std::cout << "GTPolyline: " << s2textformat::ToString(polyline) << std::endl;
}

void Dump(const GTPolygon& polygon) {
//  std::cout << "GTPolygon: " << s2textformat::ToString(polygon) << std::endl;
}

int GTTesting::RandomLevel() {
  int32 ff = rnd.Uniform(31);
  int level = ff+1;
  return level;
}

S2Point GTTesting::RandomPoint() {
  // The order of evaluation of function arguments is unspecified,
  // so we may not just call S2Point with three RandDouble-based args.
  // Use temporaries to induce sequence points between calls.
  double x = rnd.UniformDouble(-1, 1);
  double y = rnd.UniformDouble(-1, 1);
  double z = rnd.UniformDouble(-1, 1);
  return S2Point(x, y, z).Normalize();
}

S2LatLng GTTesting::RandomLatLng()  {
  // The order of evaluation of function arguments is unspecified,
  // so we may not just call S2Point with three RandDouble-based args.
  // Use temporaries to induce sequence points between calls.
  double lng = rnd.UniformDouble(-1, 1);
  double lat = rnd.UniformDouble(-1, 1);

  lng = lng * M_PI;
  lat = lat * M_2_PI;

  return S2LatLng::FromRadians(lat,lng);
}

void GTTesting::RandomIJ(uint32* pI,uint32* pJ){
  RandomIJ(pI,pJ,rnd.Uniform(GT::kMaxCellLevel + 1));
}

void GTTesting::RandomIJ(uint32* pI,uint32* pJ,int level){
  uint32 mask;

  mask = uint32{1} << (GT::kMaxCellLevel - level + 1);
  mask = ~mask + 1;

  *pI = rnd.Rand32() & mask;
  *pJ = rnd.Rand32() & mask;
}

void GTTesting::GetRandomFrame(Vector3_d* x, Vector3_d* y, Vector3_d* z) {
  *z = RandomPoint();
  GetRandomFrameAt(*z, x, y);
}

Matrix3x3_d GTTesting::GetRandomFrame() {
  return GetRandomFrameAt(RandomPoint());
}

void GTTesting::GetRandomFrameAt(const S2Point& z, S2Point* x, S2Point *y) {
  *x = z.CrossProd(RandomPoint()).Normalize();
  *y = z.CrossProd(*x).Normalize();
}

Matrix3x3_d GTTesting::GetRandomFrameAt(const S2Point& z) {
  S2Point x, y;
  GetRandomFrameAt(z, &x, &y);
  return Matrix3x3_d::FromCols(x, y, z);
}

GTCellId GTTesting::GetRandomCellId(int level) {
  int face = rnd.Uniform(GTCellId::kNumFaces);
  uint64 pos = rnd.Rand64() & ((uint64{1} << GTCellId::kPosBits) - 1);
  return GTCellId::FromFacePosLevel(face, pos, level);
}

GTCellId GTTesting::GetRandomCellId() {
  int level = RandomLevel();
  return GetRandomCellId(level);
}

GTCap GTTesting::GetRandomCap(double min_area, double max_area) {
  double cap_area = max_area * pow(min_area / max_area, rnd.RandDouble());
  S2_DCHECK_GE(cap_area, min_area);
  S2_DCHECK_LE(cap_area, max_area);

  // The surface area of a cap is 2*Pi times its height.
  return GTCap::FromCenterArea(RandomPoint(), cap_area);
}

void GTTesting::ConcentricLoopsPolygon(const S2Point& center,
                                       int num_loops,
                                       int num_vertices_per_loop,
                                       GTPolygon* polygon) {
//  Matrix3x3_d m;
//  S2::GetFrame(center, &m);
//  vector<unique_ptr<GTLoop>> loops;
//  for (int li = 0; li < num_loops; ++li) {
//    vector<S2Point> vertices;
//    double radius = 0.005 * (li + 1) / num_loops;
//    double radian_step = 2 * M_PI / num_vertices_per_loop;
//    for (int vi = 0; vi < num_vertices_per_loop; ++vi) {
//      double angle = vi * radian_step;
//      S2Point p(radius * cos(angle), radius * sin(angle), 1);
//      vertices.push_back(S2::FromFrame(m, p.Normalize()));
//    }
//    loops.push_back(make_unique<GTLoop>(vertices));
//  }
//  polygon->InitNested(std::move(loops));
}

S2Point GTTesting::SamplePoint(const GTCap& cap) {
  // We consider the cap axis to be the "z" axis.  We choose two other axes to
  // complete the coordinate frame.

  Matrix3x3_d m;
  S2::GetFrame(cap.center(), &m);

  // The surface area of a spherical cap is directly proportional to its
  // height.  First we choose a random height, and then we choose a random
  // point along the circle at that height.

  double h = rnd.RandDouble() * cap.height();
  double theta = 2 * M_PI * rnd.RandDouble();
  double r = sqrt(h * (2 - h));  // Radius of circle.

  // The result should already be very close to unit-length, but we might as
  // well make it accurate as possible.
  return S2::FromFrame(m, S2Point(cos(theta) * r, sin(theta) * r, 1 - h))
         .Normalize();
}

S2Point GTTesting::SamplePoint(const GTLatLngRect& rect) {
  // First choose a latitude uniformly with respect to area on the sphere.
  double sin_lo = sin(rect.lat().lo());
  double sin_hi = sin(rect.lat().hi());
  double lat = asin(rnd.UniformDouble(sin_lo, sin_hi));

  // Now choose longitude uniformly within the given range.
  double lng = rect.lng().lo() + rnd.RandDouble() * rect.lng().GetLength();
  return S2LatLng::FromRadians(lat, lng).Normalized().ToPoint();
}

void GTTesting::CheckCovering(const Region& region,
                              const GTCellUnion& covering,
                              bool check_tight, GTCellId id) {
  if (!id.is_valid()) {
    for (int face = 0; face < 6; ++face) {
      CheckCovering(region, covering, check_tight, GTCellId::FromFace(face));
    }
    return;
  }

  if (!region.MayIntersect(GTCell(id))) {
    // If region does not intersect id, then neither should the covering.
    if (check_tight) S2_CHECK(!covering.Intersects(id));

  } else if (!covering.Contains(id)) {
    // The region may intersect id, but we can't assert that the covering
    // intersects id because we may discover that the region does not actually
    // intersect upon further subdivision.  (MayIntersect is not exact.)
    S2_CHECK(!region.Contains(GTCell(id)));
    S2_CHECK(!id.is_leaf());
    GTCellId end = id.child_end();
    GTCellId child;
    for (child = id.child_begin(); child != end; child = child.next()) {
      CheckCovering(region, covering, check_tight, child);
    }
  }
}

GTTesting::Fractal::Fractal()
    : max_level_(-1), min_level_arg_(-1), min_level_(-1),
      dimension_(log(4)/log(3)), /* standard Koch curve */
      edge_fraction_(0), offset_fraction_(0) {
  ComputeOffsets();
}

void GTTesting::Fractal::set_max_level(int max_level) {
  S2_DCHECK_GE(max_level, 0);
  max_level_ = max_level;
  ComputeMinLevel();
}

void GTTesting::Fractal::set_min_level(int min_level_arg) {
  S2_DCHECK_GE(min_level_arg, -1);
  min_level_arg_ = min_level_arg;
  ComputeMinLevel();
}

void GTTesting::Fractal::ComputeMinLevel() {
  if (min_level_arg_ >= 0 && min_level_arg_ <= max_level_) {
    min_level_ = min_level_arg_;
  } else {
    min_level_ = max_level_;
  }
}

void GTTesting::Fractal::set_fractal_dimension(double dimension) {
  S2_DCHECK_GE(dimension, 1.0);
  S2_DCHECK_LT(dimension, 2.0);
  dimension_ = dimension;
  ComputeOffsets();
}

void GTTesting::Fractal::ComputeOffsets() {
  edge_fraction_ = pow(4.0, -1.0 / dimension_);
  offset_fraction_ = sqrt(edge_fraction_ - 0.25);
}

void GTTesting::Fractal::SetLevelForApproxMinEdges(int min_edges) {
  // Map values in the range [3*(4**n)/2, 3*(4**n)*2) to level n.
  set_min_level(round(0.5 * log2(min_edges / 3)));
}

void GTTesting::Fractal::SetLevelForApproxMaxEdges(int max_edges) {
  // Map values in the range [3*(4**n)/2, 3*(4**n)*2) to level n.
  set_max_level(round(0.5 * log2(max_edges / 3)));
}

double GTTesting::Fractal::min_radius_factor() const {
  // The minimum radius is attained at one of the vertices created by the
  // first subdivision step as long as the dimension is not too small (at
  // least kMinDimensionForMinRadiusAtLevel1, see below).  Otherwise we fall
  // back on the incircle radius of the original triangle, which is always a
  // lower bound (and is attained when dimension = 1).
  //
  // The value below was obtained by letting AE be an original triangle edge,
  // letting ABCDE be the corresponding polyline after one subdivision step,
  // and then letting BC be tangent to the inscribed circle at the center of
  // the fractal O.  This gives rise to a pair of similar triangles whose edge
  // length ratios can be used to solve for the corresponding "edge fraction".
  // This method is slightly conservative because it is computed using planar
  // rather than spherical geometry.  The value below is equal to
  // -log(4)/log((2 + cbrt(2) - cbrt(4))/6).
  const double kMinDimensionForMinRadiusAtLevel1 = 1.0852230903040407;
  if (dimension_ >= kMinDimensionForMinRadiusAtLevel1) {
    return sqrt(1 + 3 * edge_fraction_ * (edge_fraction_ - 1));
  }
  return 0.5;
}

double GTTesting::Fractal::max_radius_factor() const {
  // The maximum radius is always attained at either an original triangle
  // vertex or at a middle vertex from the first subdivision step.
  return max(1.0, offset_fraction_ * sqrt(3) + 0.5);
}

void GTTesting::Fractal::GetR2Vertices(vector<R2Point>* vertices) const {
  // The Koch "snowflake" consists of three Koch curves whose initial edges
  // form an equilateral triangle.
  R2Point v0(1.0, 0.0);
  R2Point v1(-0.5, sqrt(3)/2);
  R2Point v2(-0.5, -sqrt(3)/2);
  GetR2VerticesHelper(v0, v1, 0, vertices);
  GetR2VerticesHelper(v1, v2, 0, vertices);
  GetR2VerticesHelper(v2, v0, 0, vertices);
}

// Given the two endpoints (v0,v4) of an edge, recursively subdivide the edge
// to the desired level, and insert all vertices of the resulting curve up to
// but not including the endpoint "v4".
void GTTesting::Fractal::GetR2VerticesHelper(const R2Point& v0,
                                             const R2Point& v4, int level,
                                             vector<R2Point>* vertices) const {
  if (level >= min_level_ && GTTesting::rnd.OneIn(max_level_ - level + 1)) {
    // Stop subdivision at this level.
    vertices->push_back(v0);
    return;
  }
  // Otherwise compute the intermediate vertices v1, v2, and v3.
  Vector2_d dir = v4 - v0;
  R2Point v1 = v0 + edge_fraction_ * dir;
  R2Point v2 = 0.5 * (v0 + v4) - offset_fraction_ * dir.Ortho();
  R2Point v3 = v4 - edge_fraction_ * dir;

  // And recurse on the four sub-edges.
  GetR2VerticesHelper(v0, v1, level+1, vertices);
  GetR2VerticesHelper(v1, v2, level+1, vertices);
  GetR2VerticesHelper(v2, v3, level+1, vertices);
  GetR2VerticesHelper(v3, v4, level+1, vertices);
}

//std::unique_ptr<GTLoop> GTTesting::Fractal::MakeLoop(
//    const Matrix3x3_d& frame,
//    S1Angle nominal_radius) const {
//  vector<R2Point> r2vertices;
//  GetR2Vertices(&r2vertices);
//  vector<S2Point> vertices;
//  double r = nominal_radius.radians();
//  for (const R2Point& v : r2vertices) {
//    S2Point p(v[0] * r, v[1] * r, 1);
//    vertices.push_back(S2::FromFrame(frame, p).Normalize());
//  }
//  return make_unique<GTLoop>(vertices);
//}
