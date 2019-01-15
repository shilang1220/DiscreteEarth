//
// Created by Administrator on 2018/12/17 0017.
//
#include "gtest/gtest.h"
#include "gt.h"
#include "tools/gt_testing.h"

#define TESTTIMES_1000 1000
#define TESTTIMES_10000 1000000
#define TESTTIMES_100000 100000000

//经纬度向二维网格坐标(叶子节点)的转换测试
TEST(GTCOORDS_TEST, LLtoIJ) {
    double lng, lat, lng2, lat2;
    uint32 I, J;

    lng = 0.00;
    lat = 0.00;
    EXPECT_TRUE(GT::LLtoIJ(lng, lat, &I, &J));
    EXPECT_EQ(uint32(0X0), I);
    EXPECT_EQ(uint32(0X0), J);
    EXPECT_TRUE(GT::IJtoLL(I, J, &lng2, &lat2));
    EXPECT_NEAR(lng, lng2, GT::kMaxError);
    EXPECT_NEAR(lat, lat2, GT::kMaxError);

    lng = 180.00;
    lat = 90.00;
    EXPECT_TRUE(GT::LLtoIJ(lng, lat, &I, &J));
    EXPECT_EQ(uint32(180) << 23, I);
    EXPECT_EQ(uint32(90) << 23, J);
    EXPECT_TRUE(GT::IJtoLL(I, J, &lng2, &lat2));
    EXPECT_NEAR(lng, lng2, GT::kMaxError);
    EXPECT_NEAR(lat, lat2, GT::kMaxError);

    lng = -180.00;
    lat = 90.00;
    EXPECT_TRUE(GT::LLtoIJ(lng, lat, &I, &J));
    EXPECT_EQ(0X80000000 | (uint32(180) << 23), I);
    EXPECT_EQ(uint32(90) << 23, J);
    EXPECT_TRUE(GT::IJtoLL(I, J, &lng2, &lat2));
    EXPECT_NEAR(lng, lng2, GT::kMaxError);
    EXPECT_NEAR(lat, lat2, GT::kMaxError);

    lng = 180.00;
    lat = -90.00;
    EXPECT_TRUE(GT::LLtoIJ(lng, lat, &I, &J));
    EXPECT_EQ(uint32(180) << 23, I);
    EXPECT_EQ(0X80000000 | (uint32(90) << 23), J);
    EXPECT_TRUE(GT::IJtoLL(I, J, &lng2, &lat2));
    EXPECT_NEAR(lng, lng2, GT::kMaxError);
    EXPECT_NEAR(lat, lat2, GT::kMaxError);

    lng = -180.00;
    lat = -90.00;
    EXPECT_TRUE(GT::LLtoIJ(lng, lat, &I, &J));
    EXPECT_EQ(0X80000000 | (uint32(180) << 23), I);
    EXPECT_EQ(0X80000000 | (uint32(90) << 23), J);
    EXPECT_TRUE(GT::IJtoLL(I, J, &lng2, &lat2));
    EXPECT_NEAR(lng, lng2, GT::kMaxError);
    EXPECT_NEAR(lat, lat2, GT::kMaxError);

    S2LatLng ll;
    //经纬度向32层级网格坐标转换的函数测试
    for (int i = 0; i < TESTTIMES_10000; i++) {
        ll = GTTesting::RandomLatLng();
        double lng1 = ll.lng().degrees(), lat1 = ll.lat().degrees();
        EXPECT_TRUE(GT::LLtoIJ(lng1, lat1, &I, &J));
        EXPECT_TRUE(GT::IJtoLL(I, J, &lng, &lat));
        EXPECT_NEAR(lng, lng1, GT::GetGridInterval(32));
        EXPECT_NEAR(lat, lat1, GT::GetGridInterval(32));
    }

    //经纬度向任意层级（1-31）网格坐标转换的函数测试
    for (int i = 0; i < TESTTIMES_10000; i++) {
        int level = GTTesting::RandomLevel();
        ll = GTTesting::RandomLatLng();
        EXPECT_TRUE(GT::LLtoIJ(ll.lng().degrees(), ll.lat().degrees(), &I, &J, level));
        EXPECT_TRUE(GT::IJtoLL(I, J, &lng, &lat, level));
        EXPECT_GE(ll.lng().degrees() * lng, 0);
        EXPECT_GE(ll.lat().degrees() * lat, 0);
        EXPECT_NEAR(lng, ll.lng().degrees(), GT::GetGridInterval(level));
        EXPECT_NEAR(lat, ll.lat().degrees(), GT::GetGridInterval(level));
    }
}


//二维网格坐标（1-31级网格）向经纬度的转换测试
TEST(GTCOORDS_TEST, IJtoLL) {
    double lng, lat;
    uint32 I, J;
    uint32 I_, J_;

    I = 0x5a000000;
    J = 0x2d000000;
    EXPECT_TRUE(GT::IJtoLL(I, J, &lng, &lat));
    EXPECT_NEAR(180, lng, GT::kMaxError);
    EXPECT_NEAR(90, lat, GT::kMaxError);
    EXPECT_TRUE(GT::LLtoIJ(lng, lat, &I_, &J_));
    EXPECT_EQ(I, I_);
    EXPECT_EQ(J, J_);

    I = 0xda000000; // 0B 1101 1010 00..00
    J = 0x2d000000;
    EXPECT_TRUE(GT::IJtoLL(I, J, &lng, &lat));
    EXPECT_NEAR(-180, lng, GT::kMaxError);
    EXPECT_NEAR(90, lat, GT::kMaxError);

    I = 0x5a000000;
    J = 0xad000000;
    EXPECT_TRUE(GT::IJtoLL(I, J, &lng, &lat));
    EXPECT_NEAR(180, lng, GT::kMaxError);
    EXPECT_NEAR(-90, lat, GT::kMaxError);
    EXPECT_TRUE(GT::LLtoIJ(lng, lat, &I_, &J_));
    EXPECT_EQ(I, I_);
    EXPECT_EQ(J, J_);

    I = 0xda000000;
    J = 0xad000000;
    EXPECT_TRUE(GT::IJtoLL(I, J, &lng, &lat));
    EXPECT_NEAR(-180, lng, GT::kMaxError);
    EXPECT_NEAR(-90, lat, GT::kMaxError);
    EXPECT_TRUE(GT::LLtoIJ(lng, lat, &I_, &J_));
    EXPECT_EQ(I, I_);
    EXPECT_EQ(J, J_);



    // 测试第32层网格坐标与经纬度之间的转换函数
    for (int i = 0; i < TESTTIMES_10000; i++) {
        GTTesting::RandomIJ(&I, &J, 32);
        if (GT::IJtoLL(I, J, &lng, &lat)) {

            EXPECT_LE(abs(lng), 180 + GT::kMaxError);
            EXPECT_LE(abs(lat), 90 + GT::kMaxError);
            if (GT::LLtoIJ(lng, lat, &I_, &J_)) {
                EXPECT_EQ(I, I_);
                EXPECT_EQ(J, J_);
            };
        }
    }

    //测试任意层级网格（1-31级）坐标与经纬度之间的转换函数
    for (int i = 0; i < TESTTIMES_10000; i++) {

        int level = GTTesting::RandomLevel();

        GTTesting::RandomIJ(&I, &J, level);

        if (GT::IJtoLL(I, J, &lng, &lat, level)) {
            EXPECT_LE(abs(lng), 180 + GT::kMaxError);
            EXPECT_LE(abs(lat), 90 + GT::kMaxError);
            if (GT::LLtoIJ(lng, lat, &I_, &J_, level)) {
                EXPECT_EQ(I, I_);
                EXPECT_EQ(J, J_);
            }
        }
    }
}

//经纬度向球面坐标系的转换测试
TEST(GTCOORDS_TEST, LLtoXYZ) {
    double lng, lat;
    S2Point pnt;

    lng = 180.00;
    lat = 90.00;
    EXPECT_TRUE(GT::LLtoXYZ(lng, lat, &pnt));

//    std::cout << std::hex << pnt.x() << std::endl;
//    std::cout << std::hex << pnt.y() << std::endl;
//    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(1, pnt.z(), DBL_EPSILON);

    lng = -180.00;
    lat = 90.00;
    EXPECT_TRUE(GT::LLtoXYZ(lng, lat, &pnt));
//    std::cout << std::hex << pnt.x() << std::endl;
//    std::cout << std::hex << pnt.y() << std::endl;
//    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(1, pnt.z(), DBL_EPSILON);

    lng = 180.00;
    lat = -90.00;
    EXPECT_TRUE(GT::LLtoXYZ(lng, lat, &pnt));
    //    std::cout << std::hex << pnt.x() << std::endl;
//    std::cout << std::hex << pnt.y() << std::endl;
//    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(-1, pnt.z(), DBL_EPSILON);

    lng = -180.00;
    lat = -90.00;
    EXPECT_TRUE(GT::LLtoXYZ(lng, lat, &pnt));
    //    std::cout << std::hex << pnt.x() << std::endl;
//    std::cout << std::hex << pnt.y() << std::endl;
//    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(-1, pnt.z(), DBL_EPSILON);

    lng = -180.00;
    lat = 00.00;
    EXPECT_TRUE(GT::LLtoXYZ(lng, lat, &pnt));
    //    std::cout << std::hex << pnt.x() << std::endl;
//    std::cout << std::hex << pnt.y() << std::endl;
//    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(-1, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.z(), DBL_EPSILON);

    lng = -90.00;
    lat = 0.00;
    EXPECT_TRUE(GT::LLtoXYZ(lng, lat, &pnt));
    //    std::cout << std::hex << pnt.x() << std::endl;
//    std::cout << std::hex << pnt.y() << std::endl;
//    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(-1, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.z(), DBL_EPSILON);

    lng = +90.00;
    lat = 0.00;
    EXPECT_TRUE(GT::LLtoXYZ(lng, lat, &pnt));
    //    std::cout << std::hex << pnt.x() << std::endl;
//    std::cout << std::hex << pnt.y() << std::endl;
//    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(1, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.z(), DBL_EPSILON);

    lng = +180.00;
    lat = 0.00;
    EXPECT_TRUE(GT::LLtoXYZ(lng, lat, &pnt));
    //    std::cout << std::hex << pnt.x() << std::endl;
//    std::cout << std::hex << pnt.y() << std::endl;
//    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(-1, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.z(), DBL_EPSILON);

    lng = 0.00;
    lat = 0.00;
    EXPECT_TRUE(GT::LLtoXYZ(lng, lat, &pnt));
    //    std::cout << std::hex << pnt.x() << std::endl;
//    std::cout << std::hex << pnt.y() << std::endl;
//    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(1, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.z(), DBL_EPSILON);

    lng = 45.00;
    lat = 0.00;
    EXPECT_TRUE(GT::LLtoXYZ(lng, lat, &pnt));
    //    std::cout << std::hex << pnt.x() << std::endl;
//    std::cout << std::hex << pnt.y() << std::endl;
//    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(sqrt(2.0) / 2, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(sqrt(2.0) / 2, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0, pnt.z(), DBL_EPSILON);

    lng = 45.00;
    lat = 45.00;
    EXPECT_TRUE(GT::LLtoXYZ(lng, lat, &pnt));
    //    std::cout << std::hex << pnt.x() << std::endl;
//    std::cout << std::hex << pnt.y() << std::endl;
//    std::cout << std::hex << pnt.z() << std::endl;
    EXPECT_NEAR(0.5, pnt.x(), DBL_EPSILON);
    EXPECT_NEAR(0.5, pnt.y(), DBL_EPSILON);
    EXPECT_NEAR(0.7071067811865475, pnt.z(), DBL_EPSILON);

    for (int i = 0; i < TESTTIMES_10000; i++) {
        S2LatLng ll = GTTesting::RandomLatLng();
        EXPECT_TRUE(GT::LLtoXYZ(ll.lng().degrees(), ll.lat().degrees(), &pnt));
        EXPECT_NEAR(pnt.x() * pnt.x() + pnt.y() * pnt.y() + pnt.z() * pnt.z(), 1, 1000 * DBL_EPSILON);
        EXPECT_TRUE(GT::XYZtoLL(pnt, &lng, &lat));
        EXPECT_NEAR(ll.lng().degrees(), lng, GT::kMaxError);
        EXPECT_NEAR(ll.lat().degrees(), lat, GT::kMaxError);
    }
}


//球面坐标系向经纬度的转换测试
TEST(GTCOORDS_TEST, XYZtoLL) {
    S2Point pnt1, pnt2;
    double lng, lat;

    for (int i = 0; i < TESTTIMES_10000; i++) {
        pnt1 = GTTesting::RandomPoint();
        EXPECT_TRUE(GT::XYZtoLL(pnt1, &lng, &lat));
        EXPECT_LE(fabs(lng), 180 + DBL_EPSILON);
        EXPECT_LE(fabs(lat), 90 + DBL_EPSILON);
        EXPECT_TRUE(GT::LLtoXYZ(lng, lat, &pnt2));
        EXPECT_NEAR(pnt1.x(), pnt2.x(), 1000 * DBL_EPSILON);
        EXPECT_NEAR(pnt1.y(), pnt2.y(), 1000 * DBL_EPSILON);
        EXPECT_NEAR(pnt1.z(), pnt2.z(), 1000 * DBL_EPSILON);
        EXPECT_NEAR(pnt2.x() * pnt2.x() + pnt2.y() * pnt2.y() + pnt2.z() * pnt2.z(), 1, 1000 * DBL_EPSILON);
    }
}

//球面坐标系向二维网格坐标的转换测试
TEST(GTCOORDS_TEST, XYZtoIJ) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id;
    uint32 I_, J_;
    S2Point pnt, pnt2;

    //测试球面坐标向第32层网格坐标的转换函数
    for (int i = 0; i < TESTTIMES_10000; i++) {
        pnt = GTTesting::RandomPoint();
        EXPECT_TRUE(GT::XYZtoIJ(pnt, &I_, &J_));
        EXPECT_TRUE(GT::IJtoXYZ(I_, J_, &pnt2));

        double dx = pnt.x() - pnt2.x();
        double dy = pnt.y() - pnt2.y();
        double dz = pnt.z() - pnt2.z();

        EXPECT_LE(sqrt(dx * dx + dy * dy + dz * dz), GT::GetGridMaxDistanceError(32));
        EXPECT_NEAR(pnt2.x() * pnt2.x() + pnt2.y() * pnt2.y() + pnt2.z() * pnt2.z(), 1, 1000 * DBL_EPSILON);
    }
    //测试球面坐标向任意层级（1-31）网格坐标的转换函数
    for (int i = 0; i < TESTTIMES_10000; i++) {
        pnt = GTTesting::RandomPoint();
        int level = GTTesting::RandomLevel();

        if (level < 4) continue;
        EXPECT_TRUE(GT::XYZtoIJ(pnt, &I_, &J_, level));
        EXPECT_TRUE(GT::IJtoXYZ(I_, J_, &pnt2, level));

        double dx = pnt.x() - pnt2.x();
        double dy = pnt.y() - pnt2.y();
        double dz = pnt.z() - pnt2.z();
        EXPECT_LE(sqrt(dx * dx + dy * dy + dz * dz), GT::GetGridMaxDistanceError(level));
        EXPECT_NEAR(pnt2.x() * pnt2.x() + pnt2.y() * pnt2.y() + pnt2.z() * pnt2.z(), 1,
                    1000 * DBL_EPSILON);
    }
}

//二维网格坐标(叶子网格)向球面坐标系的转换测试
TEST(GTCOORDS_TEST, IJtoXYZ) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id;
    uint32 I_, J_, I, J;
    S2Point pnt, pnt2;

    for (int i = 0; i < TESTTIMES_10000; i++) {
        GTTesting::RandomIJ(&I_, &J_);
        if (GT::IJtoXYZ(I_, J_, &pnt2)) {
            EXPECT_NEAR(pnt2.x() * pnt2.x() + pnt2.y() * pnt2.y() + pnt2.z() * pnt2.z(), 1, 1000 * DBL_EPSILON);
            EXPECT_TRUE(GT::XYZtoIJ(pnt2, &I, &J));
            EXPECT_EQ(I, I_);
            EXPECT_EQ(J, J_);
        };
    }

    for (int i = 0; i < TESTTIMES_10000; i++) {
        int level = GTTesting::RandomLevel();
        GTTesting::RandomIJ(&I_, &J_, level);
        if (GT::IJtoXYZ(I_, J_, &pnt2, level)) {

            EXPECT_NEAR(pnt2.x() * pnt2.x() + pnt2.y() * pnt2.y() + pnt2.z() * pnt2.z(), 1, 1000 * DBL_EPSILON);
            EXPECT_TRUE(GT::XYZtoIJ(pnt2, &I, &J, level));
            EXPECT_EQ(I, I_);
            EXPECT_EQ(J, J_);
        };
    }

}

//二维网格坐标（所有网格，不一定是叶子网格）向一维编码的转换测试
TEST(GTCOORDS_TEST, IJtoID) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id, id2;
    uint32 I, J, I_, J_;
    int level, level_;

    //第32级网格坐标向CELLID的转换测试
    for (int i = 0; i < TESTTIMES_10000; i++) {
        GTTesting::RandomIJ(&I_, &J_);
        EXPECT_TRUE(GT::IJtoCellID(I_, J_, &id2));
        EXPECT_LT(id2, 0XFFFFFFFFFFFFFFFE);
        EXPECT_GT(id2, 0);
        EXPECT_TRUE(GT::CellIDtoIJ(id2, &I, &J, &level_));
        EXPECT_EQ(I, I_ & 0XFFFFFFFE);
        EXPECT_EQ(J, J_ & 0XFFFFFFFE);
        EXPECT_EQ(32, level_ + 1);
    }

    for (int i = 0; i < TESTTIMES_10000; i++) {
        level = GTTesting::RandomLevel();
        GTTesting::RandomIJ(&I_, &J_, level);
        EXPECT_TRUE(GT::IJtoCellID(I_, J_, &id2, level));
        EXPECT_LT(id2, 0XFFFFFFFFFFFFFFFE);
        EXPECT_GT(id2, 0);
        EXPECT_TRUE(GT::CellIDtoIJ(id2, &I, &J, &level_));
        EXPECT_EQ(I, I_);
        EXPECT_EQ(J, J_);
        EXPECT_EQ(level, level_);
    }

}

//一维编码向二维网格坐标（所有网格，不一定是叶子网格）的转换测试
TEST(GTCOORDS_TEST, IDtoIJ) {
    uint32 I, J, I_, J_;
    uint64 id, id2;
    int level, level_;

    //第32级网格坐标向CELLID的转换测试
    for (int i = 0; i < TESTTIMES_10000; i++) {
        GTCellId cellid = GTTesting::GetRandomCellId(31);
        EXPECT_TRUE(cellid.is_valid());
//        std::cout<<std::hex<<cellid.id()<<std::endl;
        id = cellid.id();
        EXPECT_TRUE(GT::CellIDtoIJ(id, &I_, &J_, &level_));
        EXPECT_EQ(31, level_);
        EXPECT_EQ(0, I_ & 1);
        EXPECT_EQ(0, J_ & 1);
        EXPECT_TRUE(GT::IJtoCellID(I_, J_, &id2));
        EXPECT_EQ(id, id2);
    }

    for (int i = 0; i < TESTTIMES_10000; i++) {
        level = GTTesting::RandomLevel();
        id = GTTesting::GetRandomCellId(level).id();
        GTCellId cellID(id);

        EXPECT_EQ(level, cellID.level());
        EXPECT_TRUE(GT::CellIDtoIJ(id, &I_, &J_, &level_));
        EXPECT_EQ(level, level_);
        EXPECT_EQ(0,I_&1);
        EXPECT_EQ(0,J_&1);
        EXPECT_TRUE(GT::IJtoCellID(I_, J_, &id2, level));
        EXPECT_EQ(id, id2);
    }


}

//经纬度向一维编码（叶子网格）的转换测试
TEST(GTCOORDS_TEST, LLtoID) {
    double lng, lat, lng2, lat2;
    uint64 id,id2;
    int level,level_;

    lng = 0.00;
    lat = 0.00;
    EXPECT_TRUE(GT::LLtoCellID(lng, lat, &id));
    EXPECT_EQ(uint64(0X2), id);
    EXPECT_TRUE(GT::CellIDtoLL(id, &lng2, &lat2,&level));
    EXPECT_EQ(31,level);
    EXPECT_NEAR(lng, lng2, GT::kMaxError);
    EXPECT_NEAR(lat, lat2, GT::kMaxError);

    lng = 180.00;
    lat = 90.00;
    EXPECT_TRUE(GT::LLtoCellID(lng, lat, &id));
    EXPECT_EQ(uint64(0X26d9000000000002), id);
    EXPECT_TRUE(GT::CellIDtoLL(id, &lng2, &lat2,&level));
    EXPECT_EQ(31,level);
    EXPECT_NEAR(lng, lng2, GT::kMaxError);
    EXPECT_NEAR(lat, lat2, GT::kMaxError);

    lng = -180.00;
    lat = 90.00;
    EXPECT_TRUE(GT::LLtoCellID(lng, lat, &id));
    EXPECT_EQ(uint64(0XA6d9000000000002), id);
    EXPECT_TRUE(GT::CellIDtoLL(id, &lng2, &lat2,&level));
    EXPECT_EQ(31,level);
    EXPECT_NEAR(lng, lng2, GT::kMaxError);
    EXPECT_NEAR(lat, lat2, GT::kMaxError);

    lng = 180.00;
    lat = -90.00;
    EXPECT_TRUE(GT::LLtoCellID(lng, lat, &id));
    EXPECT_EQ(uint64(0X66d9000000000002), id);
    EXPECT_TRUE(GT::CellIDtoLL(id, &lng2, &lat2,&level));
    EXPECT_EQ(31,level);
    EXPECT_NEAR(lng, lng2, GT::kMaxError);
    EXPECT_NEAR(lat, lat2, GT::kMaxError);

    lng = -180.00;
    lat = -90.00;
    EXPECT_TRUE(GT::LLtoCellID(lng, lat, &id));
    EXPECT_EQ(uint64(0Xe6d9000000000002), id);
    EXPECT_TRUE(GT::CellIDtoLL(id, &lng2, &lat2,&level));
    EXPECT_EQ(31,level);
    EXPECT_NEAR(lng, lng2, GT::kMaxError);
    EXPECT_NEAR(lat, lat2, GT::kMaxError);

    S2LatLng ll;
    //经纬度向32层级网格坐标转换的函数测试
    for (int i = 0; i < TESTTIMES_10000; i++) {
        ll = GTTesting::RandomLatLng();
        double lng1 = ll.lng().degrees(), lat1 = ll.lat().degrees();

        EXPECT_TRUE(GT::LLtoCellID(lng1, lat1, &id));
        EXPECT_TRUE(GT::CellIDtoLL(id, &lng2, &lat2,&level));
        EXPECT_EQ(31,level);
        EXPECT_NEAR(lng1, lng2, GT::GetGridInterval(31));
        EXPECT_NEAR(lat1, lat2, GT::GetGridInterval(31));
    }

    //经纬度向任意层级（1-31）网格坐标转换的函数测试
    for (int i = 0; i < TESTTIMES_10000; i++) {
        int level = GTTesting::RandomLevel();
        ll = GTTesting::RandomLatLng();
        double lng1 = ll.lng().degrees(), lat1 = ll.lat().degrees();

        EXPECT_TRUE(GT::LLtoCellID(lng1, lat1, &id, level));
        EXPECT_TRUE(GT::CellIDtoLL(id,&lng2, &lat2, &level_));
        EXPECT_EQ(level,level_);
        EXPECT_GE(lng1 * lng2, 0);
        EXPECT_GE(lat1 * lat2, 0);
        EXPECT_NEAR(lng1, lng2, GT::GetGridInterval(level));
        EXPECT_NEAR(lat1, lat2, GT::GetGridInterval(level));
    }

}
//一维编码（不一定是叶子网格）向经纬度（网格质心）的转换测试
TEST(GTCOORDS_TEST, IDtoLL) {
    double lng1,lng2,lat1,lat2;//, J, I_, J_;
    uint64 id, id2;
    int level, level_;

    //第32级网格坐标向CELLID的转换测试
    for (int i = 0; i < TESTTIMES_100000; i++) {
        GTCellId cellid = GTTesting::GetRandomCellId(31);
        EXPECT_TRUE(cellid.is_valid());
        id = cellid.id();
        if(GT::CellIDtoLL(id, &lng1, &lat1, &level_)){
            EXPECT_EQ(31, level_);
            EXPECT_TRUE(GT::LLtoCellID(lng1,lat1, &id2));
            EXPECT_EQ(id,id2);
        };
    }

    for (int i = 0; i < TESTTIMES_100000; i++) {
        level = GTTesting::RandomLevel();
        GTCellId cellid = GTTesting::GetRandomCellId(level);
        EXPECT_TRUE(cellid.is_valid());

        id = cellid.id();
        if(GT::CellIDtoLL(id, &lng1, &lat1, &level_)){
            EXPECT_EQ(level, level_);
            EXPECT_TRUE(GT::LLtoCellID(lng1,lat1, &id2,level));
            EXPECT_EQ(id,id2);
        };
    }
}

//球面坐标向一维编码（叶子网格）的转换测试
TEST(GTCOORDS_TEST, XYZtoID) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id;

}
//一维编码（不一定是叶子网格）向球面坐标（网格质心）的转换测试
TEST(GTCOORDS_TEST, IDtoXYZ) {
    double lng, lat;
    lng = 180.00;
    lat = 90.00;
    uint64 id;

}


