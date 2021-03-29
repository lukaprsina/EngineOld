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
}

extern "C"
{
    ENG_API eng::EngineSDK *allocator()
    {
        return new eng::Sandbox;
    }

    ENG_API void deleter(eng::EngineSDK *ptr)
    {
        delete ptr;
    }
}
