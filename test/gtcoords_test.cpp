//
// Created by Administrator on 2018/12/17 0017.
//
#include "gtest/gtest.h"
#include "gt.h"
#include "tools/s2testing.h"

//经纬度向二维网格坐标(叶子节点)的转换测试
TEST(GTCOORDS_TEST, LLtoIJ) {
    double lng, lat;
    uint32 I, J;

    lng = 180.00;
    lat = 90.00;
    GT::LLtoIJ(lng, lat, &I, &J);
    std::cout << std::hex << I << std::endl;
    std::cout << std::hex << J << std::endl;
    EXPECT_EQ(uint32(180) << 23, I);
    EXPECT_EQ(uint32(90) << 23, J);

    lng = -180.00;
    lat = 90.00;
    GT::LLtoIJ(lng, lat, &I, &J);
    std::cout << std::hex << I << std::endl;
    std::cout << std::hex << J << std::endl;
    EXPECT_EQ(0X80000000 | (uint32(180) << 23), I);
    EXPECT_EQ(uint32(90) << 23, J);

    lng = 180.00;
    lat = -90.00;
    GT::LLtoIJ(lng, lat, &I, &J);
    std::cout << std::hex << I << std::endl;
    std::cout << std::hex << J << std::endl;
    EXPECT_EQ(uint32(180) << 23, I);
    EXPECT_EQ(0X80000000 | (uint32(90) << 23), J);

    lng = -180.00;
    lat = -90.00;
    GT::LLtoIJ(lng, lat, &I, &J);
    std::cout << std::hex << I << std::endl;
    std::cout << std::hex << J << std::endl;
    EXPECT_EQ(0X80000000 | (uint32(180) << 23), I);
    EXPECT_EQ(0X80000000 | (uint32(90) << 23), J);

}


//二维网格坐标（1-31级网格）向经纬度（网格的质心）的转换测试
TEST(GTCOORDS_TEST, IJtoLL) {
    double lng, lat;
    uint32 I, J;

    I = 0x5a000000;
    J = 0x2d000000;
    GT::IJtoLL(I, J, &lng, &lat);
    std::cout << lng << std::endl;
    std::cout << lat << std::endl;
    EXPECT_NEAR(180, lng, DBL_EPSILON);
    EXPECT_NEAR(90, lat, DBL_EPSILON);

    I = 0xda000000;
    J = 0x2d000000;
    GT::IJtoLL(I, J, &lng, &lat);
    std::cout << lng << std::endl;
    std::cout << lat << std::endl;
    EXPECT_NEAR(-180, lng, DBL_EPSILON);
    EXPECT_NEAR(90, lat, DBL_EPSILON);

    I = 0x5a000000;
    J = 0xad000000;
    GT::IJtoLL(I, J, &lng, &lat);
    std::cout << lng << std::endl;
    std::cout << lat << std::endl;
    EXPECT_NEAR(180, lng, DBL_EPSILON);
    EXPECT_NEAR(-90, lat, DBL_EPSILON);

    I = 0xda000000;
    J = 0xad000000;
    GT::IJtoLL(I, J, &lng, &lat);
    std::cout << lng << std::endl;
    std::cout << lat << std::endl;
    EXPECT_NEAR(-180, lng, DBL_EPSILON);
    EXPECT_NEAR(-90, lat, DBL_EPSILON);

    for(int i =0; i< 1000; i++){

    }
}

//经纬度向地心坐标系的转换测试
TEST(GTCOORDS_TEST, LLtoXYZ) {
    double lng, lat;
    S2Point pnt;

    lng = 180.00;
    lat = 90.00;
    GT::LLtoXYZ(lng, lat, &pnt);

    std::cout << std::hex << pnt.x() << std::endl;
    std::cout << std::hex << pnt.y() << std::endl;
    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(1, pnt.z(), DBL_EPSILON);

    lng = -180.00;
    lat = 90.00;
    GT::LLtoXYZ(lng, lat, &pnt);
    std::cout << std::hex << pnt.x() << std::endl;
    std::cout << std::hex << pnt.y() << std::endl;
    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(1, pnt.z(), DBL_EPSILON);

    lng = 180.00;
    lat = -90.00;
    GT::LLtoXYZ(lng, lat, &pnt);
    std::cout << std::hex << pnt.x() << std::endl;
    std::cout << std::hex << pnt.y() << std::endl;
    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(-1, pnt.z(), DBL_EPSILON);

    lng = -180.00;
    lat = -90.00;
    GT::LLtoXYZ(lng, lat, &pnt);
    std::cout << std::hex << pnt.x() << std::endl;
    std::cout << std::hex << pnt.y() << std::endl;
    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(-1, pnt.z(), DBL_EPSILON);

    lng = -180.00;
    lat = 00.00;
    GT::LLtoXYZ(lng, lat, &pnt);
    std::cout << std::hex << pnt.x() << std::endl;
    std::cout << std::hex << pnt.y() << std::endl;
    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(-1, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.z(), DBL_EPSILON);

    lng = -90.00;
    lat = 0.00;
    GT::LLtoXYZ(lng, lat, &pnt);
    std::cout << std::hex << pnt.x() << std::endl;
    std::cout << std::hex << pnt.y() << std::endl;
    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(-1, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.z(), DBL_EPSILON);

    lng = +90.00;
    lat = 0.00;
    GT::LLtoXYZ(lng, lat, &pnt);
    std::cout << std::hex << pnt.x() << std::endl;
    std::cout << std::hex << pnt.y() << std::endl;
    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(1, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.z(), DBL_EPSILON);

    lng = +180.00;
    lat = 0.00;
    GT::LLtoXYZ(lng, lat, &pnt);
    std::cout << std::hex << pnt.x() << std::endl;
    std::cout << std::hex << pnt.y() << std::endl;
    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(-1, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.z(), DBL_EPSILON);

    lng = 0.00;
    lat = 0.00;
    GT::LLtoXYZ(lng, lat, &pnt);
    std::cout << std::hex << pnt.x() << std::endl;
    std::cout << std::hex << pnt.y() << std::endl;
    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(1, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.z(), DBL_EPSILON);

    lng = 45.00;
    lat = 0.00;
    GT::LLtoXYZ(lng, lat, &pnt);
    std::cout << std::hex << pnt.x() << std::endl;
    std::cout << std::hex << pnt.y() << std::endl;
    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(sqrt(2.0) / 2, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(sqrt(2.0) / 2, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.z(), DBL_EPSILON);

    lng = 45.00;
    lat = 45.00;
    GT::LLtoXYZ(lng, lat, &pnt);
    std::cout << std::hex << pnt.x() << std::endl;
    std::cout << std::hex << pnt.y() << std::endl;
    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0.5, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0.5, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0.7071067811865475, pnt.z(), DBL_EPSILON);

    for(int i =0; i<1000;i++){
        S2LatLng ll = S2Testing::RandomLatLng();
        GT::LLtoXYZ(ll.lng().degrees(),ll.lat().degrees(),&pnt);
        GT::XYZtoLL(pnt,&lng,&lat);
        EXPECT_NEAR(ll.lng().degrees(), lng, 1000*DBL_EPSILON);
        EXPECT_NEAR(ll.lat().degrees(), lat, 1000*DBL_EPSILON);
    }
}


//地心坐标系向经纬度（网格的质心）的转换测试
TEST(GTCOORDS_TEST, XYZtoLL) {
    S2Point pnt1, pnt2;
    double lng, lat;

    for (int i = 0; i < 1000; i++) {
        pnt1 = S2Testing::RandomPoint();
        GT::XYZtoLL(pnt1, &lng, &lat);
        GT::LLtoXYZ(lng, lat, &pnt2);
        EXPECT_NEAR(pnt1.x(), pnt2.x(), 1000*DBL_EPSILON);
        EXPECT_NEAR(pnt1.y(), pnt2.y(), 1000*DBL_EPSILON);
        EXPECT_NEAR(pnt1.z(), pnt2.z(), 1000*DBL_EPSILON);
    }
}



//地心坐标系向二维网格坐标的转换测试
TEST(GTCOORDS_TEST, XYZtoIJ) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng, lat, &id);
    std::cout << std::hex << id << std::endl;
    EXPECT_EQ(0X0, id);
}

//二维网格坐标（叶子节点）向地心坐标系(叶子网格的质心坐标）的转换测试
TEST(GTCOORDS_TEST, IJtoXYZ) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng, lat, &id);
    std::cout << std::hex << id << std::endl;
    EXPECT_EQ(0X0, id);
}

//二维网格坐标（所有网格，不一定是叶子网格）向一维编码的转换测试
TEST(GTCOORDS_TEST, IJtoID) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng, lat, &id);
    std::cout << std::hex << id << std::endl;
    EXPECT_EQ(0X0, id);
}

//一维编码向二维网格坐标（所有网格，不一定是叶子网格）的转换测试
TEST(GTCOORDS_TEST, IDtoIJ) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng, lat, &id);
    std::cout << std::hex << id << std::endl;
    EXPECT_EQ(0X0, id);
}

//经纬度向一维编码（叶子网格）的转换测试
TEST(GTCOORDS_TEST, LLtoID) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng, lat, &id);
    std::cout << std::hex << id << std::endl;
    EXPECT_EQ(0X0, id);
}
//一维编码（不一定是叶子网格）向经纬度（网格质心）的转换测试
TEST(GTCOORDS_TEST, IDtoLL) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng, lat, &id);
    std::cout << std::hex << id << std::endl;
    EXPECT_EQ(0X0, id);
}

//地心坐标向一维编码（叶子网格）的转换测试
TEST(GTCOORDS_TEST, XYZtoID) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng, lat, &id);
    std::cout << std::hex << id << std::endl;
    EXPECT_EQ(0X0, id);
}
//一维编码（不一定是叶子网格）向地心坐标（网格质心）的转换测试
TEST(GTCOORDS_TEST, IDtoXYZ) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id;

    GT::LLtoCellID(lng, lat, &id);
    std::cout << std::hex << id << std::endl;
    EXPECT_EQ(0X0, id);
}


