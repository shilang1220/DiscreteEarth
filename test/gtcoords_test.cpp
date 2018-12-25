//
// Created by Administrator on 2018/12/17 0017.
//
#include "gtest/gtest.h"
#include "gt.h"

//经纬度向二维网格坐标(叶子节点)的转换测试
TEST(GTCOORDS_TEST, LLtoIJ){
    double lng, lat;
    uint32 I,J;

    lng = 180.00;lat = 90.00;
    GT::LLtoIJ(lng,lat,&I,&J);
    std::cout << std::hex<<I << std::endl;
    std::cout << std::hex<<J << std::endl;
    EXPECT_EQ(uint32(180)<<23,I);
    EXPECT_EQ(uint32(90)<<23,J);

    lng = -180.00;lat = 90.00;
    GT::LLtoIJ(lng,lat,&I,&J);
    std::cout << std::hex<<I << std::endl;
    std::cout << std::hex<<J << std::endl;
    EXPECT_EQ(0X80000000|(uint32(180)<<23),I);
    EXPECT_EQ(uint32(90)<<23,J);

    lng = 180.00;lat = -90.00;
    GT::LLtoIJ(lng,lat,&I,&J);
    std::cout << std::hex<<I << std::endl;
    std::cout << std::hex<<J << std::endl;
    EXPECT_EQ(uint32(180)<<23,I);
    EXPECT_EQ(0X80000000|(uint32(90)<<23),J);

    lng = -180.00;lat = -90.00;
    GT::LLtoIJ(lng,lat,&I,&J);
    std::cout << std::hex<<I << std::endl;
    std::cout << std::hex<<J << std::endl;
    EXPECT_EQ(0X80000000|(uint32(180)<<23),I);
    EXPECT_EQ(0X80000000|(uint32(90)<<23),J);

}


//二维网格坐标（1-31级网格）向经纬度（网格的质心）的转换测试
TEST(GTCOORDS_TEST, IJtoLL){
    double lng, lat;
    uint32 I,J;

    I = 0x5a000000;J=0x2d000000;
    GT::IJtoLL(I,J,&lng,&lat);
    std::cout << lng << std::endl;
    std::cout << lat << std::endl;
    EXPECT_EQ(180,lng);
    EXPECT_EQ(90,lat);

    I = 0xda000000;J=0x2d000000;
    GT::IJtoLL(I,J,&lng,&lat);
    std::cout << lng << std::endl;
    std::cout << lat << std::endl;
    EXPECT_EQ(-180,lng);
    EXPECT_EQ(90,lat);

    I = 0x5a000000;J=0xad000000;
    GT::IJtoLL(I,J,&lng,&lat);
    std::cout << lng << std::endl;
    std::cout << lat << std::endl;
    EXPECT_EQ(180,lng);
    EXPECT_EQ(-90,lat);

    I = 0xda000000;J=0xad000000;
    GT::IJtoLL(I,J,&lng,&lat);
    std::cout << lng << std::endl;
    std::cout << lat << std::endl;
    EXPECT_EQ(-180,lng);
    EXPECT_EQ(-90,lat);
}


/*
//地心坐标系向二维网格坐标的转换测试
TEST(GTCOORDS_TEST, XYZtoIJ){
    double lng, lat;
    lng = 180.00;lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng,lat,&id);
    std::cout << std::hex<<id << std::endl;
    EXPECT_EQ(0X0,id);
}

//二维网格坐标（叶子节点）向地心坐标系(叶子网格的质心坐标）的转换测试
TEST(GTCOORDS_TEST, IJtoXYZ){
    double lng, lat;
    lng = 180.00;lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng,lat,&id);
    std::cout << std::hex<<id << std::endl;
    EXPECT_EQ(0X0,id);
}

//二维网格坐标（所有网格，不一定是叶子网格）向一维编码的转换测试
TEST(GTCOORDS_TEST, IJtoID){
    double lng, lat;
    lng = 180.00;lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng,lat,&id);
    std::cout << std::hex<<id << std::endl;
    EXPECT_EQ(0X0,id);
}

//一维编码向二维网格坐标（所有网格，不一定是叶子网格）的转换测试
TEST(GTCOORDS_TEST, IDtoIJ){
    double lng, lat;
    lng = 180.00;lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng,lat,&id);
    std::cout << std::hex<<id << std::endl;
    EXPECT_EQ(0X0,id);
}

//经纬度向一维编码（叶子网格）的转换测试
TEST(GTCOORDS_TEST, LLtoID){
    double lng, lat;
    lng = 180.00;lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng,lat,&id);
    std::cout << std::hex<<id << std::endl;
    EXPECT_EQ(0X0,id);
}
//一维编码（不一定是叶子网格）向经纬度（网格质心）的转换测试
TEST(GTCOORDS_TEST, IDtoLL){
    double lng, lat;
    lng = 180.00;lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng,lat,&id);
    std::cout << std::hex<<id << std::endl;
    EXPECT_EQ(0X0,id);
}

//地心坐标向一维编码（叶子网格）的转换测试
TEST(GTCOORDS_TEST, LLtoID){
    double lng, lat;
    lng = 180.00;lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng,lat,&id);
    std::cout << std::hex<<id << std::endl;
    EXPECT_EQ(0X0,id);
}
//一维编码（不一定是叶子网格）向地心坐标（网格质心）的转换测试
TEST(GTCOORDS_TEST, IDtoLL){
    double lng, lat;
    lng = 180.00;lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng,lat,&id);
    std::cout << std::hex<<id << std::endl;
    EXPECT_EQ(0X0,id);
}

*/
