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

#include "_fp_contract_off.h"
#include "exports.h"

#include <cfloat>
#include "base/integral_types.h"
#include "s2/s2point.h"
#include "gt_coords.h"

// GT网格系统涉及四个坐标系统和一个编码
// 经纬度          （LNG,LAT）       double
//                  -180.00 ≤ LNG ≤ +180.00，-90.00 ≤ LAT ≤ +90.00
// 扩展坐标系      （U,V）           double  在实际编程中，暂时不做考虑
//                  -512.00 ≤ U ≤ +512.00，-512.00 ≤ L ≤ +512.00
// 球面坐标系      （X,Y,Z）         double   采用单位球面表示（球半径R = 1）
//                  X**2 + Y**2 + Z**2 =1
// 网格坐标系      （I，J）          unsigned long(uint32)  网格坐标采用32层级以便于求解第31级质心或边的坐标
//                  0 ≤ I ≤ 2**32，0 ≤ J ≤ 2**32
// Z序一维网格编码     GTCellID       unsigned long long(uint64),仅使用前62bit（31级），末尾2bit预留作截止位（第32级弃用）
//                  0X1<<1(0B 0000 0000 .... 0010) ≤ GTCellID ≤ 0XFFFFFFFFFFFFFFE(0B 1111 1111 .... 1110)
//                  0X1000000000000000 特殊编码，用于标识全球
//                  0X0000000000000000 特殊编码，用于指示不合法编码
//                  0XFF FF FF FF FF FF FF FF，特殊编码，用途待定

namespace GT {
// 网格最大层级
// This is the number of levels needed to specify a leaf cell.  This
// constant is defined here so that the s2::Metric class and the conversion
// functions below can be implemented without including s2cell_id.h.  Please
// see s2cell_id.h for other useful constants and conversion functions.
    const uint8 kMaxCellLevel = 31;

// 网格坐标系（I，J）的最大值，注意：I，J划分到第32级，占满32个bit
// 主要用于方便计算第31级的质心等参数，带来的问题是通过编码无法获得层级信息
// 需要用户自己判断或通过层级参数指定
// The maximum index of a valid leaf cell plus one.  The range of valid leaf
// cell indices is [0..kLimitIJ].
    const uint32 kLimitIJ = 0XFFFFFFFF;              // I,J < 2**32

    const double kMin2Degree = 1 / 60.00;              //分向度的转换常数
    const double kSec2Degree = 1 / (2048.00 * 3600.00);  //1/2048秒向度的转换常数
    const double kMaxError = 1000 * DBL_EPSILON;  //最大可辨识误差定义为第32级网格的1/2，单位为度



    /////////////////////////////////////////////////////////////////////
    //经纬度坐标系（LNG，LAT）与网格坐标系（I，J）之间的转换函数
    /////////////////////////////////////////////////////////////////////

    //网格坐标向经纬度坐标的转换函数，求网格定位点的经纬度
    //当求某个网格的质心经纬度时，求其第3个子网格的定位点经纬度即可
    //获得<I,J>所对应的叶子网格（第32层）的定位点经纬度
    // 注意： 程序强行取I,J的前32个bits做处理，由用户对IJ的合法性进行判断
    // 注意： 由于60进制向64进制的扩充，当<I,J>不对应实际经纬度时，返回false,
    GT_API bool IJtoLL(const uint32 I, const uint32 J, double *pLng, double *pLat);
    //获得<I,J>的第level层父网格的定位点经纬度
    //注意： 程序强行取I,J的前level个bits做处理，由用户对IJ的合法性进行判断，level<=32
    //注意： 由于60进制向64进制的扩充，当<I,J>不对应实际经纬度时，返回false,
    GT_API bool IJtoLL(const uint32 I, const uint32 J, double *pLng, double *pLat, int level);
    //经纬度坐标向32级网格坐标的转换函数
    //注意： 当经纬度超出阈值范围时，返回false
    GT_API bool LLtoIJ(const double Lng, const double Lat, uint32 *pI, uint32 *pJ);
    //经纬度坐标向指定层级网格坐标的转换函数，level<=32
    //注意： 当经纬度超出阈值范围时，返回false
    GT_API bool LLtoIJ(const double Lng, const double Lat, uint32 *pI, uint32 *pJ, int level);

    /////////////////////////////////////////////////////////////////////
    //球面坐标系（X，Y,Z）与经纬度坐标系（LNG，LAT）之间的转换函数
    /////////////////////////////////////////////////////////////////////

    //球面坐标向经纬度坐标的转换函数
    //注意：XYZ可以不是单位球面上的点，返回的经纬度时XYZ矢量与单位球面交点的经纬度
    GT_API bool XYZtoLL(const S2Point &p, double *pU, double *pV);
    //经纬度坐标向球面坐标的转换函数
    //注意： XYZ返回的为单位球面上的点坐标
    GT_API bool LLtoXYZ(const double U, const double V, S2Point *pPnt);

    /////////////////////////////////////////////////////////////////////
    //球面坐标系（X,Y,Z）与网格坐标系（I,J）之间的转换函数
    /////////////////////////////////////////////////////////////////////

    //网格坐标向球面坐标的转换函数，求网格定位点的球面坐标
    //当求某个网格的质心球面坐标时，求其第3个子网格的定位点球面坐标即可
    //获得<I,J>所对应的叶子网格（第32层）的球面坐标
    //注意： 程序强行取I,J的前32个bits做处理，由用户对IJ的合法性进行判断
    //注意： XYZ返回的为单位球面上的点坐标
    //注意： 当（I,J）不对应实际球面坐标空间时，返回false
    GT_API bool IJtoXYZ(const uint32 I, const uint32 J, S2Point *pPnt);
    //获得<I,J>的第level层父网格的定位点球面坐标
    // 注意： 程序强行取I,J的前level个bits做处理，由用户对IJ的合法性进行判断,level<=32
    // 注意： XYZ返回的为单位球面上的点坐标
    // 注意： 当（I,J）不对应实际球面坐标空间时，返回false
    GT_API bool IJtoXYZ(const uint32 I, const uint32 J, S2Point *pPnt, int level);
    //球面坐标向第32级网格坐标的转换函数
    //注意：XYZ可以不是单位球面上的点，返回的经纬度是XYZ矢量与单位球面交点所在的网格I,J
    GT_API bool XYZtoIJ(const S2Point &p, uint32 *pI, uint32 *pJ);
    //球面坐标向指定层级网格坐标的转换函数，level<=32
    //注意：XYZ可以不是单位球面上的点，返回的经纬度是XYZ矢量与单位球面交点所在的网格I,J
    GT_API bool XYZtoIJ(const S2Point &p, uint32 *pI, uint32 *pJ, int level);

    /////////////////////////////////////////////////////////////////////
    //Z序编码与网格坐标系（I,J）之间的转换函数
    /////////////////////////////////////////////////////////////////////

    // 网格坐标系（I，J）向叶子网格（第31级）编码的转换
    // 注意： 网格编码只编到31级，当I，J为32级时，略去最后1个bit
    bool IJtoCellID(const uint32 I, const uint32 J, uint64 *pCellID);

    // 网格坐标系（I，J）向指定层级网格编码的转换
    // 注意： 程序强行取I,J的前level个bits做处理，由用户对IJ的合法性进行判断,level<=31
    bool IJtoCellID(const uint32 I, const uint32 J, uint64 *pCellID, int level);

    //网格ID向网格坐标系（I，J）的转换函数,level<=31
    bool CellIDtoIJ(const uint64 CellID, uint32 *pI, uint32 *pJ, int *level);

    /////////////////////////////////////////////////////////////////////
    //Z序编码与经纬度坐标系（Lng,Lat）之间的转换函数
    /////////////////////////////////////////////////////////////////////
    //网格编码向经纬度的转换函数
    GT_API bool CellIDtoLL(const uint64 CellID, double *pLng, double *pLat, int *level);
    //经纬度向叶子网格ID（第31级）的转换函数
    GT_API  bool LLtoCellID(const double Lng, const double Lat, uint64 *pCellID);
    //经纬度向指定层级网格ID的转换函数，level<=31
    GT_API  bool LLtoCellID(const double Lng, const double Lat, uint64 *pCellID, int level);

    /////////////////////////////////////////////////////////////////////
    //Z序编码与球面坐标系（X,Y,Z）之间的转换函数
    /////////////////////////////////////////////////////////////////////
    //网格编码向球面坐标的转换函数
    GT_API bool CellIDtoXYZ(const uint64 CellID, S2Point *pPnt, int *level);
    //球面坐标向叶子网格ID（第31级）的转换函数
    GT_API bool XYZtoCellID(const S2Point pnt, uint64 *pCellID);
    //球面坐标向指定层级网格ID的转换函数，level<=31
    GT_API bool XYZtoCellID(const S2Point pnt, uint64 *pCellID, int level);

}


#endif //DISCRETEEARTH_GTCOORDS_H
