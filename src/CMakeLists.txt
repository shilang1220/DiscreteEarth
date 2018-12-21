
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BIN_DIR})
set(LIBRARY_OUTPUT_PATH  ${PROJECT_LIB_DIR})



#add_library(s2geometry ${THIRDPARTY_SRCS} ${STRINGS_SRCS}  ${UTIL_SRCS} ${BASE_SRCS} ${CORE_SRCS} ${SHAPE_SRCS} ${BUILDER_SRCS} ${INDEX_SRCS} ${OBJECT_SRCS} ${QUERY_SRCS} ${ENCODE_SRCS})
#build libs from third party
add_library(third_party ${THIRDPARTY_SRCS} ${STRINGS_SRCS} ${UTIL_SRCS} ${BASE_SRCS} )
#add_library(third_party ${THIRDPARTY_SRCS})
#add_library(strings ${STRINGS_SRCS})
#add_library(util ${UTIL_SRCS})
#add_library(base ${BASE_SRCS})
#add_library(s2 ${S2_SRCS})

#build libs of self
#add_library(core SHARED ${CORE_SRCS})
add_library(gt  ${S2_SRCS} ${CORE_SRCS} ${GT_SRCS} gt/gtcell_test.cpp gt/gtcoords_test.cpp)
#add_library(shape  ${SHAPE_SRCS} )
#add_library(builder  ${BUILDER_SRCS} )
#add_library(index  ${INDEX_SRCS} )
#add_library(object ${OBJECT_SRCS})
#add_library(query ${QUERY_SRCS})
#add_library(encode ${ENCODE_SRCS})