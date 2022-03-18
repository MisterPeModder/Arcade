/// @file
/// @internal
///
/// SDL2 to arcade event translation

#ifndef ARCADE_GRAPHICS_SDL2_EVENT_HPP_
#define ARCADE_GRAPHICS_SDL2_EVENT_HPP_

#include <SDL_events.h>

namespace arcade
{
    struct Event;

    /// Attempts to convert a SDL2 event to an arcade Event.
    ///
    /// @param raw The SDL2 event
    /// @param[out] event The arcade Event.
    ///
    /// @returns Whether the conversion succeded.
    bool translateSdl2Event(SDL_Event const &raw, Event &event);
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_EVENT_HPP_)
