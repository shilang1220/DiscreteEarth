//
// Created by 濮国梁 on 2018/12/19.
//

#include "gtest/gtest.h"
#include "gt.h"

static GTCellId GetCellId(double lat_degrees, double lng_degrees) {
    GTCellId id(S2LatLng::FromDegrees(lat_degrees, lng_degrees));
    S2_LOG(INFO) << std::hex << id.id();
    return id;
}


TEST(GTCELLID_TEST, ID_CONSTRUCT) {
    GTCellId id;

    //经纬度到ID的转换测试
    S2LatLng ll;
    id = GetCellId(0.0, 0.0);
    ll = id.ToLatLng();
    EXPECT_EQ(S1Angle(0.0),ll.lat());
    EXPECT_EQ(0.0,ll.lng());

    id = GetCellId(90.00,180.00);
    ll = id.ToLatLng();
    EXPECT_EQ(90.00,ll.lat());
    EXPECT_EQ(180.00,ll.lng());

    id = GetCellId(-90.00,180.00);
    ll = id.ToLatLng();
    EXPECT_EQ(-90.00,ll.lat());
    EXPECT_EQ(180.00,ll.lng());

    id = GetCellId(-90.00,-180.00);
    ll = id.ToLatLng();
    EXPECT_EQ(-90.00,ll.lat());
    EXPECT_EQ(-180.00,ll.lng());

    id = GetCellId(+90.00,-180.00);
    ll = id.ToLatLng();
    EXPECT_EQ(90.00,ll.lat());
    EXPECT_EQ(-180.00,ll.lng());


    //地心坐标系到ID的转换测试
    S2Point pnt(1.0,1.0,1.0);

    id.FromPoint(pnt);



////    std::cout << " testcase event teardown." << std::endl;
////    GT::LLtoCellID(lng,lat,&id);
////    std::cout << id << std::endl;
////    EXPECT_EQ(0X0,id);
}

TEST(GTCELLID_TEST, ID_ATTRIB) {
    S2LatLng latLng;
    latLng.FromDegrees(0.0, 0.0);

    GTCellId id(latLng);

    ASSERT_EQ(31, id.level());
    ASSERT_TRUE(id.is_leaf());


    std::cout << " testcase event teardown." << std::endl;

    std::cout << id.id() << std::endl;

}