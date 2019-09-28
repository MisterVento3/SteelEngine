#include "Graphics/Vulkan/DeepLayer/PhysicalDevice.h"

#include "Graphics/Vulkan/DeepLayer/LogicalDevice.h"
#include "Graphics/Vulkan/DeepLayer/QueueFamilyIndices.h"

#include "Graphics/Vulkan/PresentationLayer/SwapChainSupportDetails.h"

#include "Graphics/Vulkan/Renderer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(device, surface);

        bool extensionsSupported = CheckDeviceExtensionSupport(device);
        bool swapChainAdequate = false;

        if(extensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::QuerySwapChainSupport(device, surface);
            
            swapChainAdequate = !swapChainSupport.m_Formats.empty() && !swapChainSupport.m_PresentModes.empty();
        }

        return indices.IsComplete() && extensionsSupported && swapChainAdequate;
    }

    bool PhysicalDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;

        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);

        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(Renderer::mc_DeviceExtensions.begin(), Renderer::mc_DeviceExtensions.end());

        for(const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    PhysicalDevice::PhysicalDevice()
    {

    }

    PhysicalDevice::~PhysicalDevice()
    {

    }

    Result PhysicalDevice::PickPhysicalDevice(
        VkInstance instance,
        Surface* surface)
    {
        Type::uint32 deviceCount = 0;

        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if(deviceCount == 0)
        {
            return SE_FALSE;
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);

        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for(const auto& device : devices)
        {
            if(IsDeviceSuitable(device, surface->m_Surface))
            {
                m_PhysicalDevice = device;

                break;
            }
        }

        if(m_PhysicalDevice == VK_NULL_HANDLE)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    Result PhysicalDevice::CreateLogicalDevice(LogicalDevice* logicalDevice, const VkDeviceCreateInfo& createInfo) const
    {
        if(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &logicalDevice->m_LogicalDevice) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    Type::uint32 PhysicalDevice::FindMemoryType(
        Type::uint32 typeFilter,
        VkMemoryPropertyFlags properties) const
    {
        VkPhysicalDeviceMemoryProperties memProperties;

        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);

        for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable memory type!");
    }

}}}