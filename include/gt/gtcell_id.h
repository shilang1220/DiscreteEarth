//
// Created by 濮国梁 on 2018/12/13.
//

#ifndef DISCRETEEARTH_GTCELLID_H
#define DISCRETEEARTH_GTCELLID_H

#include "core/cell_id.h"
#include "s2/s2latlng.h"
#include "gt/gtcoords.h"

#include "exports.h"

DE_API class GTCellId final : public CellId{

public:
    ///////////////////////////////////////////////
    // 基本参数定义
    //////////////////////////////////////////////
    static const int kMaxLevel = GT::kMaxCellLevel;     // 有效层数    31层   Valid levels: 0..kMaxLevel
    static const int kPosBits = 2 * kMaxLevel;          // 有效bit数   62位   最末尾两个bit只用于区分层级（以'10'结尾表示第31层网格编码）
    static const int kMaxSize = 1 << kMaxLevel;         // X/Y方向最大尺寸    2**31次方

    //////////////////////////////////////////////
    // 编码示例： 下面两个字节能够表示7级网格
    // 0b 0010 0000 0000 0000  第2级的第3个子网格
    // 0b 1101 1100 1000 0000  第4级的第1个子网格
    // 0b 1101 1100 1110 0000  第5级的第4个子网格
    // 0b 1101 1100 1110 1000  第6级的第3个子网格
    // 0b 1101 1100 1110 0010  第7级的第1个子网格
    // 0b 1101 1100 1110 0110  第7级的第2个子网格
    // 0b 1101 1100 1110 1010  第7级的第3个子网格
    // 0b 1101 1100 1110 1110  第7级的第4个子网格
    // 0b 1101 1100 1110 1100  非法网格编码
    // 0b 1101 1100 1110 0100  非法网格编码
    // 0b 1101 1100 1110 1000  合法网格编码，只能被解释为第6级的第3个子网格
    // 0b 1101 1100 1110 1100  非法网格编码
    // 0b 1101 1100 1100 0000  非法网格编码
    // 0b 1101 1100 1101 0000  非法网格编码
    // 0b 1101 1100 1111 0000  非法网格编码
    /////////////////////////////////////////////

    ///////////////////////////////////////////////
    // 构造函数
    //////////////////////////////////////////////

    explicit GTCellId(const uint64 id){id_ = id;}

    // Construct a leaf cell containing the given point "p".
    explicit GTCellId(const S2Point p);

    // Construct a leaf cell containing the given S2LatLng.
    explicit GTCellId(const S2LatLng ll);


    /************************************
    *  GEOSOT网格ID与球面坐标之间的转换函数
    ************************************/
    // 返回网格中心点对应的球面坐标
    // Return the direction vector corresponding to the center of the given
    // cell.  The vector returned by ToPointRaw is not necessarily unit length.
    // This method returns the same result as S2Cell::GetCenter().
    //
    // The maximum directional error in ToPoint() (compared to the exact
    // mathematical result) is 1.5 * DBL_EPSILON radians, and the maximum length
    // error is 2 * DBL_EPSILON (the same as Normalize).
    S2Point  ToPoint() const override;
    // 根据球面坐标创建对应的网格ID, point为地心矢量，ID对应其与球面的交点
    bool  FromPoint(S2Point point) const override;
    bool  FromPoint(S2Point point, unsigned int level) const override;

    /************************************
      *  GEOSOT网格ID与经纬度之间的转换函数
      ************************************/
    // 返回网格中心点对应的经纬度
    // Return the S2LatLng corresponding to the center of the given cell.
    S2LatLng ToLatLng() const override;
    //  根据经纬度，生成对应的网格ID
    bool FromLatLng(S2LatLng latLng) const override;
    bool FromLatLng(S2LatLng latLng, unsigned int level) const override;

    /************************************
 *  网格ID属性访问函数
 ************************************/
    // Return true if id() represents a valid cell.
    bool is_valid() const override;

    // 网格ID所在的曲线位置
    uint64 pos() const override;

    // 网格ID所在的层级
    int level() const override ;

    // 网格ID是否对应叶子节点
    // Return true if this is a leaf cell (more efficient than checking
    // whether level() == kMaxLevel).
    bool is_leaf() const override;

    // 返回该网格编码的最低有效位
    // Return the lowest-numbered bit that is on for this cell id, which is
    // equal to (uint64{1} << (2 * (kMaxLevel - level))).  So for example,
    // a.lsb() <= b.lsb() if and only if a.level() >= b.level(), but the
    // first test is more efficient.
    uint64 lsb() const ;

    // 网格ID在其父节点中的位置（0..3）
    // Return the child position (0..3) of this cell within its parent.
    // REQUIRES: level() >= 1.
    int child_position() const override;

    // 网格ID的某个层级祖先节点在其父节点中的位置
    // Return the child position (0..3) of this cell's ancestor at the given
    // level within its parent.  For example, child_position(1) returns the
    // position of this cell's level-1 ancestor within its top-level face cell.
    // REQUIRES: 1 <= level <= this->level().
    int child_position(int level) const override;

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
    // 本网格包含的子网格中ID的最小值    
    uint64 range_min() const override ;
    GTCellId range_min_cell() const ;

    // 本网格包含的子网格中ID的最大值
    uint64 range_max() const override ;
    GTCellId range_max_cell() const ;

    /************************************
    *  网格空间关系计算函数
    ************************************/
    // 判断本网格是否包含某个网格
    // Return true if the given cell is contained within this one.
    bool contains(const uint64 other) const override ;
    bool contains(const GTCellId other) const ;

    //判断本网格是否某个网格other的子孙网格
    bool isChildOf(const uint64 other) const override ;
    bool isChildOf(const GTCellId other) const ;

    // 判断本网格是否和某个网格相交
    // Return true if the given cell intersects this one.
    bool intersects(const uint64 other) const override ;
    bool intersects(const GTCellId other) const ;


    /************************************
    *  祖先节点或子孙节点访问函数
    ************************************/
    // 返回祖先网格的ID
    uint64 parentID() const override ;
    // 返回某个层级祖先网格的ID
    uint64 parentID(int level) const override;

    // 返回某个子网格的ID（0..3）
    // Return the immediate child of this cell at the given traversal order
    // position (in the range 0 to 3).  This cell must not be a leaf cell.
    uint64 child(int position) const override ;

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
    uint64 child_begin() const override ;
    uint64 child_end() const override ;

    uint64 child_begin(int level) const override ;
    uint64 child_end(int level) const override ;

    // 指定层级子孙中，沿曲线的下一个子节点或上一个子节点ID
    // Return the next/previous cell at the same level along the Hilbert curve.
    // Works correctly when advancing from one face to the next, but
    // does *not* wrap around from the last face to the first or vice versa.
    uint64 next() const override ;
    uint64 prev() const override ;

    // 在当前层级中，沿着曲线向前跳steps部的网格ID
    // This method advances or retreats the indicated number of steps along the
    // Hilbert curve at the current level, and returns the new position.  The
    // position is never advanced past End() or before Begin().
    uint64 advance(int64 steps) const override ;

    // 在当前层级中，本网格ID距曲线起点的步长
    // Returns the number of steps that this cell is from Begin(level()). The
    // return value is always non-negative.
    int64 distance_from_begin() const override ;

//    // Like next() and prev(), but these methods wrap around from the last face
//    // to the first and vice versa.  They should *not* be used for iteration in
//    // conjunction with child_begin(), child_end(), Begin(), or End().  The
//    // input must be a valid cell id.
//    virtual  CellId* next_wrap() const = 0 ;
//    virtual  CellId* prev_wrap() const = 0 ;

//    // This method advances or retreats the indicated number of steps along the
//    // Hilbert curve at the current level, and returns the new position.  The
//    // position wraps between the first and last faces as necessary.  The input
//    // must be a valid cell id.
//    virtual  CellId* advance_wrap(int64 steps) const = 0 ;

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
    uint64 maximum_tile(uint64 limit) const override;

    // Returns the level of the lowest common ancestor of this cell and "other",
    // that is, the maximum level such that parent(level) == other.parent(level).
    // Returns -1 if the two cells do not have any common ancestor (i.e., they
    // are from different faces).
    int GetCommonAncestorLevel(uint64 other) const override ;
    int GetCommonAncestorLevel(GTCellId other ) const;

    // Iterator-style methods for traversing all the cells along the Hilbert
    // curve at a given level (across all 6 faces of the cube).  Note that the
    // end value is exclusive (just like standard STL iterators), and is not a
    // valid cell id.
    uint64 Begin(int level) override;
    uint64 End(int level) override;

    /************************************
  *  邻居节点访问函数
  ************************************/

    // 返回本网格的四邻域网格ID
    // Return the four cells that are adjacent across the cell's four edges.
    // Neighbors are returned in the order defined by S2Cell::GetEdge.  All
    // neighbors are guaranteed to be distinct.
    void GetFourNeighbors(uint64 neighbors[4]) const override ;
    void GetEightNeighbors(uint64 neighbors[8]) const override;

    // 在指定的某个父层级中，与距离本网格最近的格点相邻的所有网格
    // Return the neighbors of closest vertex to this cell at the given level,
    // by appending them to "output".  Normally there are four neighbors, but
    // the closest vertex may only have three neighbors if it is one of the 8
    // cube vertices.
    //
    // Requires: level < this->level(), so that we can determine which vertex is
    // closest (in particular, level == kMaxLevel is not allowed).
    void AppendVertexNeighbors(int level, std::vector<uint64>* output) const override ;

    // 在指定的某个子层级中，与本网格相邻的所有网格
    // Append all neighbors of this cell at the given level to "output".  Two
    // cells X and Y are neighbors if their boundaries intersect but their
    // interiors do not.  In particular, two cells that intersect at a single
    // point are neighbors.

    // REQUIRES: nbr_level >= this->level().
    void AppendAllNeighbors(int nbr_level, std::vector<CellId>* output) const override ;


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
    string ToToken() const  override ;
    uint64 FromToken(const char* token, size_t length) override ;
    uint64 FromToken(const string& token) override;

    /************************************
    *  编码串行化输入输出函数
    ************************************/
    // Use encoder to generate a serialized representation of this cell id.
    // Can also encode an invalid cell.
    void Encode(Encoder* const encoder) const override ;

    // Decodes an CellId encoded by Encode(). Returns true on success.
    bool Decode(Decoder* const decoder) override;

    // Creates a human readable debug string.  Used for << and available for
    // direct usage as well.  The format is "f/dd..d" where "f" is a digit in
    // the range [0-5] representing the CellId face, and "dd..d" is a string
    // of digits in the range [0-3] representing each child's position with
    // respect to its parent.  (Note that the latter string may be empty.)
    //
    // For example "4/" represents CellId::FromFace(4), and "3/02" represents
    // CellId::FromFace(3).child(0).child(2).
    string ToString() const override ;


public:

};

//重载逻辑表达运算的全局函数
DE_API inline bool operator==(GTCellId x, GTCellId y) {
    return x.id() == y.id();
}

DE_API inline bool operator!=(GTCellId x, GTCellId y) {
    return x.id() != y.id();
}

DE_API inline bool operator<(GTCellId x, GTCellId y) {
    return x.id() < y.id();
}

DE_API inline bool operator>(GTCellId x, GTCellId y) {
    return x.id() > y.id();
}

DE_API inline bool operator<=(GTCellId x, GTCellId y) {
    return x.id() <= y.id();
}

DE_API inline bool operator>=(GTCellId x, GTCellId y) {
    return x.id() >= y.id();
}

#endif //DISCRETEEARTH_GTCELLID_H
