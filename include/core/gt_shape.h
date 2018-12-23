//
// Created by Administrator on 2018/12/22 0022.
//

#ifndef DISCRETEEARTH_GT_SHAPE_H
#define DISCRETEEARTH_GT_SHAPE_H


#include "base/integral_types.h"
#include "s2/s2point.h"
#include "s2/s2pointutil.h"

// The purpose of GTShape is to represent polygonal geometry in a flexible
// way.  It is organized as a collection of edges that optionally defines an
// interior.  All geometry represented by an GTShape must have the same
// dimension, which means that an GTShape can represent either a set of
// points, a set of polylines, or a set of polygons.
//
// GTShape is defined as an abstract base class in order to give clients
// control over the underlying data representation.  Sometimes an GTShape does
// not have any data of its own, but instead "wraps" some other class.  There
// are various useful subtypes defined in *_shape.h, and some S2 classes also
// have a nested "Shape" class (e.g., S2Polygon::Shape).  It is easy for
// clients to implement their own subtypes, since the interface is minimal.
//
// GTShape operations are typically defined on GTShapeIndex objects rather
// than individual shapes.  An GTShapeIndex is simply a collection of
// GTShapes, possibly of different dimensions (e.g. 10 points and 3 polygons),
// organized into a data structure for efficient edge access.
//
// The edges of an GTShape are identified by a contiguous range of "edge ids"
// starting at 0.  The edges are further subdivided into "chains", where each
// chain consists of a sequence of edges connected end-to-end (a polyline).
// For example, an GTShape representing two polylines AB and CDE would have
// three edges (AB, CD, DE) grouped into two chains: (AB) and (CD, DE).
// Similarly, an GTShape representing 5 points would have 5 chains consisting
// of one edge each.
//
// GTShape has methods that allow edges to be accessed either using the global
// numbering (edge id) or within a particular chain.  The global numbering is
// sufficient for most purposes, but the chain representation is useful for
// certain algorithms such as intersection (see S2BooleanOperation).

class GTShape {
 public:
  // An edge, consisting of two vertices "v0" and "v1".  Zero-length edges are
  // allowed, and can be used to represent points.
  struct Edge {
    S2Point v0, v1;
    Edge() = default;
    Edge(const S2Point& _v0, const S2Point& _v1) : v0(_v0), v1(_v1) {}

    // TODO(ericv): Define all 6 comparisons.
    friend bool operator==(const Edge& x, const Edge& y) {
      return x.v0 == y.v0 && x.v1 == y.v1;
    }
    friend bool operator<(const Edge& x, const Edge& y) {
      return x.v0 < y.v0 || (x.v0 == y.v0 && x.v1 < y.v1); }
  };

  // A range of edge ids corresponding to a chain of zero or more connected
  // edges, specified as a (start, length) pair.  The chain is defined to
  // consist of edge ids {start, start + 1, ..., start + length - 1}.
  struct Chain {
    int32 start, length;
    Chain() = default;
    Chain(int32 _start, int32 _length) : start(_start), length(_length) {}

    friend bool operator==(const Chain& x, const Chain& y) {
      return x.start == y.start && x.length == y.length;
    }
  };

  // The position of an edge within a given edge chain, specified as a
  // (chain_id, offset) pair.  Chains are numbered sequentially starting from
  // zero, and offsets are measured from the start of each chain.
  struct ChainPosition {
    int32 chain_id, offset;
    ChainPosition() = default;
    ChainPosition(int32 _chain_id, int32 _offset)
        : chain_id(_chain_id), offset(_offset) {}

    friend bool operator==(const ChainPosition& x, const ChainPosition& y) {
      return x.chain_id == y.chain_id && x.offset == y.offset;
    }
  };

  // A ReferencePoint consists of a point P and a boolean indicating whether P
  // is contained by a particular shape.
  struct ReferencePoint {
    S2Point point;
    bool contained;
    ReferencePoint() = default;
    ReferencePoint(S2Point _point, bool _contained)
        : point(_point), contained(_contained) {}

    // Returns a ReferencePoint with the given "contained" value and a default
    // "point".  It should be used when all points or no points are contained.
    static ReferencePoint Contained(bool _contained) {
      return ReferencePoint(S2::Origin(), _contained);
    }

    friend bool operator==(const ReferencePoint& x, const ReferencePoint& y) {
      return x.point == y.point && x.contained == y.contained;
    }
  };

  // A 32-bit tag that can be used to identify the type of an encoded GTShape.
  // All encodable types have a non-zero type tag.  The tag associated with a
  // given shape type can be accessed as Shape::kTypeTag, while the tag
  // associated with a given object can be accessed as shape.type_tag().
  //
  // Type tags in the range 0..8191 are reserved for use by the S2 library.
  using TypeTag = uint32;

  // Indicates that a given GTShape type cannot be encoded.
  static constexpr TypeTag kNoTypeTag = 0;

  // The minimum allowable tag for user-defined GTShape types.
  static constexpr TypeTag kMinUserTypeTag = 8192;

  GTShape() : id_(-1) {}
  virtual ~GTShape() {}

  // Returns the number of edges in this shape.  Edges have ids ranging from 0
  // to num_edges() - 1.
  virtual int num_edges() const = 0;

  // Returns the endpoints of the given edge id.
  //
  // REQUIRES: 0 <= id < num_edges()
  virtual Edge edge(int edge_id) const = 0;

  // Returns the dimension of the geometry represented by this shape.
  //
  //  0 - Point geometry.  Each point is represented as a degenerate edge.
  //
  //  1 - Polyline geometry.  Polyline edges may be degenerate.  A shape may
  //      represent any number of polylines.  Polylines edges may intersect.
  //
  //  2 - Polygon geometry.  Edges should be oriented such that the polygon
  //      interior is always on the left.  In theory the edges may be returned
  //      in any order, but typically the edges are organized as a collection
  //      of edge chains where each chain represents one polygon loop.
  //      Polygons may have degeneracies (e.g., degenerate edges or sibling
  //      pairs consisting of an edge and its corresponding reversed edge).
  //      A polygon loop may also be full (containing all points on the
  //      sphere); by convention this is represented as a chain with no edges.
  //      (See S2LaxPolygonShape for details.)
  //
  // Note that this method allows degenerate geometry of different dimensions
  // to be distinguished, e.g. it allows a point to be distinguished from a
  // polyline or polygon that has been simplified to a single point.
  virtual int dimension() const = 0;

  // Returns true if the shape contains no points.  (Note that the full
  // polygon is represented as a chain with zero edges.)
  bool is_empty() const {
    return num_edges() == 0 && (dimension() < 2 || num_chains() == 0);
  }
  // Returns true if the shape contains all points on the sphere.
  bool is_full() const {
    return num_edges() == 0 && dimension() == 2 && num_chains() > 0;
  }

  // Returns an arbitrary point P along with a boolean indicating whether P is
  // contained by the shape.  (The boolean value must be false for shapes that
  // do not have an interior.)
  //
  // This ReferencePoint may then be used to compute the containment of other
  // points by counting edge crossings.
  virtual ReferencePoint GetReferencePoint() const = 0;

  // Returns the number of contiguous edge chains in the shape.  For example,
  // a shape whose edges are [AB, BC, CD, AE, EF] would consist of two chains
  // (AB,BC,CD and AE,EF).  Every chain is assigned a "chain id" numbered
  // sequentially starting from zero.
  //
  // Note that it is always acceptable to implement this method by returning
  // num_edges() (i.e. every chain consists of a single edge), but this may
  // reduce the efficiency of some algorithms.
  virtual int num_chains() const = 0;

  // Returns the range of edge ids corresponding to the given edge chain.  The
  // edge chains must form contiguous, non-overlapping ranges that cover the
  // entire range of edge ids.  This is spelled out more formally below:
  //
  // REQUIRES: 0 <= i < num_chains()
  // REQUIRES: chain(i).length >= 0, for all i
  // REQUIRES: chain(0).start == 0
  // REQUIRES: chain(i).start + chain(i).length == chain(i+1).start,
  //           for i < num_chains() - 1
  // REQUIRES: chain(i).start + chain(i).length == num_edges(),
  //           for i == num_chains() - 1
  virtual Chain chain(int chain_id) const = 0;

  // Returns the edge at offset "offset" within edge chain "chain_id".
  // Equivalent to "shape.edge(shape.chain(chain_id).start + offset)"
  // but may be more efficient.
  virtual Edge chain_edge(int chain_id, int offset) const = 0;

  // Finds the chain containing the given edge, and returns the position of
  // that edge as a (chain_id, offset) pair.
  //
  // REQUIRES: shape.chain(pos.chain_id).start + pos.offset == edge_id
  // REQUIRES: shape.chain(pos.chain_id + 1).start > edge_id
  //
  // where     pos == shape.chain_position(edge_id).
  virtual ChainPosition chain_position(int edge_id) const = 0;

  // A unique id assigned to this shape by GTShapeIndex.  Shape ids are
  // assigned sequentially starting from 0 in the order shapes are added.
  //
  // TODO(ericv): Consider eliminating this method.
  int id() const { return id_; }

  // Returns an integer that can be used to identify the type of an encoded
  // GTShape (see TypeTag above).
  virtual TypeTag type_tag() const { return kNoTypeTag; }

  // Virtual methods that return pointers of your choice.  These methods are
  // intended to help with the problem of attaching additional data to GTShape
  // objects.  For example, you could return a pointer to a source object, or
  // a pointer to a bundle of additional data allocated with the GTShape.
  // Because this method exists in all GTShapes, you can override it in each
  // type of shape you have and call it without knowing the concrete subtype.
  // For example, if you have polyline and polygon shapes, you can do this:
  //
  //   class MyPolyline : public S2Polyline::Shape {
  //    public:
  //     virtual void* mutable_user_data() { return &my_data_; }
  //    private:
  //     MyData my_data_;
  //   };
  //   class MyPolygon : public S2Polygon::Shape {
  //    public:
  //     virtual void* mutable_user_data() { return &my_data_; }
  //    private:
  //     MyData my_data_;
  //   };
  //   ...
  //   GTShape* shape = index.shape(id);
  //   const MyData* data = static_cast<const MyData*>(shape->user_data());
  //
  // This is not the only way to map from an GTShape back to your source
  // data.  Other reasonable techniques include:
  //
  //  - Every shape has an id() assigned by GTShapeIndex.  Ids are assigned
  //    sequentially starting from 0 in the order the shapes are added to the
  //    index.  You can use this id to look up arbitrary data stored in your
  //    own vector.
  //
  //  - If all of your shapes are the same type, then you can create your own
  //    subclass of some existing GTShape type (such as S2Polyline::Shape) and
  //    add your own methods and fields.  You can access this data by
  //    downcasting the GTShape pointers returned by GTShapeIndex methods.
  virtual const void* user_data() const { return nullptr; }
  virtual void* mutable_user_data() { return nullptr; }

 private:
  // Next available type tag available for use within the S2 library: 6.

  friend class EncodedGTShapeIndex;
  friend class MutableGTShapeIndex;

  int id_;  // Assigned by GTShapeIndex when the shape is added.

  GTShape(const GTShape&) = delete;
  void operator=(const GTShape&) = delete;
};

#endif //DISCRETEEARTH_GT_SHAPE_H
