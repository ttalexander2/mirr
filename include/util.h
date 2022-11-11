#pragma once

#include <type_traits>
#include "any.h"

namespace reflection::internal
{

    // Implementation of std::invoke (from cppreference)
    namespace detail {
        template<class>
        constexpr bool is_reference_wrapper_v = false;
        template<class U>
        constexpr bool is_reference_wrapper_v<std::reference_wrapper<U>> = true;

        template<class C, class Pointed, class T1, class... Args>
        constexpr decltype(auto) invoke_memptr(Pointed C::* f, T1&& t1, Args&&... args)
        {
            if constexpr (std::is_function_v<Pointed>) {
                if constexpr (std::is_base_of_v<C, std::decay_t<T1>>)
                    return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
                else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
                    return (t1.get().*f)(std::forward<Args>(args)...);
                else
                    return ((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
            } else {
                static_assert(std::is_object_v<Pointed> && sizeof...(args) == 0);
                if constexpr (std::is_base_of_v<C, std::decay_t<T1>>)
                    return std::forward<T1>(t1).*f;
                else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
                    return t1.get().*f;
                else
                    return (*std::forward<T1>(t1)).*f;
            }
        }
    }

    template<class F, class... Args>
    constexpr std::invoke_result_t<F, Args...> invoke(F&& f, Args&&... args)
    noexcept(std::is_nothrow_invocable_v<F, Args...>)
    {
        if constexpr (std::is_member_pointer_v<std::decay_t<F>>)
            return detail::invoke_memptr(f, std::forward<Args>(args)...);
        else
            return std::forward<F>(f)(std::forward<Args>(args)...);
    }

    template<typename... Type>
    struct type_list
    {
        using type = type_list;
        static constexpr auto size = sizeof...(Type);
    };

    template<typename, typename>
    struct function_descriptor;

    template<typename Type, typename Ret, typename Class, typename... Args>
    struct function_descriptor<Type, Ret (Class::*)(Args...) const>
    {
        using return_type = Ret;
        using args_type = std::conditional_t<std::is_base_of_v<Class, Type>,
                type_list<Args...>, type_list<const Class &, Args...>>;

        static constexpr auto is_const = true;
        static constexpr auto is_static = !std::is_base_of_v<Class, Type>;
    };

    template<typename Type, typename Ret, typename Class, typename... Args>
    struct function_descriptor<Type, Ret (Class::*)(Args...)>
    {
        using return_type = Ret;
        using args_type = std::conditional_t<std::is_base_of_v<Class, Type>,
                type_list<Args...>, type_list<Class &, Args...>>;

        static constexpr auto is_const = false;
        static constexpr auto is_static = !std::is_base_of_v<Class, Type>;
    };

    template<typename Type, typename Ret, typename Class>
    struct function_descriptor<Type, Ret Class::*>
    {
        using return_type = Ret &;
        using args_type = std::conditional_t<std::is_base_of_v<Class, Type>, type_list<>, type_list<Class &>>;

        static constexpr auto is_const = false;
        static constexpr auto is_static = !std::is_base_of_v<Class, Type>;
    };

    template<typename Type, typename Ret, typename MaybeType, typename... Args>
    struct function_descriptor<Type, Ret (*)(MaybeType, Args...)>
    {
        using return_type = Ret;
        using args_type = std::conditional_t<std::is_base_of_v<std::remove_cv_t<std::remove_reference_t<MaybeType>>,
            Type>, type_list<Args...>, type_list<MaybeType, Args...>>;

        static constexpr auto is_const = std::is_base_of_v<std::remove_cv_t<std::remove_reference_t<MaybeType>>, Type>
            && std::is_const_v<std::remove_reference_t<MaybeType>>;
        static constexpr auto is_static = !std::is_base_of_v<std::remove_cv_t<std::remove_reference_t<MaybeType>>, Type>;
    };

    template<typename Type, typename Ret>
    struct function_descriptor<Type, Ret (*)()>
    {
        using return_type = Ret;
        using args_type = type_list<>;
        static constexpr auto is_const = false;
        static constexpr auto is_static = true;
    };

    template<typename Type, typename Candidate>
    class function_helper
    {
        template<typename Ret, typename... Args, typename Class>
        static constexpr function_descriptor<Type, Ret (Class::*)(Args...) const> get_rid_of_noexcept(Ret (Class::*)(Args...) const){}

        template<typename Ret, typename... Args, typename Class>
        static constexpr function_descriptor<Type, Ret (Class::*)(Args...)>
        get_rid_of_noexcept(Ret (Class::*)(Args...)){}

        template<typename Ret, typename Class>
        static constexpr function_descriptor<Type, Ret Class::*>
        get_rid_of_noexcept(Ret Class::*){}

        template<typename Ret, typename... Args>
        static constexpr function_descriptor<Type, Ret (*)(Args...)>
        get_rid_of_noexcept(Ret (*)(Args...)){}

        template<typename Class>
        static constexpr function_descriptor<Class, decltype(&Class::operator())>
        get_rid_of_noexcept(Class){}

    public:
        using type = decltype(get_rid_of_noexcept(std::declval<Candidate>()));
    };

    template<typename Type, typename Candidate>
    using function_helper_t = typename function_helper<Type, Candidate>::type;

    template <std::size_t, typename>
    struct type_list_element;

    template <std::size_t Index, typename First, typename... Other>
    struct type_list_element<Index, type_list<First, Other...>>
            : type_list_element<Index - 1u, type_list<Other...>> {};

    template <typename First, typename... Other>
    struct type_list_element<0u, type_list<First, Other...>>
    {
        using type = First;
    };

    template<std::size_t Index, typename List>
    using type_list_element_t = typename type_list_element<Index, List>::type;

    template<typename Type, auto Data>
    [[nodiscard]] bool set_function(reflection::handle handle, any value)
    {
        if (handle.type().id() != type_hash_v<Type>)
            return false;

        if constexpr(!std::is_same_v<decltype(Data), Type> && !std::is_same_v<decltype(Data), std::nullptr_t>)
        {
            if constexpr(std::is_member_function_pointer_v<decltype(Data)>
            || std::is_function_v<std::remove_reference_t<std::remove_pointer_t<decltype(Data)>>>)
            {
                using descriptor = function_helper_t<Type, decltype(Data)>;
                using data_type = type_list_element_t<descriptor::is_static, typename descriptor::args_type>;

                if (auto* const clazz = handle.try_cast<Type>(); clazz)
                {
                    if (auto* val = value.try_cast<data_type>(); val)
                    {
                        invoke(Data, *clazz, *val);
                        return true;
                    }
                }
            }
            else if constexpr (std::is_member_object_pointer_v<decltype(Data)>)
            {
                using data_type = std::remove_reference_t<typename function_helper_t<Type, decltype(Data)>::return_type>;
                if constexpr(!std::is_array_v<data_type> && !std::is_const_v<data_type>)
                {
                    if (auto *const clazz = handle.try_cast<Type>(); clazz)
                    {
                        if (auto *val = value.try_cast<data_type>(); val)
                        {
                            invoke(Data, *clazz) = *val;
                            return true;
                        }
                    }
                }

            }
            else
            {
                using data_type = std::remove_reference_t<decltype(*Data)>;

                if constexpr (!std::is_array_v<data_type> && !std::is_const_v<data_type>)
                {
                    if (auto* val = value.try_cast<data_type>(); val)
                    {
                        *Data = *val;
                        return true;
                    }
                }
            }
        }
        return true;
    }

    template<typename Type, auto Data>
    [[nodiscard]] any get_function(reflection::handle handle)
    {
        return any{};
    }
}

