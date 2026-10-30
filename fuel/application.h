#pragma once


#include "bred/platform/application.h"


namespace app_graphics3d_fuel
//  namespace app_graphics3d_fuel_window_opengl
{


   class CLASS_DECL_APP_GRAPHICS3D_FUEL application :
      virtual public ::bred::application
   {
   public:


      application();
      ~application() override;
      
      __DECLARE_APPLICATION_RELEASE_TIME();

      void init_instance() override;
      void term_instance() override;

      void on_request(::request * prequest) override;

#ifdef _DEBUG

      long long increment_reference_count() override;
      long long decrement_reference_count() override;

#endif
      
      string draw2d_get_default_implementation_name() override;


   };


} // namespace app_graphics3d_fuel



