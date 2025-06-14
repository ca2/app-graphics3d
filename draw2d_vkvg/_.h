#pragma once


#include "bred/_.h"
#if defined(WINDOWS_DESKTOP)
#include "windowing_win32/_.h"
#include "acme/_operating_system.h"
#endif
#include "gpu_vulkan/_.h"
#define VKVG_SHARED_LINKING
#include <vkh.h>
#include <vkvg.h>


#ifdef _DRAW2d_VKVG_STATIC
#define CLASS_DECL_DRAW2d_VKVG
#elif defined(_draw2d_vkvg_project)
#define CLASS_DECL_DRAW2d_VKVG  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_DRAW2d_VKVG  CLASS_DECL_IMPORT
#endif


