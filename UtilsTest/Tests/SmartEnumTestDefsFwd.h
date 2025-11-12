#pragma once

#include <cstdint>

namespace Cvs::Utils::Test
{

namespace Test1 {
    enum class TestEnum : std::uint8_t;
}

namespace Test2 {
    enum class TestEnum : std::uint32_t;
}

namespace Test3 {
    enum class TestEnum : short;
}

namespace Test4 {
    enum class TestColor;
}

} // namespace Cvs::Utils::Test
