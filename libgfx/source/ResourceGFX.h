#pragma once
#include "lib_Export.h"
lib_API typedef void(*Function) (void);
lib_API bool InitLibGRAPH(Function function);
lib_API void ResetViewPort(int initWidth, int initHeight, int width, int height);
lib_API void WireFrameGFX(bool wireframe);