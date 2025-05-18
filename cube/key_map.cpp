// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-18 07:16 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "key_map.h"
#include "impact.h"


namespace cube
{



   ::user::enum_key_state key_map::key(::cube::enum_key ekey)
   {

      return m_pimpact->get_key_state(m_ekeya[ekey]);

   }


} // namespace cube


