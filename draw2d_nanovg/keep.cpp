// From draw2d_cairo/keep.cpp by
// camilo on 2025-07-14 09:57 <3ThomasBorregaardSørensen!!
#include "platform.h"
#include "gpu_opengl/_gpu_opengl.h"
#include "keep.h"
#include "draw2d.h"
#include "acme/parallelization/synchronous_lock.h"


nanovg_keep::nanovg_keep(NVGcontext* pdraw2dgraphics, bool bSave)
{

   m_bSave = false;

   if (pdraw2dgraphics == nullptr)
   {

      m_pdc = nullptr;

      return;

   }

   m_pdc = pdraw2dgraphics;

   if (bSave)
   {

      save();

   }

}


nanovg_keep::~nanovg_keep()
{

   if (m_bSave)
   {

      restore();

   }

}



void nanovg_keep::save()
{

   if (m_pdc == nullptr)
      return;

   if (m_bSave)
      return;

   //_synchronous_lock synchronouslock(::draw2d_cairo::mutex());

   nvgSave(m_pdc);

   m_bSave = true;

}


void nanovg_keep::restore()
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

   nvgRestore(m_pdc);

   m_bSave = false;

}


void nanovg_keep::pulse()
{

   //_synchronous_lock ml(::draw2d_cairo::mutex());

   restore();

   save();

}

