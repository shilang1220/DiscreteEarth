
########################################################################
# Project setup
########################################################################
cmake_minimum_required(VERSION 3.1)

include(CMakeDependentOption)           #添加选项依赖关系的支持
include(CheckCXXCompilerFlag)           #添加编译器是否支持某些FLAG的判断支持
#include(FeatureSummary)                 #添加特征总结的支持
include(FindPackageHandleStandardArgs)  #添加FindPackage支持

project(DiscreteEarth)

#编译选项宏定义
if (WIN32)  #MSVC编译选项
    # 使用无符号字符
    add_definitions(-J)
    # 使用cmath头文件中的M_PI、DBL_EPISLON等宏定义
    add_definitions(-D_USE_MATH_DEFINES)
    # 不使用<windef.h>中的min、max，避免和标准库冲突
    add_definitions(-DNOMINMAX)
    # 警告等级设置为3级，W1) : 显示严重的警告消息；W2显示等级1警告以及某些不太严重的警告；W3显示等级2警告以及某些不太严重的警告；W4显示所有等级 3 警告以及信息性警告。
    add_definitions(-W1)
else()      #GCC编译选项
    #-w禁止所有警告；-Wall开启所有警告；
    add_definitions(-w)#Wno-attributes)
    add_definitions(-Wno-deprecated-declarations)
endif()

set(VERSION_INFO_MAJOR_VERSION 1)
set(VERSION_INFO_MINOR_VERSION 3)
set(VERSION_INFO_MAINT_VERSION 0git)

#语言支持
enable_language(CXX)                    #添加C++语言编译支持
#编译选项
set(CMAKE_CXX_STANDARD 11)              #采用c++<std 11>标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)     #强制执行std11标准
set(CMAKE_CXX_EXTENSIONS OFF)           #不支持编译器自身对标准的扩展
enable_language(C)                      #添加C语言编译支持

# 编译范围选项设置
option(BUILD_SHARED_LIBS "Build shared libraries instead of static." ON)    #编译为共享库
#add_feature_info(SHARED_LIBS BUILD_SHARED_LIBS "builds shared libraries instead of static.")
option(BUILD_EXAMPLES "Build s2 documentation examples." OFF)                #编译EXAMPLES
#add_feature_info(EXAMPLES BUILD_EXAMPLES "builds examples.")
option(BUILD_TEST "Build s2 documentation examples." OFF)                    #编译TEST
#add_feature_info(TESTS BUILD_TEST "builds tests.")

########################################################################
# Dependencies setup
########################################################################

#第三方头文件和库文件目录,OpenSSL和GoogleTest是必须项，否则编译会出现错误
if(WIN32)       #MSVC下的第三方库的头文件和链接库文件目录
    set(OPENSSL_LIBRARIES "C:/OpenSSL-Win64/lib")
    set(GTEST_LIBRARIES "C:/gtest/lib")
    include_directories("C:/OpenSSL-Win64/include")     #openssl头文件目录
    include_directories("C:/gtest/inclue")              #googletest头文件目录
else()          #GCC的第三方库头文件目录
    add_definitions(-DOPENSSL_LIBRARIES=/usr/lib/openssl)
    set(OPENSSL_LIBRARIES "/usr/lib/openssl")
    set(GTEST_LIBRARIES "/usr/lib/gtest")
    include_directories("/usr/local/include")           #openssl和gtest的头文件目录
    link_directories("/usr/lib/openssl")                  #openssl和gtest库文件目录
endif()

##########################################################################################
#  以下禁止修改
##########################################################################################


########################################################################
# Environment setup
########################################################################
# 目录环境变量设置
set(PROJECT_INC_DIR "${PROJECT_SOURCE_DIR}/include")
set(PROJECT_SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(PROJECT_BIN_DIR "${PROJECT_SOURCE_DIR}/bin")
set(PROJECT_LIB_DIR "${PROJECT_SOURCE_DIR}/lib")
set(PROJECT_DOC_DIR "${PROJECT_SOURCE_DIR}/doc")
set(PROJECT_EXM_DIR "${PROJECT_SOURCE_DIR}/examples")
set(PROJECT_RC_DIR "${PROJECT_SOURCE_DIR}/resource")
set(PROJECT_TEST_DIR "${PROJECT_SOURCE_DIR}/test")

include_directories(${PROJECT_INC_DIR})


########################################################################
# Setup doxygen
########################################################################
#find_package(Doxygen)
#if(DOXYGEN_FOUND)
#    configure_file(
#            ${PROJECT_DOC_DIR}/Doxyfile.in
#            ${PROJECT_BIN_DIR}/Doxyfile
#            @ONLY)
#
#    add_custom_target(DiscreateEarth_doc
#            ${DOXYGEN_EXECUTABLE} ${PROJECT_BIN_DIR}/Doxyfile
#            WORKING_DIRECTORY ${PROJECT_BIN_DIR}
#            COMMENT "Generating documentation with Doxygen" VERBATIM
#            )
#endif(DOXYGEN_FOUND)

########################################################################
# Setup the package config file
########################################################################
#set variables found in the pc.in file
#set(prefix ${CMAKE_INSTALL_PREFIX})
#set(exec_prefix "\${prefix}")
#set(libdir "\${exec_prefix}/lib${LIB_SUFFIX}")
#set(includedir "\${prefix}/include")

#configure_file(
#        ${CMAKE_CURRENT_SOURCE_DIR}/volk.pc.in
#        ${CMAKE_CURRENT_BINARY_DIR}/volk.pc
#        @ONLY)

#install(
#        FILES ${CMAKE_CURRENT_BINARY_DIR}/volk.pc
#        DESTINATION lib${LIB_SUFFIX}/pkgconfig
#        COMPONENT "volk_devel"
#)

########################################################################
# Install all headers in the include directories
########################################################################


########################################################################
# Create uninstall target
########################################################################
#configure_file(
#        ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
#        ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
#        @ONLY)

# Only add the target if there isn't one defined already
#if(NOT TARGET uninstall)
#    add_custom_target(uninstall
#            ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
#            )
#endif()


########################################################################
# Install our Cmake modules into $prefix/lib/cmake/volk
# See "Package Configuration Files" on page:
#    http://www.cmake.org/Wiki/CMake/Tutorials/Packaging
########################################################################

#configure_file(
#        ${CMAKE_SOURCE_DIR}/cmake/Modules/VolkConfig.cmake.in
#        ${CMAKE_BINARY_DIR}/cmake/Modules/VolkConfig.cmake
#        @ONLY)
#
#configure_file(
#        ${CMAKE_SOURCE_DIR}/cmake/Modules/VolkConfigVersion.cmake.in
#        ${CMAKE_BINARY_DIR}/cmake/Modules/VolkConfigVersion.cmake
#        @ONLY)


########################################################################
# Install cmake search routine for external use
########################################################################
#
#if(NOT CMAKE_MODULES_DIR)
#    set(CMAKE_MODULES_DIR lib${LIB_SUFFIX}/cmake)
#endif(NOT CMAKE_MODULES_DIR)
#
#install(
#        FILES
#        ${CMAKE_CURRENT_BINARY_DIR}/cmake/Modules/VolkConfig.cmake
#        ${CMAKE_CURRENT_BINARY_DIR}/cmake/Modules/VolkConfigVersion.cmake
#        DESTINATION ${CMAKE_MODULES_DIR}/volk
#        COMPONENT "volk_devel"
#)

########################################################################
# QA testing
########################################################################
if(BUILD_TEST)
    aux_source_directory("${PROJECT_TEST_DIR}" TESTFILES)
    enable_testing()
   foreach (test_cc ${TESTFILES})
        get_filename_component(test ${test_cc} NAME_WE)
        add_executable(${test} ${test_cc})
        target_link_libraries(
                    ${test}
                    s2testing gt gtest_main)
        add_test(${test} ${test})
    endforeach()
else()
    message(STATUS "QA Testing is disabled.")
endif()

########################################################################
# Option to enable post-build profiling using volk_profile, off by default
########################################################################


########################################################################
# Setup the library
########################################################################
#预先宏定义
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BIN_DIR})
set(LIBRARY_OUTPUT_PATH  ${PROJECT_LIB_DIR})

if(BUILD_SHARED_LIBS)
    add_definitions(-DDISCRETEEARTH_EXPORTS)            #按照输出库编译

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

    #add_library(s2geometry ${THIRDPARTY_SRCS} ${STRINGS_SRCS}  ${UTIL_SRCS} ${BASE_SRCS} ${CORE_SRCS} ${SHAPE_SRCS} ${BUILDER_SRCS} ${INDEX_SRCS} ${OBJECT_SRCS} ${QUERY_SRCS} ${ENCODE_SRCS})
    #build libs from third party
    #add_library(third_party SHARED ${THIRDPARTY_SRCS} ${STRINGS_SRCS} ${UTIL_SRCS} ${BASE_SRCS} )
    #add_library(third_party ${THIRDPARTY_SRCS})
    #add_library(strings ${STRINGS_SRCS})
    #add_library(util ${UTIL_SRCS})
    #add_library(base ${BASE_SRCS})
    #add_library(s2 ${S2_SRCS})

    #build libs of self
    #add_library(core SHARED ${CORE_SRCS})
    add_library(gt SHARED ${S2_SRCS} ${CORE_SRCS} ${GT_SRCS} ${THIRDPARTY_SRCS} ${STRINGS_SRCS} ${UTIL_SRCS} ${BASE_SRCS} )
    target_link_libraries(
            gt
            ${OPENSSL_LIBRARIES} ${GTEST_LIBRARIES}
    )
    #add_library(shape  ${SHAPE_SRCS} )
    #add_library(builder  ${BUILDER_SRCS} )
    #add_library(index  ${INDEX_SRCS} )
    #add_library(object ${OBJECT_SRCS})
    #add_library(query ${QUERY_SRCS})
    #add_library(encode ${ENCODE_SRCS})
endif()
if(BUILD_EXAMPLES)
    aux_source_directory("${PROJECT_SRC_DIR}/examples" EXAMPLES_SRCS)
    foreach (example_cc ${EXAMPLES_SRCS})
        get_filename_component(example ${example_cc} NAME_WE)
        add_executable(${example} ${example_cc})
        target_link_libraries(
                ${example}
                gt ${OPENSSL_LIBRARIES} ${GTEST_LIBRARIES}
        )
    endforeach()
endif()

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
