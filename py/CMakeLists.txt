cmake_minimum_required(VERSION 3.11)
project(movepy
        LANGUAGES CXX
        VERSION 0.1
        )


# run cmake with -DCMAKE_PREFIX_PATH=path-to-installed-cmake-dir-of-movetk
#find_package(movetk CONFIG REQUIRED)  ##enable only if movepy is being built as a separate project

# add dependencies
if (NOT TARGET c2d::movetk)
    find_package(movetk CONFIG REQUIRED)
endif()

find_package (Python COMPONENTS Interpreter Development REQUIRED)

# message(STATUS "Python_include_dir ${Python_INCLUDE_DIRS}")


# set(PYBIND11_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/pybind11/include/")

# message(STATUS "PYBIND11_INCLUDE_DIR ${PYBIND11_INCLUDE_DIR}")

# install(DIRECTORY  ${PYBIND11_INCLUDE_DIR}
#         DESTINATION include
#         )


# include_directories(${PYBIND11_INCLUDE_DIR})


# set(PYBIND11_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/pybind11/")

add_subdirectory(pybind11)

set(PYBIND11_CPP_STANDARD -std=c++1z)

#find_package(pybind11 REQUIRED)

#find_package(movetk REQUIRED) ##enable only if movepy is being built as a separate project

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")


add_library(movetk_geometry SHARED movetk_geometry.cpp)

#pybind11_add_module(movetk_geometry SHARED movetk_geometry.cpp)

#target_compile_definitions(movetk_geometry PUBLIC)

target_include_directories(movetk_geometry
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        )


target_link_libraries(movetk_geometry
        PUBLIC
        #${GDAL_LIBRARY}
        #${GeographicLib_LIBRARIES}
        c2d::movetk
        pybind11::module
)

set_target_properties(movetk_geometry PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
        SUFFIX "${PYTHON_MODULE_EXTENSION}")

install(TARGETS movetk_geometry EXPORT ${CMAKE_PROJECT_NAME}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )