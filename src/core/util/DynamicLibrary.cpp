#include <dlfcn.h>
#include <filesystem>
#include <stdexcept>
#include <utility>

#include "DynamicLibrary.hpp"

namespace arcade
{
    DynamicLibrary::LoadError::LoadError(std::string const &msg) : std::runtime_error(msg)
    {
    }

    DynamicLibrary::UnknownSymbolError::UnknownSymbolError(std::string const &msg) : std::runtime_error(msg)
    {
    }

    DynamicLibrary::DynamicLibrary(std::filesystem::path const &path) : _path(path)
    {
        this->_handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (this->_handle == nullptr)
            throw LoadError(dlerror());
    }

    DynamicLibrary::DynamicLibrary(DynamicLibrary &&other) : _handle(other._handle)
    {
        other._handle = nullptr;
    }

    void DynamicLibrary::operator&&(DynamicLibrary &&other)
    {
        if (this->_handle)
            dlclose(this->_handle);
        this->_handle = other._handle;
        other._handle = nullptr;
    }

    DynamicLibrary::~DynamicLibrary()
    {
        if (this->_handle)
            dlclose(this->_handle);
        this->_handle = nullptr;
    }

    void *DynamicLibrary::rawSymbolUnchecked(std::string_view name) noexcept
    {
        return dlsym(this->_handle, name.data());
    }

    void *DynamicLibrary::rawSymbol(std::string_view name)
    {
        void *symbol(this->rawSymbolUnchecked(name));

        if (!symbol)
            throw UnknownSymbolError(dlerror());
        return symbol;
    }

    std::filesystem::path const &DynamicLibrary::path()
    {
        return this->_path;
    }

    bool DynamicLibrary::hasSymbol(std::string_view name)
    {
        return dlsym(this->_handle, name.data()) != nullptr;
    }

    DynamicLibrary &DynamicLibrary::load(std::filesystem::path const &path, Registry &libs)
    {
        std::filesystem::path cpath(std::filesystem::weakly_canonical(path));
        Registry::iterator entry(libs.find(cpath.string()));

        if (entry != libs.end())
            return entry->second;
        auto [lib, _] = libs.emplace(cpath.string(), DynamicLibrary(cpath));
        return lib->second;
    }

    bool DynamicLibrary::loadDirectory(std::filesystem::path const &dir, Registry &libs)
    {
        bool success(false);

        for (auto const &entry : std::filesystem::directory_iterator(dir)) {
            std::filesystem::path cpath(std::filesystem::weakly_canonical(entry.path()));

            if (libs.contains(cpath))
                continue;
            try {
                libs.emplace(cpath, DynamicLibrary(cpath));
                success = true;
            } catch (LoadError &) {
                // if lib counldn't be loaded, skip
            }
        }
        return success;
    }
} // namespace arcade
