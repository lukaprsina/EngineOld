#pragma once

#include "pch.h"

#include "ltdl.h"

template <class T>
class Plugin
{
public:
    Plugin(std::string const &path)
        : m_Handle(nullptr), m_Path(path)
    {
    }

    ~Plugin() = default;

    void Open()
    {
        if (!(m_Handle = lt_dlopenext(m_Path.c_str())))
        {
            std::cerr << lt_dlerror() << std::endl;
        }
    }

    std::shared_ptr<T> Get()
    {
        using allocClass = T *(*)();
        using deleteClass = void (*)(T *);

        auto allocFunc = reinterpret_cast<allocClass>(
            lt_dlsym(m_Handle, "allocator"));
        auto deleteFunc = reinterpret_cast<deleteClass>(
            lt_dlsym(m_Handle, "deleter"));

        if (!allocFunc || !deleteFunc)
        {
            Close();
            std::cerr << lt_dlerror() << std::endl;
        }

        return std::shared_ptr<T>(
            allocFunc(),
            [deleteFunc](T *p) { deleteFunc(p); });
    }

    void Close()
    {
        if (lt_dlclose(m_Handle) != 0)
        {
            std::cerr << lt_dlerror() << std::endl;
        }
    }

private:
    lt_dlhandle m_Handle;
    std::string m_Path;
};