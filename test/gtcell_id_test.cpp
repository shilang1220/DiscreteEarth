//
// Created by 濮国梁 on 2018/12/19.
//

#include "gtest/gtest.h"
#include "gt.h"
#include "tools/gt_testing.h"

static GTCellId GetCellId(double lat_degrees, double lng_degrees) {
    GTCellId id(S2LatLng::FromDegrees(lat_degrees, lng_degrees));
//    S2_LOG(INFO) << std::hex << id.id();
    return id;
}

static GTCellId GetCellId(double lat_degrees, double lng_degrees, int level) {
    GTCellId id(S2LatLng::FromDegrees(lat_degrees, lng_degrees), level);
//    S2_LOG(INFO) << std::hex << id.id();
    return id;
}

TEST(GTCELLID_TEST, ID_CONSTRUCT_TRANSFORM) {
    GTCellId id;

    //////////////////////////////////////////////////////
    //经纬度到ID的转换测试
    /////////////////////////////////////////////////////
    S2LatLng ll,ll2;
    int level;

    for (int i =0; i<100000; i++){
        ll2 = S2Testing::RandomLatLng();
        level = S2Testing::RandomLevel();

//        level = 16;

        id = GetCellId(ll2.lat().degrees(),ll2.lng().degrees(),level);
        ll = id.ToLatLng();

        //CellID只编到31级，所以转换的最大误差应当为32级网格大小的两倍
        EXPECT_NEAR(ll2.lat().degrees(), ll.lat().degrees(),GT::GetGridInterval(level));
        EXPECT_NEAR(ll2.lng().degrees(), ll.lng().degrees(),GT::GetGridInterval(level));
    }

    ////////////////////////////////////////////////////
    //地心坐标系到ID的转换测试
    ////////////////////////////////////////////////////
    S2Point pnt,pnt2;
    for (int i =0; i<100000; i++){
        pnt2 = S2Testing::RandomPoint();
        level = S2Testing::RandomLevel();

        id = GTCellId(pnt2,level);
        pnt = id.ToPoint();

        //CellID只编到31级，所以转换的最大误差应当为32级网格大小的两倍
        EXPECT_NEAR(pnt2.x(),pnt.x(),GT::GetGridInterval(level));
        EXPECT_NEAR(pnt2.y(),pnt.y(),GT::GetGridInterval(level));
        EXPECT_NEAR(pnt2.z(),pnt.z(),GT::GetGridInterval(level));
    }

}



TEST(GTCELLID_TEST, ID_ATTRIB) {
    S2LatLng latLng;
    latLng.FromDegrees(0.0, 0.0);

    GTCellId id(latLng);

    ASSERT_EQ(31, id.level());
    ASSERT_TRUE(id.is_leaf());

    //面积属性计算

    std::cout << " testcase event teardown." << std::endl;

    std::cout << id.id() << std::endl;

}

TEST(GTCELLID_TEST, ID_TRANSFORM) {
    GTCellId id = GetCellId(45, 45, 1);
    S2Point pnt;
    S2LatLng ll;

    //定位点球面坐标获取
    pnt = id.ToPoint();
    //中心点球面坐标获取
    pnt = id.ToCenterPoint();
    //定位点经纬度坐标获取
    ll = id.ToLatLng();
    //中心点经纬度坐标获取
    ll = id.ToCenterLatLng();

    //四进制转换
    string str4 = id.ToString4();
    std::cout << "ID " << std::hex << id.id() << " has 4 number format:" << str4 << std::endl;

    //16进制转换
    string str16 = id.ToString16();
    std::cout << "ID " << std::hex << id.id() << " has 16 number format:" << str16 << std::endl;

    //32进制转换
    string str32 = id.ToString32();
    std::cout << "ID " << std::hex << id.id() << " has 32 number format:" << str32 << std::endl;

}