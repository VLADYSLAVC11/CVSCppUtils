#pragma once

#include "Polymorphic.h"

/*---------------------------------------------------------------------------*/

namespace Cvs::Utils::Test {

/*---------------------------------------------------------------------------*/

class Test0Obj1;
class Test0Obj2;
class Test0Obj3;
class Test0Obj4;

#define TEST_OBJECTS_0_TYPES_LIST Test0Obj1, Test0Obj2, Test0Obj3, Test0Obj4

/*---------------------------------------------------------------------------*/

class Test0Obj : public Polymorphic<1, 4, Test0Obj, TEST_OBJECTS_0_TYPES_LIST>
{
public:
    PM_VIRTUAL_CONSTRUCTOR(Test0Obj);
    PM_VIRTUAL_DESTRUCTOR(TEST_OBJECTS_0_TYPES_LIST, Test0Obj);
    id_type Init(int num);
    PM_VIRTUAL int GetValue() const;
};

/*---------------------------------------------------------------------------*/

template<int Value>
class Test0ObjImpl
{
public:
    int GetValue() const noexcept
    {
        return Value;
    }
};

class Test0Obj1 : public Test0ObjImpl<1> {};
class Test0Obj2 : public Test0ObjImpl<2> {};
class Test0Obj3 : public Test0ObjImpl<3> {};
class Test0Obj4 : public Test0ObjImpl<4> {};

/*---------------------------------------------------------------------------*/



} // namespace Cvs::Utils::Test
