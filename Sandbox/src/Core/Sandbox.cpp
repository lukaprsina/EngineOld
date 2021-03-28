#include "Core/Sandbox.h"
#include <iostream>

namespace eng
{
    Sandbox::Sandbox()
    {
    }

    Sandbox::~Sandbox()
    {
    }

    void Sandbox::SayHello()
    {
        std::cout << "Hello from the sandbox\n";
    }

    extern "C"
    {
        ENG_API EngineSDK *Create()
        {
            return new Sandbox;
        }
    }
}
