#include <string_view>

/// @file Dynamic Library (.so / .dll) wrapper.
/// @internal

namespace arcade
{
    class DynamicLibrary {
      public:
        /// Loads the dynamic library at the given path
        ///
        /// @param path Where the library is located, must be a null-terminated string.
        ///
        /// @throws std::runtime_exception if not found.
        explicit DynamicLibrary(std::string_view path);

        DynamicLibrary(DynamicLibrary const &) = delete;

        DynamicLibrary(DynamicLibrary &&);
        void operator&&(DynamicLibrary &&);

        ~DynamicLibrary();

        /// @param name the name of the symbol to fetch, must be null-terminated.
        ///
        /// @returns the address where the symbol is loaded into memory, or @c nullptr if not found.
        void *rawSymbolUnchecked(std::string_view name) noexcept;

        /// @param name the name of the symbol to fetch, must be null-terminated.
        ///
        /// @returns the address where the symbol is loaded into memory, or @c nullptr if not found.
        template <typename T> T symbolUnchecked(std::string_view name) noexcept
        {
            return reinterpret_cast<T>(this->rawSymbolUnchecked(name));
        }

        /// A safe, throwing version of DyanmicLibrary::rawSymbolUnchecked().
        ///
        /// @param name the name of the symbol to fetch, must be null-terminated.
        ///
        /// @returns the address where the symbol is loaded into memory.
        void *rawSymbol(std::string_view name);

        /// A safe, throwing version of DyanmicLibrary::symbolUnchecked().
        ///
        /// @param name the name of the symbol to fetch, must be null-terminated.
        ///
        /// @returns the address where the symbol is loaded into memory.
        template <typename T> T symbol(std::string_view name)
        {
            return reinterpret_cast<T>(this->rawSymbol(name));
        }

      private:
        void *_handle;
    };
} // namespace arcade
