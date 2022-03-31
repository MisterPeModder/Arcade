/// @file
/// @internal
///
/// SFML to arcade event translation

#ifndef ARCADE_GRAPHICS_SFML_EVENT_HPP_
#define ARCADE_GRAPHICS_SFML_EVENT_HPP_

namespace sf
{
    class Event;
}

namespace arcade
{
    struct Event;

    /// Attempts to convert a SFML event to an arcade Event.
    ///
    /// @param raw The SFML event
    /// @param[out] event The arcade Event.
    ///
    /// @returns Whether the conversion succeded.
    bool translateSfmlEvent(sf::Event const &raw, Event &event);
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SFML_EVENT_HPP_)
