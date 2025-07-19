// From draw2d_cairo/keep.h by
// camilo on 2025-07-14 09:56 <3ThomasBorregaardSørensen!!
#pragma once


class CLASS_DECL_DRAW2D_NANOVG nanovg_keep
{
public:


   NVGcontext * m_pdc;
   bool              m_bSave;


   nanovg_keep(NVGcontext* pdc, bool bSave = true);
   ~nanovg_keep();


   void save();
   void restore();
   void pulse();


};


