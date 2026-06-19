#pragma once
#include <string>

namespace Lengine
{
    struct LoadingScreenInfo
    {
        float progress = 0.0f;

        std::string title;
        std::string message;

        bool modal = true;
    };

    class ILoadingScreen
    {
    public:
        virtual ~ILoadingScreen() = default;

        virtual void Show(const LoadingScreenInfo& info) = 0;
        virtual void Hide() = 0;
    };
}