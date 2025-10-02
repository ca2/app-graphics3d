///TBS__<3tbs
#pragma once

//#pragma pack(show)


#include "gpu/_.h"
//#ifdef	_STDIO_H_
//#error "already included?!?! WHAT?!?! (At aqua:a)"
//#endif

//#undef PLATFORM_LAYER_NAME
//#define PLATFORM_LAYER_NAME gpu

#if defined(_graphics3d_project)
#define CLASS_DECL_GRAPHICS3D  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_GRAPHICS3D CLASS_DECL_IMPORT
#endif



namespace graphics3d
{


   class scene;


} // namespace graphics3d





