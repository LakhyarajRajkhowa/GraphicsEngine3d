#pragma once

#include "RenderContext.h"

namespace Lengine {

   
    class IRenderer {
    public:
        static bool enableDebugView;
        static DebugView debugViewMode;
        virtual ~IRenderer() = default;

        virtual void Render(
            const RenderContext& ctx
        ) = 0;


    private:
       
    };
    
}

