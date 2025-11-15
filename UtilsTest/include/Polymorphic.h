#pragma once

#include <tuple>
#include <array>
#include <typeinfo>
#include <cstdint>
#include <algorithm>
#include <type_traits>

#include <boost/noncopyable.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#define PM_VIRTUAL

#define PM_CASE_I(r, data, i, elem) \
case BOOST_PP_ADD(i, 1) : return ObjectPtr< BOOST_PP_ADD(i, 1) >()-> data ;

#define PM_DTOR_DECL(r, data, elem) \
static void __DestroyImpl(elem * data) noexcept;

#define PM_DTOR_DEF(r, data, elem) \
void data::__DestroyImpl(elem * obj) noexcept { \
    obj->~elem(); \
}

#define PM_VIRTUAL_METHOD_DEF(types_list, method_call) \
switch (TypeId()) \
{ \
    BOOST_PP_SEQ_FOR_EACH_I(PM_CASE_I, method_call, BOOST_PP_VARIADIC_TO_SEQ(types_list)) \
    default: throw BadPolymorphicObject {}; \
}

#define PM_VIRTUAL_CONSTRUCTOR(self_type) \
public: \
using Polymorhic::Polymorhic; \
template<class T, class ... TParams> \
self_type(TParams && ... params) \
{ \
    if(!Polymorhic::Init<T, TParams...>(std::forward<TParams>(params)...)) \
        throw BadPolymorphicObject {}; \
} \
self_type() noexcept = default; \
self_type(self_type && obj) noexcept = default; \
self_type& operator = (self_type && obj) noexcept = default

#define PM_VIRTUAL_DESTRUCTOR(types_list, self_type) \
friend class PolymorhicBase; \
protected: \
BOOST_PP_SEQ_FOR_EACH(PM_DTOR_DECL, obj, BOOST_PP_VARIADIC_TO_SEQ(types_list)) \
public: \
~self_type() noexcept

#define PM_VIRTUAL_DESTRUCTOR_DEF(types_list, self_type) \
BOOST_PP_SEQ_FOR_EACH(PM_DTOR_DEF, self_type, BOOST_PP_VARIADIC_TO_SEQ(types_list)) \
self_type::~self_type() noexcept

namespace Cvs {
namespace Utils {

class BadPolymorphicObject : public std::exception
{
public:
    BadPolymorphicObject() noexcept = default;
    ~BadPolymorphicObject() noexcept = default;

    const char * what() const noexcept override
    {
        return "Polymorphic object is bad";
    }
};

class PolymorhicBase
    : protected boost::noncopyable
{
public:
    using id_type = std::uint32_t;
    using block_type = std::uint32_t;

    PolymorhicBase() noexcept = default;
    ~PolymorhicBase() noexcept = default;

protected:
    static constexpr std::size_t AlignBlock(std::size_t value) noexcept
    {
        constexpr std::size_t alignment = sizeof(block_type);
        return ((value + alignment - 1) & ~(alignment - 1)) >> 2;
    }

    template<class TDerived, class TObj>
    static void DestroyObject(TObj* obj)
    {
        TDerived::__DestroyImpl(obj);
    }
};

template<size_t MaxTypeSizeof, size_t Alignment, class TDerived, class ... TObjectsList>
class alignas(Alignment) Polymorhic : public PolymorhicBase
{
public:
    Polymorhic() noexcept = default;

    Polymorhic(Polymorhic && rhs) noexcept
    {
        m_storageBuffer = rhs.m_storageBuffer;
        m_currentTypeId = rhs.m_currentTypeId;
        rhs.m_currentTypeId = 0;
    }

    template<class T>
    Polymorhic(const T& rhs)
    {
        SetTypeId<T>();
        new (m_storageBuffer.data()) T(rhs);
    }
    
    template<class T>
    Polymorhic(T && rhs)
    {
        SetTypeId<T>();
        new (m_storageBuffer.data()) T(rhs);
    }

    ~Polymorhic() noexcept
    {
        Destroy();
    }

    Polymorhic& operator = (Polymorhic && rhs) noexcept
    {
        if(&rhs != this)
        {
            m_storageBuffer = rhs.m_storageBuffer;
            m_currentTypeId = rhs.m_currentTypeId;
            rhs.m_currentTypeId = 0;
        }

        return *this;
    }

    bool operator == (const Polymorhic& rhs) const
    {
        return m_currentTypeId == rhs.m_currentTypeId &&
               memcmp(m_storageBuffer.data(), rhs.m_storageBuffer.data(), sizeof(m_storageBuffer)) == 0;
    }

    bool operator != (const Polymorhic& rhs) const
    {
        return m_currentTypeId != rhs.m_currentTypeId ||
               memcmp(m_storageBuffer.data(), rhs.m_storageBuffer.data(), sizeof(m_storageBuffer)) != 0;
    }

    template<class T, class ... TParams>
    id_type Init(TParams && ... params)
    {
        Destroy();
        SetTypeId<T>();
        new (m_storageBuffer.data()) T(std::forward<TParams>(params)...);
        return TypeId();
    }

    id_type TypeId() const noexcept
    {
        return m_currentTypeId;
    }

    bool IsInitialized() const noexcept
    {
        return !IsNull();
    }

    bool IsNull() const noexcept
    {
        return TypeId() == 0;
    }

    void Destroy() noexcept
    {
        if (IsInitialized())
        {
            DestroyImpl(TypeId());
            m_currentTypeId = 0;
        }
    }

    template<class T>
    T DownCast() const
    {
        static_assert(
                std::is_pointer_v<T> || std::is_reference_v<T>
            ,   "Casting type should be pointer or reference to mentioned types list"
        );

        if constexpr (std::is_pointer_v<T>)
        {
            using type = std::remove_pointer_t<T>;
            constexpr int idx = GetTypeIndex<std::remove_const_t<type>>();
            
            static_assert(
                    idx > 0 && idx < std::tuple_size_v<types_list>
                ,   "Unable to cast polymorphic object to specified type"
            );
            
            if(TypeId() == idx)
                return const_cast<T>(ObjectPtr<idx>());

            return nullptr;
        }
        else
        {
            using type = std::remove_reference_t<T>;
            constexpr int idx = GetTypeIndex<std::remove_const_t<type>>();
            
            static_assert(
                    idx > 0 && idx < std::tuple_size_v<types_list>
                ,   "Unable to cast polymorphic object to specified type"
            );
            
            if (TypeId() == idx)
                return const_cast<T>(*ObjectPtr<idx>());

            throw std::bad_cast();
        }
    }

protected:
    using types_list = std::tuple<TDerived, TObjectsList...>;

    template<int type_idx>
    auto * ObjectPtr() noexcept
    {
        using object_t = std::tuple_element_t<type_idx, types_list>;
        return reinterpret_cast<object_t*>(m_storageBuffer.data());
    }

    template<int type_idx>
    const auto * ObjectPtr() const noexcept
    {
        using object_t = std::tuple_element_t<type_idx, types_list>;
        return reinterpret_cast<const object_t*>(m_storageBuffer.data());
    }

private:
    static constexpr size_t MaxSizeof() noexcept
    {
        size_t maxSize = 0;
        ((maxSize = std::max(maxSize, sizeof(TObjectsList))), ...);
        return maxSize;
    }

    template<class T, int type_idx = 1 /* idx to start with */>
    static constexpr int GetTypeIndex() noexcept
    {
        if constexpr (type_idx < std::tuple_size_v<types_list>)
        {
            if constexpr (std::is_same_v<T, std::tuple_element_t<type_idx, types_list>>)
                return type_idx;
            return GetTypeIndex<T, type_idx + 1>();
        }

        return -1;
    }

    template<int type_idx = 1>
    void DestroyImpl(int current_idx) noexcept
    {
        if constexpr (type_idx < std::tuple_size_v<types_list>)
        {
            if (type_idx == current_idx)
                return DestroyObject<TDerived>(ObjectPtr<type_idx>());

            return DestroyImpl<type_idx + 1>(current_idx);
        }
    }

    template<class T>
    void SetTypeId() noexcept
    {
        constexpr int type_idx = GetTypeIndex<T>();

        static_assert (
                type_idx > 0 && type_idx < std::tuple_size_v<types_list>
            ,   "Type specified is not valid for current polymorphic object"
        );

        static_assert (
                sizeof(m_storageBuffer) >= MaxSizeof() + 1
            ,   "Buffer size required bytes is not enough to carry objects specified"
        );

        m_currentTypeId = static_cast<id_type>(type_idx);
    }

private:
    static_assert(MaxTypeSizeof > 0, "Polymorphic object size should be > 0");
    static constexpr size_t BUFFER_SIZE = AlignBlock(MaxTypeSizeof + 1);
    std::array<block_type, BUFFER_SIZE> m_storageBuffer = {};
    id_type m_currentTypeId = {};
};

} // namespace Utils
} // namespace Cvs
