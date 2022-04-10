/// @file
/// @internal

#ifndef ARCADE_CORE_UTIL_LIBRARY_SELECTOR_HPP_
#define ARCADE_CORE_UTIL_LIBRARY_SELECTOR_HPP_

#include <algorithm>
#include <compare>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace arcade
{
    /// The value type of the iterators inside LibrarySelector.
    ///
    /// This class is 'tuple-like', meaning it is compatible with std::get, std::tuple_size, std::tuple_element.
    template <typename Instance> class LibraryEntry {
      public:
        LibraryEntry() = default;
        LibraryEntry(std::string_view name, Instance *instance) : _name(name), _instance(instance) {}

        LibraryEntry(LibraryEntry const &) = default;
        LibraryEntry(LibraryEntry &&) = default;

        LibraryEntry &operator=(LibraryEntry const &) = default;
        LibraryEntry &operator=(LibraryEntry &&) = default;

        constexpr std::string const &name() const { return this->_name; }

        constexpr Instance *instance() const { return this->_instance; }

        bool operator==(LibraryEntry const &other) const = default;
        auto operator<=>(LibraryEntry const &other) const = default;

      private:
        std::string _name;
        Instance *_instance;
    };

    static_assert(std::three_way_comparable<LibraryEntry<void>>);
    static_assert(std::totally_ordered<LibraryEntry<void>>);
    static_assert(std::regular<LibraryEntry<void>>);

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
            void add(std::string_view name, Instance *instance)
            {
                this->_libs.push_back(LibraryEntry<Instance>(name, instance));
            }

            /// @returns The newly built instance of LibrarySelector, consumes the builder.
            LibrarySelector build() { return LibrarySelector<Instance>(std::move(this->_libs)); }

          private:
            std::vector<LibraryEntry<Instance>> _libs;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Constructors
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Default constructor.
        LibrarySelector() {}

        /// Cannot copy libraries.
        LibrarySelector(LibrarySelector const &) = delete;

        /// Move constructor.
        LibrarySelector(LibrarySelector &&other)
        {
            // Set the selection of `this` to correpond with `other`
            if (other._selected == other.end()) {
                this->_libs = std::move(other._libs);
                this->_selected = this->_libs.end();
            } else {
                this->_libs = std::move(other._libs);
                this->_selected = this->_libs.begin() + (other.end() - other._selected);
            }
            other._selected = other.end();
        }

        // Move assignment operator.
        LibrarySelector &operator=(LibrarySelector &&other)
        {
            // Set the selection of `this` to correpond with `other`
            if (other._selected == other.end()) {
                this->_libs = std::move(other._libs);
                this->_selected = this->_libs.end();
            } else {
                this->_libs = std::move(other._libs);
                this->_selected = this->_libs.begin() + (other.end() - other._selected);
            }
            other._selected = other.end();
            return *this;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Iterators
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        using iterator = typename std::vector<LibraryEntry<Instance>>::iterator;
        using const_iterator = typename std::vector<LibraryEntry<Instance>>::const_iterator;

        /// @returns an iterator to the first available library.
        iterator begin() noexcept { return this->_libs.begin(); }

        /// @returns a const iterator to the first available library.
        const_iterator begin() const noexcept { return this->_libs.begin(); }

        /// @returns a const iterator to the first available library.
        const_iterator cbegin() const noexcept { return this->_libs.begin(); }

        /// @returns an iterator to the element following the last library.
        iterator end() noexcept { return this->_libs.end(); }

        /// @returns a const iterator to the element following the last library.
        const_iterator end() const noexcept { return this->_libs.end(); }

        /// @returns a const iterator to the element following the last library.
        const_iterator cend() const noexcept { return this->_libs.end(); }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Selection API
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

        /// Selects the library at the requested index..
        ///
        /// @param index The index of the library to select, the selection is cleared if @c index is out-of-bounds.
        ///
        /// @returns The previous selected library.
        iterator select(size_t index)
        {
            if (index >= this->_libs.size())
                return this->end();
            return this->select(this->begin() + index);
        }

        /// Selects the next library in a cycle.
        ///
        /// @returns The previous selected library.
        iterator selectNext()
        {
            iterator end(this->end());

            if (this->_selected == end)
                return this->select(this->begin());
            iterator next(this->_selected + 1);

            if (next == end)
                return this->select(this->begin());
            return this->select(next);
        }

        /// Selects the previous library in a cycle.
        ///
        /// @returns The previous selected library.
        iterator selectPrevious()
        {
            iterator begin(this->begin());
            iterator last(begin + std::max((size_t)1, this->_libs.size()) - 1);
            iterator end = this->end();

            if (this->_selected == begin || this->_selected == end)
                return this->select(last);
            return this->select(this->_selected - 1);
        }

        /// @returns Whether an instance is currently selected.
        operator bool() const { return this->_selected != this->end(); }

      private:
        std::vector<LibraryEntry<Instance>> _libs;

        iterator _selected;

        /// Internal constructor used by the builder.
        explicit LibrarySelector(std::vector<LibraryEntry<Instance>> &&libs) : _libs(libs)
        {
            std::ranges::sort(this->_libs);
            this->_selected = this->end();
        }
    };

    /// @internal
    /// Used by the std::get specialization.
    namespace impl
    {
        template <size_t I> struct LibraryEntryGet;

        template <> struct LibraryEntryGet<0> {
            template <typename Instance>
            static constexpr const std::string &get(LibraryEntry<Instance> const &entry) noexcept
            {
                return entry.name();
            }
        };

        template <> struct LibraryEntryGet<1> {
            template <typename Instance>
            static constexpr const Instance *&get(LibraryEntry<Instance> const &entry) noexcept
            {
                return entry.instance();
            }
        };
    } // namespace impl
} // namespace arcade

namespace std
{
    /// Partial specialization of std::tuple_size for LibraryEntry.
    template <typename Instance>
    struct tuple_size<::arcade::LibraryEntry<Instance>> : public integral_constant<size_t, 2> {
    };

    /// Partial specialization of std::tuple_element at index 0 for LibraryEntry.
    template <typename Instance> struct tuple_element<0, ::arcade::LibraryEntry<Instance>> {
        using type = string;
    };

    /// Partial specialization of std::tuple_element at index 1 for LibraryEntry.
    template <typename Instance> struct tuple_element<1, ::arcade::LibraryEntry<Instance>> {
        using type = Instance *;
    };

    /// Partial specialization of std::get at index I for LibraryEntry.
    template <size_t I, typename Instance>
    constexpr const typename tuple_element<I, ::arcade::LibraryEntry<Instance>>::type &get(
        ::arcade::LibraryEntry<Instance> const &entry) noexcept
    {
        return ::arcade::impl::LibraryEntryGet<I>::get(entry);
    }
} // namespace std

#endif // !defined(ARCADE_CORE_UTIL_LIBRARY_SELECTOR_HPP_)
