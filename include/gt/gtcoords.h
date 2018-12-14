//
// Created by pgl on 2018/12/14.
//


// Copyright 2018 Peking University. All Rights Reserved.
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
// Author: pgl@pku.edu.cn (PU Guoliang)

#ifndef DISCRETEEARTH_GTCOORDS_H
#define DISCRETEEARTH_GTCOORDS_H


namespace GT{
// 网格最大层级
// This is the number of levels needed to specify a leaf cell.  This
// constant is defined here so that the S2::Metric class and the conversion
// functions below can be implemented without including s2cell_id.h.  Please
// see s2cell_id.h for other useful constants and conversion functions.
    const int kMaxCellLevel = 31;

// 叶子节点（I，J）索引值的最大值
// The maximum index of a valid leaf cell plus one.  The range of valid leaf
// cell indices is [0..kLimitIJ-1].
    const uint64 kLimitIJ = 1 << kMaxCellLevel;  // == S2CellId::kMaxSize

// （Si，Ti）坐标的最大值，为能够有效表示叶子节点的边缘和中心，通常在叶子节点基础上，再四分一级
// The maximum value of an si- or ti-coordinate.  The range of valid (si,ti)
// values is [0..kMaxSiTi].
    const uint64 kMaxSiTi = 1U << (kMaxCellLevel + 1);
}


#endif //DISCRETEEARTH_GTCOORDS_H
