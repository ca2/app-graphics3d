// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-17 03:22 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "context.h"
#include "app-cube/cube/impact.h"


namespace graphics3d
{


   context::context()
   {

   }


   context::~context()
   {

   }


   void context::initialize_context(::cube::impact * pimpact)
   {

      m_pimpact = pimpact;

   }


}  // namespace graphics3d


