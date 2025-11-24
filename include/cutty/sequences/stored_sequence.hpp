// Implements a sequence that stores an internal copy of the data

namespace cutty::sequences
{
template <typename Container>
class stored_sequence : public base_sequence<std::ranges::range_value_t<Container>, stored_sequence<Container>>
{
  public:
    Container container;
    using value_type = std::ranges::range_value_t<Container>;
    typename std::ranges::iterator_t<Container> current;

    stored_sequence(Container &&c) : container(std::move(c))
    {
    }

    // TODO: Do we want this?
    // Efficiency warning
    // stored_sequence(const stored_sequence & other) = delete;

    const value_type *first()
    {
        current = container.begin();
        return current == container.end() ? nullptr : &*current;
    }

    const value_type *next()
    {
        ++current;
        return current == container.end() ? nullptr : &*current;
    }

    std::size_t size()
    {
        return std::ranges::distance(container);
    }
};
} // namespace cutty::sequences
