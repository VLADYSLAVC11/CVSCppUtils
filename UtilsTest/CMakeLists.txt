cmake_minimum_required(VERSION 3.16)

project(CvsCppUtilsTest LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CvsCppUtilsTest SmartEnum.h Polymorphic.h
    Tests/TestMain.cpp
    Tests/PolymorphicObjectTestDefs.h
    Tests/PolymorphicObjectTestDefs.cpp
    Tests/SmartEnumTestDefs.h Tests/SmartEnumTestDefsFwd.h
    Tests/SmartEnumTestDefs.cpp
    Tests/SmartEnumTest.cpp
    Tests/PolymorphicObjectsTest.cpp)
add_test(NAME CvsCppUtilsTest COMMAND CvsCppUtilsTest)

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
