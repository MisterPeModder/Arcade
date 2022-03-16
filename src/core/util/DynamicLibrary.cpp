#include <dlfcn.h>
#include <stdexcept>

#include "DynamicLibrary.hpp"

namespace arcade
{
    DynamicLibrary::DynamicLibrary(std::string_view path)
    {
        this->_handle = dlopen(path.data(), RTLD_NOW | RTLD_LOCAL);
        if (this->_handle == nullptr)
            throw std::runtime_error(dlerror());
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
            throw std::runtime_error(dlerror());
        return symbol;
    }
} // namespace arcade
