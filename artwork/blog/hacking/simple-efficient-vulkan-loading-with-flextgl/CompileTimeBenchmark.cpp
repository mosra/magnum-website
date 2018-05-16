// Put volk.h/volk.c, generated flextVk.h/flextVk.cpp in this directory and
// install system vulkan.h and vulkan.hpp headers, then do the following:
//
// g++ CompileTimeBenchmark.cpp -o CompileTimeBenchmark -std=c++11 -lCorradeTestSuite-d -lCorradeUtility-d
// ./CompileTimeBenchmark --color on # until it gets white

#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/Directory.h>

using namespace Corrade;

struct CompileTimeBenchmark: TestSuite::Tester {
    explicit CompileTimeBenchmark();

    void flextVkMinimal();
    void flextVkMagnum();
    void volk();
    void vulkanH();
    void vulkanHpp();
};

CompileTimeBenchmark::CompileTimeBenchmark() {
    addBenchmarks({&CompileTimeBenchmark::flextVkMinimal,
                   &CompileTimeBenchmark::flextVkMagnum,
                   &CompileTimeBenchmark::volk,
                   &CompileTimeBenchmark::vulkanH}, 10, BenchmarkType::WallTime);

    addBenchmarks({&CompileTimeBenchmark::vulkanHpp}, 3, BenchmarkType::WallTime);
}

void CompileTimeBenchmark::flextVkMinimal() {
    setTestCaseDescription("1929 lines");

    CORRADE_VERIFY(Utility::Directory::fileExists("flextVk.h"));
    CORRADE_VERIFY(Utility::Directory::fileExists("flextVk.cpp"));

    CORRADE_COMPARE(std::system("g++ -c flextVk.cpp -std=c++11"), 0);

    Utility::Directory::writeString("minimal.cpp", R"(
#include "flextVk.h"

int main() {
    VkInstance instance;
    {
        VkInstanceCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vkCreateInstance(&info, nullptr, &instance);
    }

    flextVkInit(instance);
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ minimal.cpp flextVk.o -o minimal -lvulkan -std=c++11"), 0);
    }
}

void CompileTimeBenchmark::flextVkMagnum() {
    setTestCaseDescription("3592 lines");

    CORRADE_VERIFY(Utility::Directory::fileExists("/usr/include/MagnumExternal/Vulkan/flextVk.h"));

    Utility::Directory::writeString("magnum.cpp", R"(
#include <MagnumExternal/Vulkan/flextVk.h>

int main() {
    VkInstance instance;
    {
        VkInstanceCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vkCreateInstance(&info, nullptr, &instance);
    }

    FlextVkInstance i;
    flextVkInitInstance(instance, &i);

    VkPhysicalDevice physicalDevice;
    {
        uint32_t count = 1;
        i.EnumeratePhysicalDevices(instance, &count, &physicalDevice);
    }
    VkDevice device;
    {
        const float zero = 0.0f;

        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = 0; /* The first family from above */
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &zero;

        VkDeviceCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        info.queueCreateInfoCount = 1;
        info.pQueueCreateInfos = &queueInfo;
        i.CreateDevice(physicalDevice, &info, nullptr, &device);
    }
    FlextVkDevice d;
    flextVkInitDevice(device, &d, i.GetDeviceProcAddr);
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ magnum.cpp -o magnum -lMagnumVk-d -lvulkan -std=c++11"), 0);
    }
}

void CompileTimeBenchmark::volk() {
    setTestCaseDescription("6352 lines");

    CORRADE_VERIFY(Utility::Directory::fileExists("volk.h"));
    CORRADE_VERIFY(Utility::Directory::fileExists("volk.c"));

    CORRADE_COMPARE(std::system("g++ -c volk.c -std=c++11"), 0);

    Utility::Directory::writeString("volk.cpp", R"(
#include "volk.h"

int main() {
    volkInitialize();
    VkInstance instance;
    {
        VkInstanceCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vkCreateInstance(&info, nullptr, &instance);
    }

    volkLoadInstance(instance);
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ volk.cpp volk.o -o volk -std=c++11 -ldl"), 0);
    }
}

void CompileTimeBenchmark::vulkanH() {
    setTestCaseDescription("7363 lines");

    CORRADE_VERIFY(Utility::Directory::fileExists("/usr/include/vulkan/vulkan.h"));

    Utility::Directory::writeString("h.cpp", R"(
#include <vulkan/vulkan.h>

int main() {
    VkInstance instance;
    {
        VkInstanceCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vkCreateInstance(&info, nullptr, &instance);
    }

    void(*a[49])();
    for(int i = 0; i != 49; ++i)
        a[i] = vkGetInstanceProcAddr(instance, "CreateBuffer");
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ h.cpp -o h -std=c++11 -lvulkan"), 0);
    }
}

void CompileTimeBenchmark::vulkanHpp() {
    setTestCaseDescription("83530 lines");

    CORRADE_VERIFY(Utility::Directory::fileExists("/usr/include/vulkan/vulkan.hpp"));

    Utility::Directory::writeString("hpp.cpp", R"(
#include <vulkan/vulkan.hpp>

int main() {
    vk::Instance instance;
    {
        vk::InstanceCreateInfo info{};
        vk::createInstance(&info, nullptr, &instance);
    }

    void(*a[49])();
    for(int i = 0; i != 49; ++i)
        a[i] = vkGetInstanceProcAddr(instance, "CreateBuffer");
}
)");

    CORRADE_BENCHMARK(1) {
        CORRADE_COMPARE(std::system("g++ hpp.cpp -o hpp -std=c++11 -lvulkan"), 0);
    }
}

CORRADE_TEST_MAIN(CompileTimeBenchmark)
