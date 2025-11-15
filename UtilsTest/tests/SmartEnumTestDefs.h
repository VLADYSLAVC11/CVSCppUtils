#pragma once

#include "SmartEnum.h"
#include "SmartEnumTestDefsFwd.h"

#include <string>
#include <string_view>

SMART_ENUM (
    Cvs::Utils::Test::Test1::TestEnum, std::uint8_t, "",
        
    // Items
    (Item0, 0),
    (Item1, 1),
    (Item2, 4),
    (Item3, 3)
)

SMART_ENUM_EX (
    Cvs::Utils::Test::Test2::TestEnum, std::uint32_t, "",
        
    // Properties
    ((const char *, String)),

    // Items
    (Item0, 0),
    (Item1, 1),
    (Item2, 2)
)

SMART_ENUM_EX (
    Cvs::Utils::Test::Test3::TestEnum, short, "",
        
    // Properties
    ((std::string_view, String), (std::string_view, Description)),

    // Items
    (Item0, 2),
    (Item1, 1),
    (Item2, 0)
)

SMART_ENUM_EX (
    Cvs::Utils::Test::Test4::TestColor, int, "",
        
    // Properties
    ((std::string, Text), (std::string, Tooltip), (Cvs::Utils::Test::Test4::TestColor, NextColor)),

    // Items
    (Red,    0),
    (Green,  1),
    (Yellow, 2)
)

namespace Cvs::Utils::Test {
namespace Test1 {
    
class EnumVisitor
    : public Cvs::Utils::SmartEnum::EnumVisitor<TestEnum, EnumVisitor>
{
private:
    friend class Cvs::Utils::SmartEnum::EnumVisitor<TestEnum, EnumVisitor>;

    template<TestEnum EnumValue>
    void VisitEnum()
    {
        Visited = true;
        
        static_assert(
                Cvs::Utils::SmartEnum::IsEnumItemDefined(EnumValue)
            ,   "Item is not defained"
        );
    }

    bool Visited = false;

public:
    bool IsVisited() const { return Visited; }
};

} // namespace Test1
} // namespace CommonUtilsTest





