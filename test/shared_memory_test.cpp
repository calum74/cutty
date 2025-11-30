#include <cutty/shared_memory.hpp>

#include <cutty/check.hpp>

#include <filesystem>
#include <cstring>
#include <iostream>


namespace cy=cutty;

struct Tmpfile
{
    const std::filesystem::path path;

    Tmpfile(std::filesystem::path p = "tmpfile.bin") : path(p)
    {
        std::filesystem::remove(path);
    }

    ~Tmpfile()
    {
        std::filesystem::remove(path);
    }
};

int main()
{
    {
        cy::shared_memory m;
        cy::check(!m);
    }

    {
        Tmpfile tmp;
        std::error_code ec;
        {
            cy::shared_memory m(tmp.path.c_str(), ec, cy::shared_memory::create, 1024);
            cy::check(m);
            cy::check(m.size() == 1024);
            strcpy((char*)m.data(), "Hello");
        }

        {
            cy::shared_memory m(tmp.path.c_str(), ec, 0, 1024);
            cy::check(m);
            cy::check(m.size() == 1024);
            cy::check(strcmp((const char*)m.data(), "Hello") == 0);
        }

    }
}