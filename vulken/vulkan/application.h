// Created by camilo on 2024-07-27 <3ThomasBorregaardSorensen!!
#pragma once


namespace vulkan
{

   class application :
      virtual public ::particle
   {
   public:

      virtual void handleMouseMove(int x, int y);


      virtual void render_loop(const ::function < void(void *, int, int, int)> & callback);

   };



} // namespace vulkan


