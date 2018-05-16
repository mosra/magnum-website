// See CompileTimeBenchmark.cpp for setup details, then do the following:
//
// g++ RuntimeCostBenchmark.cpp volk.c flextVk.cpp -o RuntimeCostBenchmark -std=c++11 -lMagnumVk-d -lvulkan -lCorradeTestSuite-d -lCorradeUtility-d -ldl
// ./RuntimeCostBenchmark --color on # until it gets white
//
// volk.h needs to have the following added to avoid symbol clashes:
//
//  #define vkCreateInstance _vkCreateInstance
//  #define vkGetInstanceProcAddr _vkGetInstanceProcAddr

#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/Directory.h>

#include <MagnumExternal/Vulkan/flextVk.h>

extern "C" {

void flextVkInit(VkInstance instance);

VkResult volkInitialize();

void volkLoadInstance(VkInstance instance);

}

using namespace Corrade;

struct RuntimeCostBenchmark: TestSuite::Tester {
    explicit RuntimeCostBenchmark();

    void flextVkMinimal();
    void flextVkMagnum();
    void volk();
    void createInstance();

    VkInstance _instance;
    FlextVkInstance _i;

    VkDevice _device;
    FlextVkDevice _d;
};

RuntimeCostBenchmark::RuntimeCostBenchmark() {
    addBenchmarks({&RuntimeCostBenchmark::flextVkMinimal,
                   &RuntimeCostBenchmark::flextVkMagnum,
                   &RuntimeCostBenchmark::volk}, 500, BenchmarkType::WallTime);

    addBenchmarks({&RuntimeCostBenchmark::createInstance},
        10, BenchmarkType::WallTime);

    {
        VkInstanceCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vkCreateInstance(&info, nullptr, &_instance);
    }
    flextVkInitInstance(_instance, &_i);

    VkPhysicalDevice physicalDevice;
    {
        uint32_t count = 1;
        _i.EnumeratePhysicalDevices(_instance, &count, &physicalDevice);

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
        _i.CreateDevice(physicalDevice, &info, nullptr, &_device);
    }
    flextVkInitDevice(_device, &_d, _i.GetDeviceProcAddr);
}

void RuntimeCostBenchmark::flextVkMinimal() {
    setTestCaseDescription("49 ptrs");

    CORRADE_BENCHMARK(100) {
        flextVkInit(_instance);
    }
}

void RuntimeCostBenchmark::flextVkMagnum() {
    setTestCaseDescription("192 ptrs");

    CORRADE_BENCHMARK(100) {
        FlextVkInstance i;
        flextVkInitInstance(_instance, &i);
        FlextVkDevice d;
        flextVkInitDevice(_device, &d, i.GetDeviceProcAddr);
    }
}

void RuntimeCostBenchmark::volk() {
    setTestCaseDescription("302 ptrs");

    CORRADE_BENCHMARK(100) {
        volkInitialize();
        volkLoadInstance(_instance);
    }
}

void RuntimeCostBenchmark::createInstance() {
    VkInstance instance;
    CORRADE_BENCHMARK(50) {
        VkInstanceCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vkCreateInstance(&info, nullptr, &instance);
    }
    _i.DestroyInstance(instance, nullptr);
}

CORRADE_TEST_MAIN(RuntimeCostBenchmark)
