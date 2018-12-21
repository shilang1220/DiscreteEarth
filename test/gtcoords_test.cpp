//
// Created by Administrator on 2018/12/17 0017.
//
#include "gtest/gtest.h"
#include "gt.h"

TEST(GTCOORDS_TEST, ll){
    double lng, lat;
    lng = 180.00;lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng,lat,&id);
    std::cout << std::hex<<id << std::endl;
    EXPECT_EQ(0X0,id);
}