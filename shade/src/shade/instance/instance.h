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

        void SetState(std::unique_ptr<State> NewState);
        void Run();

        void Notify(GameNotification Notification);
        void NotifyRenderer(std::unique_ptr<RenderCommand> Command);
    private:
        std::unique_ptr<Window> mMainWindow = nullptr;
        std::unique_ptr<InputHandler> mInputHandler = nullptr;
        std::unique_ptr<RendererBase> mRenderer = nullptr;
        std::unique_ptr<State> mCurrentState = nullptr;
        // std::unique_ptr<ResourceManager> mResourceManager = nullptr;

        bool mRunning = false;
        float mDeltaSeconds = 1000.0 / 24.0;
    };
}