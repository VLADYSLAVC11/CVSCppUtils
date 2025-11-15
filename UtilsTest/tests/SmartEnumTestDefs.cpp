#include "SmartEnumTestDefs.h"

SMART_ENUM_PROPERTY_MAP (
  // Items                            // Properties
  (Cvs::Utils::Test::Test2::TestEnum,  (String, "DefaultEnumString")),
   (Item0,                             "EnumString0"),
   (Item1,                             "EnumString1"),
   (Item2,                             "EnumString2")
)

SMART_ENUM_PROPERTY_MAP (
  // Items                            // Properties
  (Cvs::Utils::Test::Test3::TestEnum,  (String, "Default Caption"), (Description, "Default Tooltip")),
   (Item0,                             "Enum Caption 0",            "Enum Tooltip 0"),
   (Item1,                             "Enum Caption 1",            "Enum Tooltip 1"),
   (Item2,                             "Enum Caption 2",            "Enum Tooltip 2")
)

constexpr const char RedColorTooltip[]    = "Some long red color text tooltip";
constexpr const char GreenColorTooltip[]  = "Some long green color text tooltip";
constexpr const char YellowColorTooltip[] = "Some long yellow color text tooltip";

using TestColor = Cvs::Utils::Test::Test4::TestColor;

SMART_ENUM_PROPERTY_MAP (
  // Items                             // Properties
  (Cvs::Utils::Test::Test4::TestColor,  (Text, ""),     (Tooltip, ""),          (NextColor, TestColor::Red)),
   (Red,                                "Red",          RedColorTooltip,        TestColor::Green),
   (Green,                              "Green",        GreenColorTooltip,      TestColor::Yellow),
   (Yellow,                             "Yellow",       YellowColorTooltip,     TestColor::Red)
)
