//
// Created by pgl on 2018/12/19.
//

#ifndef DISCRETEEARTH_GT_H
#define DISCRETEEARTH_GT_H

//核心类库（core）
#include "core/region.h"                    //区域类，各种球面区域类的抽象类，封装了球面区域的共性抽象接口
#include "core/cell_id.h"                   //网格单元编码类，球面网格单元编码的抽象类，封装了球面网格单元编码的共性抽象接口
#include "core/cell.h"                      //网格单元类，球面网格单元的抽象类，封装了球面网格单元的共性抽象接口
#include "core/gt_shape.h"                  //Shape类，点、线、环、面等shape的抽象类，封装了访问shape的共性抽象接口
#include "core/gt_cap.h"                    //球冠类，用地心坐标系表达球面坐标范围
#include "core/gt_latlng_rect.h"            //经纬度矩形类，用经纬度坐标系表达球面范围
#include "core/gt_coords.h"                 //GT剖分方案中，各种坐标系之间的转换函数API
#include "core/gt_cell_id.h"                //GT剖分方案的网格单元编码类，对网格编码进行了封装
#include "core/gt_cell.h"                   //GT剖分方案的网格单元类，对剖分网格单元进行建模，内部封装了基于剖分编码的各种网格单元操作，如：转码、单元属性、测量、关系计算等
//#include "core/gt_cell_union.h"           //GT剖分方案的网格集合类，对剖分网格单元集合进行建模，用于各种空间要素的空间范围网格化量化


//#include "core/gt_metrics.h"



//S2支撑类库（s2）
#include "s2/s1angle.h"
#include "s2/s2point.h"
#include "s2/s2latlng.h"

//GT基础类库（gt）
#include "gt/gt_point.h"                  //Point要素类，定义了由传统的点坐标模型向剖分网格模型转换的方法
#include "gt/gt_multipoint.h"             //MultiPoint要素类，定义了由传统的多点坐标模型向剖分网格模型转换的方法
#include "gt/gt_polyline.h"               //Polyline要素类，定义了由传统的线坐标串模型向剖分网格模型转换的方法
#include "gt/gt_loop.h"                   //Loop要素类，定义了由传统的环坐标串模型向剖分网格模型转换的方法
#include "gt/gt_polygon.h"                //Polygon要素类，定义了由传统的面坐标串模型向剖分网格模型转换的方法
#include "gt/gt_latlng_rect_bounder.h"    //点、线、环、面、球冠等区域类对象的外包矩形计算器，用于计算给定点集合的外包矩形经纬度范围

//#include "gt/gt_region_coverer.h"       //点、线、环、面、球冠等区域类对象的剖分网格集合计算器，输入区域类对象和剖分参数，生成对应的剖分网格集合（ID集合）

//GT索引类库（index）
#include "index/gt_point_index.h"             //点类型空间要素（索引对象为点数据，如POI点）的专用索引模型，（<cellid，PointData>的二叉树索引结构），用于POI点的邻近检索
#include "index/gt_region_index.h"            //区域型空间要素的范围索引模型（索引对象为RegionLabel），（<cellid,RegionLabel>的顺序索引结构），通过对cellid的邻近或区域查询，获得与该cellid关联的空间要素Label集合
#include "index/gt_shape_inex.h"              //点、线、面等空间要素索引模型，（<cellid, Edges>的map索引结构），每个cellid中关联的对象是空间要素的edges
#include "index/gt_region_term_generator.h"   //空间要素的地理标签生成类，用于为全文检索系统提供地理标签，采用“全文检索”+“文本地理标签”的方式实现信息的空间检索能力

//GT查询检索类库（query）
#include
#endif //DISCRETEEARTH_GT_H
