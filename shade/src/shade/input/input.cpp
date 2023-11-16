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
            // TODO: Handle key press
        }
        if (event.type == SDL_KEYUP)
        {
            const KeyCode ReleasedKey = static_cast<KeyCode>(event.key.keysym.scancode);
            // TODO: Handle key release
        }
    }
}