// From draw2d_cairo/keep.cpp by
// camilo on 2025-07-14 09:57 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "keep.h"
#include "draw2d.h"
#include "acme/parallelization/synchronous_lock.h"


vkvg_keep::vkvg_keep(VkvgContext pgraphics, bool bSave)
{

   m_bSave = false;

   if (pgraphics == nullptr)
   {

      m_pdc = nullptr;

      return;

   }

   m_pdc = pgraphics;

   if (bSave)
   {

      save();

   }

}


vkvg_keep::~vkvg_keep()
{

   if (m_bSave)
   {

      restore();

   }

}



void vkvg_keep::save()
{

   if (m_pdc == nullptr)
      return;

   if (m_bSave)
      return;

   //_synchronous_lock synchronouslock(::draw2d_cairo::mutex());

   vkvg_save(m_pdc);

   m_bSave = true;

}


void vkvg_keep::restore()
{

   //_synchronous_lock ml(::draw2d_cairo::mutex());

   if (m_pdc == nullptr)
   {

      return;

   }

   if (!m_bSave)
   {

      return;

   }

   vkvg_restore(m_pdc);

   m_bSave = false;

}


void vkvg_keep::pulse()
{

   //_synchronous_lock ml(::draw2d_cairo::mutex());

   restore();

   save();

}

