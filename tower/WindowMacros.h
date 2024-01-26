#pragma once

#define WINDOW_EXCEPTION(hResult) Window::WindowException(__LINE__, __FILE__, hResult)
#define NO_GRAPHICS_EXCEPTION() Window::NoGraphicsException(__LINE__, __FILE__)