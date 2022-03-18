#include <algorithm>

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
            case SDL_WINDOWEVENT_RESIZED:
                event.type = Event::EventType::Resized;
                event.size.height = static_cast<unsigned int>(std::max(0, raw.data1));
                event.size.width = static_cast<unsigned int>(std::max(0, raw.data2));
                return true;
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
