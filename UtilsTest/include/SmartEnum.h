#pragma once

#include <stdexcept>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/variadic.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/reverse_iterator.hpp>

//--------------------------------------------------------------------------//
// Macros used in methods implementation

#define STATIC_ASSERT_SMART_ENUM(EnumName, FunctionName) \
static_assert( Cvs::Utils::SmartEnum::IsSmart<EnumName>(), \
               BOOST_PP_STRINGIZE(FunctionName) " function is allowed only for smart enums." \
    )

#define SE_ENUM_ITEM_DECL_TPL(tuple)                BOOST_PP_TUPLE_ELEM(0, tuple)
#define SE_ENUM_ITEM_TPL(e, tuple)                  e::SE_ENUM_ITEM_DECL_TPL(tuple)
#define SE_ENUM_ITEM_VAL_TPL(tuple)                 BOOST_PP_TUPLE_ELEM(1, tuple)
#define SE_ENUM_ITEMS(r, e, tuple)                  SE_ENUM_ITEM_DECL_TPL(tuple) = SE_ENUM_ITEM_VAL_TPL(tuple),
#define SE_ENUM_ITEM_CASE_I_TUPLE(r, e, i, tuple)   case static_cast<size_t>(i): return SE_ENUM_ITEM_TPL(e, tuple);
#define SE_ITEM_CASE_DEFINED_TUPLE(r, e, tuple)     case SE_ENUM_ITEM_TPL(e, tuple): return true;
#define SE_ITEM_CASE_NAME_TUPLE(r, e, tuple)        case SE_ENUM_ITEM_TPL(e, tuple): return BOOST_PP_STRINGIZE(SE_ENUM_ITEM_DECL_TPL(tuple));
#define SE_ITEM_CASE_VISITOR_TUPLE(r, e, tuple)     case SE_ENUM_ITEM_TPL(e, tuple): return Private::VisitEnum<e, SE_ENUM_ITEM_TPL(e, tuple)>(_visitor);
#define SE_PROPERTY_ENUM_NAME(tuple)                BOOST_PP_TUPLE_ELEM(0, tuple)
#define SE_PROPERTY_METHOD_NAME(tuple)              BOOST_PP_CAT(GetEnum, BOOST_PP_TUPLE_ELEM(0, tuple))
#define SE_PROPERTY_DEFAULT_VALUE(tuple)            BOOST_PP_TUPLE_ELEM(1, tuple)

#define SE_DECLARE_PROPERTY_METHOD(r, enum_name, tuple_elem) \
BOOST_PP_TUPLE_ELEM(0, tuple_elem) BOOST_PP_CAT(GetEnum, BOOST_PP_TUPLE_ELEM(1, tuple_elem)) (enum_name); \

#define SE_DECLARE_PROPERTY_DECLVAL_METHOD(r, enum_name, tuple_elem) \
BOOST_PP_TUPLE_ELEM(0, tuple_elem) BOOST_PP_CAT(GetEnum, BOOST_PP_TUPLE_ELEM(1, tuple_elem)) ();

#define SE_CASE_ITEM_I(r, enum_name_and_i, tuple) \
    case BOOST_PP_TUPLE_ELEM(0, enum_name_and_i) :: BOOST_PP_TUPLE_ELEM(0, tuple): \
        return BOOST_PP_TUPLE_ELEM(BOOST_PP_TUPLE_ELEM(1, enum_name_and_i), tuple);

//--------------------------------------------------------------------------//
/* 
 * Smart enum class declaration; syntax:
 *      SMART_ENUM ( EnumName, UnderlyingType, DefaultItemName, (Item0, Value0), (Item1, Value1), ... (ItemN, ValueN) )
 */
#define SMART_ENUM(EnumName, UnderlyingType, DefaultItemName, ...) \
enum class EnumName : UnderlyingType { \
    BOOST_PP_SEQ_FOR_EACH( SE_ENUM_ITEMS, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
}; \
\
namespace Cvs::Utils::SmartEnum { \
\
template<> \
inline constexpr bool IsSmart<EnumName>() { return true; } \
\
template<> \
inline constexpr std::size_t GetEnumItemsCount<EnumName>() \
{ return BOOST_PP_VARIADIC_SIZE(__VA_ARGS__); } \
\
template<> \
inline constexpr bool IsEnumItemDefined<EnumName>(EnumName _val) \
{ \
    switch (_val) { \
        BOOST_PP_SEQ_FOR_EACH(SE_ITEM_CASE_DEFINED_TUPLE, EnumName, \
        BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
        default: break; \
    } \
    return false; \
} \
template<> \
inline constexpr const char * GetEnumItemName<EnumName>(EnumName _val) \
{ \
    switch (_val) { \
        BOOST_PP_SEQ_FOR_EACH(SE_ITEM_CASE_NAME_TUPLE, EnumName, \
        BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
        default: break; \
    } \
    return DefaultItemName; \
} \
template<typename TVisitor> \
bool VisitEnumItem(EnumName _val, TVisitor && _visitor) \
{ \
    switch (_val) { \
        BOOST_PP_SEQ_FOR_EACH(SE_ITEM_CASE_VISITOR_TUPLE, EnumName, \
        BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
        default: break; \
    } \
    return false; \
} \
template<> \
inline constexpr EnumName GetEnumItem<EnumName>(std::size_t _idx) \
{ \
    switch (_idx) { \
        BOOST_PP_SEQ_FOR_EACH_I(SE_ENUM_ITEM_CASE_I_TUPLE, EnumName, \
        BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
        default: throw std::out_of_range("Enum index is out of range"); \
    } \
} \
\
}

//--------------------------------------------------------------------------//
/*
 *  Smart enum class with properties declaration; syntax:
 *      SMART_ENUM_EX (
 *           EnumName, UnderlyingType, DefaultItemName,
 *       
 *           // Properties
 *           ((ReturnType0, Property0), (ReturnType1, Property1), ... (ReturnTypeN, PropertyN)),
 *       
 *           // Items
 *           (Item0, Value0),
 *           (Item1, Value1),
 *           ...
 *           (ItemN, ValueN),
 *       ) 
 */
#define SMART_ENUM_EX(EnumName, UnderlyingType, DefaultItemName, Properties, ...) \
SMART_ENUM(EnumName, UnderlyingType, DefaultItemName, __VA_ARGS__) \
namespace Cvs::Utils::SmartEnum { \
\
template<> \
inline constexpr bool Private::IsSmartEx<EnumName>() { return true; } \
\
BOOST_PP_SEQ_FOR_EACH (SE_DECLARE_PROPERTY_METHOD, EnumName, BOOST_PP_TUPLE_TO_SEQ(Properties)) \
\
template<> \
struct Private::DeclvalHelper<EnumName> { \
BOOST_PP_SEQ_FOR_EACH (SE_DECLARE_PROPERTY_DECLVAL_METHOD, EnumName, BOOST_PP_TUPLE_TO_SEQ(Properties)) \
}; \
\
}

#define SE_DEFINE_PROPERTY_METHOD(unused, EnumData, I, TupleElem) \
namespace Cvs::Utils::SmartEnum { \
decltype(Private::DeclvalHelper<SE_PROPERTY_ENUM_NAME(EnumData)>().SE_PROPERTY_METHOD_NAME(TupleElem)()) \
SE_PROPERTY_METHOD_NAME(TupleElem) (SE_PROPERTY_ENUM_NAME(EnumData) _val) { \
    switch (_val) { \
        BOOST_PP_SEQ_FOR_EACH( \
            SE_CASE_ITEM_I , \
            (SE_PROPERTY_ENUM_NAME(EnumData), BOOST_PP_ADD(I, 1)), \
            BOOST_PP_TUPLE_TO_SEQ(BOOST_PP_TUPLE_POP_FRONT(EnumData)) \
        ) \
        default: break; \
    } \
    return SE_PROPERTY_DEFAULT_VALUE(TupleElem); \
} \
}

//--------------------------------------------------------------------------//
/*
 *  Smart enum properties definition; syntax:
 *     SMART_ENUM_PROPERTY_MAP (
 *        // Items                  // Property 0                           // Property 1                                // Property N
 *        (EnumName,                (ReturnType0, DefaultPropertyValue0),   (ReturnType1, DefaultPropertyValue1)  ... ,  (ReturnTypeN, DefaultPropertyValueN)),
 *         (Item0,                   PropertyValue0,                         PropertyValue0,                      ... ,   PropertyValue0),
 *         (Item1,                   PropertyValue1,                         PropertyValue1,                      ... ,   PropertyValue1),
 *         (... ,                    ... ,                                   ... ,                                ... ,   ...           )
 *         (ItemN,                   PropertyValueN,                         PropertyValueN,                      ... ,   PropertyValueN)
 *     )
 */
#define SMART_ENUM_PROPERTY_MAP(DefaultMapping, ...) \
BOOST_PP_SEQ_FOR_EACH_I( \
    SE_DEFINE_PROPERTY_METHOD, \
    BOOST_PP_TUPLE_PUSH_FRONT(BOOST_PP_VARIADIC_TO_TUPLE(__VA_ARGS__), BOOST_PP_TUPLE_ELEM(0, DefaultMapping)), \
    BOOST_PP_TUPLE_TO_SEQ(BOOST_PP_TUPLE_POP_FRONT(DefaultMapping)) \
)

//--------------------------------------------------------------------------//

namespace Cvs {
namespace Utils {
namespace SmartEnum {

//--------------------------------------------------------------------------//
// Visitor class for generic Enum items handling
template<class TEnum, class Derived>
class EnumVisitor
{
public:
    template<TEnum EnumVal>
    bool operator()(std::integral_constant<TEnum, EnumVal>)
    {
        auto * derived = static_cast<Derived*>(this);
        derived-> template VisitEnum<EnumVal>();
        return true;
    }
};

//--------------------------------------------------------------------------//

template<typename TEnum>
constexpr bool IsSmart() { return false; }

namespace Private {

template<typename TEnum>
constexpr bool IsSmartEx() { return false; }

template<typename TEnum>
struct DeclvalHelper {};

} // namespace Private

namespace Private {

template<class TEnum, TEnum EnumVal, class TEnumVisitor>
bool VisitEnum(TEnumVisitor&& visitor)
{
    return visitor(std::integral_constant<TEnum, EnumVal>());
}

} // namespace Private

template<typename TEnum>
constexpr bool IsEnumItemDefined(TEnum _val)
{
    STATIC_ASSERT_SMART_ENUM(TEnum, IsEnumItemDefined());
    return false;
}

//--------------------------------------------------------------------------//

template<typename TEnum>
constexpr TEnum GetEnumItem(std::size_t _idx)
{
    STATIC_ASSERT_SMART_ENUM(TEnum, GetEnumItem());
    return static_cast<TEnum>(std::underlying_type_t<TEnum>{});
}

//--------------------------------------------------------------------------//

template<typename TEnum>
constexpr size_t GetEnumItemsCount()
{
    STATIC_ASSERT_SMART_ENUM(TEnum, GetEnumItemsCount());
    return 0;
}

//--------------------------------------------------------------------------//

template<typename TEnum>
constexpr const char * GetEnumItemName(TEnum _val)
{
    STATIC_ASSERT_SMART_ENUM(TEnum, GetEnumItemName());
    return nullptr;
}

//--------------------------------------------------------------------------//
// Enum iterators implementation

template<class TEnum>
class EnumIterator
    : public boost::iterator_facade <
          EnumIterator<TEnum>,
          TEnum,
          boost::random_access_traversal_tag,
          TEnum,
          std::ptrdiff_t
          >
{
public:
    explicit EnumIterator(std::size_t _idx)
        : EnumItemIdx { static_cast<std::ptrdiff_t>(_idx) }
    {
    }

    ~EnumIterator() noexcept = default;

private:
    friend class boost::iterator_core_access;

    TEnum dereference() const
    {
        if (EnumItemIdx < 0 || EnumItemIdx >= static_cast<std::ptrdiff_t>(GetEnumItemsCount<TEnum>()))
            throw std::out_of_range("Enum iterator is out of range.");

        return GetEnumItem<TEnum>(static_cast<size_t>(EnumItemIdx));
    }

    bool equal(const EnumIterator& _rhs) const
    {
        return EnumItemIdx == _rhs.EnumItemIdx;
    }

    void increment()
    {
        advance(static_cast< std::ptrdiff_t>(1));
    }

    void decrement()
    {
        advance(static_cast<std::ptrdiff_t>(-1));
    }

    void advance(std::ptrdiff_t n)
    {
        if((EnumItemIdx + n < 0) || (EnumItemIdx + n > static_cast<std::ptrdiff_t>(GetEnumItemsCount<TEnum>())))
            throw std::out_of_range("Enum iterator is out of range.");

        EnumItemIdx += n;
    }

    std::ptrdiff_t distance_to(const EnumIterator& _rhs) const
    {
        return _rhs.EnumItemIdx - EnumItemIdx;
    }

    std::ptrdiff_t EnumItemIdx;
};

template<class TEnum>
class EnumReverseIterator
    : public boost::reverse_iterator<EnumIterator<TEnum>>
{
public:
    explicit EnumReverseIterator(std::size_t _idx)
        : boost::reverse_iterator<EnumIterator<TEnum>> {
              EnumIterator<TEnum> {_idx}
          } {}
    ~EnumReverseIterator() noexcept = default;
};

//--------------------------------------------------------------------------//

template<typename TEnum>
auto GetEnumBegin()
{
    STATIC_ASSERT_SMART_ENUM(TEnum, GetEnumBegin());
    return EnumIterator<TEnum> { 0 };
}

template<typename TEnum>
auto GetEnumEnd()
{
    STATIC_ASSERT_SMART_ENUM(TEnum, GetEnumEnd());
    return EnumIterator<TEnum> { GetEnumItemsCount<TEnum>() };
}

template<typename TEnum>
auto GetEnumRBegin()
{
    STATIC_ASSERT_SMART_ENUM(TEnum, GetEnumRBegin());
    return EnumReverseIterator<TEnum> { GetEnumItemsCount<TEnum>() };
}

template<typename TEnum>
auto GetEnumREnd()
{
    STATIC_ASSERT_SMART_ENUM(TEnum, GetEnumREnd());
    return EnumReverseIterator<TEnum> { 0 };
}

namespace Private {

template<class TEnum>
class EnumRangeAdaptor
{
public:
    auto begin() { return GetEnumBegin<TEnum>(); }
    auto end()   { return GetEnumEnd<TEnum>(); }
};

template<class TEnum>
class EnumRangeReverseAdaptor
{
public:
    auto begin() { return GetEnumRBegin<TEnum>(); }
    auto end()   { return GetEnumREnd<TEnum>(); }
};

} // namespace Private

//--------------------------------------------------------------------------//

template<typename TEnum>
auto GetEnumRange()
{
    STATIC_ASSERT_SMART_ENUM(TEnum, GetEnumRange());
    return Private::EnumRangeAdaptor<TEnum>();
}

template<typename TEnum>
auto GetEnumReverseRange()
{
    STATIC_ASSERT_SMART_ENUM(TEnum, GetEnumReverseRange());
    return Private::EnumRangeReverseAdaptor<TEnum>();
}

//--------------------------------------------------------------------------//

namespace Private
{

template<typename TEnum, size_t I, size_t N, class Func>
constexpr void StaticForLoop(Func && func)
{
    if constexpr (I < N)
    {
        func(std::integral_constant<TEnum, GetEnumItem<TEnum>(I)> {});
        StaticForLoop<TEnum, I + 1, N>(func);
    }
}

} // namespace Private

//--------------------------------------------------------------------------//

template<typename TEnum, typename Func>
void EnumStaticForEach(Func && func)
{
    STATIC_ASSERT_SMART_ENUM(TEnum, EnumStaticForEach());
    Private::StaticForLoop<TEnum, 0, GetEnumItemsCount<TEnum>()>(func);
}

//--------------------------------------------------------------------------//

} // namespace SmartEnum
} // namespace Utils
} // namespace Cvs

//--------------------------------------------------------------------------//
