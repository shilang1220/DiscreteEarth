aux_source_directory("${PROJECT_SRC_DIR}/third_party/absl/base" THIRDPARTY_SRCS)
aux_source_directory("${PROJECT_SRC_DIR}/third_party/absl/base/internal" THIRDPARTY_SRCS)
aux_source_directory("${PROJECT_SRC_DIR}/third_party/absl/numeric" THIRDPARTY_SRCS)
aux_source_directory("${PROJECT_SRC_DIR}/third_party/absl/strings" THIRDPARTY_SRCS)
aux_source_directory("${PROJECT_SRC_DIR}/third_party/absl/strings/internal" THIRDPARTY_SRCS)

aux_source_directory("${PROJECT_SRC_DIR}/strings" STRINGS_SRCS)

aux_source_directory("${PROJECT_SRC_DIR}/util/bits" UTIL_SRCS)
aux_source_directory("${PROJECT_SRC_DIR}/util/coding" UTIL_SRCS)
aux_source_directory("${PROJECT_SRC_DIR}/util/math" UTIL_SRCS)
aux_source_directory("${PROJECT_SRC_DIR}/util/math/exactfloat" UTIL_SRCS)
aux_source_directory("${PROJECT_SRC_DIR}/util/unit" UTIL_SRCS)

aux_source_directory("${PROJECT_SRC_DIR}/base" BASE_SRCS)

aux_source_directory("${PROJECT_SRC_DIR}/s2" S2_SRCS)

aux_source_directory("${PROJECT_SRC_DIR}/core" CORE_SRCS)
aux_source_directory("${PROJECT_SRC_DIR}/gt" GT_SRCS)

#aux_source_directory("${PROJECT_SRC_DIR}/shape" SHAPE_SRCS)
#aux_source_directory("${PROJECT_SRC_DIR}/object" OBJECT_SRCS)
#aux_source_directory("${PROJECT_SRC_DIR}/builder" BUILDER_SRCS)
#aux_source_directory("${PROJECT_SRC_DIR}/encode" ENCODE_SRCS)
#aux_source_directory("${PROJECT_SRC_DIR}/index" INDEX_SRCS)
#aux_source_directory("${PROJECT_SRC_DIR}/query" QUERY_SRCS)

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
add_library(gt  ${S2_SRCS} ${CORE_SRCS} ${GT_SRCS} )
add_executable(gtcoords_test "${PROJECT_SRC_DIR}/gt/gtcoords_test.cpp" )
target_link_libraries(
        gtcoords_test
        gt third_party gtest_main)
add_test(gtcoords_test gtcoords_test)
#add_library(shape  ${SHAPE_SRCS} )
#add_library(builder  ${BUILDER_SRCS} )
#add_library(index  ${INDEX_SRCS} )
#add_library(object ${OBJECT_SRCS})
#add_library(query ${QUERY_SRCS})
#add_library(encode ${ENCODE_SRCS})