
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <Corrade/Containers/Pointer.h>

using namespace Corrade;

int main() {
    Containers::Pointer<int> a{new int{5}};
    return *a;
}
