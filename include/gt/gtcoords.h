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

#include "base/integral_types.h"
#include "s2/s2point.h"
#include "gt/gtcoords.h"
#include "exports.h"

// GT网格系统涉及四个坐标系统
// 经纬度          （LNG,LAT）       double
//                  -180.00 ≤ LNG ≤ +180.00，-90.00 ≤ LAT ≤ +90.00
// 扩展坐标系      （U,V）           double
//                  -512.00 ≤ U ≤ +512.00，-512.00 ≤ L ≤ +512.00
// 球面坐标系      （X,Y,Z）         double
//                  X**2 + Y**2 + Z**2 =1
// 网格坐标系      （I，J）          unsigned long(uint32)
//                  0 ≤ I ≤ 2**32，0 ≤ J ≤ 2**32
//Z序一维网格编码     GTCellID       unsigned long long(uint64),仅使用前62bit（31级），末尾2bit预留作截止位（第32级弃用）
//                  0X1<<1(0B 0000 0000 .... 0010) ≤ GTCellID ≤ 0XFFFFFFFFFFFFFFE(0B 1111 1111 .... 1110)
//                  0X0 特殊编码，用于标识全球
//                  0XFF FF FF FF FF FF FF FF，特殊编码，用途待定

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

    const double kMin2Degree =  1/(double)60.00;
    const double kSec2Degree =  2048/(double) 3600.00;

    //球面坐标系（X，Y,Z）与经纬度坐标系（LNG，LAT）之间的转换函数
     bool XYZtoLL (const S2Point &p, double *pU, double *pV) ;
     bool LLtoXYZ (const double U, const double V, S2Point *pPnt) ;


    //扩展坐标系（U，V）与网格坐标系（I，J）之间的转换函数
     bool IJtoUV(const uint32 I,const uint32 J, double* pU, double* pV);
     bool UVtoIJ(const double U,const double V,uint32* pI,uint32* pJ);

    //经纬度坐标系（LNG，LAT）与网格坐标系（I，J）之间的转换函数
     bool IJtoLL(const uint32 I, const uint32 J,double* pLng,double* pLat);  //当（I,J）不对应经纬度时，返回false
     bool LLtoIJ(const double Lng,const double Lat,uint32* pI,uint32* pJ);        //当ll经纬度超届时，返回false

    //球面坐标系（X,Y,Z）与扩展坐标系（U,V）之间的转换函数
     bool XYZtoUV(const S2Point& p, double* pU, double* pV);     //当（X,Y,Z）为非球面点时，返回false
     bool UVtoXYZ(const double U,const double V, S2Point* pPnt);  //当（U,V）不对应实际空间时，返回false

    //球面坐标系（X,Y,Z）与网格坐标系（I,J）之间的转换函数
     bool XYZtoIJ(const S2Point& p, uint32* pI, uint32* pJ);     //当（X,Y,Z）为非球面点时，返回false
     bool IJtoXYZ(const uint32 I,const uint32 J,S2Point* pPnt);  //当（I,J）不对应实际空间时，返回false

    //Z序编码与网格坐标系（I,J）之间的转换函数
     bool IJtoCellID(const uint32 I, const uint32 J, uint64* pCellID);
     bool CellIDtoIJ(const uint64 CellID, uint32* pI, uint32* pJ);

    //Z序编码与经纬度坐标系（Lng,Lat）之间的转换函数
     bool LLtoCellID(const double Lng, const double Lat, uint64* pCellID);
     bool CellIDtoLL(const uint64 CellID, double* pLng, double* pLat);

    //Z序编码与球面坐标系（X,Y,Z）之间的转换函数
     bool XYZtoCellID(const S2Point pnt, uint64* pCellID);
     bool CellIDtoXYZ(const uint64 CellID, S2Point* pPnt);

}


#endif //DISCRETEEARTH_GTCOORDS_H
