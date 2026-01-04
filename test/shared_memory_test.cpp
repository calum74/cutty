#include <cutty/shared_memory.hpp>

#include <cutty/test.hpp>

#include <cstring>
#include <filesystem>
#include <iostream>

namespace cy = cutty;

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

void fill(cy::shared_memory &sm)
{
    std::fill((char *)sm.data(), (char *)sm.data() + sm.size(), 100);
}

void check(const cy::shared_memory &sm)
{
    const char *str = (const char *)sm.data();
    for (auto i = 0; i < sm.size(); ++i)
    {
        cy::check(str[1] == 100);
    }
}

void empty_shmem()
{
    cy::shared_memory m;
    cy::check(!m);
}

void test1()
{
    Tmpfile tmp;
    std::error_code ec;
    {
        cy::shared_memory m(tmp.path.string().c_str(), ec, cy::shared_memory::create, 1024);
        cy::check(m);
        cy::check(m.size() == 1024);
        fill(m);
    }

    {
        cy::shared_memory m(tmp.path.string().c_str(), ec);
        if (!m)
        {
            std::cout << ec.message() << std::endl;
        }
        cy::check(m);
        cy::check(m.size() == 1024);
        check(m);
    }

    {
        // Moving test
        cy::shared_memory m(tmp.path.string().c_str(), ec, cy::shared_memory::create, 1024);
    }

    {
        // Extend to 5000
        cy::shared_memory m(tmp.path.string().c_str(), ec, cy::shared_memory::create, 1024);
        cy::check(m);
        auto old_address = m.data();
        m.resize(ec, 5000);
        cy::check(m);
        cy::check(old_address == m.data()); // Should not have moved
        cy::check(m.size() == 5000);
        fill(m);
    }

    {
        // Check the file now contains 5000
        cy::shared_memory m(tmp.path.string().c_str(), ec, cy::shared_memory::create, 1024);
        cy::check(m);
        cy::check(m.size() == 5000);
        check(m);
        m.resize(ec, 1000);
        cy::check(m.size() == 1000);
        check(m);

        auto new_address = (char *)m.data() + 65536;
        m.reopen_at(ec, new_address);
        cy::check(m);
        cy::check(m.data() == new_address);
        check(m);
    }

    {
        cy::shared_memory m(tmp.path.string().c_str(), ec, cy::shared_memory::trunc, 1024);
        cy::check(m);
        cy::check(m.size() == 1024);
    }
}

int main()
{
    return cy::test({empty_shmem, test1});
}
