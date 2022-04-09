/// @file
/// @internal

#ifndef ARCADE_CORE_UTIL_LIBRARY_SELECTOR_HPP_
#define ARCADE_CORE_UTIL_LIBRARY_SELECTOR_HPP_

#include <map>
#include <stdexcept>
#include <utility>

namespace arcade
{
    /// Allows selection among multiple library instances.
    ///
    /// @tparam Instance The instance type.
    template <typename Instance> class LibrarySelector {
      public:
        /// Helper class that creates instances of LibrarySelector.
        class Builder {
          public:
            /// Default constructor.
            Builder() {}

            /// Adds a new library instance under the given name.
            ///
            /// @param name The name of the library.
            /// @param instance The library instance.
            void add(std::string const &name, Instance *instance) { this->_libs.emplace(name, instance); }

            /// @returns The newly built instance of LibrarySelector, consumes the builder.
            LibrarySelector build() { return LibrarySelector<Instance>(std::move(this->_libs)); }

          private:
            std::map<std::string, Instance *> _libs;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Constructors
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Default constructor.
        LibrarySelector() {}

        /// Cannot copy libraries.
        LibrarySelector(LibrarySelector const &) = delete;

        /// Move constructor.
        LibrarySelector(LibrarySelector &&other) : _libs(std::move(other._libs))
        {
            // Set the selection of `this` to correpond with `other`
            if (other._selected == other.end())
                this->_selected = this->_libs.end();
            else
                this->_selected = this->_libs.find(other._selected->first);
            other._selected = other.end();
        }

        // Move assignment operator.
        LibrarySelector &operator=(LibrarySelector &&other)
        {
            this->_libs = std::move(other._libs);
            this->_selected = std::move(other._selected);

            // Set the selection of `this` to correpond with `other`
            if (other._selected == other.end())
                this->_selected = this->_libs.end();
            else
                this->_selected = this->_libs.find(other._selected->first);
            other._selected = other.end();
            return *this;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Iterators
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        using iterator = std::map<std::string, Instance *>::iterator;
        using const_iterator = std::map<std::string, Instance *>::const_iterator;

        /// @returns an iterator to the first available library.
        iterator begin() noexcept { return this->_libs.begin(); }

        /// @returns a const iterator to the first available library.
        const_iterator begin() const noexcept { return this->_libs.begin(); }

        /// @returns an iterator to the element following the last library.
        iterator end() noexcept { return this->_libs.end(); }

        /// @returns a const iterator to the element following the last library.
        const_iterator end() const noexcept { return this->_libs.end(); }

        /// @returns a const iterator to the element following the last library.
        const_iterator cend() const noexcept { return this->_libs.end(); }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Section API
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// @returns The currently selected library, may be the result of LibrarySelector::end().
        iterator getSelected() noexcept { return this->_selected; }

        /// @returns The currently selected library, may be the result of LibrarySelector::end().
        const_iterator getSelected() const noexcept { return this->_selected; }

        /// Selects the requested library.
        ///
        /// @param toSelect The library to select, may be the result of LibrarySelector::end().
        ///
        /// @returns The previous selected library.
        iterator select(iterator toSelect)
        {
            iterator previous(this->_selected);

            this->_selected = toSelect;
            return previous;
        }

      private:
        std::map<std::string, Instance *> _libs;

        iterator _selected;

        /// Internal constructor used by the builder.
        explicit LibrarySelector(std::map<std::string, Instance *> &&libs) : _libs(libs)
        {
            this->_selected = this->end();
        }
    };
} // namespace arcade

#endif // !defined(ARCADE_CORE_UTIL_LIBRARY_SELECTOR_HPP_)
