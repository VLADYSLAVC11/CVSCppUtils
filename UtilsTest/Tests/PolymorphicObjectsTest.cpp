
#include "PolymorphicObjectTestDefs.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <vector>

/*---------------------------------------------------------------------------*/

namespace Cvs::Utils::Test {

using namespace boost::unit_test;

/*---------------------------------------------------------------------------*/

template<size_t MaxTypeSizeof, size_t Alignment, class TDerived, class ... TObjectsList>
std::ostream& operator << (
    std::ostream& out
    ,   const Polymorhic<MaxTypeSizeof, Alignment, TDerived, TObjectsList...>& polyObj
    )
{
    if(polyObj.IsNull())
        out << "Polymorphic[Null]";
    else
        out << "Polymorphic[TypeId: " << polyObj.TypeId() << "]";

    return out;
}

BOOST_AUTO_TEST_SUITE(CVS_CPP_UTILS_TEST_SUITE)

/*---------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(CVS_CPP_UTILS_POLYMORPHIC_TEST0_DEFAULT_OBJECT)
{
    Test0Obj testObj;
    static_assert(sizeof(Test0Obj) == 8, "Invalid Test0Obj sizeof");
    static_assert(alignof(Test0Obj) == 4, "Invalid Test0Obj alignment");
    BOOST_CHECK_MESSAGE(!testObj.IsInitialized(), "testObj should not be initialized");
    BOOST_CHECK(testObj.IsNull());
}

/*---------------------------------------------------------------------------*/

BOOST_DATA_TEST_CASE(CVS_CPP_UTILS_POLYMORPHIC_TEST0_INIT_VALUES, data::xrange(1, 5), value)
{
    Test0Obj testObj;
    Test0Obj::id_type id = testObj.Init(value);
    BOOST_CHECK_MESSAGE(testObj.IsInitialized(), "testObj should be initialized");
    BOOST_CHECK_EQUAL(id, testObj.TypeId());
    BOOST_CHECK_EQUAL(id, value);
}

/*---------------------------------------------------------------------------*/

BOOST_DATA_TEST_CASE(CVS_CPP_UTILS_POLYMORPHIC_TEST0_GET_VALUES, data::xrange(1, 5), value)
{
    Test0Obj testObj;
    testObj.Init(value);
    BOOST_CHECK_EQUAL(testObj.GetValue(), value);
}

/*---------------------------------------------------------------------------*/

BOOST_DATA_TEST_CASE(CVS_CPP_UTILS_POLYMORPHIC_TEST0_DOWNCAST, data::xrange(1, 5), value)
{
    Test0Obj testObj;
    testObj.Init(value);

    auto * ptr1 = testObj.DownCast<Test0Obj1 *>();
    const auto * const_ptr1 = testObj.DownCast<const Test0Obj1 *>();

    if(value == 1)
    {
        BOOST_CHECK(ptr1 != nullptr);
        BOOST_CHECK(const_ptr1 != nullptr);
        BOOST_CHECK_NO_THROW(testObj.DownCast<Test0Obj1 &>());
        BOOST_CHECK_NO_THROW(testObj.DownCast<const Test0Obj1 &>());
    }
    else
    {
        BOOST_CHECK(ptr1 == nullptr);
        BOOST_CHECK(const_ptr1 == nullptr);
        BOOST_CHECK_THROW(testObj.DownCast<Test0Obj1 &>(), std::bad_cast);
        BOOST_CHECK_THROW(testObj.DownCast<const Test0Obj1 &>(), std::bad_cast);
    }

    auto * ptr2 = testObj.DownCast<Test0Obj2 *>();
    const auto * const_ptr2 = testObj.DownCast<const Test0Obj2 *>();

    if(value == 2)
    {
        BOOST_CHECK(ptr2 != nullptr);
        BOOST_CHECK(const_ptr2 != nullptr);
        BOOST_CHECK_NO_THROW(testObj.DownCast<Test0Obj2 &>());
        BOOST_CHECK_NO_THROW(testObj.DownCast<const Test0Obj2 &>());
    }
    else
    {
        BOOST_CHECK(ptr2 == nullptr);
        BOOST_CHECK(const_ptr2 == nullptr);
        BOOST_CHECK_THROW(testObj.DownCast<Test0Obj2 &>(), std::bad_cast);
        BOOST_CHECK_THROW(testObj.DownCast<const Test0Obj2 &>(), std::bad_cast);
    }

    auto * ptr3 = testObj.DownCast<Test0Obj3 *>();
    const auto * const_ptr3 = testObj.DownCast<const Test0Obj3 *>();

    if(value == 3)
    {
        BOOST_CHECK(ptr3 != nullptr);
        BOOST_CHECK(const_ptr3 != nullptr);
        BOOST_CHECK_NO_THROW(testObj.DownCast<Test0Obj3 &>());
        BOOST_CHECK_NO_THROW(testObj.DownCast<const Test0Obj3 &>());
    }
    else
    {
        BOOST_CHECK(ptr3 == nullptr);
        BOOST_CHECK(const_ptr3 == nullptr);
        BOOST_CHECK_THROW(testObj.DownCast<Test0Obj3 &>(), std::bad_cast);
        BOOST_CHECK_THROW(testObj.DownCast<const Test0Obj3 &>(), std::bad_cast);
    }

    auto * ptr4 = testObj.DownCast<Test0Obj4 *>();
    const auto * const_ptr4 = testObj.DownCast<const Test0Obj4 *>();

    if(value == 4)
    {
        BOOST_CHECK(ptr4 != nullptr);
        BOOST_CHECK(const_ptr4 != nullptr);
        BOOST_CHECK_NO_THROW(testObj.DownCast<Test0Obj4 &>());
        BOOST_CHECK_NO_THROW(testObj.DownCast<const Test0Obj4 &>());
    }
    else
    {
        BOOST_CHECK(ptr4 == nullptr);
        BOOST_CHECK(const_ptr4 == nullptr);
        BOOST_CHECK_THROW(testObj.DownCast<Test0Obj4 &>(), std::bad_cast);
        BOOST_CHECK_THROW(testObj.DownCast<const Test0Obj4 &>(), std::bad_cast);
    }
}

/*---------------------------------------------------------------------------*/

BOOST_DATA_TEST_CASE(CVS_CPP_UTILS_POLYMORPHIC_TEST0_DESTROY, data::xrange(1, 5), value)
{
    Test0Obj testObj;
    testObj.Init(value);
    testObj.Destroy();
    BOOST_CHECK_MESSAGE(!testObj.IsInitialized(), "testObj should not be initialized after destroying");
    BOOST_CHECK(testObj.IsNull());
}

/*---------------------------------------------------------------------------*/

BOOST_DATA_TEST_CASE(CVS_CPP_UTILS_POLYMORPHIC_TEST0_MOVING, data::xrange(1, 5), value)
{
    Test0Obj testObj;
    testObj.Init(value);

    Test0Obj testObjMoveCtor(std::move(testObj));
    BOOST_CHECK(testObj.IsNull());

    Test0Obj testObjMoveOper;
    testObjMoveOper = std::move(testObjMoveCtor);
    BOOST_CHECK(testObjMoveCtor.IsNull());

    BOOST_CHECK(testObjMoveOper.IsInitialized());
    BOOST_CHECK_EQUAL(testObjMoveOper.GetValue(), value);
}

/*---------------------------------------------------------------------------*/

BOOST_DATA_TEST_CASE(CVS_CPP_UTILS_POLYMORPHIC_TEST0_OBJECTS_IN_VECTOR, data::xrange(1, 5), value)
{
    std::vector<Test0Obj> testObjVec;
    BOOST_CHECK(testObjVec.empty());

    for(size_t i = 0; i < 10; ++i)
    {
        switch(value)
        {
        case 1: testObjVec.emplace_back(Test0Obj1()); break;
        case 2: testObjVec.emplace_back(Test0Obj2()); break;
        case 3: testObjVec.emplace_back(Test0Obj3()); break;
        case 4: testObjVec.emplace_back(Test0Obj4()); break;
        default: break;
        }

        BOOST_CHECK(testObjVec.back().IsInitialized());
        BOOST_CHECK_EQUAL(testObjVec.back().GetValue(), value);
    }

    BOOST_CHECK(!testObjVec.empty());
}

/*---------------------------------------------------------------------------*/

BOOST_AUTO_TEST_SUITE_END()

/*---------------------------------------------------------------------------*/

} // namespace Cvs::Utils::Test
