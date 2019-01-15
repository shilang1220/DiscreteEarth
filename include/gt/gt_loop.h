//
// Created by 濮国梁 on 2018/12/13.
//

#ifndef DISCRETEEARTH_GTLOOP_H
#define DISCRETEEARTH_GTLOOP_H
#include <memory>
#include <vector>

#include "_fp_contract_off.h"
#include "exports.h"

#include "base/logging.h"

#include "s2/s1angle.h"
#include "s2/s2point.h"
#include "tools/gt_debug.h"
#include "tools/gt_error.h"
#include "core/gt_latlng_rect.h"
#include "core/region.h"
#include "core/gt_cap.h"
#include "core/gt_shape.h"

 class GT_API  GTLoop final : Region {
    
    GTLoop();

    // An S2GTCell always corresponds to a particular S2GTCellId.  The other
    // constructors are just convenience methods.
    // Convenience constructors that call Init() with the given vertices.
    explicit GTLoop(const std::vector<S2Point>& vertices);
    explicit GTLoop(const std::vector<S2LatLng>& vertices);

    // Initialize a polyline that connects the given vertices. Empty polylines are
    // allowed.  Adjacent vertices should not be identical or antipodal.  All
    // vertices should be unit length.
    void Init(const std::vector<S2Point>& vertices);

    // Convenience initialization function that accepts latitude-longitude
    // coordinates rather than S2Points.
    void Init(const std::vector<S2LatLng>& vertices);

    ~GTLoop() ;

    //////////////////////////////////////////////
    ///  线要素基本操作
    /////////////////////////////////////////////
    // Return true if the given vertices form a valid polyline.
    bool IsValid() const;

    // Returns true if this is *not* a valid polyline and sets "error"
    // appropriately.  Otherwise returns false and leaves "error" unchanged.
    //
    // REQUIRES: error != nullptr
    bool FindValidationError(S2Error* error) const;

    int num_vertices() const { return num_vertices_; }

    const S2Point& vertex(int k) const {
        S2_DCHECK_GE(k, 0);
        S2_DCHECK_LT(k, num_vertices_);
        return vertices_[k];
    }

    // Return the length of the polyline.
    S1Angle GetLength() const;

    // Return the true centroid of the polyline multiplied by the length of the
    // polyline (see s2centroids.h for details on centroids).  The result is not
    // unit length, so you may want to normalize it.
    //
    // Prescaling by the polyline length makes it easy to compute the centroid
    // of several polylines (by simply adding up their centroids).
    S2Point GetCentroid() const;

    // Return the point whose distance from vertex 0 along the polyline is the
    // given fraction of the polyline's total length.  Fractions less than zero
    // or greater than one are clamped.  The return value is unit length.  This
    // cost of this function is currently linear in the number of vertices.
    // The polyline must not be empty.
    S2Point Interpolate(double fraction) const;

    // Like Interpolate(), but also return the index of the next polyline
    // vertex after the interpolated point P.  This allows the caller to easily
    // construct a given suffix of the polyline by concatenating P with the
    // polyline vertices starting at "next_vertex".  Note that P is guaranteed
    // to be different than vertex(*next_vertex), so this will never result in
    // a duplicate vertex.
    //
    // The polyline must not be empty.  Note that if "fraction" >= 1.0, then
    // "next_vertex" will be set to num_vertices() (indicating that no vertices
    // from the polyline need to be appended).  The value of "next_vertex" is
    // always between 1 and num_vertices().
    //
    // This method can also be used to construct a prefix of the polyline, by
    // taking the polyline vertices up to "next_vertex - 1" and appending the
    // returned point P if it is different from the last vertex (since in this
    // case there is no guarantee of distinctness).
    S2Point GetSuffix(double fraction, int* next_vertex) const;

    // The inverse operation of GetSuffix/Interpolate.  Given a point on the
    // polyline, returns the ratio of the distance to the point from the
    // beginning of the polyline over the length of the polyline.  The return
    // value is always betwen 0 and 1 inclusive.  See GetSuffix() for the
    // meaning of "next_vertex".
    //
    // The polyline should not be empty.  If it has fewer than 2 vertices, the
    // return value is zero.
    double UnInterpolate(const S2Point& point, int next_vertex) const;

    // Given a point, returns a point on the polyline that is closest to the given
    // point.  See GetSuffix() for the meaning of "next_vertex", which is chosen
    // here w.r.t. the projected point as opposed to the interpolated point in
    // GetSuffix().
    //
    // The polyline must be non-empty.
    S2Point Project(const S2Point& point, int* next_vertex) const;

    // Returns true if the point given is on the right hand side of the polyline,
    // using a naive definition of "right-hand-sideness" where the point is on
    // the RHS of the polyline iff the point is on the RHS of the line segment in
    // the polyline which it is closest to.
    //
    // The polyline must have at least 2 vertices.
    bool IsOnRight(const S2Point& point) const;

    // Return true if this polyline intersects the given polyline. If the
    // polylines share a vertex they are considered to be intersecting. When a
    // polyline endpoint is the only intersection with the other polyline, the
    // function may return true or false arbitrarily.
    //
    // The running time is quadratic in the number of vertices.  (To intersect
    // polylines more efficiently, or compute the actual intersection geometry,
    // use S2BooleanOperation.)
    bool Intersects(const GTLoop* line) const;

    // Reverse the order of the polyline vertices.
    void Reverse();

    // Return a subsequence of vertex indices such that the polyline connecting
    // these vertices is never further than "tolerance" from the original
    // polyline.  Provided the first and last vertices are distinct, they are
    // always preserved; if they are not, the subsequence may contain only a
    // single index.
    //
    // Some useful properties of the algorithm:
    //
    //  - It runs in linear time.
    //
    //  - The output is always a valid polyline.  In particular, adjacent
    //    output vertices are never identical or antipodal.
    //
    //  - The method is not optimal, but it tends to produce 2-3% fewer
    //    vertices than the Douglas-Peucker algorithm with the same tolerance.
    //
    //  - The output is *parametrically* equivalent to the original polyline to
    //    within the given tolerance.  For example, if a polyline backtracks on
    //    itself and then proceeds onwards, the backtracking will be preserved
    //    (to within the given tolerance).  This is different than the
    //    Douglas-Peucker algorithm, which only guarantees geometric equivalence.
    //
    // See also GTLoopSimplifier, which uses the same algorithm but is more
    // efficient and supports more features, and also S2Builder, which can
    // simplify polylines and polygons, supports snapping (e.g. to E7 lat/lng
    // coordinates or S2GTCellId centers), and can split polylines at intersection
    // points.
    void SubsampleVertices(S1Angle tolerance, std::vector<int>* indices) const;

    // Return true if two polylines are exactly the same.
    bool Equals(const GTLoop* b) const;

    // Return true if two polylines have the same number of vertices, and
    // corresponding vertex pairs are separated by no more than "max_error".
    // (For testing purposes.)
    bool ApproxEquals(const GTLoop& b,
                      S1Angle max_error = S1Angle::Radians(1e-15)) const;

    // Return true if "covered" is within "max_error" of a contiguous subpath of
    // this polyline over its entire length.  Specifically, this method returns
    // true if this polyline has parameterization a:[0,1] -> S^2, "covered" has
    // parameterization b:[0,1] -> S^2, and there is a non-decreasing function
    // f:[0,1] -> [0,1] such that distance(a(f(t)), b(t)) <= max_error for all t.
    //
    // You can think of this as testing whether it is possible to drive a car
    // along "covered" and a car along some subpath of this polyline such that no
    // car ever goes backward, and the cars are always within "max_error" of each
    // other.
    //
    // This function is well-defined for empty polylines:
    //    anything.covers(empty) = true
    //    empty.covers(nonempty) = false
    bool NearlyCovers(const GTLoop& covered, S1Angle max_error) const;

    // Returns the total number of bytes used by the polyline.
    size_t SpaceUsed() const;



    //////////////////////////////////////////////
    ///  Region interface realization
    /////////////////////////////////////////////

    GTLoop* Clone() const override;
    GTCap GetCapBound() const override;
    GTLatLngRect GetRectBound() const override;
    bool Contains(const GTCell& cell) const override;
    bool MayIntersect(const GTCell& cell) const override;
    bool Contains(const S2Point& p) const override;

    //////////////////////////////////////////////////////////
    ///  串行化函数
    /////////////////////////////////////////////////////////

    // Appends a serialized representation of the GTLoop to "encoder".
    //
    // REQUIRES: "encoder" uses the default constructor, so that its buffer
    //           can be enlarged as necessary by calling Ensure(int).
    void Encode(Encoder* const encoder) const;

    // Decodes an GTLoop encoded with Encode().  Returns true on success.
    bool Decode(Decoder* const decoder);

   //////////////////////////////////////////////////////////////////////////
   ///  内部shape类，通过访问loop的内部成员变量，提供shape所需的接口
   ///  TODO:也可尝试通过多重继承方式来做，待处理
   ///  不采用多重继承可能有一个好处：对shape索引时可以根据需要随时销毁shape对象，
   ///  而不用担心会影响到相应的loop对象
   ////////////////////////////////////////////////////////////////////////
   class Shape : public GTShape {

   };


   ///////////////////////////////////////////////////////////
    ///   成员变量
    ////////////////////////////////////////////////////////////

//private:
    // We store the vertices in an array rather than a vector because we don't
    // need any STL methods, and computing the number of vertices using size()
    // would be relatively expensive (due to division by sizeof(S2Point) == 24).
    int num_vertices_ = 0;
    std::unique_ptr<S2Point[]> vertices_;
};

#endif //DISCRETEEARTH_GTLOOP_H
