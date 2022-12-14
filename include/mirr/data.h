#pragma once

#include <cstdint>
#include "type.h"
#include "any.h"

namespace mirr
{

    // Forward declarations
    class type_data;
    class registry;
    class any;
    class handle;
    struct data_info;
    struct type_info;
    template<typename T>
    class type_factory;

    /**
     * @brief Class representing a field or piece of data within a type.
     * This class is a wrapper for data registered in the reflection system.
     */
    class data
    {
        // Friend classes
        friend class type_data;
        friend class registry;
        friend class any;
        friend class handle;
        friend class type;
        friend class data_container;
        template<typename T>
        friend class type_factory;

    public:
        /**
         * @brief Creates an empty data object.
         */
        data();

        /**
         * @brief Default copy constructor.
         */
        data(const data &) = default;

        /**
         * @brief Equality comparison operator.
         * @param rhs - Other data object to compare to.
         * @return Returns true if both data have the same ID and they are contained in the same type.
         */
        bool operator==(const data &rhs) const;

        /**
         * @brief Checks whether this data object is valid (i.e. has a reference to a valid type, and the reference
         * to the information about this data exists).
         * @return Returns true if the data object is valid, false otherwise.
         */
        [[nodiscard]] bool valid() const;

        /**
         * @brief Boolean operator.
         * @return Returns true if the data object is valid.
         */
        explicit operator bool() const
        { return valid(); }

        /**
         * @brief Gets the ID of this data.
         * @return 32 bit number representing this data. (This is not the ID of the type)
         */
        [[nodiscard]] uint32_t id() const;

        /**
         * @brief Gets the name of the data.
         * @return String containing the registered name of the data.
         */
        [[nodiscard]] std::string name() const;

        /**
         * @brief The underlying type containing this data.
         * @return
         */
        [[nodiscard]] mirr::type type() const;

        /**
         * @brief The type of the data.
         * @return A type object containing the type of this data.
         */
         [[nodiscard]] mirr::type data_type() const;

        /**
         * @brief Flag representing whether this data should be serialized.
         * @return True if the data should be serialized, false otherwise.
         */
        [[nodiscard]] bool is_serialized() const;

        /**
         * @brief Flag representing whether this data is const.
         * @return True if the data is const, false otherwise.
         */
        [[nodiscard]] bool is_const() const;

        /**
         * @brief Flag representing whether this data is static.
         * @return True if the data is static, false otherwise.
         */
        [[nodiscard]] bool is_static() const;

        /**
         * @brief Gets the data from a given object.
         * @param handle - Object to retrieve the data.
         * @return Returns an any object containing the data.
         */
        [[nodiscard]] any get(mirr::handle handle) const;

        /**
         * @brief Sets the data on a given object.
         * @param handle - Object to set the data.
         * @param value - Value to set the data to.
         * @return Returns true if the set operation was successful, false otherwise.
         */
        [[nodiscard]] bool set(mirr::handle handle, const any &value) const;

    private:
        /**
         * @brief Constructs a data object given an ID for the data, and the type of the data.
         * @param id
         * @param type_id
         */
        explicit data(uint32_t id, uint32_t type_id);

        uint32_t _id; // ID of the data.
        uint32_t _type_id; // ID of the type containing the data.
    };


}