#include <cstdlib>
#include <iostream>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "Errors.h"

namespace Lengine {

    void fatalError(std::string errorString)
    {
        std::cout << errorString << std::endl;
        std::cout << "Enter any key to quit...";
        int tmp;
        std::cin >> tmp;
        
        SDL_Quit();
        exit(-1);
    }

   }
