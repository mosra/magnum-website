
import std;

int main() {

    std::unique_ptr<int> a{new int{0}};
    return *a;
}
