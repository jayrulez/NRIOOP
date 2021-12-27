#include <array>
#include "BaseVK.h"
#include "DeviceVK.h"

#include <vulkan/vulkan.h>

const char* VULKAN_LOADER_NAME = "vulkan-1.dll";

namespace nri
{
    static bool IsValidDeviceGroup(const VkPhysicalDeviceGroupProperties& group, PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties)
    {
        VkPhysicalDeviceProperties baseProperties = {};
        vkGetPhysicalDeviceProperties(group.physicalDevices[0], &baseProperties);

        for (uint32_t j = 1; j < group.physicalDeviceCount; j++)
        {
            VkPhysicalDeviceProperties properties = {};
            vkGetPhysicalDeviceProperties(group.physicalDevices[j], &properties);

            if (properties.deviceID != baseProperties.deviceID)
                return false;
        }

        return true;
    }

    constexpr std::array<nri::PhysicalDeviceType, 5> PHYSICAL_DEVICE_TYPE = {
        nri::PhysicalDeviceType::UNKNOWN,
        nri::PhysicalDeviceType::INTEGRATED,
        nri::PhysicalDeviceType::DISCRETE,
        nri::PhysicalDeviceType::UNKNOWN,
        nri::PhysicalDeviceType::UNKNOWN
    };

    constexpr PhysicalDeviceType GetPhysicalDeviceType(VkPhysicalDeviceType physicalDeviceType)
    {
        const size_t index = std::min(PHYSICAL_DEVICE_TYPE.size() - 1, (size_t)physicalDeviceType);
        return PHYSICAL_DEVICE_TYPE[index];
    }

#define GET_VK_FUNCTION(instance, name) \
    const auto name = (PFN_##name)vkGetInstanceProcAddr(instance, #name); \
    if (name == nullptr) \
        return Result::UNSUPPORTED;

    Result nri::GetPhysicalDevices(PhysicalDeviceGroup* physicalDeviceGroups, uint32_t& physicalDeviceGroupNum)
    {
        Library* loader = LoadSharedLibrary(VULKAN_LOADER_NAME);
        if (loader == nullptr)
            return Result::UNSUPPORTED;

        const auto vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetSharedLibraryFunction(*loader, "vkGetInstanceProcAddr");
        const auto vkCreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkCreateInstance");

        VkApplicationInfo applicationInfo = {};
        applicationInfo.apiVersion = VK_API_VERSION_1_1;

        VkInstanceCreateInfo instanceCreateInfo = {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;

        VkInstance instance = VK_NULL_HANDLE;
        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

        if (result != VK_SUCCESS)
        {
            UnloadSharedLibrary(*loader);
            return Result::UNSUPPORTED;
        }

        GET_VK_FUNCTION(instance, vkDestroyInstance);
        GET_VK_FUNCTION(instance, vkEnumeratePhysicalDeviceGroups);
        GET_VK_FUNCTION(instance, vkGetPhysicalDeviceProperties);
        GET_VK_FUNCTION(instance, vkGetPhysicalDeviceProperties2);
        GET_VK_FUNCTION(instance, vkGetPhysicalDeviceMemoryProperties);

        uint32_t deviceGroupNum = 0;
        vkEnumeratePhysicalDeviceGroups(instance, &deviceGroupNum, nullptr);

        VkPhysicalDeviceGroupProperties* deviceGroupProperties = STACK_ALLOC(VkPhysicalDeviceGroupProperties, deviceGroupNum);
        result = vkEnumeratePhysicalDeviceGroups(instance, &deviceGroupNum, deviceGroupProperties);

        if (result != VK_SUCCESS)
        {
            vkDestroyInstance(instance, nullptr);
            UnloadSharedLibrary(*loader);
            return Result::UNSUPPORTED;
        }

        if (physicalDeviceGroupNum == 0)
        {
            for (uint32_t i = 0; i < deviceGroupNum; i++)
            {
                if (!IsValidDeviceGroup(deviceGroupProperties[i], vkGetPhysicalDeviceProperties))
                    deviceGroupNum--;
            }
            physicalDeviceGroupNum = deviceGroupNum;
            vkDestroyInstance(instance, nullptr);
            UnloadSharedLibrary(*loader);
            return Result::SUCCESS;
        }

        VkPhysicalDeviceIDProperties deviceIDProperties = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES };

        VkPhysicalDeviceProperties2 properties2 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
        properties2.pNext = &deviceIDProperties;

        VkPhysicalDeviceMemoryProperties memoryProperties = {};

        VkPhysicalDeviceProperties& properties = properties2.properties;

        for (uint32_t i = 0, j = 0; i < deviceGroupNum && j < physicalDeviceGroupNum; i++)
        {
            if (!IsValidDeviceGroup(deviceGroupProperties[i], vkGetPhysicalDeviceProperties))
                continue;

            const VkPhysicalDevice physicalDevice = deviceGroupProperties[i].physicalDevices[0];
            vkGetPhysicalDeviceProperties2(physicalDevice, &properties2);
            vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

#if _WIN32
            if (deviceIDProperties.deviceLUIDValid == VK_FALSE)
            {
                vkDestroyInstance(instance, nullptr);
                UnloadSharedLibrary(*loader);
                return Result::UNSUPPORTED;
            }
#endif

            PhysicalDeviceGroup& group = physicalDeviceGroups[j++];

            group.type = GetPhysicalDeviceType(properties.deviceType);
            group.vendor = GetVendorFromID(properties.vendorID);
            group.deviceID = properties.deviceID;
            group.luid = *(uint64_t*)&deviceIDProperties.deviceLUID[0];
            group.physicalDeviceGroupSize = deviceGroupProperties[i].physicalDeviceCount;

            const size_t descriptionLength = std::min(strlen(properties.deviceName), (size_t)GetCountOf(group.description));
            strncpy(group.description, properties.deviceName, descriptionLength);

            group.dedicatedVideoMemoryMB = 0;
            for (uint32_t k = 0; k < memoryProperties.memoryHeapCount; k++)
            {
                if (memoryProperties.memoryHeaps[k].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
                    group.dedicatedVideoMemoryMB += memoryProperties.memoryHeaps[k].size / (1024 * 1024);
            }
        }

        vkDestroyInstance(instance, nullptr);
        UnloadSharedLibrary(*loader);
        return Result::SUCCESS;
    }
}