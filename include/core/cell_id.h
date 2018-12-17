//
// Created by pgl on 2018/12/10.
//

#ifndef DISCRETEEARTH_CELLID_H
#define DISCRETEEARTH_CELLID_H

#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "base/integral_types.h"
#include "base/logging.h"
#include "base/port.h"
#include "_fp_contract_off.h"
#include "s2/r2.h"
#include "s2/r2rect.h"
#include "s2/s1angle.h"
#include "third_party/absl/strings/string_view.h"
#include "util/bits/bits.h"
#include "util/coding/coder.h"
#include "exports.h"
class S2LatLng;

//CellId 是球面网格编码的抽象类，用户可定义自己的编码类

// An CellId is a 64-bit unsigned integer that uniquely identifies a
// cell in the S2 cell decomposition.  It has the following format:
//
//   id = [face][face_pos]
//
//   face:     a 3-bit number (range 0..5) encoding the cube face.
//
//   face_pos: a 61-bit number encoding the position of the center of this
//             cell along the Hilbert curve over this face (see the Wiki
//             pages for details).
//
// Sequentially increasing cell ids follow a continuous space-filling curve
// over the entire sphere.  They have the following properties:
//
//  - The id of a cell at level k consists of a 3-bit face number followed
//    by k bit pairs that recursively select one of the four children of
//    each cell.  The next bit is always 1, and all other bits are 0.
//    Therefore, the level of a cell is determined by the position of its
//    lowest-numbered bit that is turned on (for a cell at level k, this
//    position is 2 * (kMaxLevel - k).)
//
//  - The id of a parent cell is at the midpoint of the range of ids spanned
//    by its children (or by its descendants at any level).
//
// Leaf cells are often used to represent points on the unit sphere, and
// this class provides methods for converting directly between these two
// representations.  For cells that represent 2D regions rather than
// discrete point, it is better to use the S2Cell class.
//
// This class is intended to be copied by value as desired.  It uses
// the default copy constructor and assignment operator.
DE_API class CellId {
public:
    // The extra position bit (61 rather than 60) let us encode each cell as its
    // Hilbert curve position at the cell center (which is halfway along the
    // portion of the Hilbert curve that fills that cell).

    CellId(){}
    explicit CellId(uint64 id) : id_(id) {}

       /************************************
     *  网格ID与球面坐标之间的转换函数
     ************************************/
    // 返回面片中心点对应的球面坐标
    // Return the direction vector corresponding to the center of the given
    // cell.  The vector returned by ToPointRaw is not necessarily unit length.
    // This method returns the same result as S2Cell::GetCenter().
    //
    // The maximum directional error in ToPoint() (compared to the exact
    // mathematical result) is 1.5 * DBL_EPSILON radians, and the maximum length
    // error is 2 * DBL_EPSILON (the same as Normalize).
    virtual S2Point  ToPoint() const = 0 ;
    // 根据球面坐标创建对应的网格ID
    virtual bool  FromPoint(S2Point point) const = 0;
    // 根据球面坐标和指定层级，创建对应的网格ID
    virtual bool  FromPoint(S2Point point, unsigned int level) const = 0;

    /************************************
      *  网格ID与经纬度之间的转换函数
      ************************************/
    // 返回面片中心点对应的经纬度
    // Return the S2LatLng corresponding to the center of the given cell.
    virtual  S2LatLng ToLatLng() const = 0;
    //  根据经纬度，生成对应的网格ID
    virtual  bool FromLatLng(S2LatLng latLng) const = 0;
//  根据经纬度和指定层级，生成对应的网格ID
    virtual  bool FromLatLng(S2LatLng latLng, unsigned int level) const = 0;

    /************************************
   *  网格ID属性访问函数
   ************************************/
    // 返回面片ID
    // The 64-bit unique identifier for this cell.
    uint64 id() const { return id_; }

    // Return true if id() represents a valid cell.
    //
    // All methods require is_valid() to be true unless otherwise specified
    // (although not all methods enforce this).
    virtual bool is_valid() const = 0;

    // 网格ID所在的曲线位置
    // The position of the cell center along the Hilbert curve over this face,
    // in the range 0..(2**kPosBits-1).
    virtual uint64 pos() const = 0;

    // 网格ID所在的层级
    // Return the subdivision level of the cell (range 0..kMaxLevel).
    virtual int level() const = 0;

    // 网格ID是否对应叶子节点
    // Return true if this is a leaf cell (more efficient than checking
    // whether level() == kMaxLevel).
    virtual bool is_leaf() const = 0;

    // 网格ID在其父节点中的位置（0..3）
    // Return the child position (0..3) of this cell within its parent.
    // REQUIRES: level() >= 1.
    virtual int child_position() const = 0;

    // 网格ID的某个层级祖先节点在其父节点中的位置
    // Return the child position (0..3) of this cell's ancestor at the given
    // level within its parent.  For example, child_position(1) returns the
    // position of this cell's level-1 ancestor within its top-level face cell.
    // REQUIRES: 1 <= level <= this->level().
    virtual int child_position(int level) const = 0;

    // These methods return the range of cell ids that are contained within this
    // cell (including itself).  The range is *inclusive* (i.e. test using >=
    // and <=) and the return values of both methods are valid leaf cell ids.
    // In other words, a.contains(b) if and only if
    //
    //     (b >= a.range_min() && b <= a.range_max())
    //
    // If you want to iterate through all the descendants of this cell at a
    // particular level, use child_begin(level) and child_end(level) instead.
    // Also see maximum_tile(), which can be used to iterate through a range of
    // cells using CellIDs at different levels that are as large as possible.
    //
    // If you need to convert the range to a semi-open interval [min, limit)
    // (e.g., in order to use a key-value store that only supports semi-open
    // range queries), do not attempt to define "limit" as range_max.next().
    // The problem is that leaf CellIDs are 2 units apart, so the semi-open
    // interval [min, limit) includes an additional value (range_max.id() + 1)
    // which is happens to be a valid CellId about one-third of the time and
    // is *never* contained by this cell.  (It always correpsonds to a cell that
    // is larger than this one.)  You can define "limit" as (range_max.id() + 1)
    // if necessary (which is not always a valid CellId but can still be used
    // with FromToken/ToToken), or you can convert range_max() to the key space
    // of your key-value store and define "limit" as Successor(key).
    //
    // Note that Sentinel().range_min() == Sentinel.range_max() == Sentinel().
    // 本网格包含的子面片中ID的最小值
    // 本网格包含的子面片中ID的最大值
    virtual uint64 range_min() const = 0 ;
    virtual uint64 range_max() const = 0 ;

    /************************************
    *  网格空间关系计算函数
    ************************************/
    // 判断本网格是否包含某个网格（other是否自己的子孙网格）
    // Return true if the given cell is contained within this one.
    virtual bool contains(const uint64 other) const = 0 ;

    //判断本网格是否某个网格other的子孙网格
    virtual bool isChildOf(const uint64 other) const = 0 ;

    // 判断本网格是否和某个网格相交
    // Return true if the given cell intersects this one.
    virtual bool intersects(const uint64 other) const = 0 ;



    /************************************
    *  祖先节点或子孙节点访问函数
    ************************************/
    // 返回父面片的ID
    // Return the cell at the previous level or at the given level (which must
    // be less than or equal to the current level).
   virtual uint64 parentID() const = 0 ;
   // 返回某个层级父面片的ID
   virtual uint64 parentID(int level) const = 0 ;

    // 返回某个子面片的ID（0..3）
    // Return the immediate child of this cell at the given traversal order
    // position (in the range 0 to 3).  This cell must not be a leaf cell.
    virtual uint64 child(int position) const = 0 ;

    // Iterator-style methods for traversing the immediate children of a cell or
    // all of the children at a given level (greater than or equal to the current
    // level).  Note that the end value is exclusive, just like standard STL
    // iterators, and may not even be a valid cell id.  You should iterate using
    // code like this:
    //
    //   for(uint64 c = id.child_begin(); c != id.child_end(); c = c.next())
    //     ...
    //
    // The convention for advancing the iterator is "c = c.next()" rather
    // than "++c" to avoid possible confusion with incrementing the
    // underlying 64-bit cell id.
    virtual uint64 child_begin() const = 0 ;
    virtual uint64 child_begin(int level) const = 0 ;
    virtual uint64 child_end() const = 0 ;
    virtual uint64 child_end(int level) const = 0 ;

    // 指定层级子孙中，沿曲线的下一个子节点或上一个子节点ID
    // Return the next/previous cell at the same level along the Hilbert curve.
    // Works correctly when advancing from one face to the next, but
    // does *not* wrap around from the last face to the first or vice versa.
    virtual  uint64 next() const = 0 ;
    virtual uint64 prev() const = 0 ;

    // 在当前层级中，沿着曲线向前跳steps部的网格ID
    // This method advances or retreats the indicated number of steps along the
    // Hilbert curve at the current level, and returns the new position.  The
    // position is never advanced past End() or before Begin().
    virtual  uint64 advance(int64 steps) const = 0 ;

    // 在当前层级中，本网格ID距曲线起点的步长
    // Returns the number of steps that this cell is from Begin(level()). The
    // return value is always non-negative.
    virtual int64 distance_from_begin() const = 0 ;

    // Return the largest cell with the same range_min() and such that
    // range_max() < limit.range_min().  Returns "limit" if no such cell exists.
    // This method can be used to generate a small set of CellIDs that covers
    // a given range (a "tiling").  This example shows how to generate a tiling
    // for a semi-open range of leaf cells [start, limit):
    //
    //   for (CellId id = start.maximum_tile(limit);
    //        id != limit; id = id.next().maximum_tile(limit)) { ... }
    //
    // Note that in general the cells in the tiling will be of different sizes;
    // they gradually get larger (near the middle of the range) and then
    // gradually get smaller (as "limit" is approached).
    virtual uint64 maximum_tile(uint64 limit) const = 0;

    //返回本网格和指定网格other的共同祖先所在的层
    // Returns the level of the lowest common ancestor of this cell and "other",
    // that is, the maximum level such that parent(level) == other.parent(level).
    // Returns -1 if the two cells do not have any common ancestor (i.e., they
    // are from different faces).
    virtual int GetCommonAncestorLevel(uint64 other) const = 0 ;

    // 迭代器风格的函数，可用于遍历访问某一层级的所有网格
    // Iterator-style methods for traversing all the cells along the Hilbert
    // curve at a given level (across all 6 faces of the cube).  Note that the
    // end value is exclusive (just like standard STL iterators), and is not a
    // valid cell id.
    virtual uint64 Begin(int level) = 0;
    virtual uint64 End(int level) = 0;

     /************************************
    *  邻居节点访问函数
    ************************************/

    // 返回本网格的四邻域网格ID
    // Return the four cells that are adjacent across the cell's four edges.
    // Neighbors are returned in the order defined by S2Cell::GetEdge.  All
    // neighbors are guaranteed to be distinct.
    virtual void GetFourNeighbors(uint64 neighbors[4]) const = 0 ;
    virtual void GetEightNeighbors(uint64 neighbors[8]) const = 0 ;

    // 在指定的某个父层级中，与距离本网格最近的格点相邻的所有网格
    // Return the neighbors of closest vertex to this cell at the given level,
    // by appending them to "output".  Normally there are four neighbors, but
    // the closest vertex may only have three neighbors if it is one of the 8
    // cube vertices.
    //
    // Requires: level < this->level(), so that we can determine which vertex is
    // closest (in particular, level == kMaxLevel is not allowed).
    virtual void AppendVertexNeighbors(int level, std::vector<uint64>* output) const = 0 ;

    // 在指定的某个子层级中，与本网格相邻的所有网格
    // Append all neighbors of this cell at the given level to "output".  Two
    // cells X and Y are neighbors if their boundaries intersect but their
    // interiors do not.  In particular, two cells that intersect at a single
    // point are neighbors.

    // REQUIRES: nbr_level >= this->level().
    virtual void AppendAllNeighbors(int nbr_level, std::vector<CellId>* output) const = 0 ;


    /************************************
    *  编码转换函数
    ************************************/
    // Methods to encode and decode cell ids to compact text strings suitable
    // for display or indexing.  Cells at lower levels (i.e. larger cells) are
    // encoded into fewer characters.  The maximum token length is 16.
    //
    // Tokens preserve ordering, i.e. ToToken(x) < ToToken(y) iff x < y.
    //
    // ToToken() returns a string by value for convenience; the compiler
    // does this without intermediate copying in most cases.
    //
    // These methods guarantee that FromToken(ToToken(x)) == x even when
    // "x" is an invalid cell id.  All tokens are alphanumeric strings.
    // FromToken() returns CellId::None() for malformed inputs.
    virtual string ToToken() const  = 0 ;
    virtual  uint64 FromToken(const char* token, size_t length) = 0 ;
    virtual  uint64 FromToken(const string& token) = 0 ;

    /************************************
    *  编码串行化输入输出函数
    ************************************/
    // Use encoder to generate a serialized representation of this cell id.
    // Can also encode an invalid cell.
    virtual  void Encode(Encoder* const encoder) const = 0 ;

    // Decodes an CellId encoded by Encode(). Returns true on success.
    virtual bool Decode(Decoder* const decoder) = 0 ;

    // Creates a human readable debug string.  Used for << and available for
    // direct usage as well.  The format is "f/dd..d" where "f" is a digit in
    // the range [0-5] representing the CellId face, and "dd..d" is a string
    // of digits in the range [0-3] representing each child's position with
    // respect to its parent.  (Note that the latter string may be empty.)
    //
    // For example "4/" represents CellId::FromFace(4), and "3/02" represents
    // CellId::FromFace(3).child(0).child(2).
    virtual string ToString() const = 0 ;


    /////////////////////////////////////////////////////////////////////
    // Low-level methods.

    // When CellId is used as a key in one of the btree container types
    // (util/btree), indicate that linear rather than binary search should be
    // used.  This is much faster when the comparison function is cheap.
    typedef std::true_type goog_btree_prefer_linear_node_search;

protected:
   uint64 id_;
};

#endif //DISCRETEEARTH_CELLID_H
