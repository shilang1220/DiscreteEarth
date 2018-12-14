
########################################################################
# Project setup
########################################################################
cmake_minimum_required(VERSION 3.1)
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose build type: None Debug Release RelWithDebInfo MinSizeRel")
project(DiscreteEarth)

set(VERSION_INFO_MAJOR_VERSION 1)
set(VERSION_INFO_MINOR_VERSION 3)
set(VERSION_INFO_MAINT_VERSION 0git)

enable_language(CXX)
enable_language(C)
enable_testing()

set(PROJECT_INC_DIR "${PROJECT_SOURCE_DIR}/include")
set(PROJECT_SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(PROJECT_BIN_DIR "${PROJECT_SOURCE_DIR}/bin")
set(PROJECT_LIB_DIR "${PROJECT_SOURCE_DIR}/lib")
set(PROJECT_DOC_DIR "${PROJECT_SOURCE_DIR}/doc")
set(PROJECT_EXM_DIR "${PROJECT_SOURCE_DIR}/examples")
set(PROJECT_RC_DIR "${PROJECT_SOURCE_DIR}/resource")
set(PROJECT_TEST_DIR "${PROJECT_SOURCE_DIR}/test")

########################################################################
# Environment setup
########################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w") #-Wall
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w") #-Wall
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${PROJECT_INC_DIR})
add_definitions(-D_USE_MATH_DEFINES)

########################################################################
# Dependencies setup
########################################################################



########################################################################
# Setup doxygen
########################################################################
find_package(Doxygen)
if(DOXYGEN_FOUND)
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
endif(DOXYGEN_FOUND)

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
# Option to enable QA testing, on by default
########################################################################
OPTION(ENABLE_TESTING "Enable QA testing" ON)
if(ENABLE_TESTING)
    message(STATUS "QA Testing is enabled.")
else()
    message(STATUS "QA Testing is disabled.")
endif()
message(STATUS "  Modify using: -DENABLE_TESTING=ON/OFF")

########################################################################
# Option to enable post-build profiling using volk_profile, off by default
########################################################################
#OPTION(ENABLE_PROFILING "Launch system profiler after build" OFF)
#if(ENABLE_PROFILING)
#    if(DEFINED VOLK_CONFIGPATH)
#        get_filename_component(VOLK_CONFIGPATH ${VOLK_CONFIGPATH} ABSOLUTE)
#        set(VOLK_CONFIGPATH "${VOLK_CONFIGPATH}/volk")
#        message(STATUS "System profiling is enabled, using path: ${VOLK_CONFIGPATH}")
#    elseif(DEFINED ENV{VOLK_CONFIGPATH})
#        set(VOLK_CONFIGPATH "$ENV{VOLK_CONFIGPATH}/volk")
#        message(STATUS "System profiling is enabled, using env path: $ENV{VOLK_CONFIGPATH}")
#    else()
#        message(STATUS "System profiling is enabled with default paths.")
#        if(DEFINED ENV{HOME})
#            set(VOLK_CONFIGPATH "$ENV{HOME}/.volk")
#        elseif(DEFINED ENV{APPDATA})
#            set(VOLK_CONFIGPATH "$ENV{APPDATA}/.volk")
#        endif()
#    endif()
#else()
#    message(STATUS "System profiling is disabled.")
#endif()
#message(STATUS "  Modify using: -DENABLE_PROFILING=ON/OFF")

########################################################################
# Setup the library
########################################################################
add_subdirectory(${PROJECT_SRC_DIR})
add_subdirectory(${PROJECT_EXM_DIR})
add_subdirectory(${PROJECT_TEST_DIR})

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
