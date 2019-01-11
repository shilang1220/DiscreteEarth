//
// Created by pgl on 2018/12/19.
//

#ifndef DISCRETEEARTH_GT_H
#define DISCRETEEARTH_GT_H

//S2支撑类库（s2）
//考虑到S2测试比较充分，直接采用S2中的一些基础类和函数定义
#include "s2/s1angle.h"                     //单位圆上的角类，用于表达经纬度等大圆弧上的角度，内部采用弧度作为角度单位
#include "s2/s1chord_angle.h"               //单位圆上的弦类，当大圆弧小到一定程度时，弦长等同于弧长
#include "s2/s1interval.h"                  //单位圆上的区间类，定义一段圆弧的起始角与结束角
#include "s2/s2centroids.h"                 //空间三角形或球面三角形的质心计算函数
#include "s2/s2earth.h"                     //地球球面模型类，定义了球面模型的各种属性，球面上各种角度、距离之间的转换和计算等函数
#include "s2/s2point.h"                     //基于地心三维坐标的点类，注意：表达球面点时，x,y,z应当是球面归一化的
// #include "s2/s2point_span.h"                //点串封装类，用于提供对不同点串实现方式（如：array、vector...)的统一访问接口
#include "s2/s2latlng.h"                    //经纬度类，用于经纬度坐标系下球面点的表达
#include "s2/s2measures.h"                  //球面角度和面积计算函数
#include "s2/s2pointutil.h"                 //球面点的常用计算函数
#include "s2/s2predicates.h"                //球面上点、线段等要素之间的距离、方向、角度等判断函数
#include "s2/s2projections.h"               //一些投影类型的定义


//GT核心类库（core）
//以下为抽象类
#include "core/region.h"                    //区域抽象类，各种球面区域类的抽象类，封装了球面区域的共性抽象接口
#include "core/gt_shape.h"                  //Shape抽象类，点、线、环、面等shape的抽象类，封装了访问shape的共性抽象接口
//以下为核心类
#include "core/gt_cap.h"                    //球冠类，用地心坐标系表达球面坐标范围，便于球面分析
#include "core/gt_latlng_rect.h"            //经纬度矩形类，用经纬度坐标系表达球面范围，便于经纬度分析
#include "core/gt_coords.h"                 //GT剖分方案中，各种坐标系之间的转换函数API
#include "core/gt_cell_id.h"                //GT剖分方案的网格单元编码类，对网格编码进行了封装
#include "core/gt_cell.h"                   //GT剖分方案的网格单元类，对剖分网格单元进行建模，内部封装了基于剖分编码的各种网格单元操作，如：转码、单元属性、测量、关系计算等
#include "core/gt_cell_union.h"             //GT剖分方案的网格集合类，对剖分网格单元集合进行建模，用于各种空间要素的空间范围网格化量化
#include "core/gt_metrics.h"                //网格的角度、长度和面积基本量化指标提取类

//GT基础类库（gt）
//所有空间要素都视为区域，GT实现了各类空间要素的网格化关联方法
#include "gt/gt_point.h"                  //Point要素类，定义了由传统的点坐标模型向剖分网格模型转换的方法
#include "gt/gt_multipoint.h"             //MultiPoint要素类，定义了由传统的多点坐标模型向剖分网格模型转换的方法
#include "gt/gt_polyline.h"               //Polyline要素类，定义了由传统的线坐标串模型向剖分网格模型转换的方法
#include "gt/gt_loop.h"                   //Loop要素类，定义了由传统的环坐标串模型向剖分网格模型转换的方法
#include "gt/gt_polygon.h"                //Polygon要素类，定义了由传统的面坐标串模型向剖分网格模型转换的方法
//#include "gt/gt_latlng_rect_bounder_generator.h"    //点、线、环、面、球冠等区域类对象的外包矩形计算器，用于计算给定点集合的外包矩形经纬度范围
//#include "gt/gt_region_cellunion_generator.h"       //点、线、环、面、球冠等区域类对象的剖分网格集合计算器，输入区域类对象和剖分参数，生成对应的剖分网格集合（ID集合）

//GT索引类库（index）
//实现了点数据索引（用于POI数据集等）、区域数据索引（用于图幅或要素的范围索引）和要素索引（索引到组成要素的边）三种形式
//同时支持为外部数据组织管理系统生成地理编码
#include "index/gt_point_index.h"             //点类型空间要素（索引对象为点数据，如POI点）的专用索引模型，（<cellid，PointData>的二叉树索引结构），用于POI点的邻近检索
#include "index/gt_region_index.h"            //区域型空间要素的范围索引模型（索引对象为RegionLabel），（<cellid,RegionLabel>的顺序索引结构），通过对cellid的邻近或区域查询，获得与该cellid关联的空间要素Label集合
#include "index/gt_shape_inex.h"              //点、线、面等空间要素索引模型，（<cellid, Edges>的map索引结构），每个cellid中关联的对象是空间要素的edges
#include "index/gt_region_term_generator.h"   //空间要素的地理标签生成类，用于为全文检索系统提供地理标签，采用“全文检索”+“文本地理标签”的方式实现信息的空间检索能力

//GT查询检索类库（query）
//#include
#endif //DISCRETEEARTH_GT_H
