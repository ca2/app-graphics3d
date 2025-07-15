// From draw2d_cairo/keep.h by
// camilo on 2025-07-14 09:56 <3ThomasBorregaardSørensen!!
#pragma once


class CLASS_DECL_DRAW2D_VKVG vkvg_keep
{
public:


   VkvgContext m_pdc;
   bool              m_bSave;


   vkvg_keep(VkvgContext pdc, bool bSave = true);
   ~vkvg_keep();


   void save();
   void restore();
   void pulse();


};


