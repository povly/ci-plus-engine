#include <vulkan/vulkan.h>   // свой заголовок Vulkan — GLFW его увидит
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdio>

int main() {
    if (!glfwInit()) return 1;

    if (glfwVulkanSupported()) {
        uint32_t count = 0;
        const char** exts = glfwGetRequiredInstanceExtensions(&count);
        printf("Vulkan OK. Inстанsu нужны расширения:\n");
        for (uint32_t i = 0; i < count; ++i) printf("  %s\n", exts[i]);
    } else {
        fprintf(stderr, "Vulkan не найден (loader/ICD)\n");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "engine", nullptr, nullptr);
    if (!window) { glfwTerminate(); return 1; }

    while (!glfwWindowShouldClose(window)) glfwPollEvents();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}