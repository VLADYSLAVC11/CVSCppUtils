#include "PolymorphicObjectTestDefs.h"

/*---------------------------------------------------------------------------*/

namespace Cvs::Utils::Test {

/*---------------------------------------------------------------------------*/

Test0Obj::id_type Test0Obj::Init(int num)
{
    switch(num)
    {
        case 1: Polymorhic::Init<Test0Obj1>(); break;
        case 2: Polymorhic::Init<Test0Obj2>(); break;
        case 3: Polymorhic::Init<Test0Obj3>(); break;
        case 4: Polymorhic::Init<Test0Obj4>(); break;
        default: break;
    }

    return TypeId();
}

/*---------------------------------------------------------------------------*/

PM_VIRTUAL_DESTRUCTOR_DEF(TEST_OBJECTS_0_TYPES_LIST, Test0Obj) = default;

/*---------------------------------------------------------------------------*/

int Test0Obj::GetValue() const
{
    PM_VIRTUAL_METHOD_DEF(TEST_OBJECTS_0_TYPES_LIST, GetValue());
}

/*---------------------------------------------------------------------------*/

} // namespace Cvs::Utils::Test
