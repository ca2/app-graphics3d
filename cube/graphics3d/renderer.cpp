// Created by camilo on 2025-05-19 22:05 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "context.h"
#include "renderer.h"


namespace graphics3d
{




   renderer::renderer()
   {

   }
   renderer::~renderer()
   {


   }


   void renderer::initialize_renderer(::graphics3d::context* pcontext)
   {


      m_pcontext = pcontext;

   }


   int renderer::width()
   {

      return 0;

   }

   int renderer::height()
   {

      return 0;

   }

   ::pointer < frame > renderer::beginFrame()
   {

      return m_pframe;


   }


   void renderer::on_begin_render(frame* pframe)
   {

   }

   void renderer::on_end_render(frame* pframe)
   {

   }

   void renderer::endFrame()
   {

   }


}  // namespace graphics3d


