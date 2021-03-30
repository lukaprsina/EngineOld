#pragma once

#include "pch.h"
#include "EngineSDK.h"

#ifdef ENG_PLATFORM_WINDOWS

#include "Windows.h"

template <class T>
class Plugin
{
public:
    Plugin(std::string const& path)
        : m_Handle(nullptr), m_Path(path)
    {
    }

    ~Plugin() = default;

    void Open()
    {
        if (!(m_Handle = LoadLibrary(m_Path.c_str())))
        {
            std::cerr << "Can't open and load " << m_Path << '\n';
        }
    }

    std::shared_ptr<T> Get()
    {
        using allocClass = T * (*)();
        using deleteClass = void (*)(T*);

        auto allocFunc = reinterpret_cast<allocClass>(
            GetProcAddress(m_Handle, "allocator"));
        auto deleteFunc = reinterpret_cast<deleteClass>(
            GetProcAddress(m_Handle, "deleter"));

        if (!allocFunc)
        {
            Close();
            std::cerr << "Can't find allocator symbol in " << m_Path << '\n';
        }
        else if (!deleteFunc)
        {
            Close();
            std::cerr << "Can't find deleter symbol in " << m_Path << '\n';
        }

        return std::shared_ptr<T>(
            allocFunc(),
            [deleteFunc](T* p) { deleteFunc(p); }
        );
    }

    void Close()
    {        
        if (!FreeLibrary(m_Handle))
        {
            std::cerr << "Can't close " << m_Path << '\n';
            std::cerr << GetLastError() << '\n';
        }
    }

private:
    HMODULE m_Handle;
    std::string m_Path;
};
#endif

#ifdef ENG_PLATFORM_UNIX

#include <dlfcn.h>

template <class T>
class Plugin
{
public:
    Plugin(std::string const& path)
        : m_Handle(nullptr), m_Path(path)
    {
    }

    ~Plugin() = default;

    void Open()
    {
        if (!(m_Handle = dlopen(m_Path.c_str())))
        {
            std::cerr << std::cerr << "Can't open and load " << m_Path << '\n';
        }
    }

    std::shared_ptr<T> Get()
    {
        using allocClass = T * (*)();
        using deleteClass = void (*)(T*);

        auto allocFunc = reinterpret_cast<allocClass>(
            dlsym(m_Handle, "allocator"));
        auto deleteFunc = reinterpret_cast<deleteClass>(
            dlsym(m_Handle, "deleter"));

        if (!allocFunc || !deleteFunc)
        {
            Close();
            std::cerr << "Can't find allocator or deleter symbol in " << m_Path << '\n';
        }

        return std::shared_ptr<T>(
            allocFunc(),
            [deleteFunc](T* p) { deleteFunc(p); });
    }

    void Close()
    {
        if (dlclose(m_Handle) != 0)
        {
            std::cerr << "Can't find allocator or deleter symbol in " << m_Path << '\n';
        }
    }

private:
    void* m_Handle;
    std::string m_Path;
};
#endif


