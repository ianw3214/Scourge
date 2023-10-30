#include <windows.h>

#include "shade/instance/instance.h"
#include "shade/module/state.h"

class GameState : public Shade::State {
public:
    GameState() {

    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    Shade::GameInstance MainGameInstance(std::make_unique<GameState>());
    MainGameInstance.Run();

    return 0;
}