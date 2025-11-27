#pragma once

#include <exception>
#include <functional>

namespace cutty
{
namespace detail
{
template <typename EntryFn, typename ExitFn, typename ExceptFn> class scope_guard
{
  public:
    scope_guard(EntryFn &&entry, ExitFn &&exit, ExceptFn &&except)
        : entry(std::move(entry)), exit(std::move(exit)), except(std::move(except))
    {
        entry();
    }

    scope_guard(const scope_guard &) = delete;
    scope_guard &operator=(const scope_guard &) = delete;

    scope_guard(scope_guard &&src)
        : entry(std::move(src.entry)), exit(std::move(src.exit)), except(std::move(src.except))
    {
    }

    ~scope_guard()
    {
        if (std::uncaught_exceptions())
            except();
        else
            exit();
    }

  private:
    EntryFn entry;
    ExitFn exit;
    ExceptFn except;
};

template <typename EntryFn, typename ExitFn, typename ExceptFn>
auto make_guard(EntryFn entry, ExitFn exit, ExceptFn except)
{
    return scope_guard<EntryFn, ExitFn, ExceptFn>{std::move(entry), std::move(exit), std::move(except)};
}

template <typename EntryFn, typename ExitFn, typename ExceptFn> class scope_hooks
{
  public:
    constexpr scope_hooks(EntryFn &&entry, ExitFn &&exit, ExceptFn &&except)
        : entry(std::move(entry)), exit(std::move(exit)), except(std::move(except))
    {
    }

    template <typename... Args> constexpr auto bind(Args &&...args) const
    {
        return scope_hooks(std::bind(entry, std::forward<Args &&>(args)...),
                           std::bind(exit, std::forward<Args &&>(args)...),
                           std::bind(except, std::forward<Args &&>(args)...));
    }

    template <typename... Args> constexpr auto operator()(Args &&...args) const
    {
        return make_guard(std::bind(entry, std::forward<Args &&>(args)...),
                          std::bind(exit, std::forward<Args &&>(args)...),
                          std::bind(except, std::forward<Args &&>(args)...));
    }

  private:
    EntryFn entry;
    ExitFn exit;
    ExceptFn except;
};

} // namespace detail

// Creates a scope_hooks with the given entry, exit and exception functions.
//
// Returns an unspecified type which can be called to create a scope guard of unspecified type.
template <typename EntryFn, typename ExitFn, typename ExceptFn>
constexpr auto scope_hooks(EntryFn &&entry, ExitFn &&exit, ExceptFn &&except)
{
    return detail::scope_hooks<EntryFn, ExitFn, ExceptFn>{
        std::forward<EntryFn &&>(entry), std::forward<ExitFn &&>(exit), std::forward<ExceptFn &&>(except)};
}

// Creates a scope_hooks with the given entry and exit functions.
// The exit function is also used for exceptional exits.
//
// Returns an unspecified type which can be called to create a scope guard of unspecified type.
template <typename EntryFn, typename ExitFn> constexpr auto scope_hooks(EntryFn &&entry, ExitFn &&exit)
{
    return scope_hooks(std::forward<EntryFn &&>(entry), std::forward<ExitFn &&>(exit), std::forward<ExitFn &&>(exit));
}

// Creates a scope_hooks with the given exit function.
// There is no entry function.
//
// Returns an unspecified type which can be called to create a scope guard of unspecified type.
template <typename ExitFn> constexpr auto scope_hooks(ExitFn &&exit)
{
    return scope_hooks([](auto &&...) {}, std::forward<ExitFn &&>(exit));
}
} // namespace cutty
