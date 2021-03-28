#include "PluginManager/PluginManager.h"

namespace eng
{
    PluginManager::PluginManager(const char *filename, int flags)
        : m_Filename(filename),
          m_Flags(flags)
    {
    }

    PluginManager::~PluginManager()
    {
        Close();
    }

    void PluginManager::Open()
    {
        m_Handle = dlopen(m_Filename.c_str(), m_Flags);

        if (0 == m_Handle)
        {
            ENG_CORE_ERROR("{0}", dlerror());
        }

        dlerror();
    }

    void PluginManager::Close()
    {
        if (nullptr != m_Handle)
        {
            dlclose(m_Handle);
        }
    }

    void *PluginManager::Lookup(const char *symbolName)
    {
        void *testFunc = nullptr;
        testFunc = dlsym(m_Handle, symbolName);

        if (0 == testFunc)
        {
            ENG_CORE_ERROR("{0}", dlerror());
        }

        return testFunc;
    }
}