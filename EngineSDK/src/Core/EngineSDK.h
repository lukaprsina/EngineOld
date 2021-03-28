#pragma once

namespace eng
{
    class EngineSDK
    {
    public:
        virtual ~EngineSDK();

        virtual void SayHello() = 0;
    };
}