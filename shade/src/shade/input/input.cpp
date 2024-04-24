#include "input.h"

#include <SDL2/SDL.h>

#include "shade/graphics/imgui/wrapper.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

// ======================================
// TODO: Currently temp code - improve this
SDL_Joystick* joystick;

// ======================================
Shade::InputHandler::InputHandler(GameInstance& instance)
    : GameInstanceNotifier(instance)
{
    SDL_JoystickEventState(SDL_ENABLE);

    // TODO: Any need to support multiple joysticks?
    joystick = SDL_JoystickOpen(0);

    LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
    logService->LogInfo(std::to_string(SDL_NumJoysticks()) + std::string(" joystick(s) connected"));
    // logService->LogInfo(std::string(SDL_JoystickName(0)));
}

// ======================================
Shade::InputHandler::~InputHandler()
{
    SDL_JoystickClose(joystick);
}

// ======================================
void Shade::InputHandler::Update()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        ImGuiWrapper::HandleEvent(&event);
        if (event.type == SDL_QUIT)
        {
            NotifyGame(GameNotification::Quit);
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE /* && event.window.windowID == SDL_GetWindowID(window)*/)
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
        // Controller events
        /*
        if (event.type == SDL_JOYAXISMOTION)
        {
            if ((event.jaxis.value < -3200) || (event.jaxis.value > 3200))
            {
                const ControllerAxis axis = static_cast<ControllerAxis>(event.jaxis.axis);
                mEvents.emplace(InputEvent::CreateAxisEvent(axis, event.jaxis.value));
            }
        }
        */
        for (int i = 0; i < static_cast<size_t>(ControllerAxis::SHADE_AXIS_MAX); ++i)
        {
            mEvents.emplace(InputEvent::CreateAxisEvent(static_cast<ControllerAxis>(i), SDL_JoystickGetAxis(joystick, i)));
        }
        if (event.type == SDL_JOYBUTTONDOWN)
        {
            mEvents.emplace(InputEvent::CreateButtonPress(static_cast<ControllerButton>(event.jbutton.button)));
        }
        if (event.type == SDL_JOYBUTTONUP)
        {
            mEvents.emplace(InputEvent::CreateButtonRelease(static_cast<ControllerButton>(event.jbutton.button)));
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