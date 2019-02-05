
#include <functional>

int main() {
    int a = 5;
    std::reference_wrapper<int> b = a;
    return b;
}
