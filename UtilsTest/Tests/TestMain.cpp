#define BOOST_TEST_MODULE My test module
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(BOOST_CONFIGURATION_TEST)
{
    BOOST_CHECK_MESSAGE(true, "BOOST TEST is not configured properly");
}
