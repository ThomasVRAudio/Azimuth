#ifndef COMMON_H
#define COMMON_H

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_DEFINE_MATH_OPERATORS

#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32

#include <Azimuth/helper.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <memory>
#include <string>

#include <vector>
#include <list>
#include <bitset>
#include <queue>
#include <unordered_map>
#include <set>

#include <dependencies/glad/glad.h>
#include <dependencies/GLFW/glfw3.h>
#include <dependencies/GLFW/glfw3native.h>
#include <dependencies/glm/glm.hpp>
#include <dependencies/glm/gtc/matrix_transform.hpp>

#include "dependencies/imgui/imgui.h"
#include "dependencies/imgui/imgui_impl_glfw.h"
#include "dependencies/imgui/imgui_impl_opengl3.h"

#include <cassert>
#include <filesystem>

#include <windows.h>
#include <commdlg.h>

#endif