#include <SDL_events.h>
#include <SDL_video.h>

#include <arcade/Event.hpp>

#include "event.hpp"

namespace arcade
{
    static bool translateSdl2WindowEvent(SDL_WindowEvent const &raw, Event &event)
    {
        switch (raw.event) {
            case SDL_WINDOWEVENT_CLOSE: event.type = Event::EventType::Closed; return true;
            default: return false;
        }
    }

    bool translateSdl2Event(SDL_Event const &raw, Event &event)
    {
        switch (raw.type) {
            case SDL_WINDOWEVENT: return translateSdl2WindowEvent(raw.window, event);
            default: return false;
        }
    }
} // namespace arcade
