#pragma once

#define ENG_BUILD_DLL
#include "EngineSDK.h"
#include "Core/Base.h"

namespace eng
{
    class Sandbox : public EngineSDK
    {
    public:
        Sandbox();
        ~Sandbox();

        void SayHello() override;
    };
}