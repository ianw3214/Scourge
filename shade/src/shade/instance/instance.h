#pragma once

#include <memory>

#include "shade/instance/notification.h"

namespace Shade {

    class Window;
    class InputHandler;
    class RendererBase;
    class RenderCommand;
    class State;

    class GameInstance {
    public:
        GameInstance(std::unique_ptr<State> InitialState);
        ~GameInstance();

        void Run();

        void Notify(GameNotification Notification);
        void NotifyRenderer(std::unique_ptr<RenderCommand> Command);
    private:
        std::unique_ptr<Window> mMainWindow = nullptr;
        std::unique_ptr<InputHandler> mInputHandler = nullptr;
        std::unique_ptr<RendererBase> mRenderer = nullptr;
        std::unique_ptr<State> mCurrentState = nullptr;

        bool mRunning = false;
    };
}