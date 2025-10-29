#pragma once
#include "TextureCache.h"

namespace Lengine {

    class ResourceManager
    {
    private:
        static TextureCache _textureCache;
    public:
        static GLTexture getTexture(std::string texturePath);

        static GLuint boundTexture;

    };
}


