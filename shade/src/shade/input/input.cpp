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
    // TODO: Hacky fix - might want a better long term solution
    static std::vector<KeyCode> sPressedKeys;

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        ImGuiWrapper::HandleEvent(&event);
        const bool ImGuiCaptureMouse = ImGuiWrapper::WantCaptureMouse();
        const bool ImGuiCaptureKeyboard = ImGuiWrapper::WantCaptureKeyboard();
        if (event.type == SDL_QUIT)
        {
            NotifyGame(GameNotification::Quit);
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE /* && event.window.windowID == SDL_GetWindowID(window)*/)
        {
            NotifyGame(GameNotification::Quit);
        }
        // Key events
        if (event.type == SDL_KEYDOWN)
        {
            const KeyCode pressedKey = static_cast<KeyCode>(event.key.keysym.scancode);
            if (std::find(sPressedKeys.begin(), sPressedKeys.end(), pressedKey) == sPressedKeys.end())
            {
                mEvents.emplace(InputEvent::CreateKeyPress(pressedKey));
                sPressedKeys.emplace_back(pressedKey);
            }
        }
        if (event.type == SDL_KEYUP)
        {
            const KeyCode releasedKey = static_cast<KeyCode>(event.key.keysym.scancode);
            auto keyIterator = std::find(sPressedKeys.begin(), sPressedKeys.end(), releasedKey);
            if (keyIterator != sPressedKeys.end())
            {
                mEvents.emplace(InputEvent::CreateKeyRelease(releasedKey));
                sPressedKeys.erase(keyIterator);
            }
        }
        // Controller events
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
        // Mouse events
        //  - SDL uses the top left as the origin for moues coordinates, so it needs to be inverted
        //  - Currently position inversion is hard coded to a default window height
        //      - TODO: Once height is adjustable, need to change this to adapt to current window height
        if (event.type == SDL_MOUSEBUTTONDOWN && !ImGuiCaptureMouse)
        {
            int x, y;
            SDL_GetMouseState( &x, &y );
            mEvents.emplace(InputEvent::CreateMousePress(x, 720.f - y));
        }
        if (event.type == SDL_MOUSEBUTTONUP && !ImGuiCaptureMouse)
        {
            int x, y;
            SDL_GetMouseState( &x, &y );
            mEvents.emplace(InputEvent::CreateMouseRelease(x, 720.f - y));
        }
        if (event.type == SDL_MOUSEMOTION && !ImGuiCaptureMouse)
        {
            int x, y;
            SDL_GetMouseState( &x, &y );
            mEvents.emplace(InputEvent::CreateMouseMotion(x, 720.f - y, static_cast<float>(event.motion.xrel), static_cast<float>(-event.motion.yrel)));
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