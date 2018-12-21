
########################################################################
# Project setup
########################################################################
cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose build type: None Debug Release RelWithDebInfo MinSizeRel")

project(DiscreteEarth)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)
include(FeatureSummary)
include(FindPackageHandleStandardArgs)

set(CMAKE_CXX_STANDARD 11)
set(VERSION_INFO_MAJOR_VERSION 1)
set(VERSION_INFO_MINOR_VERSION 3)
set(VERSION_INFO_MAINT_VERSION 0git)

enable_language(CXX)
enable_language(C)
enable_testing()

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
    aux_source_directory("${OPENSSL_LIBRARIES}" OPENSSL_LIBS)
    message(STATUS ${OPENSSL_LIBS})
#    target_link_libraries(
#            gt
#            ssl
#    )
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
                gt ${OPENSSL_LIBRARIES}
        )
    endforeach()
endif()

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
