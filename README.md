# DiscreteEarth

About GT Coordinates and its convert： 

GT网格系统涉及四个坐标系统和一个编码系统
（1） 经纬度          （LNG,LAT）       double
                     -180.00 ≤ LNG ≤ +180.00，-90.00 ≤ LAT ≤ +90.00
（2）扩展坐标系      （U,V）           double  在实际编程中，暂时不做考虑
                     -512.00 ≤ U ≤ +512.00，-512.00 ≤ L ≤ +512.00
（3）球面坐标系      （X,Y,Z）         double   采用单位球面表示（球半径R = 1）
                     X**2 + Y**2 + Z**2 =1
（4）网格坐标系      （I，J）          unsigned long(uint32)  网格坐标采用32层级以便于求解第31级网格的质心或边的坐标
                      0 ≤ I ≤ 2**32，0 ≤ J ≤ 2**32
                      

/////////////////////////////////////////////////

About layers in GT system

    // 关于层的约定
    // 从1层至第32层
    // level = 1，表示半球，网格编码中用高位起第0-1bit表示，为反Z字逆序
    // level = 2, 网格编码中用高位起第2-3bit表示，为反Z字顺序
    // level = 3, 网格编码中用高位起第4-5bit表示，为反Z字顺序
    // ......，后面以此类推
    // level = 31,网格编码中用高位起第60-61bit表示，为反Z字顺序
    // level = 32，转做截止符专用，网格编码中用高位起第62-63bit表示，为00或10,10时专指叶子网格编码
    // 所有层级网格编码均为其前序层编码从高位到低位连接而成，其后跟随10..00..00作为编码截止标识
    
About GT grid encoding
                      
GT网格编码       采用Z序编码     uint64  采用“网格Z序编码 + 10..00截止标识符”的编码方式
                 仅使用前62bit（31级），末尾2bit预留作第31级网格编码的截止位专用
                 0X0000000000000002 网格编码的最小值
                 0XFFFFFFFFFFFFFFFE 网格编码的最大值
                 0X8000000000000000 特殊编码，用于标识全球
                 0X0000000000000000 特殊编码，用于指示不合法编码
                 0X0000000000000001 特殊编码，用途待定
                 0XFFFFFFFFFFFFFFFF 特殊编码，用于指示网格编码的最大值（不含）

ID examples： 

    下面以两字节编码为例做示范，能够表示7级网格
       // 0b 1000 0000 0000 0000  第0级网格，代表全球
       // 0b 0010 0000 0000 0000  第1级的第0个子网格
       // 0b 1101 1100 1000 0000  第4级的第0个子网格
       // 0b 1101 1100 1110 0000  第5级的第3个子网格
       // 0b 1101 1100 1110 1000  第6级的第2个子网格
       // 0b 1101 1100 1110 0010  第7级的第0个子网格
       // 0b 1101 1100 1110 0110  第7级的第1个子网格
       // 0b 1101 1100 1110 1010  第7级的第2个子网格
       // 0b 1101 1100 1110 1110  第7级的第3个子网格
       // 0b 1101 1100 1110 1100  非法网格编码
       // 0b 1101 1100 1110 0100  非法网格编码
       // 0b 1101 1100 1110 1000  合法网格编码，只能被解释为第6级的第2个子网格
       // 0b 1101 1100 1110 1100  非法网格编码
       // 0b 1101 1100 1100 0000  非法网格编码
       // 0b 1101 1100 1101 0000  非法网格编码
       // 0b 1101 1100 1111 0000  非法网格编码
       /////////////////////////////////////////////
       // 注意几个特殊网格ID：
       // 1000..00..00  代表全球网格
       // 0010  00..00  代表东北半球
       // 0110  00..00  代表西北半球
       // 1010  00..00  代表东南半球
       // 1110  00..00  代表西南半球
       // 0000..00..00  全0代表未赋值网格或非法网格
       // 1111  11..11  全1作为网格编码的最大值指示器
       /////////////////////////////////////////////
                  
About cell ID's relation
    （1）网格及其子孙的编码范围特点
    一个网格及其所有子网格的编码在一个有限区间内，但该区间内的编码不一定是该网格的子孙网格ID（存在一些非法网格编码）
    设zID为某个网格的Z序编码（去除截止符，长度为2*level）
    则其和所有子孙网格的编码位于 cat(zID,00..00..10) 和 cat（zID， FF..FF..FE）之间
    而该网格自身的编码为 cat(zID,10..00.00)
    （2）
FOR USERS:
1. add ${GTROOT} to your include directories.
2. add gt.lib to your link libs.
3. put gt.dll to your dll searching directories.
4. add 'include "gt.h"' in your source code.
5. using gt lib freely. 


FOR DEVELOPERS:

This lib composed by three main parts.

PART I: 

four thirdparty source code. Including:

base/
util/
strings/
thirdparty/

PART II:

classes for angles,arcs,3d points,latlng points,distances,areas etc. on sphere face. Including:

s2/

PART III:

classes for GT libs, modified refer to S2Geometry. Including:

core/	 	-- 	classes for discrete cell,cellid,cap,latlng_rect etc.

gt/		 	--  classes for spatial feature.
 
index/		--	classes for spatial index in memory.

query/      --  classes for spatial query in memory.

tools/		-- 	tools for debugging and logging.

