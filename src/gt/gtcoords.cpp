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

#include "gt/gtcoords.h"

#include <cmath>
#include <cfloat>
#include <gt/gtcoords.h>


#include "base/logging.h"
#include "util/math/mathutil.h"
#include "s2/s2latlng.h"

////////////////////////////////////////////////////////
//扩展坐标系（U，V）与网格坐标系（I，J）之间的转换函数
///////////////////////////////////////////////////////
bool GT::IJtoUV (const uint32 I, const uint32 J, double *pU, double *pV) {
    bool bRet = false;

    //边界检查
    S2_DCHECK_LE(I , 0XFFFFFFFE);
    S2_DCHECK_LE(J , 0XFFFFFFFE);

    unsigned int int_binary2code_B = J,int_binary2code_L = I;
    unsigned int  degreeLat,degreeLng,minuteLat,minuteLng,secondLat,secondLng;

    int_binary2code_B = J & 0X7FFFFFFFF;
    int_binary2code_L = I & 0X7FFFFFFFF;

    //整度
    degreeLat = (int)(int_binary2code_B >> 23);                 // 9bit度
    degreeLng = (int)(int_binary2code_L >> 23);                 // 9bit度

    S2_DCHECK_LE(degreeLat , 90);                              //不得大于90度
    S2_DCHECK_LE(degreeLng , 180);                              //不得大于180度
    //整分
    minuteLat = (int)((int_binary2code_B  & 0X007FFFFF) >> 17); // 6bit分
    minuteLng = (int)((int_binary2code_B  & 0X007FFFFF) >> 17); // 6bit分
    S2_DCHECK_LT(minuteLat , 60);                               //不得大于60分
    S2_DCHECK_LT(minuteLng , 60);                               //不得大于60分

    //整1/2048秒
    secondLat = (int)(int_binary2code_B & 0X0001FFFF);          //17bit秒
    secondLng = (int)(int_binary2code_L & 0X0001FFFF);          //17bit秒

    S2_DCHECK_LT(secondLat , 122880);                           //不得大于60秒
    S2_DCHECK_LT(secondLng , 122880);                           //不得大于60秒

    *pV = degreeLat + minuteLat * kMin2Degree + secondLat * kSec2Degree;
    *pU = degreeLng + minuteLng * kMin2Degree + secondLng * kSec2Degree;

    if (I & 0X80000000) *pU = - *pU;
    if (J & 0X80000000) *pV = - *pV;

    return bRet;
}

bool GT::UVtoIJ (const double U, const double V, uint32 *pI, uint32 *pJ) {
    bool bRet = false;
    
    // 检查是否越界
    S2_DCHECK_LE(fabs(U),512.00 );
    S2_DCHECK_LE(fabs(V),512.00 );
    
    unsigned int int_binary2code_B = 0, int_binary2code_L = 0, int_B, int_L;
    double minuteB, minuteL, secondB, secondL;

    double codeU = fabs(U),codeV = fabs(V);

    //全球四分（第1级）
    if (U < 0)//西经
        int_binary2code_L = 1 << 31;
    if (V < 0)//南纬
        int_binary2code_B = 1 << 31;

    //度级（第2到9级为连续四分,最高层级尺度为1度）
    int_B = ((unsigned int) codeV);
    int_L = ((unsigned int) codeU);
    int_binary2code_B |= (int_B << 23);
    int_binary2code_L |= (int_L << 23);

    //分级（第10到15级为连续四分，最高层级尺度为1分）
    minuteB = (codeV - (int)codeV)*60.0;
    minuteL = (codeU - (int)codeU)*60.0;
    int_B = ((unsigned int)minuteB);  //整分
    int_L = ((unsigned int)minuteL);  //整分
    int_binary2code_B |= (int_B << 17);
    int_binary2code_L |= (int_L << 17);

    //秒级（第16到31级为连续四分，最高层级尺度为1/2048秒）
    secondB = (minuteB - (int)minuteB)*60.0;
    secondL = (minuteL - (int)minuteL)*60.0;

    int_B = (int) (secondB * 2048.0);   //整1/2048秒
    int_L = (int) (secondL * 2048.0);   //整1/2048秒

    int_binary2code_B |= int_B ;
    int_binary2code_L |= int_L;

    *pJ = int_binary2code_B;
    *pI = int_binary2code_L;

    return bRet;
}

////////////////////////////////////////////////////////
//经纬度坐标系（LNG，LAT）与网格坐标系（I，J）之间的转换函数
///////////////////////////////////////////////////////
bool GT::IJtoLL (const uint32 I, const uint32 J, double *pLng, double *pLat) {
    return(IJtoUV(I,J,pLng,pLat));
}

bool GT::LLtoIJ (const double Lng, const double Lat, uint32 *pI, uint32 *pJ) {
    return(UVtoIJ(Lng,Lat,pI,pJ));
}

////////////////////////////////////////////////////////
//球面坐标系（X，Y,Z）与经纬度坐标系（LNG，LAT）之间的转换函数
///////////////////////////////////////////////////////
 bool GT::XYZtoLL (const S2Point &p, double *pLng, double *pLat) {

    *pLat = S2LatLng::Latitude(p).degrees();
    *pLng = S2LatLng::Longitude(p).degrees();

    return false;
}

bool GT::LLtoXYZ (const double Lng, const double Lat, S2Point *pPnt) {
    S2LatLng latLng;

    latLng.FromDegrees(Lat,Lng);
    if(latLng.is_valid()) {
        *pPnt = latLng.ToPoint();
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////
//球面坐标系（X，Y,Z）与扩展坐标系（U，V）之间的转换函数
///////////////////////////////////////////////////////
 bool GT::XYZtoUV (const S2Point &p, double *pU, double *pV) {
    XYZtoLL(p,pU,pV);
    return false;
}

bool GT::UVtoXYZ (const double U, const double V, S2Point *pPnt) {
    LLtoXYZ(U,V,pPnt);
    return false;
}

bool GT::XYZtoIJ (const S2Point &p, uint32 *pI, uint32 *pJ) {
    double u,v;
    XYZtoUV(p,&u,&v);
    UVtoIJ(u,v,pI,pJ);
    return false;
}

bool GT::IJtoXYZ (const uint32 I, const uint32 J, S2Point *pPnt) {
    double u,v;
    IJtoUV(I,J,&u,&v);
    UVtoXYZ(u,v,pPnt);
    return false;
}

bool GT::IJtoCellID (const uint32 I, const uint32 J, uint64 *pCellID) {

   uint64 cellID = 0X0;
   uint64 bitI,bitJ,mask;

   for (int i = 0; i<32; i++)
    {
        mask = 1U << i;
        bitI = ((uint64) I & mask) << 2*i;
        bitJ = ((uint64) J & mask) << 2*i+1;
        cellID = cellID | bitI | bitJ;
    }
   //处理末尾2bit截止位
   cellID = cellID & 0XFFFFFFFFFFFFFFFE;
   cellID = cellID | 0X0000000000000002;

   return true;
}

bool GT::CellIDtoIJ (const uint64 CellID, uint32 *pI, uint32 *pJ) {
    uint32 bitI,bitJ,tmpI=0,tmpJ=0;
    uint64 maskI,maskJ;

    for (int i=0; i<32; i++){
        maskI = 1U << 2*i;
        maskJ = 1U << 2*i+1;

        bitI = CellID & maskI;
        bitJ = CellID & maskJ;

        if(bitI){
            tmpI = tmpI | ((uint32)1 << i);
        }
        if(bitJ){
            tmpJ = tmpJ | ((uint32)1 << i);
        }
    }
    *pI = tmpI;
    *pJ = tmpJ;

    return true;
}

bool GT::LLtoCellID(const double Lng, const double Lat, uint64 *pCellID) {
    uint32 I,J;
    LLtoIJ(Lng,Lat,&I,&J);
    IJtoCellID(I,J,pCellID);
    return true;
}

bool GT::CellIDtoLL(const uint64 CellID, double *pLng, double *pLat) {
    uint32 I,J;
    CellIDtoIJ(CellID,&I,&J);
    IJtoLL(I,J,pLng,pLat);
    return false;
}

bool GT::XYZtoCellID(const S2Point pnt, uint64 *pCellID) {
    uint32 I,J;

    XYZtoIJ(pnt,&I,&J);
    IJtoCellID(I,J,pCellID);

    return true;
}

bool GT::CellIDtoXYZ(const uint64 CellID, S2Point *pPnt) {
    uint32 I,J;

    CellIDtoIJ(CellID,&I,&J);
    IJtoXYZ(I,J,pPnt);

    return true;
}
