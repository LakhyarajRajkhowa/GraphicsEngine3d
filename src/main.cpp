#include <Windows.h>

#define SDL_MAIN_HANDLED

#include "GraphicsEngine.h"

int main(int argc, char* argv[]) {

    Lengine::GraphicsEngine engine;
    engine.initSettings();
    engine.run();

    return 0;

}



