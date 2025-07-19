#pragma once


#include "bred/_.h"
#if defined(WINDOWS_DESKTOP)
#include "windowing_win32/_.h"
#include "acme/_operating_system.h"
#endif
#include "gpu_opengl/_.h"



#ifdef _DRAW2D_NANOVG_STATIC
#define CLASS_DECL_DRAW2D_NANOVG
#elif defined(_draw2d_nanovg_project)
#define CLASS_DECL_DRAW2D_NANOVG  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_DRAW2D_NANOVG  CLASS_DECL_IMPORT
#endif

#include "_nanovg.h"
