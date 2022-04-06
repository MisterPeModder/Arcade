/// @file Dynamic Library (.so / .dll) wrapper.
/// @internal

#ifndef ARCADE_CORE_UTIL_DYNAMIC_LIBRARY_HPP_
#define ARCADE_CORE_UTIL_DYNAMIC_LIBRARY_HPP_

#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

namespace arcade
{
    /// Abstraction over raw libdl functions.
    class DynamicLibrary {
      public:
        using Registry = std::unordered_map<std::string, DynamicLibrary>;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Errors
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        class LoadError : public std::runtime_error {
          private:
            LoadError(std::string const &msg);

            friend DynamicLibrary;
        };

        class UnknownSymbolError : public std::runtime_error {
          private:
            UnknownSymbolError(std::string const &msg);

            friend DynamicLibrary;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Instantiation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        DynamicLibrary(DynamicLibrary const &) = delete;

        DynamicLibrary(DynamicLibrary &&);
        DynamicLibrary &operator=(DynamicLibrary &&);

        ~DynamicLibrary();

        /// Attempts to load a dynamic library.
        ///
        /// @param path The path of the .so/.dll file.
        /// @param[in, out] libs The currenly loaded libraries.
        ///
        /// @throws DynamicLibrary::LoadError if the library has failed to load.
        static DynamicLibrary &load(std::filesystem::path const &path, Registry &libs);

        /// Attempts to load all dynamic libraries in the given directory.
        ///
        /// Skips libraries that fail to load or are already present in @c libs.
        ///
        /// @param dir Where to load the libraries from.
        /// @param[in, out] libs The currenly loaded libraries.
        ///
        /// @return Whether at least one library has successfully loaded.
        static bool loadDirectory(std::filesystem::path const &dir, Registry &libs);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Symbol Query API
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
        ///
        /// @throws DynamicLibrary::UnknownSymbolError if not found.
        void *rawSymbol(std::string_view name);

        /// A safe, throwing version of DyanmicLibrary::symbolUnchecked().
        ///
        /// @param name the name of the symbol to fetch, must be null-terminated.
        ///
        /// @returns the address where the symbol is loaded into memory.
        ///
        /// @throws DynamicLibrary::UnknownSymbolError if not found.
        template <typename T> T symbol(std::string_view name) { return reinterpret_cast<T>(this->rawSymbol(name)); }

        /// @param name the name of the symbol to query, must be null-terminated.
        ///
        /// @returns Whether the library posseses the requested symbol.
        bool hasSymbol(std::string_view name);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Miscellaneous
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// @returns The path of this dynamic library.
        std::filesystem::path const &path();

      private:
        std::filesystem::path _path;
        void *_handle;

        /// Loads the dynamic library at the given path
        ///
        /// @param path Where the library is located.
        ///
        /// @throws DynamicLibrary::LoadError if the library has failed to load.
        explicit DynamicLibrary(std::filesystem::path const &path);
    };
} // namespace arcade

#endif // !defined(ARCADE_CORE_UTIL_DYNAMIC_LIBRARY_HPP_)
