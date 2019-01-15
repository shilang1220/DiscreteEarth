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
// The following are various constants that describe the shapes and sizes of
// GTCells (see s2coords.h and s2cell_id.h).  They are useful for deciding
// which cell level to use in order to satisfy a given condition (e.g. that
// cell vertices must be no further than "x" apart).  All of the raw constants
// are differential quantities; you can use the GetValue(level) method to
// compute the corresponding length or area on the unit sphere for cells at a
// given level.  The minimum and maximum bounds are valid for cells at all
// levels, but they may be somewhat conservative for very large cells
// (e.g. face cells).

#ifndef GT_GTMETRICS_H_
#define GT_GTMETRICS_H_

#include <algorithm>
#include <cmath>

#include "core/gt_coords.h"
#include "util/math/mathutil.h"


namespace GT {
    //获取每个尺度上的经纬度误差范围
    //level 【1..32】，返回值单位为度
    double GetGridInterval(int level);

    double GetGridAverageErrorX(int level);
    double GetGridAverageErrorY(int level);
    double GetGridAverageErrorZ(int level);
    //获得某一层级的最大距离误差（原点处的网格对角线长度）
    double GetGridMaxDistanceError(int level);

//    double GetGridExactErrorX(uint64 id);
    double GetGridExactDistanceErrorRMS(uint64 id);
    double GetGridExactAreaErrorRMS(uint64 id);

}  // namespace GT

#endif  // GT_GTMETRICS_H_
