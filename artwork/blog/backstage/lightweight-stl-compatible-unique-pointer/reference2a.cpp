
#define CORRADE_NO_DEBUG
#define CORRADE_STANDARD_ASSERT
#include <Corrade/Containers/Reference.h>

using namespace Corrade;

int main() {
    int a = 5;
    Containers::Reference<int> b = a;
    return b;
}
