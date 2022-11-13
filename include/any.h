#pragma once


#include <utility>
#include <iostream>
#include "type.h"
#include "type_hash.h"
#include "type_traits.h"

namespace reflection
{


    namespace internal
    {
        enum class any_operation : uint8_t
        {
            copy,
            move,
            transfer,
            assign,
            destroy,
            compare,
            get
        };

        enum class any_policy : uint8_t
        {
            owner,
            ref,
            cref
        };
    }


    class any;
    class handle;


    class any
    {
        friend class handle;


        static constexpr std::size_t Len = sizeof(double[2]);
        static constexpr std::size_t Align = alignof(std::aligned_storage_t<Len + !Len>);

        using storage_type = std::aligned_storage_t<Len + !Len, Align>;
        using vtable_type = const void *(const internal::any_operation, const any &, const void *);

        template<typename Type>
        static constexpr bool in_situ = Len && alignof(Type) <= alignof(storage_type) &&
                                        sizeof(Type) <= sizeof(storage_type) &&
                                        std::is_nothrow_move_constructible_v<Type>;

    public:

        any() noexcept // NOLINT(cppcoreguidelines-pro-type-member-init)
                : instance{},
                  type_info{type_hash<void>::value()},
                  policy{internal::any_policy::owner},
                  vtable{}
        {}

        template<typename Type, typename = std::enable_if_t<!std::is_same_v<std::decay_t<Type>, any>>>
        any(Type &&value) // NOLINT(google-explicit-constructor)
                :any{}
        {
            initialize<std::decay_t<Type>>(std::forward<Type>(value));
        }

        template<typename Type, typename... Args>
        explicit any(std::in_place_type_t<Type>, Args &&... args)
                :any{}
        {
            initialize<Type>(std::forward<Args>(args)...);
        }

        any(const any &other)
                : any{}
        {
            if (other.vtable)
            {
                other.vtable(internal::any_operation::copy, other, this);
            }
        }

        any(const any &&other) noexcept // NOLINT(cppcoreguidelines-pro-type-member-init)
                : instance{},
                  type_info{other.type_info},
                  vtable{other.vtable},
                  policy{other.policy}
        {
            if (other.vtable)
            {
                other.vtable(internal::any_operation::move, other, this);
            }
        }

        ~any()
        {
            if (vtable && owner())
            {
                vtable(internal::any_operation::destroy, *this, nullptr);
            }
        }

        any &operator=(const any &other) noexcept // NOLINT(bugprone-unhandled-self-assignment)
        {
            reset();
            if (other.vtable)
            {
                other.vtable(internal::any_operation::copy, other, this);
            }
            return *this;
        }

        any &operator=(any &&other) noexcept // NOLINT(bugprone-unhandled-self-assignment)
        {
            reset();
            if (other.vtable)
            {
                other.vtable(internal::any_operation::move, other, this);
                type_info = other.type_info;
                vtable = other.vtable;
                policy = other.policy;
            }
            return *this;
        }

        template<typename Type>
        std::enable_if_t<!std::is_same_v<std::decay_t<Type>, any>, any&> // NOLINT(misc-unconventional-assign-operator)
        operator=(Type &&value)
        {
            emplace<std::decay_t<Type>>(std::forward<Type>(value));
            return *this;
        }

        [[nodiscard]] const void *data() const noexcept
        {
            return vtable ? vtable(internal::any_operation::get, *this, nullptr) : nullptr;
        }

        [[nodiscard]] const void *data(type_id info) const noexcept
        {
            return type_info._id == info ? data() : nullptr;
        }

        [[nodiscard]] void *data() noexcept
        {
            return (!vtable || policy == internal::any_policy::cref) ? nullptr
            : const_cast<void *>(vtable(internal::any_operation::get, *this, nullptr));
        }

        [[nodiscard]] void *data(type_id info) noexcept
        {
            return type_info._id == info ? data() : nullptr;
        }


        [[nodiscard]] const reflection::type &type() const noexcept
        {
            return type_info;
        }

        [[nodiscard]] bool owner() const noexcept
        {
            return policy == internal::any_policy::owner;
        }

        void reset()
        {
            if (vtable && owner())
            {
                vtable(internal::any_operation::destroy, *this, nullptr);
            }

            type_info._id = type_hash_v<void>;
            vtable = nullptr;
            policy = internal::any_policy::owner;
        }

        template<typename Type, typename... Args>
        void emplace(Args &&...args)
        {
            reset();
            initialize<Type>(std::forward<Args>(args)...);
        }

        bool assign(const any &other)
        {
            if (vtable && policy != internal::any_policy::cref && type_info == other.type_info) {
                return (vtable(internal::any_operation::assign, *this, other.data()) != nullptr);
            }
            return false;
        }

        [[nodiscard]] explicit operator bool() const noexcept {
            return vtable != nullptr;
        }

        bool operator==(const any &other) const noexcept
        {
            if (vtable && type_info == other.type_info) {
                return (vtable(internal::any_operation::compare, *this, other.data()) != nullptr);
            }
            return (!vtable && !other.vtable);
        }


        template <typename Type>
        Type* try_cast() noexcept
        {
            auto id = type_hash<std::remove_cv_t<std::remove_reference_t<Type>>>::value();
            if (id == type_info._id)
            {
                return static_cast<Type *>(this->data(id));
            }
            return nullptr;
        }

        template <typename Type>
        Type* try_cast_or_convert() noexcept
        {
            auto id = type_hash<std::remove_cv_t<std::remove_reference_t<Type>>>::value();
            if (id == type_info._id)
            {
                return static_cast<Type *>(this->data(id));
            }
            else if (type_info.is_convertible<Type>())
            {
                any result = try_conversion(id);
                reset();
                if (result.vtable)
                {
                    result.vtable(internal::any_operation::move, result, this);
                    type_info = result.type_info;
                    vtable = result.vtable;
                    policy = result.policy;
                }
                return try_cast<Type>();
            }
            return nullptr;
        }

        template <typename Type>
        any convert() noexcept
        {
            auto id = type_hash<std::remove_cv_t<std::remove_reference_t<Type>>>::value();
            if (type_info.is_convertible<Type>())
            {
                return try_conversion(id);
            }
            return any{};
        }

        template <typename Type, typename Ret = std::remove_cv_t<std::remove_reference_t<Type>>>
        Ret cast() noexcept
        {
            Ret* ptr = this->try_cast<Ret>();
            return *ptr;
        }

        template <typename Type, typename Ret = std::remove_cv_t<std::remove_reference_t<Type>>>
        Ret cast_or_convert() noexcept
        {
            if (can_convert<Ret>())
            {
                any val = convert<Ret>();
                return *val.try_cast<Ret>();
            }
            return *try_cast<Ret>();
        }

        template <typename Type>
        bool can_cast() noexcept
        {
            return try_cast<std::remove_cv_t<std::remove_reference_t<Type>>>() != nullptr;
        }

        template <typename Type>
        bool can_convert() noexcept
        {
            return type_info.is_convertible<Type>();
        }

        template <typename Type>
        bool can_cast_or_convert()
        {
            return can_cast<Type>() || can_convert<Type>();
        }

    private:


        template<typename Type, typename... Args>
        void initialize([[maybe_unused]] Args &&... args)
        {
            if constexpr (!std::is_void_v<Type>)
            {
                type_info._id = type_hash_v<Type>;
                vtable = basic_vtable<std::remove_cv_t<std::remove_reference_t<Type>>>;

                if constexpr (std::is_lvalue_reference_v<Type>)
                {
                    instance = (std::addressof(args), ...);
                    policy = std::is_const_v<std::remove_reference_t<Type>> ?
                             internal::any_policy::cref : internal::any_policy::ref;
                } else if constexpr (in_situ<Type>)
                {
                    if constexpr (sizeof...(Args) != 0u && std::is_aggregate_v<Type>)
                    {
                        new(&storage) Type{std::forward<Args>(args)...};
                    } else
                    {
                        new(&storage) Type(std::forward<Args>(args)...);
                    }
                } else
                {
                    if constexpr (sizeof...(Args) != 0u && std::is_aggregate_v<Type>)
                    {
                        instance = new Type{std::forward<Args>(args)...};
                    }
                    else if constexpr (!std::is_abstract<Type>())
                    {
                        instance = new Type(std::forward<Args>(args)...);
                    }
                }
            }
        }



        template<typename Type>
        static const void *basic_vtable(const internal::any_operation op, const any &value, const void *other)
        {
            static_assert(
                    !std::is_void_v<Type> && std::is_same_v<std::remove_cv_t<std::remove_reference_t<Type>>, Type>,
                    "Invalid type!");
            const Type *val;

            if constexpr (in_situ<Type>)
            {
                val = value.owner() ? reinterpret_cast<const Type *>(&value.storage)
                                    : static_cast<const Type *>(value.instance);
            } else
            {
                val = static_cast<const Type *>(value.instance);
            }

            switch (op)
            {
                case internal::any_operation::copy:
                    if constexpr (std::is_copy_constructible_v<Type>)
                    {
                        static_cast<any *>(const_cast<void *>(other))->initialize<Type>(*val);
                    }
                    break;
                case internal::any_operation::move:
                    if constexpr (in_situ<Type>)
                    {
                        if (value.owner())
                        {
                            return new(&static_cast<any *>(const_cast<void *>(other))->storage)
                                    Type{std::move(*const_cast<Type *>(val))};
                        }
                    }
                    return (static_cast<any *>(const_cast<void *>(other))->instance =
                                    std::exchange(const_cast<any &>(value).instance, nullptr));
                case internal::any_operation::transfer:
                    if constexpr (std::is_move_assignable_v<Type>)
                    {
                        *const_cast<Type *>(val) = std::move(*static_cast<Type *>(const_cast<void *>(other)));
                        return other;
                    }
                    [[fallthrough]];
                case internal::any_operation::assign:
                    if constexpr (std::is_copy_assignable_v<Type>)
                    {
                        *const_cast<Type *>(val) = *static_cast<const Type *>(other);
                        return other;
                    }
                    break;
                case internal::any_operation::destroy:
                    if constexpr (in_situ<Type>)
                    {
                        val->~Type();
                    } else if constexpr (std::is_array_v<Type>)
                    {
                        delete[] val;
                    } else
                    {
                        delete val;
                    }
                    break;
                case internal::any_operation::compare:
                    if constexpr (!std::is_function_v<Type> && !std::is_array_v<Type> &&
                                  internal::is_equality_comparable_v<Type>)
                    {
                        return *val == *static_cast<const Type * >(other) ? other : nullptr;
                    } else
                    {
                        return (val == other) ? other : nullptr;
                    }
                case internal::any_operation::get:
                    return val;
            }

            return nullptr;
        }

        any try_conversion(uint32_t type_id);

    private:
        union
        {
            const void *instance{};
            storage_type storage;
        };
        reflection::type type_info{};
        internal::any_policy policy = internal::any_policy::owner;
        vtable_type *vtable;
    };

    class handle : public reflection::any
    {
    public:
        template<typename Type, typename = std::enable_if_t<!std::is_same_v<std::decay_t<Type>, any>>>
        handle(Type& value) // NOLINT(google-explicit-constructor)
        {
            initialize<Type&>(std::forward<Type&>(value));
        }
    };

}