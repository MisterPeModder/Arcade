/// @file
/// @internal

#ifndef ARCADE_CORE_UTIL_LIBRARY_INSTANCE_HPP_
#define ARCADE_CORE_UTIL_LIBRARY_INSTANCE_HPP_

#include <cstddef>

namespace arcade
{
    template <typename T> class LibraryInstance {
      public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Constructors
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Default contructor, constructs a LibraryInstance with no instance in it.
        LibraryInstance() : _instance(nullptr) {}

        /// Constructs a LibraryInstance with no instance in it.
        LibraryInstance(nullptr_t) : _instance(nullptr) {}

        /// Constructs a LibraryInstance with the given instance and calls its setup function.
        ///
        /// @param instance The instance.
        /// @param args Optional arguments to the setup function, may be empty.
        template <typename... SetupArgs> LibraryInstance(T *instance, SetupArgs... args) : _instance(instance)
        {
            this->_instance->setup(args...);
        }

        /// Cannot copy library instances.
        LibraryInstance(LibraryInstance const &) = delete;

        /// Move constructor.
        LibraryInstance(LibraryInstance &&other) : _instance(other._instance) { other._instance = nullptr; }

        /// Move assignment.
        LibraryInstance &operator=(LibraryInstance &&other)
        {
            this->clear();
            this->_instance = other._instance;
            other->_instance = nullptr;
            return *this;
        }

        /// Destructor.
        ~LibraryInstance() { this->clear(); }

        /// Unloads the contained instance, if any.
        void clear()
        {
            if (this->_instance) {
                this->_instance->close();
                this->_instance = nullptr;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Instance Query API
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Sets the current active instance to the given one and calls its setup function.
        ///
        /// The previous instance (if exists) is unloaded.
        ///
        /// @param instance The new instance.
        /// @param args Optional arguments to the setup function, may be empty.
        ///
        /// @returns The previous (now unloaded) instance, or null if none.
        template <typename... SetupArgs> T *set(T *instance, SetupArgs... args)
        {
            T *previous(this->_instance);

            this->clear();
            if (instance)
                instance->setup(args...);
            this->_instance = instance;
            return previous;
        }

        /// @returns The contained instance, may be null.
        T const *get() const { return this->_instance; }

        /// @returns The contained instance, may be null.
        T *get() { return this->_instance; }

        /// Provides constant access to the instance's members.
        ///
        /// @note Using this operator when nothing is contained results in <b>undefined behavior</b>.
        T const *operator->() const { return this->_instance; }

        /// Provides mutable access to the instance's members.
        ///
        /// @note Using this operator when nothing is contained results in <b>undefined behavior</b>.
        T *operator->() { return this->_instance; }

        /// Provides an immutable reference to the instance.
        ///
        /// @note Using this operator when nothing is contained results in <b>undefined behavior</b>.
        ///
        /// @returns An immutable reference to the wrapped instance.
        T const &operator*() const { return *this->_instance; }

        /// Provides a mutable reference to the instance.
        ///
        /// @note Using this operator when nothing is contained results in <b>undefined behavior</b>.
        ///
        /// @returns A mutable reference to the wrapped instance.
        T &operator*() { return *this->_instance; }

        /// @returns Whether this currently contains a loaded instance.
        operator bool() const { return this->_instance != nullptr; }

      private:
        T *_instance;
    };
} // namespace arcade

#endif // !defined(ARCADE_CORE_UTIL_LIBRARY_INSTANCE_HPP_)
