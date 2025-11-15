
#include "SmartEnumTestDefs.h"
#include <boost/test/unit_test.hpp>

namespace Cvs::Utils::Test {

constexpr bool strings_equal(char const* a, char const* b) {
    return std::string_view(a) == std::string_view(b);
}

namespace Test1 {
    std::ostream& operator << (std::ostream& _out, TestEnum _enum_item)
    {
        _out << Cvs::Utils::SmartEnum::GetEnumItemName(_enum_item);
        return _out;
    }
} // namespace Test1

namespace Test4 {
    std::ostream& operator << (std::ostream& _out, TestColor _enum_item)
    {
        using namespace Cvs::Utils::SmartEnum;
        _out << GetEnumText(_enum_item);
        return _out;
    }
} // namespace Test4

} // namespace CommonUtilsTest

namespace Cvs {
namespace Utils {
namespace Test {

using namespace Cvs::Utils::SmartEnum;

BOOST_AUTO_TEST_SUITE(SmartEnumTestSuite);

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_1_Constexpr_Functions)
{
    static_assert(IsSmart<Test1::TestEnum>(), "Enum is not smart");
    static_assert(std::is_same<std::underlying_type_t<Test1::TestEnum>, std::uint8_t>::value, "Invalid Enum underlying type");

    static_assert(GetEnumItemsCount<Test1::TestEnum>() == 4, "Invalid Enum Items count");

    static_assert(IsEnumItemDefined(Test1::TestEnum::Item0), "Item0 is not defained");
    static_assert(IsEnumItemDefined(Test1::TestEnum::Item1), "Item1 is not defained");
    static_assert(IsEnumItemDefined(Test1::TestEnum::Item2), "Item2 is not defained");
    static_assert(IsEnumItemDefined(Test1::TestEnum::Item3), "Item3 is not defained");
    static_assert(!IsEnumItemDefined(static_cast<Test1::TestEnum>(5)), "Item should not be defined");

    static_assert(strings_equal(GetEnumItemName(Test1::TestEnum::Item0), "Item0"), "Item0 name is invalid");
    static_assert(strings_equal(GetEnumItemName(Test1::TestEnum::Item1), "Item1"), "Item1 name is invalid");
    static_assert(strings_equal(GetEnumItemName(Test1::TestEnum::Item2), "Item2"), "Item2 name is invalid");
    static_assert(strings_equal(GetEnumItemName(Test1::TestEnum::Item3), "Item3"), "Item3 name is invalid");
    static_assert(strings_equal(GetEnumItemName(static_cast<Test1::TestEnum>(6)), ""), "Enum name should be empty");

    static_assert(static_cast<Test1::TestEnum>(0) == Test1::TestEnum::Item0, "Invalid Item0 value");
    static_assert(static_cast<Test1::TestEnum>(1) == Test1::TestEnum::Item1, "Invalid Item1 value");
    static_assert(static_cast<Test1::TestEnum>(4) == Test1::TestEnum::Item2, "Invalid Item2 value");
    static_assert(static_cast<Test1::TestEnum>(3) == Test1::TestEnum::Item3, "Invalid Item3 value");

    static_assert(GetEnumItem<Test1::TestEnum>(0) == Test1::TestEnum::Item0, "Item not equal to Item0");
    static_assert(GetEnumItem<Test1::TestEnum>(1) == Test1::TestEnum::Item1, "Item not equal to Item1");
    static_assert(GetEnumItem<Test1::TestEnum>(2) == Test1::TestEnum::Item2, "Item not equal to Item2");
    static_assert(GetEnumItem<Test1::TestEnum>(3) == Test1::TestEnum::Item3, "Item not equal to Item3");

    BOOST_CHECK_THROW(
        GetEnumItem<Test1::TestEnum>(4),
        std::out_of_range
    );
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_1_Visitors)
{
    // Enum item is defined, so it is visited correctly
    bool visited = VisitEnumItem(Test1::TestEnum::Item2,
        [](const auto enum_item_t) -> bool {
            BOOST_CHECK_EQUAL(static_cast<std::uint8_t>(decltype(enum_item_t)::value), 4);
            return true;
        });

    BOOST_CHECK_MESSAGE(visited, "Test1::TestEnum::Item2 was not visited correctly.");

    // Enum item is not defined, so it is not visited
    visited = VisitEnumItem(static_cast<Test1::TestEnum>(7),
        [](const auto enum_item_t) -> bool {
            return true;
        });

    BOOST_CHECK_MESSAGE(!visited, "Test1::TestEnum(7) was visited, but shouldn't");

    Test1::EnumVisitor v;
    visited = VisitEnumItem(Test1::TestEnum::Item1, v);

    BOOST_CHECK_MESSAGE(visited, "Test1::TestEnum::Item1 was not visited correctly.");
    BOOST_CHECK_EQUAL(visited, v.IsVisited());
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_1_For_Loop_Iteration)
{
    for (size_t i = 0; i < GetEnumItemsCount<Test1::TestEnum>(); ++i)
    {
        if (i == 0) { BOOST_CHECK(GetEnumItem<Test1::TestEnum>(i) == Test1::TestEnum::Item0); }
        if (i == 1) { BOOST_CHECK(GetEnumItem<Test1::TestEnum>(i) == Test1::TestEnum::Item1); }
        if (i == 2) { BOOST_CHECK(GetEnumItem<Test1::TestEnum>(i) == Test1::TestEnum::Item2); }
        if (i == 3) { BOOST_CHECK(GetEnumItem<Test1::TestEnum>(i) == Test1::TestEnum::Item3); }
    }
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_1_For_Loop_Iterators)
{
    int i = 0;

    for (auto it = GetEnumBegin<Test1::TestEnum>(); it != GetEnumEnd<Test1::TestEnum>(); ++it)
    {
        if (i == 0) { BOOST_CHECK(*it == Test1::TestEnum::Item0); }
        if (i == 1) { BOOST_CHECK(*it == Test1::TestEnum::Item1); }
        if (i == 2) { BOOST_CHECK(*it == Test1::TestEnum::Item2); }
        if (i == 3) { BOOST_CHECK(*it == Test1::TestEnum::Item3); }

        ++i;
    }

    auto begin = GetEnumBegin<Test1::TestEnum>();
    BOOST_CHECK_THROW(--begin, std::out_of_range);
    BOOST_CHECK_THROW(begin--, std::out_of_range);
    BOOST_CHECK_THROW(begin + 5, std::out_of_range);
    BOOST_CHECK_THROW(begin - 5, std::out_of_range);

    auto end = GetEnumEnd<Test1::TestEnum>();
    BOOST_CHECK_THROW(++end, std::out_of_range);
    BOOST_CHECK_THROW(end++, std::out_of_range);
    BOOST_CHECK_THROW(end + 5, std::out_of_range);
    BOOST_CHECK_THROW(end - 5, std::out_of_range);
    BOOST_CHECK_THROW(*end, std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_1_For_Loop_Reverse_Iterators)
{
    int i = 0;

    for (auto it = GetEnumRBegin<Test1::TestEnum>(); it != GetEnumREnd<Test1::TestEnum>(); ++it)
    {
        if (i == 0) { BOOST_CHECK(*it == Test1::TestEnum::Item3); }
        if (i == 1) { BOOST_CHECK(*it == Test1::TestEnum::Item2); }
        if (i == 2) { BOOST_CHECK(*it == Test1::TestEnum::Item1); }
        if (i == 3) { BOOST_CHECK(*it == Test1::TestEnum::Item0); }

        ++i;
    }

    auto begin = GetEnumRBegin<Test1::TestEnum>();
    BOOST_CHECK_THROW(--begin, std::out_of_range);
    BOOST_CHECK_THROW(begin--, std::out_of_range);
    BOOST_CHECK_THROW(begin + 5, std::out_of_range);
    BOOST_CHECK_THROW(begin - 5, std::out_of_range);

    auto end = GetEnumREnd<Test1::TestEnum>();
    BOOST_CHECK_THROW(++end, std::out_of_range);
    BOOST_CHECK_THROW(end++, std::out_of_range);
    BOOST_CHECK_THROW(end + 5, std::out_of_range);
    BOOST_CHECK_THROW(end - 5, std::out_of_range);
    BOOST_CHECK_THROW(*end, std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_1_For_Loop_Range_Based)
{
    int i = 0;

    for (auto enum_item : GetEnumRange<Test1::TestEnum>())
    {
        if (i == 0) { BOOST_CHECK(enum_item == Test1::TestEnum::Item0); }
        if (i == 1) { BOOST_CHECK(enum_item == Test1::TestEnum::Item1); }
        if (i == 2) { BOOST_CHECK(enum_item == Test1::TestEnum::Item2); }
        if (i == 3) { BOOST_CHECK(enum_item == Test1::TestEnum::Item3); }

        ++i;
    }
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_1_For_Loop_Range_Based_Reversed)
{
    int i = 0;

    for (auto enum_item : GetEnumReverseRange<Test1::TestEnum>())
    {
        if (i == 0) { BOOST_CHECK(enum_item == Test1::TestEnum::Item3); }
        if (i == 1) { BOOST_CHECK(enum_item == Test1::TestEnum::Item2); }
        if (i == 2) { BOOST_CHECK(enum_item == Test1::TestEnum::Item1); }
        if (i == 3) { BOOST_CHECK(enum_item == Test1::TestEnum::Item0); }

        ++i;
    }
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_1_Static_For_Each)
{
    std::vector<Test1::TestEnum> enum_items;

    EnumStaticForEach<Test1::TestEnum>([&enum_items](auto enum_item_t) {
        static_assert(
            IsEnumItemDefined<Test1::TestEnum>(enum_item_t),
            "Enum item should be defined."
        );
        enum_items.push_back(enum_item_t);
    });

    const std::vector<Test1::TestEnum> expected_enum_items{
        Test1::TestEnum::Item0,
        Test1::TestEnum::Item1,
        Test1::TestEnum::Item2,
        Test1::TestEnum::Item3
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        enum_items.begin(),
        enum_items.end(),
        expected_enum_items.begin(),
        expected_enum_items.end()
    );
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_2_Constexpr_Functions)
{
    static_assert(IsSmart<Test2::TestEnum>(), "Enum is not smart");
    static_assert(std::is_same<std::underlying_type_t<Test2::TestEnum>, std::uint32_t>::value, "Invalid Enum underlying type");

    static_assert(GetEnumItemsCount<Test2::TestEnum>() == 3, "Invalid Enum Items count");

    static_assert(IsEnumItemDefined(Test2::TestEnum::Item0), "Item0 is not defained");
    static_assert(IsEnumItemDefined(Test2::TestEnum::Item1), "Item1 is not defained");
    static_assert(IsEnumItemDefined(Test2::TestEnum::Item2), "Item2 is not defained");
    static_assert(!IsEnumItemDefined(static_cast<Test2::TestEnum>(5)), "Item should not be defined");

    static_assert(strings_equal(GetEnumItemName(Test2::TestEnum::Item0), "Item0"), "Item0 name is invalid");
    static_assert(strings_equal(GetEnumItemName(Test2::TestEnum::Item1), "Item1"), "Item1 name is invalid");
    static_assert(strings_equal(GetEnumItemName(Test2::TestEnum::Item2), "Item2"), "Item2 name is invalid");
    static_assert(strings_equal(GetEnumItemName(static_cast<Test2::TestEnum>(6)), ""), "Enum name should be empty");

    static_assert(static_cast<Test2::TestEnum>(0) == Test2::TestEnum::Item0, "Invalid Item0 value");
    static_assert(static_cast<Test2::TestEnum>(1) == Test2::TestEnum::Item1, "Invalid Item1 value");
    static_assert(static_cast<Test2::TestEnum>(2) == Test2::TestEnum::Item2, "Invalid Item2 value");

    static_assert(GetEnumItem<Test2::TestEnum>(0) == Test2::TestEnum::Item0, "Item not equal to Item0");
    static_assert(GetEnumItem<Test2::TestEnum>(1) == Test2::TestEnum::Item1, "Item not equal to Item1");
    static_assert(GetEnumItem<Test2::TestEnum>(2) == Test2::TestEnum::Item2, "Item not equal to Item2");

    BOOST_CHECK_THROW(
        GetEnumItem<Test2::TestEnum>(5),
        std::out_of_range
    );
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_2_String_Property)
{
    BOOST_CHECK_EQUAL(GetEnumString(Test2::TestEnum::Item0), std::string("EnumString0"));
    BOOST_CHECK_EQUAL(GetEnumString(Test2::TestEnum::Item1), std::string("EnumString1"));
    BOOST_CHECK_EQUAL(GetEnumString(Test2::TestEnum::Item2), std::string("EnumString2"));
    BOOST_CHECK_EQUAL(GetEnumString(static_cast<Test2::TestEnum>(3)), std::string("DefaultEnumString"));
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_3_String_Propery)
{
    BOOST_CHECK_EQUAL(GetEnumString(Test3::TestEnum::Item0), std::string_view("Enum Caption 0"));
    BOOST_CHECK_EQUAL(GetEnumString(Test3::TestEnum::Item1), std::string_view("Enum Caption 1"));
    BOOST_CHECK_EQUAL(GetEnumString(Test3::TestEnum::Item2), std::string_view("Enum Caption 2"));
    BOOST_CHECK_EQUAL(GetEnumString(static_cast<Test3::TestEnum>(13)), std::string_view("Default Caption"));
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_3_Description_Property)
{
    BOOST_CHECK_EQUAL(GetEnumDescription(Test3::TestEnum::Item0), std::string_view("Enum Tooltip 0"));
    BOOST_CHECK_EQUAL(GetEnumDescription(Test3::TestEnum::Item1), std::string_view("Enum Tooltip 1"));
    BOOST_CHECK_EQUAL(GetEnumDescription(Test3::TestEnum::Item2), std::string_view("Enum Tooltip 2"));
    BOOST_CHECK_EQUAL(GetEnumDescription(static_cast<Test3::TestEnum>(13)), std::string_view("Default Tooltip"));
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_4_Constexpr_Functions)
{
    static_assert(IsSmart<Test4::TestColor>(), "Enum is not smart");
    static_assert(std::is_same<std::underlying_type_t<Test4::TestColor>, int>::value, "Invalid Enum underlying type");

    static_assert(GetEnumItemsCount<Test4::TestColor>() == 3, "Invalid Enum Items count");

    static_assert(IsEnumItemDefined(Test4::TestColor::Red), "Red is not defained");
    static_assert(IsEnumItemDefined(Test4::TestColor::Green), "Green is not defained");
    static_assert(IsEnumItemDefined(Test4::TestColor::Yellow), "Yellow is not defained");
    static_assert(!IsEnumItemDefined(static_cast<Test4::TestColor>(3)), "Item should not be defined");

    static_assert(strings_equal(GetEnumItemName(Test4::TestColor::Red), "Red"), "Red name is invalid");
    static_assert(strings_equal(GetEnumItemName(Test4::TestColor::Green), "Green"), "Green name is invalid");
    static_assert(strings_equal(GetEnumItemName(Test4::TestColor::Yellow), "Yellow"), "Yellow name is invalid");
    static_assert(strings_equal(GetEnumItemName(static_cast<Test4::TestColor>(6)), ""), "Enum name should be empty");

    static_assert(static_cast<Test4::TestColor>(0) == Test4::TestColor::Red, "Invalid Red value");
    static_assert(static_cast<Test4::TestColor>(1) == Test4::TestColor::Green, "Invalid Green value");
    static_assert(static_cast<Test4::TestColor>(2) == Test4::TestColor::Yellow, "Invalid Yellow value");

    static_assert(GetEnumItem<Test4::TestColor>(0) == Test4::TestColor::Red, "Item not equal to Red");
    static_assert(GetEnumItem<Test4::TestColor>(1) == Test4::TestColor::Green, "Item not equal to Green");
    static_assert(GetEnumItem<Test4::TestColor>(2) == Test4::TestColor::Yellow, "Item not equal to Yellow");

    BOOST_CHECK_THROW(
        GetEnumItem<Test4::TestColor>(5),
        std::out_of_range
    );
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_4_Text_Property)
{
    BOOST_CHECK_EQUAL(GetEnumText(Test4::TestColor::Red), "Red");
    BOOST_CHECK_EQUAL(GetEnumText(Test4::TestColor::Green), "Green");
    BOOST_CHECK_EQUAL(GetEnumText(Test4::TestColor::Yellow), "Yellow");
    BOOST_CHECK_EQUAL(GetEnumText(static_cast<Test4::TestColor>(13)), std::string(""));
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_4_Tooltip_Property)
{
    BOOST_CHECK_EQUAL(GetEnumTooltip(Test4::TestColor::Red), std::string("Some long red color text tooltip"));
    BOOST_CHECK_EQUAL(GetEnumTooltip(Test4::TestColor::Green), std::string("Some long green color text tooltip"));
    BOOST_CHECK_EQUAL(GetEnumTooltip(Test4::TestColor::Yellow), std::string("Some long yellow color text tooltip"));
    BOOST_CHECK_EQUAL(GetEnumTooltip(static_cast<Test4::TestColor>(13)), std::string(""));
}

BOOST_AUTO_TEST_CASE(Test_Smart_Enum_4_NextColor_Property)
{
    BOOST_CHECK_EQUAL(GetEnumNextColor(Test4::TestColor::Red), Test4::TestColor::Green);
    BOOST_CHECK_EQUAL(GetEnumNextColor(Test4::TestColor::Green), Test4::TestColor::Yellow);
    BOOST_CHECK_EQUAL(GetEnumNextColor(Test4::TestColor::Yellow), Test4::TestColor::Red);
    BOOST_CHECK_EQUAL(GetEnumNextColor(static_cast<Test4::TestColor>(13)), Test4::TestColor::Red);
}

BOOST_AUTO_TEST_SUITE_END();

} // Test
} // Utils
} // Cvs
