#pragma once

#include "Graphics.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace App
{

class Window
{
  private:
    GLFWwindow* _window;
    Graphics::Renderer* _renderer;

  public:
    Window();
    ~Window();

    void SetRenderer(Graphics::Renderer* renderer);

    void Render();
};

} // namespace App
