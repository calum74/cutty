#include <cutty/test.hpp>
#include <cutty/with.hpp>

#include <iostream>
#include <map>
#include <vector>
#include <unordered_set>

namespace cy = cutty;

cy::test_fixture::test_fixture(test_function fn) : function(fn), name(nullptr)
{
}

cy::test_fixture::test_fixture(const char *name, test_function fn) : function(fn), name(name)
{
}

namespace
{
using test_function = void (*)();

struct run_test_fixture
{
    std::string display_name;
    test_function function;
};

std::vector<run_test_fixture> get_tests(int argc, char **argv, std::initializer_list<cy::test_fixture> tests)
{
    std::map<std::string, test_function> test_map;

    std::unordered_set<std::string> to_run{argv + 1, argv + argc};
    std::vector<run_test_fixture> result;
    int index;

    for (auto &test : tests)
    {
        ++index;
        const std::string name = test.name ? std::string(test.name) : std::to_string(index);

        if (to_run.count(name))
        {
            result.emplace_back(name, test.function);
        }
    }

    return result;
}
} // namespace

int cy::test(std::initializer_list<test_fixture> list)
{
    const char *args[] = {""};
    return test(1, args, list);
}

int cy::test(int argc, const char **argv, std::initializer_list<test_fixture> list)
{
    std::map<std::string, test_function> available_test_map;
    std::vector<std::string> requested_tests;

    bool run_all_tests = argc == 1;
    int index = 0;

    for (auto &test : list)
    {
        // !! Check that the name is not reused
        ++index;
        const std::string name = test.name ? std::string(test.name) : std::to_string(index);
        available_test_map[name] = test.function;
        if (run_all_tests)
        {
            requested_tests.push_back(name);
        }
    }

    for (int arg = 1; arg < argc; ++arg)
    {
        if (!available_test_map.count(argv[arg]))
        {
            std::cout << "ERROR: The test '" << argv[arg] << "' was not found\n";
            return 1;
        }
        requested_tests.push_back(argv[arg]);
    }

    int passed = 0, failed = 0;
    std::cout << "Running " << requested_tests.size() << " tests\n";
    for (auto &name : requested_tests)
    {
        std::cout << "  " << name << ": " << std::flush;
        std::source_location src;
        with<std::source_location, detail::checkpoint_tag> _ (src);

        try
        {
            available_test_map[name]();
            std::cout << "PASSED\n";
            ++passed;
        }
        catch (std::exception &ex)
        {
            std::cout << "FAILED: " << ex.what() << std::endl;
            if (src.line())
            {
                std::cout << "  Last checkpoint: " << src.file_name() << ":" << src.line() << std::endl;
            }
            ++failed;
        }
        catch (...)
        {
            std::cout << "FAILED (exception)\n";
            ++failed;
        }
    }
    if (failed == 0)
    {
        std::cout << "Tests PASSED\n";
        return 0;
    }
    else
    {
        std::cout << failed << " tests FAILED\n";
        return 1;
    }
}