#pragma once


#include "aura/graphics/image/image.h"


namespace draw2d_directx12
{


   class CLASS_DECL_DRAW2D_DIRECTX12 image :
      virtual public ::image::image
   {
   public:


      HRESULT                             m_hrEndDraw;
      //comptr<ID2D1Bitmap1>                m_pbitmap1Map;

      image();
      ~image() override;



      //void copy_from(::image::image * pimage, enum_flag eflagCreate = e_flag_success) override;


      //virtual ::draw2d::graphics * _get_graphics() const override;
      ::draw2d::bitmap_pointer get_bitmap_as_target(::draw2d::graphics * pdraw2dgraphics = nullptr) const override;
      ::draw2d::bitmap_pointer get_bitmap_as_source(::draw2d::graphics * pdraw2dgraphics = nullptr) const override;
      ::draw2d::bitmap_pointer detach_bitmap() override;


      void dc_select(bool bSelect = true);


      //using ::image::image::create;
      //void create(const ::i32_size & size, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG, int iStride = -1, bool bPreserve = false) override;
      //using ::image::image::initialize;
      //void initialize(const ::i32_size & size, ::image32_t * pimage32, int iScan, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG) override;
      void create_from_graphics(::draw2d::graphics * pgraphics) override;
      void destroy() override;
      void destroy_os_data() override;


      //virtual void create_ex(const ::i32_size & size, ::image32_t * pimage32, int iScan, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG, int iStride = -1, bool bPreserve = false);

      //virtual bool _draw_raw(const ::i32_rectangle & rectangleTarget, ::image::image * pimage, const ::i32_point & pointSrc) override;


      virtual void set_image_icon(::image::icon * picon, int cx, int cy) override;


      virtual void defer_realize(::draw2d::graphics * pgraphics) const;
      //virtual bool realize(::draw2d::graphics * pgraphics) const;
      virtual void unrealize() const;
      virtual bool is_realized() const;

      //using ::image::image::blend;
      //virtual void blend(const ::i32_point & pointDst, ::image::image * pimageSrc, const ::i32_point & pointSrc, const ::i32_size & size, unsigned char bA) override;

      //using ::image::image::tint;
      //virtual void tint(::image::image * pimage, ::color::color color32) override;

      protected:
   
         
         ::image_pixmap_lease _map(const ::i32_rectangle & rectangle) override;
         void _unmap(::image_pixmap_lease * pimagepixmaplease) override;



   };


} // namespace draw2d_directx12



