//
// Created by 濮国梁 on 2018/12/13.
//

#ifndef DISCRETEEARTH_REGION_H
#define DISCRETEEARTH_REGION_H

#include <vector>

#include "_fp_contract_off.h"
#include "exports.h"

#include "s2/s1angle.h"

class GTCap;
class GTLatLngRect;
class GTCell;
class GTCellId;

//区域类，用于封装一些标准接口

class GT_API Region {
public:
    virtual ~Region() {}

    // Returns a deep copy of the region.
    //
    // Note that each subtype of Region returns a pointer to an object of its
    // own type (e.g., Cap::Clone() returns an Cap*).
    virtual Region* Clone() const = 0;

    // Returns a bounding spherical cap that contains the region.  The bound may
    // not be tight.
    virtual GTCap GetCapBound() const = 0;

    // Returns a bounding latitude-longitude rectangle that contains the region.
    // The bound may not be tight.
    virtual GTLatLngRect GetRectBound() const = 0;

    // Returns true if the region completely contains the given cell, otherwise
    // returns false.
    virtual bool Contains(const GTCell& cell) const = 0;

    // If this method returns false, the region does not intersect the given
    // cell.  Otherwise, either region intersects the cell, or the intersection
    // relationship could not be determined.
    //
    // Note that there is currently exactly one implementation of this method
    // (LatLngRect::MayIntersect) that takes advantage of the semantics above
    // to be more efficient.  For all other Region subtypes, this method
    // returns true if the region intersect the cell and false otherwise.
    virtual bool MayIntersect(const GTCell& cell) const = 0;

    // Returns true if and only if the given point is contained by the region.
    // The point 'p' is generally required to be unit length, although some
    // subtypes may relax this restriction.
    virtual bool Contains(const S2Point& p) const = 0;

    void GetCellUnionBound(std::vector<GTCellId> *cell_ids) const;
};


#endif //DISCRETEEARTH_REGION_H
