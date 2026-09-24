#include <cassert>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>


namespace
{


   std::string read(const std::filesystem::path & path)
   {

      std::ifstream stream(path, std::ios::binary);
      assert(stream);
      return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};

   }


   std::string section(
      const std::string & source,
      const std::string & beginMarker,
      const std::string & endMarker)
   {

      const auto begin = source.find(beginMarker);
      const auto end = source.find(endMarker, begin);
      assert(begin != std::string::npos);
      assert(end != std::string::npos);
      return source.substr(begin, end - begin);

   }


} // namespace


int main()
{

   const auto draw2dVkvg = std::filesystem::path(__FILE__).parent_path().parent_path();
   const auto header = read(draw2dVkvg / "graphics.h");
   const auto source = read(draw2dVkvg / "graphics.cpp");

   assert(header.find("void _draw_raw(const ::image::image_drawing & imagedrawing) override;") != std::string::npos);
   const auto imageDraw = section(source,
      "void graphics::_draw_raw(const ::image::image_drawing & imagedrawing)",
      "void graphics::_draw_raw(const ::f64_rectangle& rectangleTarget");
   assert(imageDraw.find("::gpu::graphics::_draw_raw") == std::string::npos);
   assert(imageDraw.find("vkvg_surface_create_source_for_VkhImage") != std::string::npos);
   assert(imageDraw.find("vkvg_set_source_surface_transform") != std::string::npos);
   assert(imageDraw.find("ptexture->wait_fence()") != std::string::npos);
   assert(imageDraw.find("ptexture->from_external_state") != std::string::npos);

   assert(header.find(
      "bool renders_layer_externally(::gpu::layer * pgpulayer) override;") !=
      std::string::npos);
   assert(header.find("class direct_target") != std::string::npos);
   assert(header.find("m_pdirecttargetActive") != std::string::npos);
   assert(header.find("prepare_vkvg_render_target(::gpu::texture * pgputexture, bool bMemoryImage = false)") !=
      std::string::npos);
   const auto acquire = section(source, "void graphics::on_acquire_memory_graphics(",
      "void graphics::on_release_memory_graphics()");
   assert(acquire.find("prepare_vkvg_render_target(texture, true)") != std::string::npos);
   const auto release = section(source, "void graphics::on_release_memory_graphics()",
      "void graphics::set_target_image(");
   assert(release.find("vkvg_flush(ctx)") < release.find("::gpu::graphics::on_release_memory_graphics()"));
   assert(release.find("texture->from_external_state") != std::string::npos);

   const auto targetSelection = section(
      source,
      "::gpu::texture_site* graphics::current_target_texture(",
      "bool graphics::is_gpu_oriented()");
   const auto composed = targetSelection.find(
      "pgpulayer->m_bIncludeInFrameComposition");
   const auto rendererTarget = targetSelection.find(
      "current_render_target_texture(pgpulayer)", composed);
   const auto privateSurface = targetSelection.find(
      "vkvg_surface_get_vk_image(m_vkvgsurface)", rendererTarget);
   assert(composed < rendererTarget && rendererTarget < privateSurface);

   const auto prepare = section(
      source,
      "void graphics::prepare_vkvg_render_target(",
      "void graphics::maintain_vkvg_direct_target_cache()");
   for (const auto * required : {
      "::gpu_vulkan::texture",
      "VK_FORMAT_B8G8R8A8_UNORM",
      "VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT",
      "VK_IMAGE_USAGE_SAMPLED_BIT",
      "auto sizeRaw = ptexture->raw_size();",
      "vkh_image_import",
      "vkh_image_create_view",
      "vkvg_surface_create_for_VkhImage",
      "beginSingleTimeCommands",
      "e_texture_state_color_attachment",
      "endSingleTimeCommands",
      "from_external_state",
      "m_pdirecttargetActive"})
   {

      assert(prepare.find(required) != std::string::npos);

   }

   const auto import = section(
      prepare,
      "vkhimage = vkh_image_import(",
      "if (!vkhimage");
   assert(import.find("(::u32)sizeRaw.cx") != std::string::npos);
   assert(import.find("(::u32)sizeRaw.cy") != std::string::npos);

   const auto activeClear = prepare.find("m_pdirecttargetActive.release();");
   const auto firstFalliblePreparation = prepare.find("if (!pgputexture)");
   const auto hitPublish = prepare.find(
      "m_pdirecttargetActive = pdirecttarget;", firstFalliblePreparation);
   const auto missCreate = prepare.find("vkvgcontext = vkvg_create", hitPublish);
   const auto missFailure = prepare.find("if (!vkvgcontext", missCreate);
   const auto missPublish = prepare.find(
      "m_pdirecttargetActive = pdirecttargetNew;", missFailure);
   assert(activeClear < firstFalliblePreparation &&
      firstFalliblePreparation < hitPublish &&
      hitPublish < missCreate && missCreate < missFailure &&
      missFailure < missPublish);

   const auto missBranch = section(
      prepare,
      "::cast<::gpu_vulkan::context> pcontextVulkan",
      "      maintain_vkvg_direct_target_cache();");
   const auto transactionalTry = missBranch.find("try");
   const auto transactionalCatch = missBranch.find("catch (...)" , transactionalTry);
   const auto destroyContextOnFailure = missBranch.find(
      "vkvg_destroy(vkvgcontext);", transactionalCatch);
   const auto destroySurfaceOnFailure = missBranch.find(
      "vkvg_surface_destroy(vkvgsurface);", destroyContextOnFailure);
   const auto destroyImageOnFailure = missBranch.find(
      "vkh_image_destroy(vkhimage);", destroySurfaceOnFailure);
   const auto cachePublish = missBranch.find(
      "m_directtargeta.add(pdirecttargetNew);", transactionalTry);
   const auto transferContext = missBranch.find(
      "pdirecttargetNew->m_vkvgcontext = vkvgcontext;", cachePublish);
   const auto publishComplete = missBranch.find(
      "bDirectTargetPublished = true;", transferContext);
   const auto unpublishedGuard = missBranch.find(
      "if (!bDirectTargetPublished)", transactionalCatch);
   assert(transactionalTry < cachePublish && cachePublish < transferContext &&
      transferContext < publishComplete && publishComplete < transactionalCatch &&
      transactionalCatch < unpublishedGuard &&
      unpublishedGuard < destroyContextOnFailure &&
      destroyContextOnFailure < destroySurfaceOnFailure &&
      destroySurfaceOnFailure < destroyImageOnFailure);

   const auto directStartTrace = prepare.find(
      "draw2d_vkvg direct start layer=");
   const auto directStartDebugGuard = prepare.rfind("#ifdef _DEBUG", directStartTrace);
   const auto directStartDebugEnd = prepare.find("#endif", directStartTrace);
   assert(directStartDebugGuard < directStartTrace &&
      directStartTrace < directStartDebugEnd);

   const auto cacheMaintenance = section(
      source,
      "void graphics::maintain_vkvg_direct_target_cache()",
      "void graphics::destroy_vkvg_direct_target(");
   const auto persistentLayers = cacheMaintenance.find("m_pgpulayera");
   const auto composedLayer = cacheMaintenance.find(
      "m_bIncludeInFrameComposition", persistentLayers);
   const auto layerAwareCapacity = cacheMaintenance.find(
      "iFrameCount * maximum(1, cComposedLayer) + 1", composedLayer);
   assert(persistentLayers < composedLayer && composedLayer < layerAwareCapacity);

   const auto startHook = section(
      source,
      "void graphics::on_start_layer_before_begin_render(",
      "} // namespace draw2d_vkvg");
   assert(startHook.find("prepare_vkvg_render_target") <
      startHook.find("vkvg_clear(vkvgcontext)"));

   const auto endLayer = section(
      source,
      "void graphics::end_layer(bool bClosingLayer)",
      "void graphics::on_present()");
   const auto flush = endLayer.find("vkvg_flush(vkvgcontext);");
   const auto state = endLayer.find("from_external_state(", flush);
   const auto genericEnd = endLayer.find(
      "::gpu::graphics::end_layer(bClosingLayer);", state);
   const auto unlock = endLayer.find("m_queuehostcalllock.unlock();", genericEnd);
   assert(flush < state && state < genericEnd && genericEnd < unlock);

   const auto directEndTrace = endLayer.find("draw2d_vkvg direct end layer=");
   const auto directEndDebugGuard = endLayer.rfind("#ifdef _DEBUG", directEndTrace);
   const auto directEndDebugEnd = endLayer.find("#endif", directEndTrace);
   assert(directEndDebugGuard < directEndTrace &&
      directEndTrace < directEndDebugEnd);

   const auto destroyTarget = section(
      source,
      "void graphics::destroy_vkvg_direct_target(",
      "void graphics::clear_vkvg_direct_target_cache()");
   assert(destroyTarget.find("vkvg_destroy") <
      destroyTarget.find("vkvg_surface_destroy"));
   assert(destroyTarget.find("vkvg_surface_destroy") <
      destroyTarget.find("vkh_image_destroy"));
   assert(destroyTarget.find("vkh_image_destroy") <
      destroyTarget.find("m_ptexture.release()"));

   assert(source.find(
      "return pgpulayer && pgpulayer->m_bIncludeInFrameComposition;") !=
      std::string::npos);
   assert(source.find("set_target_image(", source.find(
      "void graphics::prepare_vkvg_render_target(")) == std::string::npos);

   return 0;

}
