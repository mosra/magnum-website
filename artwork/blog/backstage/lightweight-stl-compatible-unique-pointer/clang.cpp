
#include <memory>

int main() {
    std::unique_ptr<int> a{new int{5}};
    return *a;
}
