#pragma once

#include <memory>

#include "shade/instance/notification.h"
#include "shade/instance/service/provider.h"

namespace Shade {

    class Window;
    class InputHandler;
    class RendererBase;
    class RenderCommand;
    class ResourceManager;
    class State;

    class GameInstance : public ServiceProvider {
    public:
        GameInstance();
        ~GameInstance();

        void SetState(std::unique_ptr<State> state);
        void Run();

        void Notify(GameNotification notification);
        void NotifyRenderer(std::unique_ptr<RenderCommand> command);
    private:
        std::unique_ptr<Window> mMainWindow = nullptr;
        std::unique_ptr<InputHandler> mInputHandler = nullptr;
        std::unique_ptr<RendererBase> mRenderer = nullptr;
        std::unique_ptr<State> mCurrentState = nullptr;

        bool mRunning = false;
        float mdeltaSeconds = 1000.0 / 24.0;
    };
}