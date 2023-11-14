#include <windows.h>

#include "shade/instance/instance.h"
#include "shade/module/state.h"
#include "shade/game/world.h"

class GameState : public Shade::State {
public:
    GameState() {
        AddModule(std::make_unique<Shade::GameWorldModule>());
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    Shade::GameInstance MainGameInstance(std::make_unique<GameState>());
    MainGameInstance.Run();

    return 0;
}