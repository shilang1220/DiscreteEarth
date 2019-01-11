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
//
// All of the values below were obtained by a combination of hand analysis and
// Mathematica.  In general, S2_TAN_PROJECTION produces the most uniform
// shapes and sizes of cells, S2_LINEAR_PROJECTION is considerably worse, and
// S2_QUADRATIC_PROJECTION is somewhere in between (but generally closer to
// the tangent projection than the linear one).
//
// Note that S2_LINEAR_PROJECTION can be useful for analysis even when another
// projection is being used, since it allows many cell metrics to be bounded
// in terms of (u,v) coordinates rather than (s,t) coordinates.  (With the
// linear projection, u = 2 * s - 1 and similarly for v.)  Similarly,
// S2_TAN_PROJECTION allows cell metrics to be bounded in terms of (u,v)
// coordinate changes when they are measured as distances on the unit sphere.

#include <core/gt_metrics.h>

#include "core/gt_metrics.h"

namespace GT {

    double GetGridInterval(int level) {

        double interval, base;

        if (level >= 1 && level <= 9) {
            base = 1;
            interval = base * (1 << (9 - level));
        }

        if (level >= 10 && level <= 15) {
            base = GT::kMin2Degree;
            interval = base * (1 << (15 - level));
        }
        if (level >= 16 && level <= 32) {
            base = GT::kSec2Degree;
            interval = base * (1 << (32 - level));
        }
        return interval;
    }

    double GetGridAverageErrorX(int level) {
        return 0;
    }

    double GetGridAverageErrorY(int level) {
        return 0;
    }

    double GetGridAverageErrorZ(int level) {
        return 0;
    }

    double GetGridExactDistanceErrorRMS(uint64 id) {
        return 0;
    }

    double GetGridExactAreaErrorRMS(uint64 id) {
        return 0;
    }

}  // namespace GT
