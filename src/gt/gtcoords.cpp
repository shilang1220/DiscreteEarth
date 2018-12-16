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
////////////////////////////////////////////////////////
//扩展坐标系（U，V）与网格坐标系（I，J）之间的转换函数
///////////////////////////////////////////////////////
inline bool GT::IJtoUV (const uint32 I, const uint32 J, double *pU, double *pV) {
    bool bRet = false;



    return bRet;
}

inline bool GT::UVtoIJ (const double U, const double V, uint32 *pI, uint32 *pJ) {
    bool bRet = false;

    return bRet;
}

////////////////////////////////////////////////////////
//经纬度坐标系（LNG，LAT）与网格坐标系（I，J）之间的转换函数
///////////////////////////////////////////////////////
inline bool GT::IJtoLL (const uint32 I, const uint32 J, double *pLng, double *pLat) {
    bool bRet = false;


    return bRet;
}

inline bool GT::LLtoIJ (const double Lng, const double Lat, uint32 *pI, uint32 *pJ) {
    bool bRet = false;

    S2_CHECK_LE(Lng,180.00 );
    S2_CHECK_GE(Lng,-180.00 );
    S2_CHECK_LE(Lat,90.00);
    S2_CHECK_GE(Lat,-90.00);

    UVtoIJ(Lng,Lat,pI,pJ);

    return bRet;
}

////////////////////////////////////////////////////////
//球面坐标系（X，Y,Z）与经纬度坐标系（LNG，LAT）之间的转换函数
///////////////////////////////////////////////////////
inline  bool GT::XYZtoLL (const S2Point &p, double *pU, double *pV) {
    return false;
}

inline bool GT::LLtoXYZ (const double U, const double V, S2Point *pPnt) {
    return false;
}

////////////////////////////////////////////////////////
//球面坐标系（X，Y,Z）与扩展坐标系（U，V）之间的转换函数
///////////////////////////////////////////////////////
inline  bool GT::XYZtoUV (const S2Point &p, double *pU, double *pV) {
    return false;
}

inline bool GT::UVtoXYZ (const double U, const double V, S2Point *pPnt) {
    return false;
}

inline bool GT::XYZtoIJ (const S2Point &p, uint32 *pI, uint32 *pJ) {
    return false;
}

inline bool GT::IJtoXYZ (const uint32 I, const uint32 J, S2Point *pPnt) {
    return false;
}

inline bool GT::IJtoCellID (const uint32 I, const uint32 J, uint64 *pCellID) {
    return false;
}

inline bool GT::CellIDtoIJ (const uint64 CellID, uint32 *I, const uint32 *J) {
    return false;
}
