//
// Created by 濮国梁 on 2018/12/13.
//

#ifndef DISCRETEEARTH_GTCELL_H
#define DISCRETEEARTH_GTCELL_H

#include "_fp_contract_off.h"
#include "exports.h"

#include "core/region.h"
#include "core/gt_cap.h"
#include "core/gt_cell_id.h"
#include "gt_latlng_rect.h"



class GT_API GTCell:Region {
    /***************************************
     * 构造函数
     *************************************/
public:
    // Default constructor is set id to illegal number 0.
    GTCell() {  };

    // A GTCell always corresponds to a particular GTCellId.
    explicit GTCell(GTCellId id):id_(id){};

    // A GTCell from spherical 3d coordinates.
    explicit GTCell(const S2Point &p) : GTCell(GTCellId(p)) {}
    // A GTCell from latitude and longitude. The S2LatLng must within[-90..+90,-180..+180]
    explicit GTCell(const S2LatLng &ll) : GTCell(GTCellId(ll)) {}

    ~GTCell() {};

    /***************************************
     * 面片属性访问函数
     *
     * *************************************/
    GTCellId  id() const {return id_;}
    int level()const{return 0;}
    bool is_leaf() const ;

    S2Point GetVertex(int k) const ;

    S2Point GetEdge(int k) const ;

    S2Point GetCenter() const ;

    /***************************************
     * 面片距离与面积度量函数
     *
     * *************************************/
    static double AverageArea(int level) ;

    double AverageArea() const ;

    double ApproxArea() const ;

    double ExactArea() const ;

    S1ChordAngle GetDistance(const S2Point &target) const ;

    S1ChordAngle GetBoundaryDistance(const S2Point &target) const ;

    S1ChordAngle GetMaxDistance(const S2Point &target) const ;

    S1ChordAngle GetDistance(const S2Point &a, const S2Point &b) const ;

    S1ChordAngle GetMaxDistance(const S2Point &a, const S2Point &b) const ;

    S1ChordAngle GetDistance(const GTCell &target) const ;

    S1ChordAngle GetMaxDistance(const GTCell &target) const ;

    /***************************************
     * 面片空间关系分析函数
     * 包含、相交、相邻、分离
     * *************************************/


    /***************************************
     * 面片距离与面积度量函数
     *
     * *************************************/

    bool Subdivide(GTCell *pChildren) const ;

    //////////////////////////////////////////////
    ///  Region interface realization
    /////////////////////////////////////////////

    GTCell* Clone() const override;

    GTCap GetCapBound() const override;

    GTLatLngRect GetRectBound() const override;

    bool Contains(const GTCell &cell) const override;

    bool MayIntersect(const GTCell &cell) const override;
    // The point "p" should be a unit-length vector.
    bool Contains(const S2Point& p) const override;

    //////////////////////////////////////////////////////////
    ///  串行化函数
    /////////////////////////////////////////////////////////

    // Appends a serialized representation of the GTPolyline to "encoder".
    //
    // REQUIRES: "encoder" uses the default constructor, so that its buffer
    //           can be enlarged as necessary by calling Ensure(int).
    void Encode(Encoder *const encoder) const;

    // Decodes an GTPolyline encoded with Encode().  Returns true on success.
    bool Decode(Decoder *const decoder);

    ///////////////////////////////////////////////////////////
    ///   成员变量
    ////////////////////////////////////////////////////////////
private:
    GTCellId id_;
};


#endif //DISCRETEEARTH_GTCELL_H
