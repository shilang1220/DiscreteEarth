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

#include "core/gt_coords.h"

#include <cmath>
#include <cfloat>
#include <core/gt_coords.h>


#include "base/logging.h"
#include "util/math/mathutil.h"
#include "util/bits/bits.h"
#include "util/bits/bit-interleave.h"
#include "s2/s2latlng.h"

////////////////////////////////////////////////////////
//经纬度坐标系（LNG，LAT）与网格坐标系（I，J）之间的转换函数
///////////////////////////////////////////////////////
bool GT::IJtoLL(const uint32 I, const uint32 J, double *pLng, double *pLat) {
    // uint32 I_=I,J_=J;
    //边界检查
    unsigned int int_binary2code_B = J, int_binary2code_L = I;
    unsigned int degreeLat, degreeLng, minuteLat, minuteLng, secondLat, secondLng;

    int_binary2code_B = J & 0X7FFFFFFF;
    int_binary2code_L = I & 0X7FFFFFFF;

    //整度
    degreeLat = (int) (int_binary2code_B >> 23);                 // 9bit度
    degreeLng = (int) (int_binary2code_L >> 23);                 // 9bit度

    if (degreeLat > 90 || degreeLng > 180) return false;

    //不得大于180度
    //整分
    minuteLat = (int) ((int_binary2code_B & 0X007FFFFF) >> 17); // 6bit分
    minuteLng = (int) ((int_binary2code_B & 0X007FFFFF) >> 17); // 6bit分

    if (minuteLat > 60 || minuteLng > 60) return false;                               //不得大于60分

    //整1/2048秒
    secondLat = (int) (int_binary2code_B & 0X0001FFFF);          //17bit秒
    secondLng = (int) (int_binary2code_L & 0X0001FFFF);          //17bit秒

    if (secondLat > 122880 || secondLng > 122880) return false;                           //不得大于60秒

    *pLat = degreeLat + minuteLat * kMin2Degree + secondLat * kSec2Degree;
    *pLng = degreeLng + minuteLng * kMin2Degree + secondLng * kSec2Degree;

    if (I & 0X80000000) *pLng = -*pLng;
    if (J & 0X80000000) *pLat = -*pLat;

    return true;
}


bool GT::IJtoLL(const uint32 I, const uint32 J, double *pLng, double *pLat, int level) {
    uint32 I_, J_;
    uint32 mask = 1 << (kMaxCellLevel - level + 1);
    mask = (~mask + 1);

    I_ = I & mask;
    J_ = J & mask;

    return IJtoLL(I_, J_, pLng, pLat);
}

bool GT::LLtoIJ(const double Lng, const double Lat, uint32 *pI, uint32 *pJ) {
    bool bRet = false;

    // 检查是否越界
    S2_DCHECK_LT(fabs(Lng), 180 + DBL_EPSILON);
    S2_DCHECK_LT(fabs(Lat), 90 + DBL_EPSILON);

    unsigned int int_binary2code_B = 0, int_binary2code_L = 0, int_B, int_L;
    double minuteB, minuteL, secondB, secondL;

    double codeU = fabs(Lng), codeV = fabs(Lat);

    //全球四分（第1级）
    if (Lng < 0)//西经
        int_binary2code_L = 0X80000000;
    if (Lat < 0)//南纬
        int_binary2code_B = 0X80000000;

    //度级（第2到9级为连续四分,最高层级尺度为1度）
    int_B = ((unsigned int) codeV);
    int_L = ((unsigned int) codeU);
    S2_DCHECK_LE(int_B, 90);
    S2_DCHECK_LE(int_L, 180);

    int_binary2code_B |= (int_B << 23);
    int_binary2code_L |= (int_L << 23);

    //分级（第10到15级为连续四分，最高层级尺度为1分）
    minuteB = (codeV - (int) codeV) * 60.0;
    minuteL = (codeU - (int) codeU) * 60.0;
    int_B = ((unsigned int) minuteB);  //整分
    int_L = ((unsigned int) minuteL);  //整分
    S2_DCHECK_LE(int_B, 60);
    S2_DCHECK_LE(int_L, 60);


    int_binary2code_B |= (int_B << 17);
    int_binary2code_L |= (int_L << 17);

    //秒级（第16到31级为连续四分，最高层级尺度为1/2048秒）
    secondB = (minuteB - (int) minuteB) * 60.0;
    secondL = (minuteL - (int) minuteL) * 60.0;

    int_B = (int) (secondB * 2048.0);   //整1/2048秒
    int_L = (int) (secondL * 2048.0);   //整1/2048秒
    S2_DCHECK_LE(int_B, 122880);
    S2_DCHECK_LE(int_L, 122880);

    int_binary2code_B |= int_B;
    int_binary2code_L |= int_L;

    *pJ = int_binary2code_B;
    *pI = int_binary2code_L;

    return bRet;
}

bool GT::LLtoIJ(const double Lng, const double Lat, uint32 *pI, uint32 *pJ, int level) {

    uint32 I, J;
    uint32 mask = 1 << (kMaxCellLevel - level + 1);
    mask = (~mask + 1);

    LLtoIJ(Lng, Lat, &I, &J);

    *pI = I & mask;
    *pJ = J & mask;

    return true;
}

////////////////////////////////////////////////////////
//球面坐标系（X，Y,Z）与经纬度坐标系（LNG，LAT）之间的转换函数
///////////////////////////////////////////////////////
bool GT::XYZtoLL(const S2Point &p, double *pLng, double *pLat) {

    double lng,lat;

    lat  = S2LatLng::Latitude(p).degrees();
    lng = S2LatLng::Longitude(p).degrees();

    S2_DCHECK_LE(fabs(lat),90 + DBL_EPSILON);
    S2_DCHECK_LE(fabs(lng),180 + DBL_EPSILON);

    *pLat = lat;
    *pLng = lng;
    return false;
}

bool GT::LLtoXYZ(const double Lng, const double Lat, S2Point *pPnt) {
    S2LatLng latLng;

    latLng.FromDegrees(Lat, Lng);
    if (latLng.is_valid()) {
        *pPnt = latLng.ToPoint();
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////
//球面坐标系（X，Y,Z）与网格坐标系（I，J）之间的转换函数
///////////////////////////////////////////////////////

bool GT::XYZtoIJ(const S2Point &p, uint32 *pI, uint32 *pJ) {
    double u, v;
    uint32 I,J;

    if(XYZtoLL(p, &u, &v) && LLtoIJ(u, v, &I, &J)){
        *pI = I;
        *pJ = J;
        return true;
    }
    return false;
}

bool GT::XYZtoIJ(const S2Point &p, uint32 *pI, uint32 *pJ, int level) {
    double u, v;
    uint32 I,J;

    if(XYZtoLL(p, &u, &v) && LLtoIJ(u, v, &I, &J, level)){
        *pI = I;
        *pJ = J;
        return true;
    };
    return false;
}

bool GT::IJtoXYZ(const uint32 I, const uint32 J, S2Point *pPnt) {
    double u, v;
    IJtoLL(I, J, &u, &v);
    LLtoXYZ(u, v, pPnt);
    return false;
}

bool GT::IJtoXYZ(const uint32 I, const uint32 J, S2Point *pPnt, int level) {
    uint32 I_, J_;
    uint32 mask = 1 << (kMaxCellLevel - level + 1);
    mask = (~mask + 1);

    I_ = I & mask;
    J_ = J & mask;

    IJtoXYZ(I_, J_, pPnt);

    return true;
}

////////////////////////////////////////////////////////
//网格坐标系（I，J）与一维编码ID之间的转换函数
///////////////////////////////////////////////////////
bool GT::IJtoCellID(const uint32 I, const uint32 J, uint64 *pCellID) {

    IJtoCellID(I, J, pCellID, kMaxCellLevel);

    return true;
}

bool GT::IJtoCellID(const uint32 I, const uint32 J, uint64 *pCellID, int level) {

    uint64 cellID = 0X0;
    uint64 lsb = 1 << ((kMaxCellLevel - level) * 2 + 1);

    cellID = util_bits::InterleaveUint32(I, J);

    cellID = cellID & (~lsb + 1) | lsb;

    *pCellID = cellID;

    return true;
}


bool GT::CellIDtoIJ(const uint64 CellID, uint32 *pI, uint32 *pJ, int *level) {
    uint32 I, J;
    uint64 id;

    //去除标识位
    id = CellID - CellID & (~CellID + 2);

            S2_DCHECK(CellID != 0);

    util_bits::DeinterleaveUint32(id, &I, &J);

    *pI = I;
    *pJ = J;
    *level = kMaxCellLevel - (Bits::FindLSBSetNonZero64(CellID) >> 1);

    return true;
}

////////////////////////////////////////////////////////
//经纬度坐标系（LNG，LAT）与网格ID之间的转换函数
///////////////////////////////////////////////////////
bool GT::LLtoCellID(const double Lng, const double Lat, uint64 *pCellID) {
    uint32 I, J;
    LLtoIJ(Lng, Lat, &I, &J);
    IJtoCellID(I, J, pCellID);
    return true;
}

bool GT::LLtoCellID(const double Lng, const double Lat, uint64 *pCellID, int level) {

    uint32 I, J;

    LLtoIJ(Lng, Lat, &I, &J, level);
    IJtoCellID(I, J, pCellID, level);

    return true;
}

bool GT::CellIDtoLL(const uint64 CellID, double *pLng, double *pLat, int *level) {
    uint32 I, J;
    int le;

    CellIDtoIJ(CellID, &I, &J, &le);
    IJtoLL(I, J, pLng, pLat, le);
    *level = le;

    return false;
}

////////////////////////////////////////////////////////
//球面坐标系（X，Y,Z）与网格ID之间的转换函数
///////////////////////////////////////////////////////
bool GT::XYZtoCellID(const S2Point pnt, uint64 *pCellID) {
    uint32 I, J;

    XYZtoIJ(pnt, &I, &J);
    IJtoCellID(I, J, pCellID);

    return true;
}

bool GT::XYZtoCellID(const S2Point pnt, uint64 *pCellID, int level) {
    uint32 I, J;

    XYZtoIJ(pnt, &I, &J, level);
    IJtoCellID(I, J, pCellID, level);

    return true;
}

bool GT::CellIDtoXYZ(const uint64 CellID, S2Point *pPnt, int *level) {
    uint32 I, J;
    int le;

    CellIDtoIJ(CellID, &I, &J, &le);
    IJtoXYZ(I, J, pPnt, le);
    *level = le;

    return true;
}






