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
static_assert(cy::tensor_rank<cy::double_tensor<>> == 0, "rank 0 double tensor");
static_assert(std::same_as<double, cy::double_tensor<>>, "rank 0 double tensor is a double");
static_assert(cy::tensor<cy::double_tensor<1, 2>>, "create a double tensor");
static_assert(cy::tensor_size<cy::double_tensor<2, 3>, 0> == 2, "first axis of the tensor has size 2");
static_assert(cy::tensor_size<cy::double_tensor<2, 3>, 1> == 3, "second axis of the tensor has size 3");

void output()
{
    double values[2][3] = {1, 2, 3, 4, 5, 6};
    cy::check_equal(cy::print_str(values), "{{1, 2, 3}, {4, 5, 6}}");

    double values2[2][3][2] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    cy::check_equal(cy::print_str(values2), "{{{1, 2}, {3, 4}, {5, 6}}, {{7, 8}, {9, 10}, {11, 12}}}");
}

void values()
{
    cy::double_tensor<2, 2> t { 1, 2, 3, 4};

    // Accessors
}

void labels()
{
    cy::labelled_tensor<"ij", double, 2, 2> x {  1, 2, 3, 4 }; 
}

int main()
{
    return cy::test({output, values});
}
