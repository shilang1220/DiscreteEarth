# DiscreteEarth

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

