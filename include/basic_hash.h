#pragma once

#include <type_traits>
#include <string>

namespace reflection
{

    class type;

    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    class basic_hash
    {

    public:
        static constexpr basic_hash hash(const char *const str) noexcept
        {
            return basic_hash(hash_internal(str, val_const));
        }

        static constexpr basic_hash hash(const std::string &str) noexcept
        {
            return basic_hash(hash_internal(str.c_str(), val_const));
        }

        explicit constexpr basic_hash(const T hash) noexcept:
                m_Hash(hash)
        {
        }

        explicit constexpr basic_hash(const char *const pText) noexcept:
                m_Hash(hash_internal(pText, val_const))
        {
        }

        explicit basic_hash(const std::string &text) :
                m_Hash(hash_internal(text.c_str(), val_const))
        {
        }

        constexpr operator T() const
        { return m_Hash; }

        bool operator==(const basic_hash &rhs) const
        { return m_Hash == rhs.m_Hash; }

        bool operator!=(const basic_hash &rhs) const
        { return m_Hash != rhs.m_Hash; }

        bool operator<(const basic_hash &rhs) const
        { return m_Hash < rhs.m_Hash; }

        bool operator>(const basic_hash &rhs) const
        { return m_Hash > rhs.m_Hash; }

    private:
        static constexpr T val_const{static_cast<T>(0xcbf29ce484222325)};
        static constexpr T prime_const{static_cast<T>(0x100000001b3)};

        static constexpr T hash_internal(const char *const str, const T value) noexcept // NOLINT(misc-no-recursion)
        {
            return (str[0] == '\0') ? value : hash_internal(&str[1], (value ^ T(str[0])) * prime_const);
        }

        T m_Hash;
    };

    using string_hash = basic_hash<size_t>;

    static constexpr size_t operator "" _hs(const char *str, size_t) noexcept
    {
        return string_hash::hash(str);
    }
}

namespace std
{
    template<>
    struct hash<reflection::string_hash>
    {
        std::size_t operator()(const reflection::string_hash &hash) const
        {
            return hash;
        }
    };
}



