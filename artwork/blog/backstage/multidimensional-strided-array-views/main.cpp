
#include </home/mosra/Code/mdspan/include/experimental/mdspan>

int main() {
    int data[]{1, 3, 42, 1337, -3, 15};

    std::experimental::mdspan a{data, {3, 2}};
}
