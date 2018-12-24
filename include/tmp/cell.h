//
// Created by 濮国梁 on 2018/12/13.
//

#ifndef DISCRETEEARTH_CELL_H
#define DISCRETEEARTH_CELL_H

#include "_fp_contract_off.h"
#include "exports.h"

#include "base/integral_types.h"
#include "base/logging.h"

#include "s2/r2rect.h"
#include "s2/s1chord_angle.h"
#include "cell_id.h"
#include "core/region.h"
#include "util/math/vector.h"


/// Cell剖分网格单元的抽象类，为方便以后扩展新的剖分方案
/// Cell和CellId两个类应根据自定义的剖分和编码方案同步派生出新的类
//
//An Cell is abstract class for grid cell,it derived from Region.
//Unlike CellIds, it's main purpose is to realize efficient
//containment and intersection operations(Spatial Operation).
//However, it is a more expensive representation than CellId.

// This class is intended to be copied by value as desired.  It uses
// the default copy constructor and assignment operator, however it is
// not a "plain old datatype" (POD) because it has virtual functions.
 class GT_API Cell : public Region {
public:

    /***************************************
     * 构造函数
     *
     * *************************************/

    // The default constructor is required in order to use freelists.
    // Cells should otherwise always be constructed explicitly.
    Cell() {};
    ~Cell(){};

    /***************************************
     * 面片属性访问函数
     *
     * *************************************/
    //面片属性获取函数
    //面片ID
    virtual uint64 id() const =0;
    //面片层级
    virtual int level() const=0;
    //面片方位
    virtual int orientation() const =0;

       //是否叶子节点
    virtual bool is_leaf() const = 0 ;

    // 返回面片第k个格点的球面坐标
    // Returns the k-th vertex of the cell (k = 0,1,2,3).  Vertices are returned
    // in CCW order (lower left, lower right, upper right, upper left in the UV
    // plane).  The points returned by GetVertexRaw are not normalized.
    // For convenience, the argument is reduced modulo 4 to the range [0..3].
    virtual S2Point GetVertex(int k) const = 0;

    // 返回第k和第k+1个格点构成的大圆弧的法线
    // Returns the inward-facing normal of the great circle passing through the
    // edge from vertex k to vertex k+1 (mod 4).  The normals returned by
    // GetEdgeRaw are not necessarily unit length.  For convenience, the
    // argument is reduced modulo 4 to the range [0..3].
    virtual S2Point GetEdge(int k) const = 0;

    // 返回面片中心点的球面坐标
    // Returns the direction vector corresponding to the center in (s,t)-space of
    // the given cell.  This is the point at which the cell is divided into four
    // subcells; it is not necessarily the centroid of the cell in (u,v)-space
    // or (x,y,z)-space.  The point returned by GetCenterRaw is not necessarily
    // unit length.
    virtual S2Point GetCenter() const  = 0;




    /***************************************
     * 面片距离与面积度量函数
     *
     * *************************************/
    // 返回指定层级面片的平均面积
    // Returns the average area for cells at the given level.
    virtual double AverageArea(int level) = 0;

    // 返回当前层级面片的平均面积（立体弧度）
    // Returns the average area of cells at this level in steradians.  This is
    // accurate to within a factor of 1.7 (for S2_QUADRATIC_PROJECTION) and is
    // extremely cheap to compute.
    virtual double AverageArea() const = 0;
    
    // 返回当前层级面片的大概面积（立体弧度）
    // Returns the approximate area of this cell in steradians.  This method is
    // accurate to within 3% percent for all cell sizes and accurate to within
    // 0.1% for cells at level 5 or higher (i.e. squares 350km to a side or
    // smaller on the Earth's surface).  It is moderately cheap to compute.
    virtual double ApproxArea() const = 0;

    // 返回当前面片的精确面积（立体弧度）
    // Returns the area of this cell as accurately as possible.  This method is
    // more expensive but it is accurate to 6 digits of precision even for leaf
    // cells (whose area is approximately 1e-18).
    virtual double ExactArea() const = 0;

     // 返回从面片到球面上某点的距离
    // Returns the distance from the cell to the given point.  Returns zero if
    // the point is inside the cell.
    virtual S1ChordAngle GetDistance(const S2Point& target) const = 0;

    // 返回面片边界到球面上指定点之间的距离
    // Return the distance from the cell boundary to the given point.
    virtual S1ChordAngle GetBoundaryDistance(const S2Point& target) const = 0;

    // 返回面片到球面上某点的最大距离
    // Returns the maximum distance from the cell (including its interior) to the
    // given point.
    virtual S1ChordAngle GetMaxDistance(const S2Point& target) const = 0;

    // 返回面片到指定边ab之间的最小球面距离
    // Returns the minimum distance from the cell to the given edge AB.  Returns
    // zero if the edge intersects the cell interior.
    virtual S1ChordAngle GetDistance(const S2Point& a, const S2Point& b) const = 0;

    // 返回面片到指定边ab之间的最大球面距离
    // Returns the maximum distance from the cell (including its interior) to the
    // given edge AB.
    virtual S1ChordAngle GetMaxDistance(const S2Point& a, const S2Point& b) const = 0;

    // 返回两个面片之间的球面距离
    // Returns the distance from the cell to the given cell.  Returns zero if
    // one cell contains the other.
    virtual S1ChordAngle GetDistance(const Cell& target) const = 0;

    // 返回两个面片之间的最大球面距离
    // Returns the maximum distance from the cell (including its interior) to the
    // given target cell.
    virtual S1ChordAngle GetMaxDistance(const Cell& target) const = 0;

    /***************************************
     * 面片空间关系分析函数
     * 包含、相交、相邻、分离
     * *************************************/


    /***************************************
     * 面片距离与面积度量函数
     *
     * *************************************/

    //返回子面片集合
    // If this is not a leaf cell, sets children[0..3] to the four children of
    // this cell (in traversal order) and return true.  Otherwise returns false.
    // This method is equivalent to the following:
    //
    // for (pos=0, id=child_begin(); id != child_end(); id = id.next(), ++pos)
    //   children[pos] = Cell(id);
    //
    // except that it is more than two times faster.
    virtual bool Subdivide(Cell* pChildren) const = 0;


    ////////////////////////////////////////////////////////////////////////
    // S2Region interface (see s2region.h for details):

private:

};

#endif //DISCRETEEARTH_CELL_H
