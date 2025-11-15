cmake_minimum_required(VERSION 3.16)

project(CvsCppUtilsTest LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CvsCppUtilsTest
    include/SmartEnum.h
    include/Polymorphic.h
    tests/TestMain.cpp
    tests/PolymorphicObjectTestDefs.h
    tests/PolymorphicObjectTestDefs.cpp
    tests/SmartEnumTestDefs.h
    tests/SmartEnumTestDefsFwd.h
    tests/SmartEnumTestDefs.cpp
    tests/SmartEnumTest.cpp
    tests/PolymorphicObjectsTest.cpp)
add_test(NAME CvsCppUtilsTest COMMAND CvsCppUtilsTest)

target_include_directories(CvsCppUtilsTest PRIVATE include)

if (DEFINED ENV{BOOST_INCLUDE_DIR})
    set(BOOST_INCLUDE_DIR $ENV{BOOST_INCLUDE_DIR})
elseif (NOT DEFINED BOOST_INCLUDE_DIR)
    set(BOOST_INCLUDE_DIR "") # set by Qt Creator wizard
endif ()
if (BOOST_INCLUDE_DIR STREQUAL "")
    message("BOOST_INCLUDE_DIR is not set, assuming Boost can be found automatically in your system")
elseif (EXISTS ${BOOST_INCLUDE_DIR})
    include_directories(${BOOST_INCLUDE_DIR})
endif ()
