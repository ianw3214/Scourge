#include "input.h"

#include <SDL2/SDL.h>

// ======================================
Shade::InputHandler::InputHandler(GameInstance& Instance)
    : GameInstanceNotifier(Instance)
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
            const KeyCode PressedKey = static_cast<KeyCode>(event.key.keysym.scancode);
            mEvents.emplace(InputEvent::CreateKeyPress(PressedKey));
        }
        if (event.type == SDL_KEYUP)
        {
            const KeyCode ReleasedKey = static_cast<KeyCode>(event.key.keysym.scancode);
            mEvents.emplace(InputEvent::CreateKeyRelease(ReleasedKey));
        }
    }
}

// ======================================
std::optional<Shade::InputEvent> Shade::InputHandler::GetNextEvent()
{
    if (!mEvents.empty())
    {
        InputEvent ResultEvent = mEvents.front();
        mEvents.pop();
        return ResultEvent;
    }
    return std::nullopt;
}