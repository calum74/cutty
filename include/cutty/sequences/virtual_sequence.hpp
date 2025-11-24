// Implement a sequence that wraps another sequence in virtual functions
namespace cutty::sequences
{
template <typename T, typename Seq> class virtual_sequence : public sequence<T>
{
    Seq seq;

  public:
    virtual_sequence(Seq seq) : seq(seq)
    {
    }

    const T *first() override
    {
        return seq.first();
    }
    const T *next() override
    {
        return seq.next();
    }
    std::size_t size() override
    {
        return std::ranges::distance(seq);
    }
};
} // namespace cutty::sequences
