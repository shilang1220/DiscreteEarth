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

// GT网格系统涉及四个坐标系统
// 经纬度          （LNG,LAT）       double
//                  -180.00 ≤ LNG ≤ +180.00，-90.00 ≤ LAT ≤ +90.00
// 扩展坐标系      （U,V）           double
//                  -512.00 ≤ U ≤ +512.00，-512.00 ≤ L ≤ +512.00
// 球面坐标系      （X,Y,Z）         double
//                  X**2 + Y**2 + Z**2 =1
// 网格坐标系      （I，J）          unsigned long long
//                  0 ≤ I ≤ 2**kMaxCellLevel，0 ≤ J ≤ 2**kMaxCellLevel


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


    //扩展坐标系（U，V）与网格坐标系（I，J）之间的转换函数
    double IJtoUV(uint32 IJ);
    uint32 UVtoIJ(double UV);

    //经纬度坐标系（LNG，LAT）与网格坐标系（I，J）之间的转换函数
    bool IJtoLL(const uint32 I, const uint32 J,R2Point* ll);  //当（I,J）不对应经纬度时，返回false
    bool LLtoIJ(const R2Point ll,uint32* I,uint32* J);        //当ll经纬度超届时，返回false

    //球面坐标系（X,Y,Z）与扩展坐标系（U,V）之间的转换函数
    bool XYZtoUV(const S2Point& p, double* pu, double* pv);
    R2Point XYZtoUV(const S2Point& p);
    S2Point UVtoXYZ(const double u,const double v);

}


#endif //DISCRETEEARTH_GTCOORDS_H
