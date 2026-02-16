#pragma once

namespace cutty
{
namespace config
{
/** An extension point for custom tensors. ?? Rename to tensor_traits */
template <typename T> struct tensor;
}; // namespace config

template <typename T>
concept numeric = requires(T t) {
    t = 0;
    t = 1;
    t = t + t;
    t = t * t;
};

template <typename T, typename U>
concept decays_to = std::same_as<std::decay_t<T>, U>;

template <typename T>
concept tensor = requires(const T &t, const size_t *index) {
    numeric<typename config::tensor<T>::value_type>;
    { config::tensor<T>::rank() } -> std::same_as<size_t>;
};

template <typename T>
concept input_tensor = tensor<T> && requires(const T &t, const size_t *index) {
    { config::tensor<T>::at(t, index) } -> decays_to<typename config::tensor<T>::value_type>;
};

template <typename T>
concept output_tensor = input_tensor<T> && requires(T &t, const size_t *index) {
    { config::tensor<T>::at(t, index) } -> std::same_as<typename config::tensor<T>::value_type &>;
};

template <typename T>
concept proper_tensor = tensor<T> && config::tensor<T>::rank() > 0 && requires(const T &t) {
    { config::tensor<T>::size<0>(t) } -> std::same_as<size_t>;
};

template <typename T>
concept improper_tensor = tensor<T> && config::tensor<T>::rank() == 0;

template <typename T>
concept fixed_proper_tensor = tensor<T> && requires(T &t) {
    { config::tensor<T>::template size<0>() } -> std::same_as<size_t>;
};

template <typename T>
concept fixed_tensor = fixed_proper_tensor<T> || improper_tensor<T>;

// The Kronecker delta tensor
template <size_t N> struct delta
{
};

// Levi-Civita tensor
template <size_t N> struct epsilon
{
};

// Extension points
namespace config
{
// Specialisation of tensor to handle rank-0 numerical values
template <numeric T> struct tensor<T>
{
    using value_type = T;
    static constexpr size_t rank()
    {
        return 0;
    }

    static T &at(T &t, const size_t *)
    {
        return t;
    }
    static const T &at(const T &t, const size_t *)
    {
        return t;
    }
};

// Specialisation of tensor to handle C-style arrays
template <cutty::tensor T, size_t N> struct tensor<T[N]>
{
    using value_type = tensor<T>::value_type;
    static constexpr size_t rank()
    {
        return 1 + tensor<T>::rank();
    }

    template <size_t Axis> static constexpr size_t size(const T (&t)[N])
    {
        if constexpr (Axis == 0)
        {
            return N;
        }
        else
        {
            return tensor<T>::template size<Axis - 1>(t[0]);
        }
    }

    template <size_t Axis> static constexpr size_t size()
    {
        if constexpr (Axis == 0)
        {
            return N;
        }
        else
        {
            return tensor<T>::template size<Axis - 1>();
        }
    }

    static value_type &at(T (&t)[N], const size_t *index)
    {
        return tensor<T>::at(t[index[0]], index + 1);
    }

    static const value_type &at(const T (&t)[N], const size_t *index)
    {
        return tensor<T>::at(t[index[0]], index + 1);
    }
};

// Specialisation of tensor to handle std::array
template <cutty::tensor T, size_t N> struct tensor<std::array<T, N>>
{
    using value_type = tensor<T>::value_type;

    static constexpr size_t rank()
    {
        return 1 + tensor<T>::rank();
    }

    template <size_t Axis> constexpr static size_t size(const std::array<T, N> &t)
    {
        if constexpr (Axis == 0)
        {
            return N;
        }
        else
        {
            return tensor<T>::template size<Axis - 1>(t[0]);
        }
    }

    template <size_t Axis> constexpr static size_t size()
    {
        if constexpr (Axis == 0)
        {
            return N;
        }
        else
        {
            return tensor<T>::template size<Axis - 1>();
        }
    }

    static value_type &at(std::array<T, N> &t, const size_t *index)
    {
        return tensor<T>::at(t[index[0]], index + 1);
    }

    static const value_type &at(const std::array<T, N> &t, const size_t *index)
    {
        return tensor<T>::at(t[index[0]], index + 1);
    }
};

template <size_t N> struct tensor<delta<N>>
{
    using value_type = int;

    static constexpr size_t rank()
    {
        return 2;
    }

    template <size_t Axis> static size_t size(const delta<N> &)
    {
        return N;
    }

    template <size_t Axis> static constexpr size_t size()
    {
        return N;
    }

    static int at(const delta<N> &, const size_t *index)
    {
        return index[0] == index[1];
    }
};

template <size_t N> struct tensor<epsilon<N>>
{
    using value_type = int;

    static constexpr size_t rank()
    {
        return N;
    }

    template <size_t Axis> static size_t size(const epsilon<N> &)
    {
        return N;
    }

    template <size_t Axis> static constexpr size_t size()
    {
        return N;
    }

    static int at(const epsilon<N> &, const size_t *index) // Initial version by AI
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            for (std::size_t j = i + 1; j < N; ++j)
            {
                if (index[i] == index[j])
                    return 0;
            }
        }

        int sign = 1;
        for (std::size_t i = 0; i < N; ++i)
        {
            for (std::size_t j = i + 1; j < N; ++j)
            {
                if (index[i] > index[j])
                    sign = -sign;
            }
        }

        return sign;
    }
};
} // namespace config

template <tensor T> constexpr int tensor_rank = config::tensor<T>::rank();
template <tensor T, int Axis> constexpr size_t tensor_size = config::tensor<T>::template size<Axis>();

namespace detail
{
// Helper to construct a tensor using a std::array
template <numeric T, size_t... Dims> struct make_tensor;

template <numeric T> struct make_tensor<T>
{
    using type = T;
};

template <numeric T, size_t H, size_t... Ts> struct make_tensor<T, H, Ts...>
{
    using type = std::array<typename make_tensor<T, Ts...>::type, H>;
};

template<fixed_string L, tensor T> requires(tensor_rank<T> == L.size())
struct labelled_tensor
{
    T tensor;
};

template<fixed_string L, typename T>
labelled_tensor<L, T> label(T&&tensor)
{
    return {tensor};
}

}  // detail

// A fixed-size tensor created using std::array
template <typename T, size_t... Dims> using array_tensor = typename detail::make_tensor<T, Dims...>::type;

// A fixed size tensor of doubles
template <size_t... Dims> using double_tensor = array_tensor<double, Dims...>;

template<fixed_string L, typename T, size_t...Dims> using labelled_tensor = detail::labelled_tensor<L, array_tensor<T, Dims...>>;

} // namespace cutty
