#pragma once

#include "mirr/type_factory.h"
#include "mirr/type_data.h"
#include "mirr/type_flags.h"
#include "mirr/type.h"
#include "mirr/type.inl"
#include "mirr/data.h"
#include "mirr/function.h"
#include "mirr/registry.h"
#include "mirr/initializer.h"
#include "mirr/iterators/data_container.h"
#include "mirr/iterators/function_container.h"
#include "mirr/iterators/type_container.h"
#include "mirr/iterators/argument_container.h"

namespace mirr
{

    /**
     * @brief Registers a type to the reflection system.
     * @tparam - T Type to register.
     * @param - name Identifier for the type.
     * @return Type factory object, used as named parameter idiom, to declare information about the type.
     */
    template<typename T>
    inline type_factory<T> register_type(const std::string &name)
    {
        return registry::register_type<T>(name);
    }

    /**
     * @brief Registers a template specialization to the reflection system.
     * @tparam -  T Template specialization to register.
     * @param - name Identifier for the type.
     * @return Type factory object to, used to declare information about the type.
     */
    template<template<typename> typename T>
    inline auto register_type(const std::string &name)
    {
        return registry::register_type<T>(name);
    }

    /**
     * @brief Retrieves type information for a given identifier.
     * @param - name The type's existing identifier.
     * @return Object containing information about the type.
     */
    inline type resolve(const std::string &name) noexcept
    {
        return registry::resolve(name);
    }

    /**
     * @brief Retrieves type information for a given id.
     * @param - name The type's id.
     * @return Object containing information about the type.
     */
    inline type resolve(uint32_t id) noexcept
    {
        return registry::resolve(id);
    }

    /**
     * @brief Retrieves a list of all the types registered in the reflection system.
     * @return Iterable container with all the registered types.
     */
    inline type_container resolve() noexcept
    {
        return registry::resolve();
    }

    /**
     * @brief Retrieves type information for the given type.
     * @tparam - T Type to resolve.
     * @return Object containing information about the type.
     */
    template<typename T>
    inline type resolve() noexcept
    {
        return registry::resolve<T>();
    }

    /**
     * @brief Checks whether a given identifier has been registered for a type.
     * @param - name Identifier to validate.
     * @return Whether the identifier is valid (there exists a type registered with the given name).
     */
    inline bool valid(const std::string &name)
    {
        return registry::valid(name);
    }

    /**
     * @brief Checks whether a type with the given ID exists in the registry.
     * @param - id ID to validate.
     * @return Whether the ID is valid (there exists a type registered with the given ID).
     */
    inline bool valid(uint32_t id)
    {
        return registry::valid(id);
    }

    /**
     * @brief Gets the name of a type from the given id.
     * @param - id Assigned id to for the type.
     * @return Assigned name to the type.
     */
    inline std::string type_name_from_id(uint32_t id)
    {
        return registry::resolve(id).name();
    }

    /**
 * @brief Gets the name of a type from the given id.
 * @param - id Assigned id to for the type.
 * @return Assigned name to the type.
 */
    inline uint32_t type_id_from_name(const std::string &name)
    {
        return registry::resolve(name).id();
    }

#ifdef MIRR_USE_EXPLICIT_MACROS
#ifndef MIRR_REFLECT
#define MIRR_REFLECT(_TYPE_NAME_) \
        static mirr::type_factory<_TYPE_NAME_> register_type(); \
        static inline mirr::type_initializer<_TYPE_NAME_> _reflection_type_initializer{};
#endif
#else
#ifndef REFLECT
#define REFLECT(_TYPE_NAME_) \
        using type_factory = mirr::type_factory<_TYPE_NAME_>;  \
        static inline mirr::type_initializer<_TYPE_NAME_> _reflection_type_initializer{};
#endif
#endif


}

