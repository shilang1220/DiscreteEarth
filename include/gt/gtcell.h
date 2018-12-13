//
// Created by 濮国梁 on 2018/12/13.
//

#ifndef DISCRETEEARTH_GTCELL_H
#define DISCRETEEARTH_GTCELL_H

#include "core/cell.h"
#include "core/cap.h"
#include "core/latlng_rect.h"

class GTCell final : Cell{
    /***************************************
     * 构造函数
     *************************************/

    // The default constructor is required in order to use freelists.
    // Cells should otherwise always be constructed explicitly.
    GTCell() {};
    ~GTCell(){};

    /***************************************
     * 面片属性访问函数
     *
     * *************************************/
    R2Rect GetBoundPlanar() override;
    bool is_leaf() const override ;
    S2Point GetVertex(int k) const override;
    S2Point GetEdge(int k) const override;
    S2Point GetCenter() const  override;

    /***************************************
     * 面片距离与面积度量函数
     *
     * *************************************/
   double AverageArea(int level) override;
   double AverageArea() const override;
   double ApproxArea() const override;
   double ExactArea() const override;
   S1ChordAngle GetDistance(const S2Point& target) const override;
   S1ChordAngle GetBoundaryDistance(const S2Point& target) const override;
   S1ChordAngle GetMaxDistance(const S2Point& target) const override;
   S1ChordAngle GetDistance(const S2Point& a, const S2Point& b) const override;
   S1ChordAngle GetMaxDistance(const S2Point& a, const S2Point& b) const override;
   S1ChordAngle GetDistance(const Cell& target) const override;
   S1ChordAngle GetMaxDistance(const Cell& target) const override;

    /***************************************
     * 面片空间关系分析函数
     * 包含、相交、相邻、分离
     * *************************************/


    /***************************************
     * 面片距离与面积度量函数
     *
     * *************************************/

    bool Subdivide(Cell* pChildren) const override;


    //////////////////////////////////////////////
    ///  Region interface realization
    /////////////////////////////////////////////

    Cell* Clone() const override;
    Cap GetCapBound() const override;
    LatLngRect GetRectBound() const override;
    bool Contains(const Cell& cell) const override;
    bool MayIntersect(const Cell& cell) const override;

    //////////////////////////////////////////////////////////
    ///  串行化函数
    /////////////////////////////////////////////////////////

    // Appends a serialized representation of the GTPolyline to "encoder".
    //
    // REQUIRES: "encoder" uses the default constructor, so that its buffer
    //           can be enlarged as necessary by calling Ensure(int).
    void Encode(Encoder* const encoder) const;

    // Decodes an GTPolyline encoded with Encode().  Returns true on success.
    bool Decode(Decoder* const decoder);

    ///////////////////////////////////////////////////////////
    ///   成员变量
    ////////////////////////////////////////////////////////////

};


#endif //DISCRETEEARTH_GTCELL_H
