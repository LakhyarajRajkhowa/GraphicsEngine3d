#pragma once
#include "GLTexture.h"
#include <string>
namespace Lengine {


    class ImageLoader
    {
    private:

    public:
        static GLTexture loadPNG(std::string filePath);
    };
}
