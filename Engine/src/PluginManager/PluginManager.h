#pragma once

#include "pch.h"
#include "Core/Log.h"
#include "dlfcn.h"

namespace eng
{
    class PluginManager
    {
    public:
        PluginManager(const char *filename, int flags = RTLD_NOW);
        virtual ~PluginManager();

        void Open();
        void Close();
        void *Lookup(const char *symbolStr);

    private:
        const std::string m_Filename;
        const int m_Flags;
        void *m_Handle;
    };
}