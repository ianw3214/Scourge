#include "input.h"

#include <SDL2/SDL.h>

// ======================================
Shade::InputHandler::InputHandler(GameInstance& instance)
    : GameInstanceNotifier(instance)
{

}

// ======================================
void Shade::InputHandler::Update()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            NotifyGame(GameNotification::Quit);
        }
        if (event.type == SDL_KEYDOWN)
        {
            const KeyCode pressedKey = static_cast<KeyCode>(event.key.keysym.scancode);
            mEvents.emplace(InputEvent::CreateKeyPress(pressedKey));
        }
        if (event.type == SDL_KEYUP)
        {
            const KeyCode releasedKey = static_cast<KeyCode>(event.key.keysym.scancode);
            mEvents.emplace(InputEvent::CreateKeyRelease(releasedKey));
        }
    }
}

// ======================================
std::optional<Shade::InputEvent> Shade::InputHandler::GetNextEvent()
{
    if (!mEvents.empty())
    {
        InputEvent event = mEvents.front();
        mEvents.pop();
        return event;
    }
    return std::nullopt;
}