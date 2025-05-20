// Created by camilo on 2025-05-19 21:49 <3ThomasBorregaardSorensen!!
#pragma once


namespace graphics3d
{


   class CLASS_DECL_CUBE renderer :
      virtual public ::particle
   {
   public:


      ::pointer < frame > m_pframe;
      ::pointer < context > m_pcontext;


      renderer();
      ~renderer();


      virtual void initialize_renderer(::graphics3d::context* pcontext);


      virtual int width();
      virtual int height();

      virtual ::pointer < frame > beginFrame();


      virtual void on_begin_render(frame* pframe);
      virtual void on_end_render(frame* pframe);

      virtual void endFrame();


      //virtual void on_layout(int cx, int cy);


   };


}  // namespace graphics3d


