#include <cutty/test.hpp>

#include <cutty/fixed_string.hpp>
#include <cutty/tensor.hpp>

namespace cy = cutty;

static_assert(cy::tensor<double>, "numerical values tensors");
static_assert(cy::tensor_rank<double> == 0, "numerical values have rank 0");

static_assert(cy::tensor<double[10]>, "C arrays are tensors");
static_assert(cy::tensor_rank<double[10]> == 1, "C arrays have rank 1");

static_assert(cy::tensor<double[10][20]>, "C multidimensional arrays are tensors");
static_assert(cy::tensor_rank<double[10][20]> == 2, "Rank is 2");

void output()
{
    double values[2][3] = { 1, 2, 3, 4, 5, 6 };
    cy::check_equal(cy::print_str(values), "{{1, 2, 3}, {4, 5, 6}}");
}

int main()
{
    return cy::test({output});
}
