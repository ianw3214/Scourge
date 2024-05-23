#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Shade {

    class RenderCommand;
    struct InputEvent;

    class EditorBase {
    public:
        EditorBase(const std::string& name);
        virtual ~EditorBase();

        const std::string& GetName() const;

        virtual void OnEnter() = 0;
        virtual void OnExit() = 0;

        virtual void Update(float deltaSeconds) = 0;
        virtual void Render(std::vector<std::unique_ptr<RenderCommand>>& commandQueue) = 0;
        virtual bool HandleEvent(const InputEvent& event) = 0;
    private:
        std::string mName;
    };

}