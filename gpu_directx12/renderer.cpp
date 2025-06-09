#include "framework.h"
#include "approach.h"
#include "depth_stencil.h"
#include "descriptors.h"
#include "frame.h"
#include "renderer.h"
#include "texture.h"
#include "offscreen_render_target_view.h"
#include "physical_device.h"
#include "swap_chain_render_target_view.h"
#include "initializers.h"
#include "aura/graphics/gpu/cpu_buffer.h"
#include "gpu_directx12/shader.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/platform/application.h"
#include "aura/graphics/image/target.h"
#include "aura/user/user/interaction.h"
#include "aura/windowing/window.h"
//#include "tools.h"
//#include "aura/user/user/graphics3d.h"

//#include <array>
//#include <cassert>
//#include <stdexcept>
//

using namespace directx12;


namespace gpu_directx12
{

   //// Create vertex and index buffers
   //void create_quad_buffers(VkDevice device, VkPhysicalDevice physicalDevice,
   //   VkBuffer* vertexBuffer, VkDeviceMemory* vertexMemory,
   //   VkBuffer* indexBuffer, VkDeviceMemory* indexMemory, bool bYSwap);


   // Fragment shader (GLSL -> SPIR-V):
   // layout(set = 0, binding = 0) uniform sampler2D srcImage;
   // layout(location = 0) in vec2 fragUV;
   // layout(location = 0) out vec4 outColor;
   // void main() {
   //     outColor = texture(srcImage, fragUV);
   // }
   static unsigned int g_uaImageBlendFragmentShader[] = {
 #include "shader/image_blend.frag.spv.inl"
   };
   // 
   // Vertex shader (GLSL -> SPIR-V):
   // layout(location = 0) in vec2 inPos;
   // layout(location = 1) in vec2 inUV;
   // layout(location = 0) out vec2 fragUV;
   // void main() {
   //     fragUV = inUV;
   //     gl_Position = vec4(inPos, 0.0, 1.0);
   // }
   static unsigned int g_uaImageBlendVertexShader[] = {
 #include "shader/image_blend.vert.spv.inl"
   };


   static unsigned int g_uaResolveFragmentShader[] = {
 #include "shader/resolve.frag.spv.inl"
   };


   // renderer::renderer(VkWindow& window, context* pvkcdevice) : vkcWindow{ window }, m_pgpucontext{ pvkcdevice } 
   renderer::renderer()
   {
      m_pGlobalUBO = nullptr;
      m_pPushProperties = nullptr;
      m_hlsClear.m_dL = 0.75;
      m_hlsClear.m_dS = 0.5;
   }


   //int renderer::width()
   //{

   //   return m_prendertargetview->width();

   //}

   //int renderer::height()
   //{

   //   return m_prendertargetview->height();

   //}


   void renderer::initialize_renderer(::gpu::context* pgpucontext, ::gpu::enum_output eoutput, ::gpu::enum_scene escene)
   {

      ::gpu::renderer::initialize_renderer(pgpucontext, eoutput, escene);

      m_pgpucontext = pgpucontext;
      ::cast < ::gpu_directx12::device > pgpudevice = m_pgpucontext->m_pgpudevice;
      // Describe and create a constant buffer view (CBV) descriptor heap.
// Flags indicate that this descriptor heap can be bound to the pipeline 
// and that descriptors contained in it can be referenced by a root table.
      D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
      cbvHeapDesc.NumDescriptors = 1;
      cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
      cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
      HRESULT hrCreateDescriptorHeapCbv=pgpudevice->m_pdevice->CreateDescriptorHeap(&cbvHeapDesc, __interface_of(m_cbvHeap));
      pgpudevice->defer_throw_hresult(hrCreateDescriptorHeapCbv);

      const UINT constantBufferSize = ::directx12::Align256(64_KiB);    // CB size is required to be 256-byte aligned.
      //::cast < renderer > prenderer = m_pgpucontext->m_pgpurenderer;
      CD3DX12_HEAP_PROPERTIES heapproperties(D3D12_HEAP_TYPE_UPLOAD);
      auto resourcedesc = CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize);
      HRESULT hrCreateCommittedResource = pgpudevice->m_pdevice->CreateCommittedResource(
         &heapproperties,
         D3D12_HEAP_FLAG_NONE,
         &resourcedesc,
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         __interface_of(m_presourcePushProperties));
      pgpudevice->defer_throw_hresult(hrCreateCommittedResource);
      // Describe and create a constant buffer view.
      D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
      cbvDesc.BufferLocation = m_presourcePushProperties->GetGPUVirtualAddress();
      cbvDesc.SizeInBytes = constantBufferSize;
      auto cbvHeap = m_cbvHeap;
      pgpudevice->m_pdevice->CreateConstantBufferView(&cbvDesc, cbvHeap->GetCPUDescriptorHandleForHeapStart());
      CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
      auto hrMap = m_presourcePushProperties->Map(0, &readRange, reinterpret_cast<void**>(&m_pPushProperties));
      defer_throw_hresult(hrMap);



      if (eoutput == ::gpu::e_output_cpu_buffer)
      {

         //m_pimpact = pgpucontext->m_pimpact;

         pgpucontext->create_cpu_buffer(pgpucontext->rectangle().size());

         __construct_new(m_pcpubuffersampler);

         m_pcpubuffersampler->initialize_cpu_buffer_sampler(pgpucontext);

         m_pcpubuffersampler->m_prenderer = this;

      }

      //m_poffscreensampler->set_storing_flag

      //defer_layout();

      createCommandBuffers();

   }


   renderer::~renderer()
   {

      freeCommandBuffers();

   }


   renderer::command_buffer* renderer::getCurrentCommandBuffer2()
   {

      assert(isFrameStarted && "Cannot get command buffer when frame not in progress");

      //return m_commandbuffera[get_frame_index()];
      return m_pcommandbuffer;

   }


   int renderer::get_frame_index() const
   {

      assert(m_iFrameSerial2 >= 0
         && m_iCurrentFrame2 >= 0
         && m_estate != e_state_initial
         && "Cannot get frame index when frame not in progress");

      return (int)m_iCurrentFrame2;

   }


   void renderer::on_new_frame()
   {

      if (m_iCurrentFrame2 >= 0)
      {

         WaitForGpu();

         isFrameStarted = false;

      }

      m_iFrameSerial2++;

      m_iCurrentFrame2 = (m_iCurrentFrame2 + 1) % render_target_view::MAX_FRAMES_IN_FLIGHT;

      on_happening(e_happening_new_frame);

   }


   void renderer::restart_frame_counter()
   {

      m_iCurrentFrame2 = -1;
      m_iFrameSerial2 = -1;

      on_happening(e_happening_reset_frame_counter);

   }



   int renderer::get_frame_count() const
   {

      return ::gpu_directx12::render_target_view::MAX_FRAMES_IN_FLIGHT;

   }


   void renderer::on_context_resize()
   {

      //m_pgpucontext->m_rectangle = rectanglePlacement.size();

      //::gpu::renderer::set_placement(rectanglePlacement);

      //auto size = m_pimpact->size();

      defer_update_renderer();

      //auto extent = vkcWindow.getExtent();
      //while (extent.width == 0 || extent.height == 0) {
      //	glfwWaitEvents();
      //}
      //vkDeviceWaitIdle(m_pgpucontext->logicalDevice());

      //if (vkcSwapChain == nullptr) {
      //	vkcSwapChain = std::make_unique<swap_chain_render_target_view>(m_pgpucontext, extent);
      //}
      //else {
      //	::pointer<swap_chain_render_target_view> oldSwapChain = std::move(vkcSwapChain);
      //	vkcSwapChain = std::make_unique<swap_chain_render_target_view>(m_pgpucontext, extent, oldSwapChain);
      //	if (!oldSwapChain->compareSwapFormats(*vkcSwapChain.get())) {
      //		throw ::exception(error_failed, "Swap chain image(or depth) format has changed!");
      //	}

      //}
   }


   void renderer::defer_update_renderer()
   {

      if (m_sizeRenderer.width() == m_pgpucontext->rectangle().width()
         && m_sizeRenderer.height() == m_pgpucontext->rectangle().height())
      {

         return;

      }

      m_bNeedToRecreateSwapChain = true;

      auto size = m_pgpucontext->rectangle().size();

      m_sizeRenderer = size;

      auto eoutput = m_eoutput;

      if (eoutput == ::gpu::e_output_cpu_buffer
         || eoutput == ::gpu::e_output_gpu_buffer)
      {

         auto poffscreenrendertargetview = __allocate offscreen_render_target_view(this, size, m_prendertargetview);
         //#ifdef WINDOWS_DESKTOP
         //         poffscreenrendertargetview->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
         //#else
         //         poffscreenrendertargetview->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
         //#endif
         m_prendertargetview = poffscreenrendertargetview;
         //         //m_prendererResolve.release();
         //
      }
      else if (eoutput == ::gpu::e_output_swap_chain)
      {

         m_prendertargetview = __allocate swap_chain_render_target_view(this, size, m_prendertargetview);
         //m_prendererResolve.release();

      }
      //      else if (eoutput == ::gpu::e_output_gpu_buffer)
      //      {
      //
      //         auto poffscreenrendertargetview = __allocate offscreen_render_target_view(this, m_extentRenderer, m_prendertargetview);
      //#ifdef WINDOWS_DESKTOP
      //         poffscreenrendertargetview->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
      //#else
      //         poffscreenrendertargetview->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
      //#endif
      //         m_prendertargetview = poffscreenrendertargetview;
      //         //m_prendererResolve;
      //
      //      }
      //      else if (eoutput == ::gpu::e_output_color_and_alpha_accumulation_buffers)
      //      {
      //
      //         auto paccumulationrendertargetview = __allocate accumulation_render_target_view(this, m_extentRenderer, m_prendertargetview);
      //         paccumulationrendertargetview->m_formatImage = VK_FORMAT_R32G32B32A32_SFLOAT;
      //         paccumulationrendertargetview->m_formatAlphaAccumulation = VK_FORMAT_R32_SFLOAT;
      //         m_prendertargetview = paccumulationrendertargetview;
      //
      //         //__construct_new(m_prendererResolve);
      //
      //         //m_prendererResolve->initialize_renderer(m_pgpucontext, ::gpu::e_output_resolve_color_and_alpha_accumulation_buffers);
      //
      //         //m_prendererResolve->set_placement(m_pgpucontext->rectangle);
      //         //
      //         //            auto poffscreenrendertargetview = __allocate offscreen_render_target_view(m_pgpucontext, m_extentRenderer, m_prendertargetviewResolve);
      //         //#ifdef WINDOWS_DESKTOP
      //         //            poffscreenrendertargetview->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
      //         //#else
      //         //            poffscreenrendertargetview->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
      //         //#endif
      //         //            m_prendertargetviewResolve = poffscreenrendertargetview;
      //      }
      //      else if (eoutput == ::gpu::e_output_resolve_color_and_alpha_accumulation_buffers)
      //      {
      //
      //         auto poffscreenrendertargetview = __allocate offscreen_render_target_view(this, m_extentRenderer, m_prendertargetview);
      //#ifdef WINDOWS_DESKTOP
      //         poffscreenrendertargetview->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
      //#else
      //         poffscreenrendertargetview->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
      //#endif
      //         m_prendertargetview = poffscreenrendertargetview;
      //
      //      }
      //      else
      //      {
      //
      //         throw ::exception(error_wrong_state, "Unexpected gpu e_output");
      //
      //      }
      //
      if (!m_prendertargetview->has_ok_flag() && m_sizeRenderer.area() > 0)
      {

         m_prendertargetview->init();

      }

      //if (m_prendererResolve)
      //{

      //	if (m_prendererResolve->m_prendertargetview->m_images.is_empty())
      //	{

      //		m_prendererResolve->defer_update_render_target_view();

      //	}

      //}

      m_bNeedToRecreateSwapChain = false;

   }


   void renderer::createCommandBuffers()
   {

      ::cast < ::gpu_directx12::device > pdevice = m_pgpucontext->m_pgpudevice;

      D3D12_COMMAND_QUEUE_DESC descCopyQueue = {};
      descCopyQueue.Type = D3D12_COMMAND_LIST_TYPE_COPY;
      pdevice->m_pdevice->CreateCommandQueue(&descCopyQueue, __interface_of(m_pcommandqueueCopy));

      // 2. Create command queue
      D3D12_COMMAND_QUEUE_DESC queueDesc = {};
      queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
      pdevice->m_pdevice->CreateCommandQueue(&queueDesc, __interface_of(m_pcommandqueue));

      //m_commandbuffera.set_size(get_frame_count());

      //for (int iFrame = 0; iFrame < m_commandbuffera.size(); iFrame++)
      for (int iFrame = 0; iFrame < 1; iFrame++)
      {

         //auto& pcommandbuffer = m_commandbuffera[iFrame];
         auto& pcommandbuffer = m_pcommandbuffer;

         if (__defer_construct_new(pcommandbuffer))
         {

            pcommandbuffer->initialize_command_buffer(D3D12_COMMAND_LIST_TYPE_DIRECT, this, m_pcommandqueue);

         }

      }


      //commandBuffers.resize(render_target_view::MAX_FRAMES_IN_FLIGHT);

      //VkCommandBufferAllocateInfo allocInfo{};
      //allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      //allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      //allocInfo.commandPool = m_pgpucontext->m_pgpudevice->getCommandPool();
      //allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

      //if (vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &allocInfo, commandBuffers.data()) !=
      //   VK_SUCCESS) {
      //   throw ::exception(error_failed, "failed to allocate command buffers!");
      //}

   }


   renderer::command_buffer::command_buffer()
   {

      m_fenceValue = 0;

      m_hFenceEvent = nullptr;

   }


   renderer::command_buffer::~command_buffer()
   {

      if (m_hFenceEvent)
      {

         ::CloseHandle(m_hFenceEvent);

         m_hFenceEvent = nullptr;

      }

   }


   void renderer::command_buffer::initialize_command_buffer(D3D12_COMMAND_LIST_TYPE ecommandlisttype, ::gpu_directx12::renderer* prenderer, ID3D12CommandQueue* pcommandqueue)
   {

      m_prenderer = prenderer;

      m_pcommandqueue = pcommandqueue;

      m_ecommandlisttype = ecommandlisttype;

      // D3D12_COMMAND_LIST_TYPE_DIRECT
      // D3D12_COMMAND_LIST_TYPE_DIRECT

      ::cast<gpu_directx12::device> pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      HRESULT hr = pdevice->m_pdevice->CreateCommandAllocator(
         m_ecommandlisttype,  // Type: DIRECT for graphics
         __interface_of(m_pcommandallocator)
      );

      ::defer_throw_hresult(hr);

      //auto& pcommandlist = m_framea.element_at_grow(iFrame);
      // 4. Create command list (can be reused)
      ::defer_throw_hresult(pdevice->m_pdevice->CreateCommandList(
         0,
         m_ecommandlisttype,
         m_pcommandallocator, // initial allocator
         nullptr, // No PSO yet
         __interface_of(m_pcommandlist)
      ));

      ::defer_throw_hresult(m_pcommandlist->Close()); // Must be closed before Reset()

      HRESULT hrCreateFeence =
         pdevice->m_pdevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
            __interface_of(m_pfence));

      ::defer_throw_hresult(hrCreateFeence);

      // 5. Create fence + event for GPU sync
      m_hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

   }


   //::pointer <renderer::command_buffer >renderer::beginSingleTimeCommands()
   //{

   //   ::pointer <renderer::command_buffer > pcommandbuffer;

   //   __defer_construct_new(pcommandbuffer);

   //   ::cast < device > pdevice= m_pgpucontext->m_pgpudevice;

   //   pcommandbuffer->initialize_command_buffer(pdevice);

   //   return pcommandbuffer;

   //}


   void renderer::command_buffer::submit_command_buffer()
   {

      ::cast < ::gpu_directx12::device > pdevice = m_prenderer->m_pgpucontext->m_pgpudevice;

      HRESULT hrCloseCommandList = m_pcommandlist->Close();

      pdevice->defer_throw_hresult(hrCloseCommandList);

      ID3D12CommandList* ppCommandLists[] = { m_pcommandlist };

      m_pcommandqueue->ExecuteCommandLists(1, ppCommandLists);

      UINT64 uploadFenceValue = ++m_fenceValue;

      HRESULT hrSignalCommandQueue = m_pcommandqueue->Signal(m_pfence, uploadFenceValue);

      pdevice->defer_throw_hresult(hrSignalCommandQueue);

   }


   void renderer::command_buffer::wait_for_gpu()
   {

      //const UINT64 fenceValue = ++m_fenceValue;

      //prenderer->m_pcommandqueue->Signal(m_pfence, m_fenceValue);

      if (m_pfence->GetCompletedValue() < m_fenceValue)
      {

         m_pfence->SetEventOnCompletion(m_fenceValue, m_hFenceEvent);

         ::WaitForSingleObject(m_hFenceEvent, INFINITE);

      }

      //m_iCurrentFrame2 = (m_iCurrentFrame2 + 1) % get_frame_count();


   }


   void renderer::command_buffer::reset()
   {

      auto pcommandallocator = m_pcommandallocator;

      HRESULT hrResetCommandAllocator = pcommandallocator->Reset();

      ::defer_throw_hresult(hrResetCommandAllocator);

      m_pcommandlist->Reset(pcommandallocator, nullptr);

   }


   bool renderer::command_buffer::has_finished()
   {

      return m_pfence->GetCompletedValue() >= m_fenceValue;

   }


   // Sync CPU to GPU so we can reuse this frame's allocator
   void renderer::WaitForGpu()
   {

      auto pcommandbuffer = getCurrentCommandBuffer2();

      pcommandbuffer->wait_for_gpu();

   }


   renderer::command_buffer* renderer::getLoadAssetsCommandBuffer()
   {

      if (!m_pcommandbufferLoadAssets)
      {

         __defer_construct_new(m_pcommandbufferLoadAssets);

         ::cast<gpu_directx12::device> pdevice = m_pgpucontext->m_pgpudevice;

         m_pcommandbufferLoadAssets->initialize_command_buffer(D3D12_COMMAND_LIST_TYPE_COPY, this, m_pcommandqueueCopy);

         m_pcommandbufferLoadAssets->reset();

      }

      return m_pcommandbufferLoadAssets;

   }


   ::pointer <renderer::command_buffer >renderer::beginSingleTimeCommands(D3D12_COMMAND_LIST_TYPE ecommandlisttype)
   {

      ::pointer <renderer::command_buffer > pcommandbuffer;

      __defer_construct_new(pcommandbuffer);

      ::cast<gpu_directx12::device> pdevice = m_pgpucontext->m_pgpudevice;

      if (ecommandlisttype == D3D12_COMMAND_LIST_TYPE_COPY)
      {

         pcommandbuffer->initialize_command_buffer(D3D12_COMMAND_LIST_TYPE_COPY, this, m_pcommandqueueCopy);

      }
      else
      {

         pcommandbuffer->initialize_command_buffer(D3D12_COMMAND_LIST_TYPE_DIRECT, this, m_pcommandqueue);

      }

      pcommandbuffer->reset();

      return pcommandbuffer;

   }


   void renderer::endSingleTimeCommands(command_buffer* pcommandbuffer)
   {

      pcommandbuffer->submit_command_buffer();

      pcommandbuffer->wait_for_gpu();

   }


   void renderer::freeCommandBuffers()
   {

      //vkFreeCommandBuffers(
      //   m_pgpucontext->logicalDevice(),
      //   m_pgpucontext->m_pgpudevice->getCommandPool(),
      //   (uint32_t)commandBuffers.size(),
      //   commandBuffers.data());
      //commandBuffers.clear();

   }


   void renderer::on_begin_draw()
   {

      if (m_pgpucontext->m_rectangle.is_empty())
      {

         throw ::exception(error_wrong_state, "please call set size before at least once with no empty preferrably good initial size");

         //set_size(size);

      }

      assert(!isFrameStarted && "Can't call beginFrame while already in progress");

      //if (m_bOffScreen)
      {

         auto result = m_prendertargetview->acquireNextImage();

         //if (result == VK_ERROR_OUT_OF_DATE_KHR
         //   || m_prendertargetview->m_bNeedRebuild)
         //{
         //   vkDeviceWaitIdle(m_pgpucontext->logicalDevice());
         //   m_prendertargetview->init();
         //   //set_placement(size);
         //   //throw ::exception(todo, "resize?!?!");
         //   //return nullptr;
         //   return;
         //}
         //if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
         //   throw ::exception(error_failed, "Failed to aquire swap chain image");
         //}

         

         //auto commandBuffer = getCurrentCommandBuffer();

         //VkCommandBufferBeginInfo beginInfo{};
         //beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

         //if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
         //   throw ::exception(error_failed, "failed to begin recording command buffer!");
         //}

         //m_
         //auto pframe = __create_new < frame >();
         //pframe->commandBuffer = commandBuffer;
         //m_pframe = pframe;
         //return m_pframe;

      }
      //else
      //{


      //	auto result = m_pvkcswapchain->acquireNextImage(&currentImageIndex);

      //	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      //		recreateRenderPass();
      //		return nullptr;
      //	}
      //	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      //		throw ::exception(error_failed, "Failed to aquire swap chain image");
      //	}

      //	isFrameStarted = true;

      //	auto commandBuffer = getCurrentCommandBuffer();

      //	VkCommandBufferBeginInfo beginInfo{};
      //	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      //	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      //		throw ::exception(error_failed, "failed to begin recording command buffer!");
      //	}
      //	return commandBuffer;

      //}

      _on_begin_render();


      auto pcommandbuffer = getCurrentCommandBuffer2();

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      //::cast < ::gpu_directx12::renderer > prenderer = m_pgpurenderer;

      //if (prenderer)
      {

         auto pgpurendertargetview = m_prendertargetview;

         if (pgpurendertargetview)
         {

            auto presourceTexture = pgpurendertargetview->current_texture()->m_presource;

            if (presourceTexture)
            {

               CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
                  pgpurendertargetview->m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
                  get_frame_index(),
                  pgpurendertargetview->m_rtvDescriptorSize);
               CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(pgpurendertargetview->m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

               pcommandlist->OMSetRenderTargets(
                  1,
                  &rtvHandle,
                  true,
                  &dsvHandle);


               // 1. Define viewport and scissor rectangle
               D3D12_VIEWPORT viewport = {};
               viewport.TopLeftX = 0.0f;
               viewport.TopLeftY = 0.0f;
               viewport.Width = static_cast<float>(m_pgpucontext->m_rectangle.width());
               viewport.Height = static_cast<float>(m_pgpucontext->m_rectangle.height());
               viewport.MinDepth = 0.0f;
               viewport.MaxDepth = 1.0f;

               D3D12_RECT scissorRect = {};
               scissorRect.left = 0;
               scissorRect.top = 0;
               scissorRect.right = m_pgpucontext->m_rectangle.width();
               scissorRect.bottom = m_pgpucontext->m_rectangle.height();

               //// 2. Begin command recording
               //commandAllocator->Reset();
               //pcommandlist->Reset(commandAllocator.Get(), pipelineState.Get());

               //// 3. Set the pipeline and root signature
               //pcommandlist->SetPipelineState(pipelineState.Get());
               //pcommandlist->SetGraphicsRootSignature(rootSignature.Get());

               //// 4. Set the viewport and scissor
               //commandList->RSSetViewports(1, &viewport);
               //commandList->RSSetScissorRects(1, &scissorRect);
               ////D3D11_VIEWPORT vp = {};
               ////vp.TopLeftX = 0;
               ////vp.TopLeftY = 0;
               ////vp.Width = static_cast<float>(m_rectangle.width());
               ////vp.Height = static_cast<float>(m_rectangle.height());
               ////vp.MinDepth = 0.0f;
               ////vp.MaxDepth = 1.0f;

               ////m_pcontext->RSSetViewports(1, &vp);

            }

            auto presourceDepthStencilBuffer = pgpurendertargetview->current_depth_stencil()->m_presource;

            if (presourceDepthStencilBuffer)
            {

               //m_pcontext->OMSetDepthStencilState(pdepthstencilstate, 0);
               CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
                  pgpurendertargetview->m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
                  get_frame_index(),
                  pgpurendertargetview->m_rtvDescriptorSize);
               CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(pgpurendertargetview->m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

               // You set the RTV and DSV like this:
               pcommandlist->OMSetRenderTargets(
                  1,                    // One render target
                  &rtvHandle,           // D3D12_CPU_DESCRIPTOR_HANDLE to your RTV
                  FALSE,                // Not using RTV arrays
                  &dsvHandle            // D3D12_CPU_DESCRIPTOR_HANDLE to your DSV (can be null)
               );
               //m_pcontext->ClearDepthStencilView(pdepthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);

               pcommandlist->ClearDepthStencilView(
                  pgpurendertargetview->m_dsvHeap->GetCPUDescriptorHandleForHeapStart(),
                  D3D12_CLEAR_FLAG_DEPTH,
                  1.0f, 0,
                  0, nullptr
               );

            }

            ::cast < offscreen_render_target_view > poffscreenrendertargetview = pgpurendertargetview;

            if (poffscreenrendertargetview)
            {

               //auto psamplerstate = poffscreenrendertargetview->m_psamplerstate;

               //if (psamplerstate)
               {

                  //m_pcontext->PSSetSamplers(0, 1, psamplerstate.pp());

               }

            }


         }

      }


      //if (!m_prasterizerstate)
      //{

      //   // 1. Define rasterizer state descriptor
      //   D3D11_RASTERIZER_DESC rasterizerDesc = {};
      //   rasterizerDesc.FillMode = D3D11_FILL_SOLID;
      //   rasterizerDesc.CullMode = D3D11_CULL_BACK;        // Cull back faces
      //   rasterizerDesc.FrontCounterClockwise = false; // Treat CCW as front-facing
      //   rasterizerDesc.DepthClipEnable = TRUE;

      //   // 2. Create rasterizer state object
      //   //ID3D11RasterizerState* pRasterizerState = nullptr;
      //   HRESULT hr = m_pgpudevice->m_pdevice->CreateRasterizerState(&rasterizerDesc, &m_prasterizerstate);
      //   if (FAILED(hr)) {
      //      // Handle error (e.g., log or exit)
      //      throw ::hresult_exception(hr);
      //   }

      //   // 3. Set rasterizer state on the device context
      //   

      //}

      //m_pcontext->RSSetState(m_prasterizerstate);

   }


   renderer::cpu_buffer_sampler::cpu_buffer_sampler()
   {

      clear();

   }


   renderer::cpu_buffer_sampler::~cpu_buffer_sampler()
   {

      destroy();

   }


   void renderer::cpu_buffer_sampler::initialize_cpu_buffer_sampler(::gpu::context* pgpucontext)
   {

      m_pgpucontext = pgpucontext;

   }


   void renderer::cpu_buffer_sampler::clear()
   {

      //m_vkextent2d.width = 0;
      //m_vkextent2d.height = 0;
      //m_vkdevicememory = nullptr;
      //m_vkimage = nullptr;

   }


   //void renderer::cpu_buffer_sampler::update(VkExtent2D vkextent2d)
   //{

   //   //if (vkextent2d.width == m_vkextent2d.width
   //   //   && vkextent2d.height == m_vkextent2d.height)
   //   //{

   //   //   return;

   //   //}

   //   //destroy();

   //   //if (vkextent2d.width <= 0 ||
   //   //   vkextent2d.height <= 0)
   //   //{

   //   //   return;

   //   //}

   //   //m_vkextent2d.width = vkextent2d.width;
   //   //m_vkextent2d.height = vkextent2d.height;

   //   //VkImageCreateInfo imgCreateInfo(initializers::imageCreateInfo());
   //   //imgCreateInfo.imageType = VK_IMAGE_TYPE_2D;
   //   //imgCreateInfo.format = VK_FORMAT_B8G8R8A8_UNORM;
   //   //imgCreateInfo.extent.width = m_vkextent2d.width;
   //   //imgCreateInfo.extent.height = m_vkextent2d.height;
   //   //imgCreateInfo.extent.depth = 1;
   //   //imgCreateInfo.arrayLayers = 1;
   //   //imgCreateInfo.mipLevels = 1;
   //   //imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
   //   //imgCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
   //   //imgCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
   //   //imgCreateInfo.usage =
   //   //   VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
   //   //   VK_IMAGE_USAGE_SAMPLED_BIT;
   //   //// Create the image
   //   ////VkImage dstImage;
   //   //VK_CHECK_RESULT(vkCreateImage(m_pgpucontext->logicalDevice(), &imgCreateInfo, nullptr, &m_vkimage));
   //   //// Create memory to back up the image
   //   //VkMemoryRequirements memRequirements;
   //   //VkMemoryAllocateInfo memAllocInfo(initializers::memoryAllocateInfo());
   //   //vkGetImageMemoryRequirements(m_pgpucontext->logicalDevice(), m_vkimage, &memRequirements);
   //   //memAllocInfo.allocationSize = memRequirements.size;
   //   //// Memory must be host visible to copy from
   //   //memAllocInfo.memoryTypeIndex = m_pgpucontext->m_pgpudevice->m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
   //   //VK_CHECK_RESULT(vkAllocateMemory(m_pgpucontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
   //   //VK_CHECK_RESULT(vkBindImageMemory(m_pgpucontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));

   //   //if (1)
   //   //{
   //   //   auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

   //   //   VkImageMemoryBarrier barrier = {
   //   //      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //      .srcAccessMask = 0,
   //   //      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //      .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
   //   //      .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      .image = m_vkimage,
   //   //      .subresourceRange = {
   //   //         .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //         .baseMipLevel = 0,
   //   //         .levelCount = 1,
   //   //         .baseArrayLayer = 0,
   //   //         .layerCount = 1
   //   //      },
   //   //   };

   //   //   vkCmdPipelineBarrier(
   //   //      cmdBuffer,
   //   //      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
   //   //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //   //      0,
   //   //      0, NULL,
   //   //      0, NULL,
   //   //      1, &barrier
   //   //   );
   //   //   m_pgpucontext->endSingleTimeCommands(cmdBuffer);
   //   //}


   //}


   void renderer::cpu_buffer_sampler::destroy()
   {

      //if (m_vkdevicememory)
      //{

      //   vkFreeMemory(m_pgpucontext->logicalDevice(), m_vkdevicememory, nullptr);
      //   vkDestroyImage(m_pgpucontext->logicalDevice(), m_vkimage, nullptr);

      //   clear();

      //}

   }


   void renderer::cpu_buffer_sampler::sample(texture* ptexture)
   {

      //D3D11_TEXTURE2D_DESC desc;

      m_desc = ptexture->m_presource->GetDesc();
      //EnsureStagingTextureMatches(pTexture, desc);


      ::cast < ::gpu_directx12::context > pcontext = m_pgpucontext;
      ::cast<gpu_directx12::device> pdevice = m_pgpucontext->m_pgpudevice;
      ::cast < ::gpu_directx12::renderer > prenderer = pcontext->m_pgpurenderer;

      ::pointer <renderer::command_buffer > pcommandbufferBarrier;

      auto pcommandbuffer = prenderer->getCurrentCommandBuffer2();

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      ptexture->_new_state(pcommandlist, D3D12_RESOURCE_STATE_COPY_SOURCE);

      if (m_desc.Width != m_size.width() || m_desc.Height != m_size.height())
      {

         m_presourceStagingTexture.Release();

         if (m_desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM)
         {
            //printf("Unsupported format for readback.\n");
            throw ::exception(error_failed, "Unsupported format");
         }

         auto rowPitch = ((m_desc.Width * 4 /*bytes per pixel*/) + 255) & ~255; // must align to 256 bytes
         auto bufferSize = rowPitch * m_desc.Height;

         D3D12_HEAP_PROPERTIES heapProps = {};
         heapProps.Type = D3D12_HEAP_TYPE_READBACK;

         D3D12_RESOURCE_DESC bufferDesc = {};
         bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
         bufferDesc.Width = bufferSize;
         bufferDesc.Height = 1;
         bufferDesc.DepthOrArraySize = 1;
         bufferDesc.MipLevels = 1;
         bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
         bufferDesc.SampleDesc.Count = 1;
         bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

         HRESULT hr = pdevice->m_pdevice->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &bufferDesc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            __interface_of(m_presourceStagingTexture));

         pdevice->defer_throw_hresult(hr);

         m_size.cx() = (int)m_desc.Width;

         m_size.cy() = (int)m_desc.Height;

      }

      //pcontext->m_pcontext->Flush();

      //pcontext->m_pcontext->CopyResource((ID3D11Resource*)m_ptextureStaging, (ID3D11Resource*)ptexture);

      //prenderer->WaitForGpu();

      //// Command allocator + list (reuse if created earlier)
      //::comptr < ID3D12CommandAllocator > pcommandallocatorQuick;
      //pdevice->defer_throw_hresult(pdevice->m_pdevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __interface_of(pcommandallocatorQuick)));
      //::comptr < ID3D12GraphicsCommandList > pcommandlistQuick;
      //pdevice->defer_throw_hresult(pdevice->m_pdevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pcommandallocatorQuick, nullptr, __interface_of(pcommandlistQuick)));

      //pdevice->defer_throw_hresult(pcommandlistQuick->Close());

      //pdevice->defer_throw_hresult(pcommandallocatorQuick->Reset());

      //pdevice->defer_throw_hresult(pcommandlistQuick->Reset(pcommandallocatorQuick, nullptr));

      // Setup footprint
      UINT64 totalBytes = 0;

      m_pgpucontext->m_pgpudevice->m_pdevice->GetCopyableFootprints(
         &m_desc,
         0, 1, 0,
         &m_footprint, nullptr, nullptr, &totalBytes);


      //// Make compute queue wait for graphics
      //UINT64 graphicsSignalValue = ++graphicsFenceValue;
      //graphicsQueue->Signal(graphicsFence.Get(), graphicsSignalValue);
      //computeQueue->Wait(graphicsFence.Get(), graphicsSignalValue);

      //// Execute compute work
      //computeQueue->ExecuteCommandLists(1, ppComputeLists);



      auto location1 = CD3DX12_TEXTURE_COPY_LOCATION(m_presourceStagingTexture, m_footprint);

      auto location2 = CD3DX12_TEXTURE_COPY_LOCATION(ptexture->m_presource, 0);
      // Copy to staging buffer
      pcommandlist->CopyTextureRegion(
         &location1,
         0, 0, 0,
         &location2,
         nullptr
      );


      //auto  pcommandlist = pframe->getCurrentCommandList();

      //prenderer->endSingleTimeCommands(pcommandbufferSingleTime);


      //HRESULT hrCloseCommandList = pcommandlistQuick->Close();

      //::defer_throw_hresult(hrCloseCommandList);

      //// 4. Execute the command list
      //ID3D12CommandList* ppCommandLists[] = { pcommandlistQuick };
      //prenderer->m_pcommandqueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);


      //stagingTexture->lpVtbl->Release(stagingTexture);

      //return rgbaData;

  //    if (!m_vkimage || !m_vkdevicememory)
  //    {

  //       return;

  //    }


  //    // Create the linear tiled destination image to copy to and to read the memory from

  //// Do the actual blit from the offscreen image to our host visible destination image
  //    VkCommandBufferAllocateInfo cmdBufAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->m_pgpudevice->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
  //    VkCommandBuffer copyCmd;
  //    VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &cmdBufAllocateInfo, &copyCmd));
  //    VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
  //    VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

  //    // Transition source image to transfer destination layout
  //    insertImageMemoryBarrier(
  //       copyCmd,
  //       vkimage,
  //       0,
  //       VK_ACCESS_TRANSFER_WRITE_BIT,
  //       VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  //       VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
  //       VK_PIPELINE_STAGE_TRANSFER_BIT,
  //       VK_PIPELINE_STAGE_TRANSFER_BIT,
  //       VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

  //    // Transition destination image to transfer destination layout
  //    insertImageMemoryBarrier(
  //       copyCmd,
  //       m_vkimage,
  //       0,
  //       VK_ACCESS_TRANSFER_WRITE_BIT,
  //       VK_IMAGE_LAYOUT_UNDEFINED,
  //       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
  //       VK_PIPELINE_STAGE_TRANSFER_BIT,
  //       VK_PIPELINE_STAGE_TRANSFER_BIT,
  //       VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

  //    // colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, and does not need to be transitioned

  //    VkImageCopy imageCopyRegion{};
  //    imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  //    imageCopyRegion.srcSubresource.layerCount = 1;
  //    imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  //    imageCopyRegion.dstSubresource.layerCount = 1;
  //    imageCopyRegion.extent.width = m_vkextent2d.width;
  //    imageCopyRegion.extent.height = m_vkextent2d.height;
  //    imageCopyRegion.extent.depth = 1;

  //    vkCmdCopyImage(
  //       copyCmd,
  //       //colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
  //       vkimage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
  //       m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
  //       1,
  //       &imageCopyRegion);

  //    // Transition destination image to general layout, which is the required layout for mapping the image memory later on
  //    insertImageMemoryBarrier(
  //       copyCmd,
  //       m_vkimage,
  //       VK_ACCESS_TRANSFER_WRITE_BIT,
  //       VK_ACCESS_MEMORY_READ_BIT,
  //       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
  //       VK_IMAGE_LAYOUT_GENERAL,
  //       VK_PIPELINE_STAGE_TRANSFER_BIT,
  //       VK_PIPELINE_STAGE_TRANSFER_BIT,
  //       VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

  //    VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

  //    ::cast < offscreen_render_target_view > ppass = m_prenderer->m_prendertargetview;

  //    ppass->submitSamplingWork(copyCmd);

  //    vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

  //    vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_pgpudevice->getCommandPool(), 1, &copyCmd);


   }
   void GetTextureSizeInfo(ID3D12Device* device, ID3D12Resource* texture,
      UINT& width, UINT& height, UINT& rowPitch)
   {
      auto desc = texture->GetDesc();
      width = static_cast<UINT>(desc.Width);
      height = desc.Height;

      D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
      UINT numRows;
      UINT64 rowSize;
      UINT64 totalBytes;

      device->GetCopyableFootprints(
         &desc, 0, 1, 0,
         &layout, &numRows, &rowSize, &totalBytes
      );

      rowPitch = layout.Footprint.RowPitch;
   }


   void renderer::cpu_buffer_sampler::send_sample()
   {

      void* data = nullptr;
      m_presourceStagingTexture->Map(0, nullptr, &data);
      ::cast < ::gpu_directx12::context > pcontext = m_pgpucontext;
      ::cast<gpu_directx12::device> pdevice = m_pgpucontext->m_pgpudevice;
      ::cast < ::gpu_directx12::renderer > prenderer = pcontext->m_pgpurenderer;
      auto prendertargetview = prenderer->m_prendertargetview;
      ::cast < offscreen_render_target_view > poffscreenrendertargetview = prendertargetview;
      ID3D12Resource* presourceOffscreenTexture = poffscreenrendertargetview->current_texture()->m_presource;


      //m_pcpubuffersampler->sample(poffscreenrendertargetview->current_texture());

      // You can now read `m_size.cy()` rows, each of aligned pitch `footprint.Footprint.RowPitch`
      UINT w = 0;
      UINT h = 0;
      UINT s = 0;
      GetTextureSizeInfo(pdevice->m_pdevice, presourceOffscreenTexture, w, h, s);
      auto pcpubuffer = m_pgpucontext->m_pcpubuffer;

      if (pcpubuffer && w > 0 && h > 0 && s >0)
      {

         auto pimagetarget = pcpubuffer->m_pimagetarget;

         if (pimagetarget)
         {

            //auto size = m_pgpucontext->m_rectangle.size();

            pimagetarget->set_image_pixels(
               (const ::image32_t *) data,
               w,
               h,
               s,
               true);

         }

      }

      //if()
      //// Example:
      //uint8_t* row = reinterpret_cast<uint8_t*>(data);
      //for (int y = 0; y < m_size.cy(); ++y)
      //{
      //   auto pixel = reinterpret_cast<uint32_t*>(row);
      //   for (int x = 0; x < m_size.cx(); ++x)
      //   {
      //      uint32_t rgba = pixel[x];
      //      // process pixel
      //   }
      //   row += m_footprint.Footprint.RowPitch;
      //}

      m_presourceStagingTexture->Unmap(0, nullptr);
      //}


   //   D3D11_MAPPED_SUBRESOURCE mapped;

   //   ::cast < ::gpu_directx12::context > pcontext = m_pgpucontext;

   //   D3D11_TEXTURE2D_DESC desc;

   //   m_ptextureStaging->GetDesc(&desc);

   //   if (FAILED(pcontext->m_pcontext->Map((ID3D11Resource*)m_ptextureStaging, 0, D3D11_MAP_READ, 0, &mapped)))
   //   {
   //      
   //      warning() << "Failed to map staging texture.";
   //      
   //      throw ::exception(error_wrong_state);

   //   }

   //   int width = desc.Width;
   //   int height = desc.Height;
   //   UINT rowPitch = mapped.RowPitch;
   //   auto data = mapped.pData;

   //   //m_pimagetarget->m_pimage->create({ width, height });

   //   // Allocate RGBA buffer (contiguous memory)
   //   //auto lock = m_pimagetarget->no_padded_lock(::image::e_copy_disposition_none);

   //   //if (!lock.data()) {
   //   //   context->Unmap((ID3D11Resource*)stagingTexture, 0);
   //   //   throw ::exception(error_wrong_state);
   //   //}

   //   auto pgpucontext = m_pgpucontext;

   //   auto pcpubuffer = pgpucontext->m_pcpubuffer;

   //   pcpubuffer->set_pixels(
   //      data,
   //      width,
   //      height,
   //      (int)rowPitch,
   //      //false);
   //      false);

   //   //// Copy row by row
   //   //for (UINT y = 0; y < height; ++y) {
   //   //   memcpy(lock.data() + y * width, (unsigned char*)mapped.pData + y * rowPitch, width * 4);
   //   //}

   //   pcontext->m_pcontext->Unmap((ID3D11Resource*)m_ptextureStaging, 0);
   ////   if (!m_vkimage || !m_vkdevicememory)
   ////   {

   ////      return;

   ////   }

   ////   //auto pimpact = m_pgpucontext->m_pimpact;

   ////   //auto callback = pimpact->m_callbackImage32CpuBuffer;

   ////   //auto callback = m_prenderer->m_callbackImage32CpuBuffer;

   ////   // Get layout of the image (including row pitch)
   ////   VkImageSubresource subResource{};
   ////   subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   ////   VkSubresourceLayout subResourceLayout;

   ////   vkGetImageSubresourceLayout(m_pgpucontext->logicalDevice(), m_vkimage, &subResource, &subResourceLayout);

   ////   const char* imagedata = nullptr;
   ////   // Map image memory so we can start copying from it
   ////   vkMapMemory(m_pgpucontext->logicalDevice(), m_vkdevicememory, 0, VK_WHOLE_SIZE, 0, (void**)&imagedata);
   ////   imagedata += subResourceLayout.offset;

   ////   /*
   ////      Save host visible framebuffer image to disk (ppm format)
   ////   */

   ////   //::memory mem;

   ////   //mem.set_size(m_width * m_height * 4);

   ////   //::array<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
   ////   //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());
   ////   //{

   //   //auto pgpucontext = m_pgpucontext;

   //   //auto pcpubuffer = pgpucontext->m_pcpubuffer;

   //   //pcpubuffer->set_pixels(
   //   //   imagedata,
   //   //   m_vkextent2d.width,
   //   //   m_vkextent2d.height,
   //   //   (int)subResourceLayout.rowPitch);

   ////   //_synchronous_lock synchronouslock(m_pgpucontext->m_pmutexOffscreen);
   ////   //   m_pgpucontext->m_sizeOffscreen.cx() = m_vkextent2d.width;
   ////   //m_pgpucontext->m_sizeOffscreen.cy() = m_vkextent2d.height;
   ////   //m_pgpucontext->m_iScanOffscreen = subResourceLayout.rowPitch;
   ////   //auto area = m_pgpucontext->m_iScanOffscreen * m_pgpucontext->m_sizeOffscreen.cy();
   ////   //m_pgpucontext->m_memoryOffscreen.set_size(area);
   ////   //m_pgpucontext->m_memoryOffscreen.assign(imagedata, area);
   ////   //callback((void *)imagedata,
   ////     // m_vkextent2d.width,
   ////      //m_vkextent2d.height,
   ////      //subResourceLayout.rowPitch);

   //////}
   //////else
   //////{


   //////}


   ////   vkUnmapMemory(m_pgpucontext->logicalDevice(), m_vkdevicememory);

   }


   void renderer::sample()
   {

      //::cast < ::gpu_directx12::offscreen_render_target_view > ptargetview = m_prendertargetview;

      //__defer_construct(m_pgpucontext->m_pcpubuffer);

      //m_pgpucontext->m_pcpubuffer->gpu_read();

      /////auto& memory = m_pimagetarget->m_imagebuffer.m_memory;
      ::cast< context > pgpucontext = m_pgpucontext;
      ::cast< renderer > prenderer = pgpucontext->m_pgpurenderer;
      auto prendertargetview = prenderer->m_prendertargetview;
      ::cast < offscreen_render_target_view > poffscreenrendertargetview = prendertargetview;
      ::cast< device > pgpudevice = pgpucontext->m_pgpudevice;
      ID3D12Device* device = pgpudevice->m_pdevice;
      //ID3D11DeviceContext* context = pgpucontext->m_pcontext;
      ID3D12Resource* presourceOffscreenTexture = poffscreenrendertargetview->current_texture()->m_presource;
      //if (!pdevice || !context || !offscreenTexture)
      if (!device || !presourceOffscreenTexture)
      {
         throw ::exception(error_wrong_state);
      }


      m_pcpubuffersampler->sample(poffscreenrendertargetview->current_texture());

      //auto callback = m_callbackImage32CpuBuffer;

  //      if (callback)
      //{

      //   m_pcpubuffersampler->update(m_prendertargetview->getExtent());
      //   /*
      //      Copy framebuffer image to host visible image
      //   */
      //   /*const char* imagedata;*/
      //   {

      //      m_pcpubuffersampler->sample(m_prendertargetview->m_images[get_frame_index()]);

      //      //// Create the linear tiled destination image to copy to and to read the memory from

      //      //// Do the actual blit from the offscreen image to our host visible destination image
      //      //VkCommandBufferAllocateInfo cmdBufAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
      //      //VkCommandBuffer copyCmd;
      //      //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &cmdBufAllocateInfo, &copyCmd));
      //      //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
      //      //VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

      //      //// Transition destination image to transfer destination layout
      //      //::graphics3d_directx12::tools::insertImageMemoryBarrier(
      //      //	copyCmd,
      //      //	m_poffscreensampler->m_vkimage,
      //      //	0,
      //      //	VK_ACCESS_TRANSFER_WRITE_BIT,
      //      //	VK_IMAGE_LAYOUT_UNDEFINED,
      //      //	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      //	VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      //	VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      //	VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      //      //// colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, and does not need to be transitioned

      //      //VkImageCopy imageCopyRegion{};
      //      //imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //      //imageCopyRegion.srcSubresource.layerCount = 1;
      //      //imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //      //imageCopyRegion.dstSubresource.layerCount = 1;
      //      //imageCopyRegion.extent.width = m_prendertargetview->width();
      //      //imageCopyRegion.extent.height = m_prendertargetview->height();
      //      //imageCopyRegion.extent.depth = 1;

      //      //vkCmdCopyImage(
      //      //	copyCmd,
      //      //	//colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      //      //	m_prendertargetview->m_images[iIndex], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      //      //	dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      //	1,
      //      //	&imageCopyRegion);

      //      //// Transition destination image to general layout, which is the required layout for mapping the image memory later on
      //      //::graphics3d_directx12::tools::insertImageMemoryBarrier(
      //      //	copyCmd,
      //      //	dstImage,
      //      //	VK_ACCESS_TRANSFER_WRITE_BIT,
      //      //	VK_ACCESS_MEMORY_READ_BIT,
      //      //	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      //	VK_IMAGE_LAYOUT_GENERAL,
      //      //	VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      //	VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      //	VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      //      //VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

      //      //submitWork(copyCmd, queue);

      //      //submitInfo.commandBufferCount = 1;
      //      //submitInfo.pCommandBuffers = &copyCmd;
      //      //VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));


      //      //// Get layout of the image (including row pitch)
      //      //VkImageSubresource subResource{};
      //      //subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //      //VkSubresourceLayout subResourceLayout;

      //      //vkGetImageSubresourceLayout(m_pgpucontext->logicalDevice(), dstImage, &subResource, &subResourceLayout);

      //      //// Map image memory so we can start copying from it
      //      //vkMapMemory(m_pgpucontext->logicalDevice(), dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void**)&imagedata);
      //      //imagedata += subResourceLayout.offset;


      //      ///*
      //      //	Save host visible framebuffer image to disk (ppm format)
      //      //*/

      //      ////::memory mem;

      //      ////mem.set_size(m_width * m_height * 4);

      //      ////::array<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
      //      ////const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());
      //      //if (callback)
      //      //{
      //      //	callback((void*)imagedata, 
      //      //		m_prendertargetview->width(),
      //      //		m_prendertargetview->height(),
      //      //		subResourceLayout.rowPitch);

      //      //}

      //      //// ppm binary pixel data
      //      //for (int32_t y = 0; y < m_height; y++) {
      //      //   unsigned int * row = (unsigned int *)imagedata;
      //      //   for (int32_t x = 0; x < m_width; x++) {
      //      //      //               if (colorSwizzle) {
      //      //         file.write((char *)row + 2, 1);
      //      //         file.write((char *)row + 1, 1);
      //      //         file.write((char *)row, 1);
      //      //      }
      //      //      //else {
      //      //      //   file.write((char *)row, 3);
      //      //      //}
      //      //      row++;
      //      //   }
      //      //   imagedata += subResourceLayout.rowPitch;
      //      //}


      //  //         callback

      //  //#if defined (VK_USE_PLATFORM_ANDROID_KHR)
      //  //         const char * filename = strcat(getenv("EXTERNAL_STORAGE"_ansi), "/headless.ppm"_ansi);
      //  //#else
      //  //         const char * filename = "headless.ppm"_ansi;
      //  //#endif
      //  //         std::ofstream file(filename, std::ios::out | std::ios::binary);
      //  //
      //  //         // ppm header
      //  //         file << "P6\n"_ansi << m_width << "\n"_ansi << m_height << "\n"_ansi << 255 << "\n"_ansi;

      //         //// If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'hi have to manually swizzle color components
      //         //// Check if source is BGR and needs swizzle
      //         //::array<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
      //         //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());

      //         //// ppm binary pixel data
      //         //for (int32_t y = 0; y < m_height; y++) {
      //         //   unsigned int * row = (unsigned int *)imagedata;
      //         //   for (int32_t x = 0; x < m_width; x++) {
      //         //      if (colorSwizzle) {
      //         //         file.write((char *)row + 2, 1);
      //         //         file.write((char *)row + 1, 1);
      //         //         file.write((char *)row, 1);
      //         //      }
      //         //      else {
      //         //         file.write((char *)row, 3);
      //         //      }
      //         //      row++;
      //         //   }
      //         //   imagedata += subResourceLayout.rowPitch;
      //         //}
      //         //file.close();

      //         //LOG("Framebuffer image saved to %s\n"_ansi, filename);

      //         // Clean up resources
      //   }


      //}
      //vkQueueWaitIdle(m_pgpucontext->graphicsQueue());


   }

   void renderer::gpu_blend(::draw2d::graphics* pgraphics)
   {

      //::cast < ::gpu_directx12::offscreen_render_target_view > ptargetview = m_prendertargetview;

      //__defer_construct(m_pgpucontext->m_pcpubuffer);

      //m_pgpucontext->m_pcpubuffer->gpu_read();

      ///auto& memory = m_pimagetarget->m_imagebuffer.m_memory;
      //::cast< context > pgpucontext = m_pgpucontext;
      //::cast< renderer > prenderer = pgpucontext->m_pgpurenderer;
      //auto prendertargetview = prenderer->m_prendertargetview;
      //::cast < offscreen_render_target_view > poffscreenrendertargetview = prendertargetview;
      //::cast< device > pgpudevice = pgpucontext->m_pgpudevice;
      //ID3D11Device* device = pgpudevice->m_pdevice;
      //ID3D11DeviceContext* context = pgpucontext->m_pcontext;
      //ID3D11Texture2D* offscreenTexture = poffscreenrendertargetview->m_ptextureOffscreen;
      //if (!device || !context || !offscreenTexture)
      //{
      //   throw ::exception(error_wrong_state);
      //}

      //::cast < ::draw2d_direct2d::graphics > pgraphics2d = pgraphics;

      ////D3D11_TEXTURE2D_DESC texDesc = {};
      ////texDesc.Width = width;
      ////texDesc.Height = height;
      ////texDesc.MipLevels = 1;
      ////texDesc.ArraySize = 1;
      ////texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      ////texDesc.SampleDesc.Count = 1;
      ////texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
      ////texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

      //// ... Create texture using device->CreateTexture2D

      //// 2. Wrap the texture in a DXGI surface
      //ComPtr<IDXGISurface> dxgiSurface;
      //texture->QueryInterface(IID_PPV_ARGS(&dxgiSurface));

      //// 3. Create the Direct2D bitmap
      //D2D1_BITMAP_PROPERTIES1 bitmapProps =
      //   D2D1::BitmapProperties1(
      //      D2D1_BITMAP_OPTIONS_NONE,
      //      D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
      //   );

      //ComPtr<ID2D1Bitmap1> bitmap;
      //d2dDeviceContext->CreateBitmapFromDxgiSurface(
      //   dxgiSurface.Get(),
      //   &bitmapProps,
      //   &bitmap
      //);

      //// 4. Draw into the D2D1RenderTarget
      //d2dDeviceContext->BeginDraw();

      //d2dDeviceContext->DrawBitmap(
      //   bitmap.Get(),
      //   D2D1::RectF(0, 0, width, height),
      //   1.0f, // opacity
      //   D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
      //   nullptr // source rect (optional)
      //);

      //m_pcpubuffersampler->sample(offscreenTexture);

      //auto callback = m_callbackImage32CpuBuffer;

  //      if (callback)
      //{

      //   m_pcpubuffersampler->update(m_prendertargetview->getExtent());
      //   /*
      //      Copy framebuffer image to host visible image
      //   */
      //   /*const char* imagedata;*/
      //   {

      //      m_pcpubuffersampler->sample(m_prendertargetview->m_images[get_frame_index()]);

      //      //// Create the linear tiled destination image to copy to and to read the memory from

      //      //// Do the actual blit from the offscreen image to our host visible destination image
      //      //VkCommandBufferAllocateInfo cmdBufAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
      //      //VkCommandBuffer copyCmd;
      //      //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &cmdBufAllocateInfo, &copyCmd));
      //      //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
      //      //VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

      //      //// Transition destination image to transfer destination layout
      //      //::graphics3d_directx12::tools::insertImageMemoryBarrier(
      //      //	copyCmd,
      //      //	m_poffscreensampler->m_vkimage,
      //      //	0,
      //      //	VK_ACCESS_TRANSFER_WRITE_BIT,
      //      //	VK_IMAGE_LAYOUT_UNDEFINED,
      //      //	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      //	VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      //	VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      //	VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      //      //// colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, and does not need to be transitioned

      //      //VkImageCopy imageCopyRegion{};
      //      //imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //      //imageCopyRegion.srcSubresource.layerCount = 1;
      //      //imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //      //imageCopyRegion.dstSubresource.layerCount = 1;
      //      //imageCopyRegion.extent.width = m_prendertargetview->width();
      //      //imageCopyRegion.extent.height = m_prendertargetview->height();
      //      //imageCopyRegion.extent.depth = 1;

      //      //vkCmdCopyImage(
      //      //	copyCmd,
      //      //	//colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      //      //	m_prendertargetview->m_images[iIndex], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      //      //	dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      //	1,
      //      //	&imageCopyRegion);

      //      //// Transition destination image to general layout, which is the required layout for mapping the image memory later on
      //      //::graphics3d_directx12::tools::insertImageMemoryBarrier(
      //      //	copyCmd,
      //      //	dstImage,
      //      //	VK_ACCESS_TRANSFER_WRITE_BIT,
      //      //	VK_ACCESS_MEMORY_READ_BIT,
      //      //	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      //	VK_IMAGE_LAYOUT_GENERAL,
      //      //	VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      //	VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      //	VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      //      //VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

      //      //submitWork(copyCmd, queue);

      //      //submitInfo.commandBufferCount = 1;
      //      //submitInfo.pCommandBuffers = &copyCmd;
      //      //VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));


      //      //// Get layout of the image (including row pitch)
      //      //VkImageSubresource subResource{};
      //      //subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //      //VkSubresourceLayout subResourceLayout;

      //      //vkGetImageSubresourceLayout(m_pgpucontext->logicalDevice(), dstImage, &subResource, &subResourceLayout);

      //      //// Map image memory so we can start copying from it
      //      //vkMapMemory(m_pgpucontext->logicalDevice(), dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void**)&imagedata);
      //      //imagedata += subResourceLayout.offset;

      m_pcpubuffersampler->send_sample();

      //      ///*
      //      //	Save host visible framebuffer image to disk (ppm format)
      //      //*/

      //      ////::memory mem;

      //      ////mem.set_size(m_width * m_height * 4);

      //      ////::array<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
      //      ////const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());
      //      //if (callback)
      //      //{
      //      //	callback((void*)imagedata, 
      //      //		m_prendertargetview->width(),
      //      //		m_prendertargetview->height(),
      //      //		subResourceLayout.rowPitch);

      //      //}

      //      //// ppm binary pixel data
      //      //for (int32_t y = 0; y < m_height; y++) {
      //      //   unsigned int * row = (unsigned int *)imagedata;
      //      //   for (int32_t x = 0; x < m_width; x++) {
      //      //      //               if (colorSwizzle) {
      //      //         file.write((char *)row + 2, 1);
      //      //         file.write((char *)row + 1, 1);
      //      //         file.write((char *)row, 1);
      //      //      }
      //      //      //else {
      //      //      //   file.write((char *)row, 3);
      //      //      //}
      //      //      row++;
      //      //   }
      //      //   imagedata += subResourceLayout.rowPitch;
      //      //}


      //  //         callback

      //  //#if defined (VK_USE_PLATFORM_ANDROID_KHR)
      //  //         const char * filename = strcat(getenv("EXTERNAL_STORAGE"_ansi), "/headless.ppm"_ansi);
      //  //#else
      //  //         const char * filename = "headless.ppm"_ansi;
      //  //#endif
      //  //         std::ofstream file(filename, std::ios::out | std::ios::binary);
      //  //
      //  //         // ppm header
      //  //         file << "P6\n"_ansi << m_width << "\n"_ansi << m_height << "\n"_ansi << 255 << "\n"_ansi;

      //         //// If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'hi have to manually swizzle color components
      //         //// Check if source is BGR and needs swizzle
      //         //::array<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
      //         //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());

      //         //// ppm binary pixel data
      //         //for (int32_t y = 0; y < m_height; y++) {
      //         //   unsigned int * row = (unsigned int *)imagedata;
      //         //   for (int32_t x = 0; x < m_width; x++) {
      //         //      if (colorSwizzle) {
      //         //         file.write((char *)row + 2, 1);
      //         //         file.write((char *)row + 1, 1);
      //         //         file.write((char *)row, 1);
      //         //      }
      //         //      else {
      //         //         file.write((char *)row, 3);
      //         //      }
      //         //      row++;
      //         //   }
      //         //   imagedata += subResourceLayout.rowPitch;
      //         //}
      //         //file.close();

      //         //LOG("Framebuffer image saved to %s\n"_ansi, filename);

      //         // Clean up resources
      //   }


      //}
      //vkQueueWaitIdle(m_pgpucontext->graphicsQueue());


   }



   //	void renderer::resolve_color_and_alpha_accumulation_buffers()
   //	{
   //
   //		auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();
   //
   //		::cast < accumulation_render_target_view > ppass = m_prendertargetview;
   //
   //		auto iPassCurrentFrame = get_frame_index();
   //
   //		auto image = ppass->m_images[iPassCurrentFrame];
   //
   //		insertImageMemoryBarrier(cmdBuffer,
   //			image,
   //			0,
   //			VK_ACCESS_TRANSFER_WRITE_BIT,
   //			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //			VK_PIPELINE_STAGE_TRANSFER_BIT,
   //			VK_PIPELINE_STAGE_TRANSFER_BIT,
   //			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
   //
   //		auto imageAlphaAccumulation = ppass->m_imagesAlphaAccumulation[iPassCurrentFrame];
   //
   //		insertImageMemoryBarrier(cmdBuffer,
   //			imageAlphaAccumulation,
   //			0,
   //			VK_ACCESS_TRANSFER_WRITE_BIT,
   //			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //			VK_PIPELINE_STAGE_TRANSFER_BIT,
   //			VK_PIPELINE_STAGE_TRANSFER_BIT,
   //			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
   //
   //
   //		VkSubmitInfo submitInfo{};
   //		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
   //		submitInfo.commandBufferCount = 1;
   //		submitInfo.pCommandBuffers = &cmdBuffer;
   //		::array<VkSemaphore> waitSemaphores;
   //		::array<VkPipelineStageFlags> waitStages;
   //		waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
   //		waitSemaphores.add(m_prendertargetview->renderFinishedSemaphores[iPassCurrentFrame]);
   //		submitInfo.waitSemaphoreCount = waitSemaphores.size();
   //		submitInfo.pWaitSemaphores = waitSemaphores.data();
   //		submitInfo.pWaitDstStageMask = waitStages.data();
   //		m_pgpucontext->endSingleTimeCommands(cmdBuffer, 1, &submitInfo);
   //
   //		//m_prendererResolve->m_prendertargetview->m_semaphoreaWaitToSubmit.add(
   //		//   m_prendertargetview->renderFinishedSemaphores[iPassCurrentFrame]
   //		//);
   ////
   //		//m_prendererResolve->_resolve_color_and_alpha_accumulation_buffers();
   //
   //	}


      //void renderer::_resolve_color_and_alpha_accumulation_buffers()
      //{

      //	on_new_frame();

      //	auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

      //	auto iFrameIndex1 = get_frame_index();

      //	VkImage image1 = m_prendertargetview->m_images[iFrameIndex1];

      //	if (is_starting_frame())
      //	{

      //		insertImageMemoryBarrier(cmdBuffer,
      //			image1,
      //			0,
      //			VK_ACCESS_TRANSFER_WRITE_BIT,
      //			VK_IMAGE_LAYOUT_UNDEFINED,
      //			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //			VK_PIPELINE_STAGE_TRANSFER_BIT,
      //			VK_PIPELINE_STAGE_TRANSFER_BIT,
      //			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
      //	}
      //	else
      //	{

      //		insertImageMemoryBarrier(cmdBuffer,
      //			image1,
      //			0,
      //			VK_ACCESS_TRANSFER_WRITE_BIT,
      //			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //			VK_PIPELINE_STAGE_TRANSFER_BIT,
      //			VK_PIPELINE_STAGE_TRANSFER_BIT,
      //			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
      //	}

      //	m_pgpucontext->endSingleTimeCommands(cmdBuffer);

      //	auto pframe = beginFrame();

      //	on_begin_render(pframe);

      //	// Resolve (Color and Alpha Accumulation Buffers) descriptors
      //	if (!m_psetdescriptorlayoutResolve)
      //	{

      //		m_psetdescriptorlayoutResolve = ::gpu_directx12::set_descriptor_layout::Builder(m_pgpucontext)
      //			.addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //			.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //			.build();

      //		int iFrameCount = get_frame_count();

      //		auto pdescriptorpoolbuilder = __allocate::gpu_directx12::descriptor_pool::Builder();

      //		pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
      //		pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      //		pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      //		m_pdescriptorpoolResolve = pdescriptorpoolbuilder->build();

      //	}

      //	if (!m_pshaderResolve)
      //	{

      //		auto pshadervertexinput = __allocate  shader_vertex_input();

      //		pshadervertexinput->m_bindings.add(
      //			{
      //			   .binding = 0,
      //			   .stride = sizeof(float) * 4,
      //			   .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
      //			});

      //		pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
      //		pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

      //		auto pshaderResolve = __create_new<::gpu_directx12::shader>();

      //		m_pshaderResolve = pshaderResolve;

      //		pshaderResolve->m_bDisableDepthTest = true;

      //		//VkDescriptorSetLayoutBinding samplerLayoutBinding = {
      //		//   .binding = 0,
      //		//   .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      //		//   .descriptorCount = 1,
      //		//   .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
      //		//   .pImmutableSamplers = NULL,
      //		//};

      //		//VkDescriptorSetLayoutCreateInfo layoutInfo =
      //		//{
      //		//   .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
      //		//   .bindingCount = 1,
      //		//   .pBindings = &samplerLayoutBinding,
      //		//};

      //		//VkDescriptorSetLayout descriptorSetLayout;
      //		//if (vkCreateDescriptorSetLayout(device, &layoutInfo, NULL, &descriptorSetLayout) != VK_SUCCESS) 
      //		//{
      //		//   // Handle error
      //		//}

      //		::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

      //		pshaderResolve->initialize_shader_with_block(
      //			this,
      //			as_memory_block(g_uaImageBlendVertexShader),
      //			as_memory_block(g_uaResolveFragmentShader),
      //			{ ::gpu::shader::e_descriptor_set_slot_local },
      //			m_psetdescriptorlayoutResolve,
      //			pshadervertexinput);

      //	}

      //	auto pshader = m_pshaderResolve;

      //	pshader->bind();

      //	auto& pdescriptor = m_pdescriptorResolve;

      //	if (__defer_construct_new(pdescriptor))
      //	{

      //		pdescriptor->m_descriptorsets.set_size(get_frame_count());

      //		::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

      //		::cast < renderer > pgpurendererParent = m_pgpucontext->m_pgpurenderer;

      //		::cast < accumulation_render_target_view > paccumulationrendertargetview = pgpurendererParent->m_prendertargetview;

      //		for (int i = 0; i < get_frame_count(); i++)
      //		{
      //			VkDescriptorImageInfo imageinfo;

      //			imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      //			imageinfo.imageView = paccumulationrendertargetview->m_imageviews[i];
      //			imageinfo.sampler = m_pgpucontext->_001VkSampler();

      //			VkDescriptorImageInfo imageinfo2;

      //			imageinfo2.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      //			imageinfo2.imageView = paccumulationrendertargetview->m_imageviewsAlphaAccumulation[i];
      //			imageinfo2.sampler = m_pgpucontext->_001VkSampler();

      //			auto& playout = m_psetdescriptorlayoutResolve;

      //			auto& ppool = m_pdescriptorpoolResolve;

      //			descriptor_writer(*playout, *ppool)
      //				.writeImage(0, &imageinfo)
      //				.writeImage(1, &imageinfo2)
      //				.build(pdescriptor->m_descriptorsets[i]);

      //		}

      //		auto descriptorSetLayout = m_psetdescriptorlayoutResolve->getDescriptorSetLayout();

      //		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
      //   .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      //   .setLayoutCount = 1,
      //   .pSetLayouts = &descriptorSetLayout,
      //		};

      //		//VkPipelineLayout pipelineLayout;
      //		if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
      //			// Handle error
      //		}

      //	}

      //	auto commandBuffer = getCurrentCommandBuffer();

      //	//auto commandBuffer = this->getCurrentCommandBuffer();

      //	// Bind pipeline and descriptor sets
      // //      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
      //   //    vkCmdBindDescriptorSets(commandBuffer, ...);
      //	vkCmdBindDescriptorSets(
      //		commandBuffer,
      //		VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
      //		pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
      //		0,                                  // First set (set = 0)
      //		1,                                  // Descriptor set count
      //		&pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
      //		0,                                  // Dynamic offset count
      //		NULL                                // Dynamic offsets
      //	);


      //	auto& pmodel = m_pmodelResolve;

      //	if (__defer_construct_new(pmodel))
      //	{

      //		create_quad_buffers(m_pgpucontext->logicalDevice(),
      //			m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
      //			&pmodel->m_vertexBuffer,
      //			&pmodel->m_vertexMemory,
      //			&pmodel->m_indexBuffer,
      //			&pmodel->m_indexMemory);

      //	}



      //	VkDeviceSize offsets[] = { 0 };
      //	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
      //	vkCmdBindIndexBuffer(commandBuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);
      //	auto rectangle = m_rectangle;
      //	VkViewport vp = {
      //	   (float)rectangle.left(),
      //	   (float)rectangle.top(),
      //	   (float)rectangle.width(),
      //	   (float)rectangle.height(),
      //	   0.0f, 1.0f };
      //	VkRect2D sc = {
      //	   {
      //	   (float)rectangle.left(),
      //	   (float)rectangle.top(),
      //	   },
      //	   {
      //				   (float)rectangle.width(),
      //	   (float)rectangle.height(),


      //	}
      //	};
      //	vkCmdSetViewport(commandBuffer, 0, 1, &vp);
      //	vkCmdSetScissor(commandBuffer, 0, 1, &sc);

      //	vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
      //	// Draw full-screen quad
      //	//vkCmdDraw(commandBuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

      //	pshader->unbind();

      //	on_end_render(pframe);

      //	endFrame();

      //}


   void renderer::swap_chain()
   {



      //assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      //auto commandBuffer = getCurrentCommandBuffer();
      //if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      //   throw ::exception(error_failed, "failed to record command buffer!");
      //}
      //auto result = m_prendertargetview->submitCommandBuffers(&commandBuffer);
      //if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      //   m_bNeedToRecreateSwapChain)
      //{
      //   m_bNeedToRecreateSwapChain = false;
      //   defer_update_render_target_view();
      //}
      //else if (result != VK_SUCCESS)
      //{
      //   throw ::exception(error_failed, "failed to present swap chain image!");
      //}


      //vkQueueWaitIdle(m_pgpucontext->graphicsQueue());


   }



   void renderer::on_end_draw()
   {
      _on_end_render();

      for (auto& procedure : m_procedureaAfterEndRender)
      {

         procedure();

      }

      m_procedureaAfterEndRender.clear();
      //if (m_bOffScreen)
      //{

      //   assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      //   auto commandBuffer = getCurrentCommandBuffer();
      //   if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      //      throw ::exception(error_failed, "failed to record command buffer!");
      //   }
      //   auto result = m_prendertargetview->submitCommandBuffers(&commandBuffer, &currentImageIndex);
      //   //if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      //   //	vkcWindow.wasWindowResized()) 
      //   //{
      //   //	vkcWindow.resetWindowResizedFlag();
      //   //	recreateSwapchain();
      //   //}
      //   //else 
      //   //	if (result != VK_SUCCESS) {
      //   //	throw ::exception(error_failed, "failed to present swap chain image!");
      //   //}

      if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      {
         sample();
      }
      else if (m_pgpucontext->m_eoutput == ::gpu::e_output_swap_chain)
      {
         swap_chain();


      }
      //isFrameStarted = false;
      //currentFrameIndex = (currentFrameIndex + 1) % render_target_view::MAX_FRAMES_IN_FLIGHT;

      //}
      //else
      //{


      //	assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      //	auto commandBuffer = getCurrentCommandBuffer();
      //	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      //		throw ::exception(error_failed, "failed to record command buffer!");
      //	}
      //	auto result = m_pvkcswapchain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
      //	//if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      //	//	vkcWindow.wasWindowResized()) 
      //	//{
      //	//	vkcWindow.resetWindowResizedFlag();
      //	//	recreateSwapchain();
      //	//}
      //	//else 
      //	//	if (result != VK_SUCCESS) {
      //	//	throw ::exception(error_failed, "failed to present swap chain image!");
      //	//}
      //	isFrameStarted = false;
      //	currentFrameIndex = (currentFrameIndex + 1) % swap_chain_render_target_view::MAX_FRAMES_IN_FLIGHT;

      //}

   }

   ::gpu::shader* renderer::get_image_blend_shader()
   {

      if (!m_pshaderImageBlend)
      {

         auto pshadervertexinput = __allocate  shader_vertex_input();

         //pshadervertexinput->m_bindings.add(
         //   {
         //      .binding = 0,
         //      .stride = sizeof(float) * 4,
         //      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
         //   });

         //pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
         //pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

         auto pshaderImageBlend = __create_new<::gpu_directx12::shader>();

         m_pshaderImageBlend = pshaderImageBlend;

         pshaderImageBlend->m_bEnableBlend = true;
         pshaderImageBlend->m_bDisableDepthTest = true;

         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

         pshaderImageBlend->initialize_shader_with_block(
            this,
            as_memory_block(g_uaImageBlendVertexShader),
            as_memory_block(g_uaImageBlendFragmentShader),
            { ::gpu::shader::e_descriptor_set_slot_local },
            m_psetdescriptorlayoutImageBlend,
            pshadervertexinput);

      }

      return m_pshaderImageBlend;

   }


   ::gpu::shader* renderer::get_image_set_shader()
   {

      if (!m_pshaderImageBlend)
      {

         auto pshadervertexinput = __allocate  shader_vertex_input();

         //pshadervertexinput->m_bindings.add(
         //   {
         //      .binding = 0,
         //      .stride = sizeof(float) * 4,
         //      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
         //   });

         //pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
         //pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

         auto pshaderImageBlend = __create_new<::gpu_directx12::shader>();

         m_pshaderImageBlend = pshaderImageBlend;

         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

         pshaderImageBlend->initialize_shader_with_block(
            this,
            as_memory_block(g_uaImageBlendVertexShader),
            as_memory_block(g_uaImageBlendFragmentShader),
            { ::gpu::shader::e_descriptor_set_slot_local },
            m_psetdescriptorlayoutImageBlend,
            pshadervertexinput);

      }

      return m_pshaderImageBlend;

   }



   //// Fullscreen quad vertex data
   //float quadVertices[] = {
   //   // pos     // uv
   //   -1.0f, -1.0f, 0.0f, 0.0f,
   //    1.0f, -1.0f, 1.0f, 0.0f,
   //    1.0f,  1.0f, 1.0f, 1.0f,
   //   -1.0f,  1.0f, 0.0f, 1.0f,
   //};
   //uint16_t quadIndices[] = {
   //   0, 1, 2,
   //   2, 3, 0
   //};

   //// Create vertex and index buffers
   //void create_quad_buffers(VkDevice device, VkPhysicalDevice physicalDevice,
   //   VkBuffer* vertexBuffer, VkDeviceMemory* vertexMemory,
   //   VkBuffer* indexBuffer, VkDeviceMemory* indexMemory, bool bYSwap)
   //{

   //   float y0;
   //   float y1;

   //   if (bYSwap)
   //   {

   //      y0 = 1.0f;
   //      y1 = -1.0f;


   //   }
   //   else
   //   {

   //      y0 = -1.0f;
   //      y1 = 1.0f;

   //   }


   //   // Fullscreen quad vertex data
   //   float quadVertices[] = {
   //      // pos     // uv
   //      -1.0f, y0, 0.0f, 0.0f,
   //       1.0f, y0, 1.0f, 0.0f,
   //       1.0f, y1, 1.0f, 1.0f,
   //      -1.0f, y1, 0.0f, 1.0f,
   //   };
   //   uint16_t quadIndices[] = {
   //      0, 1, 2,
   //      2, 3, 0
   //   };


   //   VkDeviceSize vertexSize = sizeof(quadVertices);
   //   VkDeviceSize indexSize = sizeof(quadIndices);

   //   VkBufferCreateInfo bufferInfo = {
   //      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
   //      .size = vertexSize,
   //      .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
   //      .sharingMode = VK_SHARING_MODE_EXCLUSIVE
   //   };
   //   vkCreateBuffer(device, &bufferInfo, NULL, vertexBuffer);

   //   VkMemoryRequirements memReq;
   //   vkGetBufferMemoryRequirements(device, *vertexBuffer, &memReq);

   //   VkMemoryAllocateInfo allocInfo = {
   //      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
   //      .allocationSize = memReq.size,
   //      .memoryTypeIndex = 0
   //   };

   //   VkPhysicalDeviceMemoryProperties memProps;
   //   vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);
   //   for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
   //      if ((memReq.memoryTypeBits & (1 << i)) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
   //         allocInfo.memoryTypeIndex = i;
   //         break;
   //      }
   //   }

   //   vkAllocateMemory(device, &allocInfo, NULL, vertexMemory);
   //   vkBindBufferMemory(device, *vertexBuffer, *vertexMemory, 0);

   //   void* data;
   //   vkMapMemory(device, *vertexMemory, 0, vertexSize, 0, &data);
   //   memcpy(data, quadVertices, (size_t)vertexSize);
   //   vkUnmapMemory(device, *vertexMemory);

   //   // Index buffer
   //   bufferInfo.size = indexSize;
   //   bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
   //   vkCreateBuffer(device, &bufferInfo, NULL, indexBuffer);
   //   vkGetBufferMemoryRequirements(device, *indexBuffer, &memReq);
   //   allocInfo.allocationSize = memReq.size;

   //   for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
   //      if ((memReq.memoryTypeBits & (1 << i)) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
   //         allocInfo.memoryTypeIndex = i;
   //         break;
   //      }
   //   }

   //   vkAllocateMemory(device, &allocInfo, NULL, indexMemory);
   //   vkBindBufferMemory(device, *indexBuffer, *indexMemory, 0);
   //   vkMapMemory(device, *indexMemory, 0, indexSize, 0, &data);
   //   memcpy(data, quadIndices, (size_t)indexSize);
   //   vkUnmapMemory(device, *indexMemory);
   //}


   //void renderer::_blend_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap)
   //{

   //   // Image Blend descriptors
   //   if (!m_psetdescriptorlayoutImageBlend)
   //   {

   //      m_psetdescriptorlayoutImageBlend = ::gpu_directx12::set_descriptor_layout::Builder(m_pgpucontext)
   //         .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
   //         .build();

   //      int iFrameCount = get_frame_count();

   //      auto pdescriptorpoolbuilder = __allocate::gpu_directx12::descriptor_pool::Builder();

   //      pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
   //      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
   //      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

   //      m_pdescriptorpoolImageBlend = pdescriptorpoolbuilder->build();

   //   }

   //   //vkCmdBeginRenderPass(...);

   //   auto commandBuffer = this->getCurrentCommandBuffer();

   //   //VkCommandBufferAllocateInfo commandBufferAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

   //   //VkCommandBuffer commandBuffer;
   //   //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &commandBufferAllocateInfo, &commandBuffer));
   //   //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
   //   //VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));


   //   //m_procedureaAfterEndRender.add(
   //   //   [this, image, commandBuffer]()
   //   //   {

   //   //      //            {
   //   //      //            VkImageMemoryBarrier barrier = {
   //   //      //.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //      //.pNext = nullptr,
   //   //      //   .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //   //      //.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //      //
   //   //      //.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //   //      //.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //      //.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //.image = image,
   //   //      //.subresourceRange = {
   //   //      //    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //      //    .baseMipLevel = 0,
   //   //      //    .levelCount = 1,
   //   //      //    .baseArrayLayer = 0,
   //   //      //    .layerCount = 1,
   //   //      //},
   //   //      //            };
   //   //      //
   //   //      //            vkCmdPipelineBarrier(
   //   //      //               commandBuffer,
   //   //      //               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
   //   //      //               VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //   //      //               0,
   //   //      //               0, NULL,
   //   //      //               0, NULL,
   //   //      //               1, &barrier
   //   //      //            );
   //   //      //
   //   //      //         }
   //   //      //            {
   //   //      //               VkImageMemoryBarrier barrier = {
   //   //      //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //      //         .pNext = nullptr,
   //   //      //         .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //      //         .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //   //      //
   //   //      //         .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //      //         .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //   //      //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //         .image = image,  // <-- your VkImage here
   //   //      //         .subresourceRange = {
   //   //      //             .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //      //             .baseMipLevel = 0,
   //   //      //             .levelCount = 1,
   //   //      //             .baseArrayLayer = 0,
   //   //      //             .layerCount = 1,
   //   //      //         },
   //   //      //               };
   //   //      //
   //   //      //               vkCmdPipelineBarrier(
   //   //      //                  commandBuffer,
   //   //      //                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // srcStageMask
   //   //      //                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,         // dstStageMask
   //   //      //                  0,
   //   //      //                  0, NULL,
   //   //      //                  0, NULL,
   //   //      //                  1, &barrier
   //   //      //               );
   //   //      //
   //   //      //            }
   //   //      //
   //   //      //         }

   //   //if(1)
   //   //      {


   //   //   VkImageMemoryBarrier barrier = {
   //   //       .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //       .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //       .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //   //       .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //       .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //   //       .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //       .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //       .image = image,
   //   //       .subresourceRange = {
   //   //           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //           .baseMipLevel = 0,
   //   //           .levelCount = 1,
   //   //           .baseArrayLayer = 0,
   //   //           .layerCount = 1
   //   //       },
   //   //   };

   //   //   vkCmdPipelineBarrier(
   //   //      commandBuffer,
   //   //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //   //      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
   //   //      0,
   //   //      0, NULL,
   //   //      0, NULL,
   //   //      1, &barrier
   //   //   );

   //   //      }

   //   /*   }
   //      );*/

   //   auto& pmodel = m_imagemodel[image];

   //   if (__defer_construct_new(pmodel))
   //   {

   //      create_quad_buffers(m_pgpucontext->logicalDevice(),
   //         m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
   //         &pmodel->m_vertexBuffer,
   //         &pmodel->m_vertexMemory,
   //         &pmodel->m_indexBuffer,
   //         &pmodel->m_indexMemory, true);

   //   }

   //   auto pshader = get_image_blend_shader();

   //   pshader->bind();

   //   auto& pdescriptor = m_imagedescriptor[image];

   //   if (__defer_construct_new(pdescriptor))
   //   {

   //      pdescriptor->m_descriptorsets.set_size(get_frame_count());

   //      VkImageViewCreateInfo viewInfo = {
   //   .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
   //   .image = image,  // <-- Your existing VkImage
   //   .viewType = VK_IMAGE_VIEW_TYPE_2D,
   //   .format = VK_FORMAT_B8G8R8A8_UNORM,  // <-- Match your image's format
   //   .components = {
   //      .r = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .g = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .b = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .a = VK_COMPONENT_SWIZZLE_IDENTITY,
   //   },
   //   .subresourceRange = {
   //      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //      .baseMipLevel = 0,
   //      .levelCount = 1,
   //      .baseArrayLayer = 0,
   //      .layerCount = 1,
   //   },
   //      };

   //      VkImageView imageView;
   //      if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, NULL, &imageView) != VK_SUCCESS) {
   //         // Handle error
   //      }

   //      ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

   //      for (int i = 0; i < get_frame_count(); i++)
   //      {
   //         VkDescriptorImageInfo imageinfo;

   //         imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
   //         imageinfo.imageView = imageView;
   //         imageinfo.sampler = m_pgpucontext->_001VkSampler();

   //         auto& playout = m_psetdescriptorlayoutImageBlend;

   //         auto& ppool = m_pdescriptorpoolImageBlend;

   //         descriptor_writer(*playout, *ppool)
   //            .writeImage(0, &imageinfo)
   //            .build(pdescriptor->m_descriptorsets[i]);

   //      }

   //      auto descriptorSetLayout = m_psetdescriptorlayoutImageBlend->getDescriptorSetLayout();

   //      VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
   //   .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
   //   .setLayoutCount = 1,
   //   .pSetLayouts = &descriptorSetLayout,
   //      };

   //      //VkPipelineLayout pipelineLayout;
   //      if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
   //         // Handle error
   //      }

   //   }

   //   //auto commandBuffer = this->getCurrentCommandBuffer();

   //   // Bind pipeline and descriptor sets
   // //      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
   //   //    vkCmdBindDescriptorSets(commandBuffer, ...);
   //   vkCmdBindDescriptorSets(
   //      commandBuffer,
   //      VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
   //      pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
   //      0,                                  // First set (set = 0)
   //      1,                                  // Descriptor set count
   //      &pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
   //      0,                                  // Dynamic offset count
   //      NULL                                // Dynamic offsets
   //   );



   //   VkDeviceSize offsets[] = { 0 };
   //   vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
   //   vkCmdBindIndexBuffer(commandBuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

   //   VkViewport vp = {
   //      (float)rectangle.left(),
   //      (float)rectangle.top(),
   //      (float)rectangle.width(),
   //      (float)rectangle.height(),
   //      0.0f, 1.0f };
   //   VkRect2D sc = {
   //      {
   //      (float)rectangle.left(),
   //      (float)rectangle.top(),
   //      },
   //      {
   //               (float)rectangle.width(),
   //      (float)rectangle.height(),


   //   }
   //   };
   //   vkCmdSetViewport(commandBuffer, 0, 1, &vp);
   //   vkCmdSetScissor(commandBuffer, 0, 1, &sc);

   //   vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
   //   // Draw full-screen quad
   //   //vkCmdDraw(commandBuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

   //   pshader->unbind();

   //   //vkCmdEndRenderPass(...);

   //   vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

   //   //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_vkcommandpool, 1, &commandBuffer);


   //}


   //void renderer::_copy_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap)
   //{

   //   // Image Blend descriptors
   //   if (!m_psetdescriptorlayoutImageBlend)
   //   {

   //      m_psetdescriptorlayoutImageBlend = ::gpu_directx12::set_descriptor_layout::Builder(m_pgpucontext)
   //         .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
   //         .build();

   //      int iFrameCount = get_frame_count();

   //      auto pdescriptorpoolbuilder = __allocate::gpu_directx12::descriptor_pool::Builder();

   //      pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
   //      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
   //      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

   //      m_pdescriptorpoolImageBlend = pdescriptorpoolbuilder->build();

   //   }

   //   VkCommandBuffer commandBuffer = this->getCurrentCommandBuffer();

   //   auto& pmodel = m_imagemodel[image];

   //   if (__defer_construct_new(pmodel))
   //   {

   //      create_quad_buffers(m_pgpucontext->logicalDevice(),
   //         m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
   //         &pmodel->m_vertexBuffer,
   //         &pmodel->m_vertexMemory,
   //         &pmodel->m_indexBuffer,
   //         &pmodel->m_indexMemory, bYSwap);

   //   }

   //   auto pshader = get_image_blend_shader();

   //   pshader->bind();

   //   auto& pdescriptor = m_imagedescriptor[image];

   //   if (__defer_construct_new(pdescriptor))
   //   {

   //      pdescriptor->m_descriptorsets.set_size(get_frame_count());

   //      VkImageViewCreateInfo viewInfo =
   //      {
   //         .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
   //         .image = image,  // <-- Your existing VkImage
   //         .viewType = VK_IMAGE_VIEW_TYPE_2D,
   //         .format = VK_FORMAT_B8G8R8A8_UNORM,  // <-- Match your image's format
   //         .components = 
   //         {
   //            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
   //            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
   //            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
   //            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
   //         },
   //         .subresourceRange = 
   //         {
   //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //            .baseMipLevel = 0,
   //            .levelCount = 1,
   //            .baseArrayLayer = 0,
   //            .layerCount = 1,
   //         },
   //      };

   //      VkImageView imageView;

   //      if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, NULL, &imageView) != VK_SUCCESS)
   //      {
   //         // Handle error
   //      }

   //      ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

   //      for (int i = 0; i < get_frame_count(); i++)
   //      {

   //         VkDescriptorImageInfo imageinfo;

   //         imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
   //         imageinfo.imageView = imageView;
   //         imageinfo.sampler = m_pgpucontext->_001VkSampler();

   //         auto& playout = m_psetdescriptorlayoutImageBlend;

   //         auto& ppool = m_pdescriptorpoolImageBlend;

   //         descriptor_writer(*playout, *ppool)
   //            .writeImage(0, &imageinfo)
   //            .build(pdescriptor->m_descriptorsets[i]);

   //      }

   //      auto descriptorSetLayout = m_psetdescriptorlayoutImageBlend->getDescriptorSetLayout();

   //      VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
   //   .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
   //   .setLayoutCount = 1,
   //   .pSetLayouts = &descriptorSetLayout,
   //      };

   //      //VkPipelineLayout pipelineLayout;
   //      if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
   //         // Handle error
   //      }

   //   }

   //   //auto commandBuffer = this->getCurrentCommandBuffer();

   //   // Bind pipeline and descriptor sets
   // //      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
   //   //    vkCmdBindDescriptorSets(commandBuffer, ...);
   //   vkCmdBindDescriptorSets(
   //      commandBuffer,
   //      VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
   //      pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
   //      0,                                  // First set (set = 0)
   //      1,                                  // Descriptor set count
   //      &pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
   //      0,                                  // Dynamic offset count
   //      NULL                                // Dynamic offsets
   //   );



   //   VkDeviceSize offsets[] = { 0 };
   //   vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
   //   vkCmdBindIndexBuffer(commandBuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

   //   VkViewport vp = {
   //      (float)rectangle.left(),
   //      (float)rectangle.top(),
   //      (float)rectangle.width(),
   //      (float)rectangle.height(),
   //      0.0f, 1.0f };
   //   VkRect2D sc = {
   //      {
   //      (float)rectangle.left(),
   //      (float)rectangle.top(),
   //      },
   //      {
   //               (float)rectangle.width(),
   //      (float)rectangle.height(),


   //   }
   //   };
   //   vkCmdSetViewport(commandBuffer, 0, 1, &vp);
   //   vkCmdSetScissor(commandBuffer, 0, 1, &sc);

   //   vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
   //   // Draw full-screen quad
   //   //vkCmdDraw(commandBuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

   //   pshader->unbind();

   //   //vkCmdEndRenderPass(...);

   //   vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

   //   //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_vkcommandpool, 1, &commandBuffer);


   //}


   //void renderer::_set_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap)
   //{

   //   // Image Blend descriptors
   //   if (!m_psetdescriptorlayoutImageBlend)
   //   {

   //      m_psetdescriptorlayoutImageBlend = ::gpu_directx12::set_descriptor_layout::Builder(m_pgpucontext)
   //         .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
   //         .build();

   //      int iFrameCount = get_frame_count();

   //      auto pdescriptorpoolbuilder = __allocate::gpu_directx12::descriptor_pool::Builder();

   //      pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
   //      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
   //      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

   //      m_pdescriptorpoolImageBlend = pdescriptorpoolbuilder->build();

   //   }

   //   //vkCmdBeginRenderPass(...);

   //   auto commandBuffer = this->getCurrentCommandBuffer();

   //   //VkCommandBufferAllocateInfo commandBufferAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

   //   //VkCommandBuffer commandBuffer;
   //   //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &commandBufferAllocateInfo, &commandBuffer));
   //   //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
   //   //VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));


   //   //m_procedureaAfterEndRender.add(
   //   //   [this, image, commandBuffer]()
   //   //   {

   //   //      //            {
   //   //      //            VkImageMemoryBarrier barrier = {
   //   //      //.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //      //.pNext = nullptr,
   //   //      //   .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //   //      //.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //      //
   //   //      //.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //   //      //.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //      //.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //.image = image,
   //   //      //.subresourceRange = {
   //   //      //    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //      //    .baseMipLevel = 0,
   //   //      //    .levelCount = 1,
   //   //      //    .baseArrayLayer = 0,
   //   //      //    .layerCount = 1,
   //   //      //},
   //   //      //            };
   //   //      //
   //   //      //            vkCmdPipelineBarrier(
   //   //      //               commandBuffer,
   //   //      //               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
   //   //      //               VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //   //      //               0,
   //   //      //               0, NULL,
   //   //      //               0, NULL,
   //   //      //               1, &barrier
   //   //      //            );
   //   //      //
   //   //      //         }
   //   //      //            {
   //   //      //               VkImageMemoryBarrier barrier = {
   //   //      //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //      //         .pNext = nullptr,
   //   //      //         .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //      //         .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //   //      //
   //   //      //         .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //      //         .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //   //      //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //         .image = image,  // <-- your VkImage here
   //   //      //         .subresourceRange = {
   //   //      //             .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //      //             .baseMipLevel = 0,
   //   //      //             .levelCount = 1,
   //   //      //             .baseArrayLayer = 0,
   //   //      //             .layerCount = 1,
   //   //      //         },
   //   //      //               };
   //   //      //
   //   //      //               vkCmdPipelineBarrier(
   //   //      //                  commandBuffer,
   //   //      //                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // srcStageMask
   //   //      //                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,         // dstStageMask
   //   //      //                  0,
   //   //      //                  0, NULL,
   //   //      //                  0, NULL,
   //   //      //                  1, &barrier
   //   //      //               );
   //   //      //
   //   //      //            }
   //   //      //
   //   //      //         }

   //   //if(1)
   //   //      {


   //   //   VkImageMemoryBarrier barrier = {
   //   //       .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //       .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //       .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //   //       .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //       .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //   //       .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //       .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //       .image = image,
   //   //       .subresourceRange = {
   //   //           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //           .baseMipLevel = 0,
   //   //           .levelCount = 1,
   //   //           .baseArrayLayer = 0,
   //   //           .layerCount = 1
   //   //       },
   //   //   };

   //   //   vkCmdPipelineBarrier(
   //   //      commandBuffer,
   //   //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //   //      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
   //   //      0,
   //   //      0, NULL,
   //   //      0, NULL,
   //   //      1, &barrier
   //   //   );

   //   //      }

   //   /*   }
   //      );*/

   //   auto& pmodel = m_imagemodel[image];

   //   if (__defer_construct_new(pmodel))
   //   {

   //      create_quad_buffers(m_pgpucontext->logicalDevice(),
   //         m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
   //         &pmodel->m_vertexBuffer,
   //         &pmodel->m_vertexMemory,
   //         &pmodel->m_indexBuffer,
   //         &pmodel->m_indexMemory, 
   //         bYSwap);

   //   }

   //   auto pshader = get_image_blend_shader();

   //   pshader->bind();

   //   auto& pdescriptor = m_imagedescriptor[image];

   //   if (__defer_construct_new(pdescriptor))
   //   {

   //      pdescriptor->m_descriptorsets.set_size(get_frame_count());

   //      VkImageViewCreateInfo viewInfo = {
   //   .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
   //   .image = image,  // <-- Your existing VkImage
   //   .viewType = VK_IMAGE_VIEW_TYPE_2D,
   //   .format = VK_FORMAT_B8G8R8A8_UNORM,  // <-- Match your image's format
   //   .components = {
   //      .r = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .g = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .b = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .a = VK_COMPONENT_SWIZZLE_IDENTITY,
   //   },
   //   .subresourceRange = {
   //      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //      .baseMipLevel = 0,
   //      .levelCount = 1,
   //      .baseArrayLayer = 0,
   //      .layerCount = 1,
   //   },
   //      };

   //      VkImageView imageView;
   //      if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, NULL, &imageView) != VK_SUCCESS) {
   //         // Handle error
   //      }

   //      ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

   //      for (int i = 0; i < get_frame_count(); i++)
   //      {
   //         VkDescriptorImageInfo imageinfo;

   //         imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
   //         imageinfo.imageView = imageView;
   //         imageinfo.sampler = m_pgpucontext->_001VkSampler();

   //         auto& playout = m_psetdescriptorlayoutImageBlend;

   //         auto& ppool = m_pdescriptorpoolImageBlend;

   //         descriptor_writer(*playout, *ppool)
   //            .writeImage(0, &imageinfo)
   //            .build(pdescriptor->m_descriptorsets[i]);

   //      }

   //      auto descriptorSetLayout = m_psetdescriptorlayoutImageBlend->getDescriptorSetLayout();

   //      VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
   //   .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
   //   .setLayoutCount = 1,
   //   .pSetLayouts = &descriptorSetLayout,
   //      };

   //      //VkPipelineLayout pipelineLayout;
   //      if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
   //         // Handle error
   //      }

   //   }

   //   //auto commandBuffer = this->getCurrentCommandBuffer();

   //   // Bind pipeline and descriptor sets
   // //      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
   //   //    vkCmdBindDescriptorSets(commandBuffer, ...);
   //   vkCmdBindDescriptorSets(
   //      commandBuffer,
   //      VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
   //      pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
   //      0,                                  // First set (set = 0)
   //      1,                                  // Descriptor set count
   //      &pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
   //      0,                                  // Dynamic offset count
   //      NULL                                // Dynamic offsets
   //   );



   //   VkDeviceSize offsets[] = { 0 };
   //   vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
   //   vkCmdBindIndexBuffer(commandBuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

   //   VkViewport vp = {
   //      (float)rectangle.left(),
   //      (float)rectangle.top(),
   //      (float)rectangle.width(),
   //      (float)rectangle.height(),
   //      0.0f, 1.0f };
   //   VkRect2D sc = {
   //      {
   //      (float)rectangle.left(),
   //      (float)rectangle.top(),
   //      },
   //      {
   //               (float)rectangle.width(),
   //      (float)rectangle.height(),


   //   }
   //   };
   //   vkCmdSetViewport(commandBuffer, 0, 1, &vp);
   //   vkCmdSetScissor(commandBuffer, 0, 1, &sc);

   //   vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
   //   // Draw full-screen quad
   //   //vkCmdDraw(commandBuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

   //   pshader->unbind();

   //   //vkCmdEndRenderPass(...);

   //   vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

   //   //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_vkcommandpool, 1, &commandBuffer);


   //}




   void renderer::_blend_renderer(::gpu_directx12::renderer* prendererSrc, bool bYSwap)
   {

      //  VkImage image = prendererSrc->m_prendertargetview->m_images[prendererSrc->get_frame_index()];

      //  auto rectanglePlacement = prendererSrc->m_pgpucontext->m_rectangle;

      //  // Image Blend descriptors
      //  if (!m_psetdescriptorlayoutImageBlend)
      //  {

      //     m_psetdescriptorlayoutImageBlend = ::gpu_directx12::set_descriptor_layout::Builder(m_pgpucontext)
      //        .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //        .build();

      //     int iFrameCount = get_frame_count();

      //     auto pdescriptorpoolbuilder = __allocate::gpu_directx12::descriptor_pool::Builder();

      //     pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
      //     pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      //     pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      //     m_pdescriptorpoolImageBlend = pdescriptorpoolbuilder->build();

      //  }

      //  //vkCmdBeginRenderPass(...);

      //  auto commandBuffer = this->getCurrentCommandBuffer();

      //  //VkCommandBufferAllocateInfo commandBufferAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

      //  //VkCommandBuffer commandBuffer;
      //  //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &commandBufferAllocateInfo, &commandBuffer));
      //  //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
      //  //VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));


      //  //m_procedureaAfterEndRender.add(
      //  //   [this, image, commandBuffer]()
      //  //   {

      //  //      //            {
      //  //      //            VkImageMemoryBarrier barrier = {
      //  //      //.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //  //      //.pNext = nullptr,
      //  //      //   .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //  //      //.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //  //      //
      //  //      //.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //  //      //.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //  //      //.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //  //      //.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //  //      //.image = image,
      //  //      //.subresourceRange = {
      //  //      //    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //  //      //    .baseMipLevel = 0,
      //  //      //    .levelCount = 1,
      //  //      //    .baseArrayLayer = 0,
      //  //      //    .layerCount = 1,
      //  //      //},
      //  //      //            };
      //  //      //
      //  //      //            vkCmdPipelineBarrier(
      //  //      //               commandBuffer,
      //  //      //               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      //  //      //               VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //  //      //               0,
      //  //      //               0, NULL,
      //  //      //               0, NULL,
      //  //      //               1, &barrier
      //  //      //            );
      //  //      //
      //  //      //         }
      //  //      //            {
      //  //      //               VkImageMemoryBarrier barrier = {
      //  //      //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //  //      //         .pNext = nullptr,
      //  //      //         .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //  //      //         .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //  //      //
      //  //      //         .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //  //      //         .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //  //      //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //  //      //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //  //      //         .image = image,  // <-- your VkImage here
      //  //      //         .subresourceRange = {
      //  //      //             .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //  //      //             .baseMipLevel = 0,
      //  //      //             .levelCount = 1,
      //  //      //             .baseArrayLayer = 0,
      //  //      //             .layerCount = 1,
      //  //      //         },
      //  //      //               };
      //  //      //
      //  //      //               vkCmdPipelineBarrier(
      //  //      //                  commandBuffer,
      //  //      //                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // srcStageMask
      //  //      //                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,         // dstStageMask
      //  //      //                  0,
      //  //      //                  0, NULL,
      //  //      //                  0, NULL,
      //  //      //                  1, &barrier
      //  //      //               );
      //  //      //
      //  //      //            }
      //  //      //
      //  //      //         }

      //  //if(1)
      //  //      {


      //  //   VkImageMemoryBarrier barrier = {
      //  //       .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //  //       .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //  //       .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //  //       .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //  //       .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //  //       .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //  //       .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //  //       .image = image,
      //  //       .subresourceRange = {
      //  //           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //  //           .baseMipLevel = 0,
      //  //           .levelCount = 1,
      //  //           .baseArrayLayer = 0,
      //  //           .layerCount = 1
      //  //       },
      //  //   };

      //  //   vkCmdPipelineBarrier(
      //  //      commandBuffer,
      //  //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //  //      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      //  //      0,
      //  //      0, NULL,
      //  //      0, NULL,
      //  //      1, &barrier
      //  //   );

      //  //      }

      //  /*   }
      //     );*/

      //  auto& pmodel = m_imagemodel[image];

      //  if (__defer_construct_new(pmodel))
      //  {

      //     create_quad_buffers(m_pgpucontext->logicalDevice(),
      //        m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
      //        &pmodel->m_vertexBuffer,
      //        &pmodel->m_vertexMemory,
      //        &pmodel->m_indexBuffer,
      //        &pmodel->m_indexMemory, 
      //        bYSwap);

      //  }

      //  auto pshader = get_image_blend_shader();

      //  pshader->bind();

      //  auto& pdescriptor = m_imagedescriptor[image];

      //  if (__defer_construct_new(pdescriptor))
      //  {

      //     pdescriptor->m_descriptorsets.set_size(get_frame_count());

      //     //     VkImageViewCreateInfo viewInfo = {
      //     //.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      //     //.image = image,  // <-- Your existing VkImage
      //     //.viewType = VK_IMAGE_VIEW_TYPE_2D,
      //     //.format = VK_FORMAT_B8G8R8A8_UNORM,  // <-- Match your image's format
      //     //.components = {
      //     //    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
      //     //    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
      //     //    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
      //     //    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
      //     //},
      //     //.subresourceRange = {
      //     //    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //     //    .baseMipLevel = 0,
      //     //    .levelCount = 1,
      //     //    .baseArrayLayer = 0,
      //     //    .layerCount = 1,
      //     //},
      //     //     };

      //     //     VkImageView imageView;
      //     //     if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, NULL, &imageView) != VK_SUCCESS) {
      //     //        // Handle error
      //     //     }

      //     ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;
      //     ::cast < accumulation_render_target_view > ppass = prendererSrc->m_prendertargetview;

      //     for (int i = 0; i < get_frame_count(); i++)
      //     {

      //        VkDescriptorImageInfo imageinfo;

      //        auto imageview = ppass->m_imageviews[i];

      //        imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      //        imageinfo.imageView = imageview;
      //        imageinfo.sampler = m_pgpucontext->_001VkSampler();

      //        VkDescriptorImageInfo imageinfoAlpha;

      //        auto imageviewAlpha = ppass->m_imageviewsAlphaAccumulation[i];

      //        imageinfoAlpha.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      //        imageinfoAlpha.imageView = imageviewAlpha;
      //        imageinfoAlpha.sampler = m_pgpucontext->_001VkSampler();


      //        auto& playout = m_psetdescriptorlayoutImageBlend;

      //        auto& ppool = m_pdescriptorpoolImageBlend;

      //        descriptor_writer(*playout, *ppool)
      //           .writeImage(0, &imageinfo)
      //           .writeImage(0, &imageinfoAlpha)
      //           .build(pdescriptor->m_descriptorsets[i]);

      //     }

      //     auto descriptorSetLayout = m_psetdescriptorlayoutImageBlend->getDescriptorSetLayout();

      //     VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
      //  .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      //  .setLayoutCount = 1,
      //  .pSetLayouts = &descriptorSetLayout,
      //     };

      //     //VkPipelineLayout pipelineLayout;
      //     if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
      //        // Handle error
      //     }

      //  }

      //  //auto commandBuffer = this->getCurrentCommandBuffer();

      //  // Bind pipeline and descriptor sets
      ////      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
      //  //    vkCmdBindDescriptorSets(commandBuffer, ...);
      //  vkCmdBindDescriptorSets(
      //     commandBuffer,
      //     VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
      //     pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
      //     0,                                  // First set (set = 0)
      //     1,                                  // Descriptor set count
      //     &pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
      //     0,                                  // Dynamic offset count
      //     NULL                                // Dynamic offsets
      //  );



      //  VkDeviceSize offsets[] = { 0 };
      //  vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
      //  vkCmdBindIndexBuffer(commandBuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);


      //  VkViewport vp = {
      //     (float)rectanglePlacement.left(),
      //     (float)rectanglePlacement.top(),
      //     (float)rectanglePlacement.width(),
      //     (float)rectanglePlacement.height(),
      //     0.0f, 1.0f };
      //  VkRect2D sc = {
      //     {
      //     (float)rectanglePlacement.left(),
      //     (float)rectanglePlacement.top(),
      //     },
      //     {
      //     (float)rectanglePlacement.width(),
      //     (float)rectanglePlacement.height(),


      //  }
      //  };
      //  vkCmdSetViewport(commandBuffer, 0, 1, &vp);
      //  vkCmdSetScissor(commandBuffer, 0, 1, &sc);

      //  vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
      //  // Draw full-screen quad
      //  //vkCmdDraw(commandBuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

      //  pshader->unbind();

      //  //vkCmdEndRenderPass(...);

      //  vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

      //  //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_vkcommandpool, 1, &commandBuffer);


   }


   void renderer::_on_begin_render()
   {

      auto ptexture = m_prendertargetview->current_texture();

      auto pcommandlist = getCurrentCommandBuffer2()->m_pcommandlist;

      ptexture->_new_state(pcommandlist, D3D12_RESOURCE_STATE_RENDER_TARGET);

      ID3D12DescriptorHeap* ppHeaps[] = { m_cbvHeap };
      pcommandlist->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    
      ////::cast < frame > pframe = pframeParam;

      ////auto commandBuffer = pframe->commandBuffer;

      //auto commandBuffer = this->getCurrentCommandBuffer();

      ////if (m_bOffScreen)
      //{

      //   assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      //   assert(
      //      commandBuffer == getCurrentCommandBuffer() &&
      //      "Can't begin render pass on command buffer from a different frame");

      //   VkRenderPassBeginInfo renderPassInfo{};
      //   renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      //   renderPassInfo.renderPass = m_prendertargetview->getRenderPass();
      //   renderPassInfo.framebuffer = m_prendertargetview->getCurrentFrameBuffer();

      //   renderPassInfo.renderArea.offset = { 0, 0 };
      //   renderPassInfo.renderArea.extent = m_prendertargetview->getExtent();

      //   VkClearValue clearValues[2];
      //   //clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
      //   clearValues[0].color = { 0.f, 0.0f, 0.0f, 0.0f };
      //   clearValues[1].depthStencil = { 1.0f, 0 };
      //   renderPassInfo.clearValueCount = 2;
      //   renderPassInfo.pClearValues = clearValues;

      //   vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      //   VkViewport viewport{};
      //   viewport.x = 0.0f;
      //   viewport.y = 0.0f;
      //   viewport.width = static_cast<float>(m_prendertargetview->getExtent().width);
      //   viewport.height = static_cast<float>(m_prendertargetview->getExtent().height);
      //   viewport.minDepth = 0.0f;
      //   viewport.maxDepth = 1.0f;
      //   VkRect2D scissor{ {0, 0}, m_prendertargetview->getExtent() };
      //   vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      //   vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

      //}
      ////else
      ////{

      ////	assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      ////	assert(
      ////		commandBuffer == getCurrentCommandBuffer() &&
      ////		"Can't begin render pass on command buffer from a different frame");

      ////	VkRenderPassBeginInfo renderPassInfo{};
      ////	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      ////	renderPassInfo.renderPass = m_pvkcswapchain->getRenderPass();
      ////	renderPassInfo.framebuffer = m_pvkcswapchain->getFrameBuffer(currentImageIndex);

      ////	renderPassInfo.renderArea.offset = { 0, 0 };
      ////	renderPassInfo.renderArea.extent = m_pvkcswapchain->getExtent();

      ////	std::array<VkClearValue, 2> clearValues{};
      ////	clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
      ////	clearValues[1].depthStencil = { 1.0f, 0 };
      ////	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      ////	renderPassInfo.pClearValues = clearValues.data();

      ////	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      ////	VkViewport viewport{};
      ////	viewport.x = 0.0f;
      ////	viewport.y = 0.0f;
      ////	viewport.width = static_cast<float>(vkcSwapChain->getSwapChainExtent().width);
      ////	viewport.height = static_cast<float>(vkcSwapChain->getSwapChainExtent().height);
      ////	viewport.minDepth = 0.0f;
      ////	viewport.maxDepth = 1.0f;
      ////	VkRect2D scissor{ {0, 0}, vkcSwapChain->getSwapChainExtent() };
      ////	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      ////	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


      ////}
   }


   void renderer::on_begin_render(::gpu::frame* pframeParam)
   {

      _on_begin_render();

      //::cast < frame > pframe = pframeParam;

      //auto commandBuffer = pframe->commandBuffer;

      ////m_prendertargetview->m_iFrameSerial++;

      ////m_prendertargetview->m_iCurrentFrame = (m_prendertargetview->m_iCurrentFrame + 1) % 
      ////   get_frame_count();

      //m_prendertargetview->on_before_begin_render(pframe);

      ////if (m_bOffScreen)
      //{

      //   assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      //   assert(
      //      commandBuffer == getCurrentCommandBuffer() &&
      //      "Can't begin render pass on command buffer from a different frame");

      //   VkRenderPassBeginInfo renderPassInfo{};
      //   renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      //   renderPassInfo.renderPass = m_prendertargetview->getRenderPass();
      //   renderPassInfo.framebuffer = m_prendertargetview->getCurrentFrameBuffer();

      //   renderPassInfo.renderArea.offset = { 0, 0 };
      //   renderPassInfo.renderArea.extent = m_prendertargetview->getExtent();

      //   VkClearValue clearValues[2];
      //   //clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
      //   clearValues[0].color = { 0.0f, 0.0f, 0.0f, 0.0f };
      //   clearValues[1].depthStencil = { 1.0f, 0 };
      //   renderPassInfo.clearValueCount = 2;
      //   renderPassInfo.pClearValues = clearValues;

      //   vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      //   VkViewport viewport{};
      //   viewport.x = 0.0f;
      //   viewport.y = 0.0f;
      //   viewport.width = static_cast<float>(m_prendertargetview->getExtent().width);
      //   viewport.height = static_cast<float>(m_prendertargetview->getExtent().height);
      //   viewport.minDepth = 0.0f;
      //   viewport.maxDepth = 1.0f;
      //   VkRect2D scissor{ {0, 0}, m_prendertargetview->getExtent() };
      //   vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      //   vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

      //}
      ////else
      ////{

      ////	assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      ////	assert(
      ////		commandBuffer == getCurrentCommandBuffer() &&
      ////		"Can't begin render pass on command buffer from a different frame");

      ////	VkRenderPassBeginInfo renderPassInfo{};
      ////	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      ////	renderPassInfo.renderPass = m_pvkcswapchain->getRenderPass();
      ////	renderPassInfo.framebuffer = m_pvkcswapchain->getFrameBuffer(currentImageIndex);

      ////	renderPassInfo.renderArea.offset = { 0, 0 };
      ////	renderPassInfo.renderArea.extent = m_pvkcswapchain->getExtent();

      ////	std::array<VkClearValue, 2> clearValues{};
      ////	clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
      ////	clearValues[1].depthStencil = { 1.0f, 0 };
      ////	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      ////	renderPassInfo.pClearValues = clearValues.data();

      ////	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      ////	VkViewport viewport{};
      ////	viewport.x = 0.0f;
      ////	viewport.y = 0.0f;
      ////	viewport.width = static_cast<float>(vkcSwapChain->getSwapChainExtent().width);
      ////	viewport.height = static_cast<float>(vkcSwapChain->getSwapChainExtent().height);
      ////	viewport.minDepth = 0.0f;
      ////	viewport.maxDepth = 1.0f;
      ////	VkRect2D scissor{ {0, 0}, vkcSwapChain->getSwapChainExtent() };
      ////	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      ////	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

      auto pcommandbuffer = getCurrentCommandBuffer2();

      auto pcommandlist = pcommandbuffer->m_pcommandlist;


      {

         auto pgpurendertargetview = m_prendertargetview;

         if (pgpurendertargetview)
         {

            auto presourceTexture = pgpurendertargetview->current_texture()->m_presource;

            if (presourceTexture)
            {

               auto presourceDepthStencilBuffer = pgpurendertargetview->current_depth_stencil()->m_presource;

               if (presourceDepthStencilBuffer)
               {

                  //m_pcontext->OMSetDepthStencilState(pdepthstencilstate, 0);
                  int iDescriptorSize = pgpurendertargetview->m_rtvDescriptorSize;
                  int iFrameIndex = get_frame_index();
                  auto hRtv = pgpurendertargetview->m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
                  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
                     hRtv, 
                     iFrameIndex, 
                     iDescriptorSize);
                  auto hDsv = pgpurendertargetview->m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
                  CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(hDsv);
                  // You set the RTV and DSV like this:
                  pcommandlist->OMSetRenderTargets(
                     1,                    // One render target
                     &rtvHandle,           // D3D12_CPU_DESCRIPTOR_HANDLE to your RTV
                     FALSE,                // Not using RTV arrays
                     &dsvHandle            // D3D12_CPU_DESCRIPTOR_HANDLE to your DSV (can be null)
                  );
                  //m_pcontext->ClearDepthStencilView(pdepthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);
                  pcommandlist->ClearDepthStencilView(
                     pgpurendertargetview->m_dsvHeap->GetCPUDescriptorHandleForHeapStart(),
                     D3D12_CLEAR_FLAG_DEPTH,
                     1.0f, 0,
                     0, nullptr
                  );

               }
               else
               {

                  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
                     pgpurendertargetview->m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
                     get_frame_index(),
                     pgpurendertargetview->m_rtvDescriptorSize);

                  pcommandlist->OMSetRenderTargets(
                     1,
                     &rtvHandle,
                     true,
                     nullptr);

               }


               // 1. Define viewport and scissor rectangle
               D3D12_VIEWPORT viewport = {};
               viewport.TopLeftX = 0.0f;
               viewport.TopLeftY = 0.0f;
               viewport.Width = static_cast<float>(m_pgpucontext->m_rectangle.width());
               viewport.Height = static_cast<float>(m_pgpucontext->m_rectangle.height());
               viewport.MinDepth = 0.0f;
               viewport.MaxDepth = 1.0f;

               D3D12_RECT scissorRect = {};
               scissorRect.left = 0;
               scissorRect.top = 0;
               scissorRect.right = m_pgpucontext->m_rectangle.width();
               scissorRect.bottom = m_pgpucontext->m_rectangle.height();

               //// 2. Begin command recording
               //commandAllocator->Reset();
               //pcommandlist->Reset(commandAllocator.Get(), pipelineState.Get());

               //// 3. Set the pipeline and root signature
               //pcommandlist->SetPipelineState(pipelineState.Get());
               //pcommandlist->SetGraphicsRootSignature(rootSignature.Get());

               //// 4. Set the viewport and scissor
               //commandList->RSSetViewports(1, &viewport);
               //commandList->RSSetScissorRects(1, &scissorRect);
               ////D3D11_VIEWPORT vp = {};
               ////vp.TopLeftX = 0;
               ////vp.TopLeftY = 0;
               ////vp.Width = static_cast<float>(m_rectangle.width());
               ////vp.Height = static_cast<float>(m_rectangle.height());
               ////vp.MinDepth = 0.0f;
               ////vp.MaxDepth = 1.0f;

               ////m_pcontext->RSSetViewports(1, &vp);

            }

            //auto presourceDepthStencilBuffer = pgpurendertargetview->m_presourceDepthStencilBuffer;

            //if (presourceDepthStencilBuffer)
            //{

            //   //m_pcontext->OMSetDepthStencilState(pdepthstencilstate, 0);

            //   // You set the RTV and DSV like this:
            //   pcommandlist->OMSetRenderTargets(
            //      1,                    // One render target
            //      &pgpurendertargetview->m_ptexture->m_handleRenderTargetView,           // D3D12_CPU_DESCRIPTOR_HANDLE to your RTV
            //      FALSE,                // Not using RTV arrays
            //      &pgpurendertargetview->m_handleDepthStencilView            // D3D12_CPU_DESCRIPTOR_HANDLE to your DSV (can be null)
            //   );
            //   //m_pcontext->ClearDepthStencilView(pdepthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);

            //}

            ::cast < offscreen_render_target_view > poffscreenrendertargetview = pgpurendertargetview;

            if (poffscreenrendertargetview)
            {

               //auto psamplerstate = poffscreenrendertargetview->m_psamplerstate;

               //if (psamplerstate)
               {

                  //m_pcontext->PSSetSamplers(0, 1, psamplerstate.pp());

               }

            }


         }

      }


      //if (!m_prasterizerstate)
      //{

      //   // 1. Define rasterizer state descriptor
      //   D3D11_RASTERIZER_DESC rasterizerDesc = {};
      //   rasterizerDesc.FillMode = D3D11_FILL_SOLID;
      //   rasterizerDesc.CullMode = D3D11_CULL_BACK;        // Cull back faces
      //   rasterizerDesc.FrontCounterClockwise = false; // Treat CCW as front-facing
      //   rasterizerDesc.DepthClipEnable = TRUE;

      //   // 2. Create rasterizer state object
      //   //ID3D11RasterizerState* pRasterizerState = nullptr;
      //   HRESULT hr = m_pgpudevice->m_pdevice->CreateRasterizerState(&rasterizerDesc, &m_prasterizerstate);
      //   if (FAILED(hr)) {
      //      // Handle error (e.g., log or exit)
      //      throw ::hresult_exception(hr);
      //   }


      ::cast < ::gpu_directx12::context > pcontext = m_pgpucontext;

      m_hlsClear.m_dH = fmod(m_hlsClear.m_dH + 0.0001, 1.0);
      ::color::color colorClear = m_hlsClear;
      // Clear render target
      float clear[4] = {
         colorClear.f32_red() * .5f,
         colorClear.f32_green() * .5f,
         colorClear.f32_blue() * .5f, .5f };


      ///pcontext->g_pImmediateContext->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);

            ///pcontext->m_pcontext->ClearRenderTargetView(m_prendertargetview->m_prendertargetview, clear);

            // Assumes:
      // - commandList is a valid ID3D12GraphicsCommandList*
      // - rtvHandle is a valid D3D12_CPU_DESCRIPTOR_HANDLE to the render target view
      // - clearColor is a float[4] array (same as in DX11)

      // Example clear color
      float clearColor[4] = { 0.5f * 0.5f, 0.75f * 0.5f, 0.9f * 0.5f, 0.5f };
      CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
         m_prendertargetview->m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), 
         get_frame_index(),
         m_prendertargetview->m_rtvDescriptorSize);
      pcommandlist->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

      on_happening(e_happening_begin_render);

   }


   void renderer::on_end_render(::gpu::frame* pframeParam)
   {

      on_happening(e_happening_end_render);

      //::cast < frame > pframe = pframeParam;

      //auto commandBuffer = pframe->commandBuffer;

      //assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
      //assert(
      //   commandBuffer == getCurrentCommandBuffer() &&
      //   "Can't end render pass on command buffer from a different frame");
      //vkCmdEndRenderPass(commandBuffer);
   }


   //void renderer::on_end_render(::graphics3d::frame * pframeParam)
   void renderer::_on_end_render()
   {

      ////::cast < frame > pframe = pframeParam;

      ////auto commandBuffer = pframe->commandBuffer;

      //auto commandBuffer = getCurrentCommandBuffer();

      //assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
      //assert(
      //   commandBuffer == getCurrentCommandBuffer() &&
      //   "Can't end render pass on command buffer from a different frame");
      //vkCmdEndRenderPass(commandBuffer);
   }


   ::pointer < ::gpu::frame > renderer::beginFrame()
   {

      //defer_layout();

      assert(!isFrameStarted && "Can't call beginFrame while already in progress");

      ////if (m_bOffScreen)
      //{

      //   auto result = m_prendertargetview->acquireNextImage();

      //   if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      //      //defer_layout();
      //      m_prendertargetview->init();
      //      //throw ::exception(todo, "resize happened?!?!");
      //      return nullptr;
      //   }
      //   if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      //      throw ::exception(error_failed, "Failed to aquire swap chain image");
      //   }

      //   isFrameStarted = true;

      //   auto commandBuffer = getCurrentCommandBuffer();

      //   VkCommandBufferBeginInfo beginInfo{};
      //   beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      //   if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      //      throw ::exception(error_failed, "failed to begin recording command buffer!");
      //   }
        //auto pframe = __create_new < ::gpu_directx12::frame >();
      //   pframe->commandBuffer = commandBuffer;
      __defer_construct(m_pframe);
      //   on_happening(e_happening_begin_frame);
      //   return m_pframe;

      //}
      ////else
      ////{


      //auto& pcommandbuffer = m_commandbuffera[get_frame_index()];
      auto& pcommandbuffer = m_pcommandbuffer;

      auto pcommandbufferLoadAssets = ::transfer(m_pcommandbufferLoadAssets);
      if (pcommandbufferLoadAssets)
      {

         //if (prenderer->m_pcommandbufferLoadAssets)
         //{

         //   auto pcommandbufferLoadAssets = ::transfer(prenderer->m_pcommandbufferLoadAssets);

         //   m_papplication->fork([pcommandbufferLoadAssets]()
         //      {

                  pcommandbufferLoadAssets->submit_command_buffer();

                  //pcommandbufferLoadAssets->wait_for_gpu();

      //         });

      //   }

      //}

         // Wait on the graphics queue for the copy to complete
         pcommandbuffer->m_pcommandqueue->Wait(
            pcommandbufferLoadAssets->m_pfence,
            pcommandbufferLoadAssets->m_fenceValue);

      }


      pcommandbuffer->reset();

      //::defer_throw_hresult(
      //   m_pgraphicscommandlist->Reset(pcommandallocator, pipelineState));

      ////	auto result = m_pvkcswapchain->acquireNextImage(&currentImageIndex);

      ////	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      ////		recreateRenderPass();
      ////		return nullptr;
      ////	}
      ////	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      ////		throw ::exception(error_failed, "Failed to aquire swap chain image");
      ////	}

      isFrameStarted = true;

      ////	auto commandBuffer = getCurrentCommandBuffer();

      ////	VkCommandBufferBeginInfo beginInfo{};
      ////	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      ////	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      ////		throw ::exception(error_failed, "failed to begin recording command buffer!");
      ////	}
      ////	return commandBuffer;

      ////}
      on_happening(e_happening_begin_frame);

      return m_pframe;

   }


   void renderer::endFrame()
   {

      on_happening(e_happening_end_frame);

      //// 5. Signal and wait (optional but recommended for CPU/GPU sync)
      //m_fences[get_frame_index()]++;
      //HRESULT hrSignalCommandQueue = m_pcommandqueue->Signal(m_pfence, m_fences[get_frame_index()]);

      //::defer_throw_hresult(hrSignalCommandQueue);

      //// Wait until the GPU has completed execution
      //if (m_pfence->GetCompletedValue() < m_fences[get_frame_index()])
      //{
      //   m_pfence->SetEventOnCompletion(m_fences[get_frame_index()], pfrmae->m_hFenceEvent);
      //   WaitForSingleObject(pframe->m_hFenceEvent, INFINITE);
      //}


      ////if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      ////{

      //assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      //auto commandBuffer = getCurrentCommandBuffer();
      //if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
      //{
      //   throw ::exception(error_failed, "failed to record command buffer!");
      //}

      //auto result = m_prendertargetview->submitCommandBuffers(&commandBuffer);

      //if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      //   m_bNeedToRecreateSwapChain)
      //{
      //   m_bNeedToRecreateSwapChain = false;
      //   defer_update_render_target_view();
      //}
      //else if (result != VK_SUCCESS)
      //{
      //   throw ::exception(error_failed, "failed to present swap chain image!");
      //}


      //if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      //{
      //   sample();

      //}
      ////else if (m_eoutput == ::gpu::e_output_color_and_alpha_accumulation_buffers)
      ////{

      ////	resolve_color_and_alpha_accumulation_buffers();

      ////}

      //auto pcommandlist = pcommandbuffer->m_pcommandlist;

      //HRESULT hrCloseCommandList = pcommandlist->Close();

      //::defer_throw_hresult(hrCloseCommandList);

      //// 4. Execute the command list
      //ID3D12CommandList* ppCommandLists[] = { pcommandlist };
      //m_pcommandqueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

      auto eoutput = m_pgpucontext->m_eoutput;

      if (eoutput == ::gpu::e_output_swap_chain)
      {

         m_pgpucontext->swap_buffers();

      }
      else if (eoutput == ::gpu::e_output_cpu_buffer)
      {

         this->sample();

      }
      //else if (eoutput == ::gpu::e_output_gpu_buffer)
      //{

      //   this->gpu_blend();

      //}

      ////rrentImageIndex = m_prendertargetview->currentFrame;
      ////currentFrameIndex = (currentFrameIndex + 1) % ::gpu_directx12::render_target_view::MAX_FRAMES_IN_FLIGHT;

      ////}
      ////else
      ////{


      ////	assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      ////	auto commandBuffer = getCurrentCommandBuffer();
      ////	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      ////		throw ::exception(error_failed, "failed to record command buffer!");
      ////	}
      ////	auto result = m_pvkcswapchain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
      ////	//if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      ////	//	vkcWindow.wasWindowResized()) 
      ////	//{
      ////	//	vkcWindow.resetWindowResizedFlag();
      ////	//	recreateSwapchain();
      ////	//}
      ////	//else 
      ////	//	if (result != VK_SUCCESS) {
      ////	//	throw ::exception(error_failed, "failed to present swap chain image!");
      ////	//}
      ////	isFrameStarted = false;
      ////	currentFrameIndex = (currentFrameIndex + 1) % swap_chain_render_target_view::MAX_FRAMES_IN_FLIGHT;

      ////}

      auto pcommandbuffer = getCurrentCommandBuffer2();

      pcommandbuffer->submit_command_buffer();

      if (eoutput == ::gpu::e_output_cpu_buffer)
      {

         pcommandbuffer->wait_for_gpu();

         m_pcpubuffersampler->send_sample();

      }

   }


   //void renderer::_on_graphics_end_draw(VkImage image, const ::int_rectangle& rectangle)
   //{

   //   m_pgpucontext->set_placement(rectangle);

   //   on_new_frame();

   //   if (1)
   //   {
   //      auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

   //      VkImageMemoryBarrier barrier = {
   //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //         .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //         .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //         .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //         .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //         .image = image,
   //         .subresourceRange = {
   //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //            .baseMipLevel = 0,
   //            .levelCount = 1,
   //            .baseArrayLayer = 0,
   //            .layerCount = 1
   //         },
   //      };

   //      vkCmdPipelineBarrier(
   //         cmdBuffer,
   //         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
   //         0,
   //         0, NULL,
   //         0, NULL,
   //         1, &barrier
   //      );
   //      m_pgpucontext->endSingleTimeCommands(cmdBuffer);
   //   }


   //   if (auto pframe = beginFrame())
   //   {

   //      //on_begin_frame();
   //      // render
   //      on_begin_render(pframe);



   //      //if (m_pimpact->global_ubo_block().size() > 0)
   //      //{

   //        // update_global_ubo(m_pgpucontext);

   //      //}

   //      //m_pscene->on_render(m_pgpucontext);

   //      _blend_image(image, rectangle, false);

   //      on_end_render(pframe);

   //      endFrame();

   //   }

   //   if (1)
   //   {
   //      auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

   //      VkImageMemoryBarrier barrier = {
   //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //         .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //         .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //         .oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //         .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //         .image = image,
   //         .subresourceRange = {
   //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //            .baseMipLevel = 0,
   //            .levelCount = 1,
   //            .baseArrayLayer = 0,
   //            .layerCount = 1
   //         },
   //      };

   //      vkCmdPipelineBarrier(
   //         cmdBuffer,
   //         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
   //         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //         0,
   //         0, NULL,
   //         0, NULL,
   //         1, &barrier
   //      );
   //      m_pgpucontext->endSingleTimeCommands(cmdBuffer);
   //   }


   //}


   void renderer::_on_graphics_end_draw(::gpu_directx12::renderer* prendererSrc)
   {

      ::gpu_directx12::renderer* prenderer;

      //if (prendererSrc->m_prendererResolve)
      //{

      //	prenderer = prendererSrc->m_prendererResolve;

      //}
      //else
      {

         prenderer = prendererSrc;

      }

      m_pgpucontext->set_placement(prenderer->m_pgpucontext->rectangle());

      //VkImage image = prenderer->m_prendertargetview->m_images[prenderer->get_frame_index()];

      defer_update_renderer();

      //on_new_frame();

      //if (1)
      //{
      //   //auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

      //   //VkImageMemoryBarrier barrier = {
      //   //    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //   //    .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //   //    .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //   //    .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //   //    .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //   //    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //   //    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //   //    .image = image,
      //   //    .subresourceRange = {
      //   //        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //   //        .baseMipLevel = 0,
      //   //        .levelCount = 1,
      //   //        .baseArrayLayer = 0,
      //   //        .layerCount = 1
      //   //    },
      //   //};

      //   //vkCmdPipelineBarrier(
      //   //   cmdBuffer,
      //   //   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //   //   VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      //   //   0,
      //   //   0, NULL,
      //   //   0, NULL,
      //   //   1, &barrier
      //   //);

      //   //VkSubmitInfo submitInfo = {};
      //   //submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

      //   //VkSemaphore waitSemaphores[] = { prendererSrc->m_prendertargetview->renderFinishedSemaphores[prendererSrc->m_prendertargetview->currentFrame] };
      //   //VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
      //   //submitInfo.waitSemaphoreCount = 1;
      //   //submitInfo.pWaitSemaphores = waitSemaphores;
      //   //submitInfo.pWaitDstStageMask = waitStages;

      //   //submitInfo.commandBufferCount = 1;
      //   //submitInfo.pCommandBuffers = &cmdBuffer;

      //   //m_pgpucontext->endSingleTimeCommands(cmdBuffer, 1, &submitInfo);
      //   auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();


      //   insertImageMemoryBarrier(cmdBuffer,
      //      image,
      //      0,
      //      VK_ACCESS_TRANSFER_WRITE_BIT,
      //      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //      VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });



      //   VkSubmitInfo submitInfo{};
      //   submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      //   submitInfo.commandBufferCount = 1;
      //   submitInfo.pCommandBuffers = &cmdBuffer;
      //   ::array<VkSemaphore> waitSemaphores;
      //   ::array<VkPipelineStageFlags> waitStages;
      //   waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
      //   waitSemaphores.add(prenderer->m_prendertargetview->renderFinishedSemaphores[prenderer->get_frame_index()]);
      //   submitInfo.waitSemaphoreCount = waitSemaphores.size();
      //   submitInfo.pWaitSemaphores = waitSemaphores.data();
      //   submitInfo.pWaitDstStageMask = waitStages.data();
      //   m_pgpucontext->endSingleTimeCommands(cmdBuffer, 1, &submitInfo);

      //   //m_prendererResolve->m_prendertargetview->m_semaphoreaWaitToSubmit.add(
      //   //   m_prendertargetview->renderFinishedSemaphores[iPassCurrentFrame]
      //   //);


      //}

      //if (auto pframe = beginFrame())
      //{

      //   m_prendertargetview->m_semaphoreaSignalOnSubmit.add(prendererSrc->m_prendertargetview->imageAvailableSemaphores[prendererSrc->get_frame_index()]);


      //   //on_begin_frame();
      //   // render
      //   on_begin_render(pframe);



      //   //if (m_pimpact->global_ubo_block().size() > 0)
      //   //{

      //     // update_global_ubo(m_pgpucontext);

      //   //}

      //   //m_pscene->on_render(m_pgpucontext);

      //   _blend_image(image, m_pgpucontext->rectangle(), false);

      //   on_end_render(pframe);

      //   endFrame();

      //}

      ////if (1)
      ////{
      ////   auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

      ////   VkImageMemoryBarrier barrier = {
      ////       .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      ////       .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
      ////      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      ////      .oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      ////       .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      ////       .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      ////       .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      ////       .image = image,
      ////       .subresourceRange = {
      ////           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      ////           .baseMipLevel = 0,
      ////           .levelCount = 1,
      ////           .baseArrayLayer = 0,
      ////           .layerCount = 1
      ////       },
      ////   };

      ////   vkCmdPipelineBarrier(
      ////      cmdBuffer,
      ////      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      ////      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      ////      0,
      ////      0, NULL,
      ////      0, NULL,
      ////      1, &barrier
      ////   );
      ////   m_pgpucontext->endSingleTimeCommands(cmdBuffer);
      ////}


   }


   //void renderer::_on_frame_draw(::gpu_directx12::renderer* prendererUpper)
   //{




   //}


   void renderer::endDraw(::user::interaction* puserinteraction, ::gpu::renderer* pgpurendererSrc)
   {

      ::cast < renderer > prenderer = pgpurendererSrc;

      if (m_pgpucontext->m_eoutput == ::gpu::e_output_swap_chain)
      {

         m_pgpucontext->swap_buffers();

      }

      //VkImage vkimage = prenderer->m_prendertargetview->m_images[prenderer->get_frame_index()];

      //::int_rectangle rectangle = prenderer->m_pgpucontext->rectangle();

      //auto copyCmd = m_pgpucontext->beginSingleTimeCommands();

      //insertImageMemoryBarrier(
      //   copyCmd,
      //   vkimage,
      //   0,
      //   VK_ACCESS_TRANSFER_WRITE_BIT,
      //   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //   VK_PIPELINE_STAGE_TRANSFER_BIT,
      //   VK_PIPELINE_STAGE_TRANSFER_BIT,
      //   VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      //VkSubmitInfo submitInfo{};
      //submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      //submitInfo.commandBufferCount = 1;
      //submitInfo.pCommandBuffers = &copyCmd;
      //::array<VkSemaphore> waitSemaphores;
      //::array<VkPipelineStageFlags> waitStages;
      //waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
      //waitSemaphores.add(prenderer->m_prendertargetview->renderFinishedSemaphores[prenderer->get_frame_index()]);
      //submitInfo.waitSemaphoreCount = waitSemaphores.size();
      //submitInfo.pWaitSemaphores = waitSemaphores.data();
      //submitInfo.pWaitDstStageMask = waitStages.data();

      //m_pgpucontext->endSingleTimeCommands(copyCmd);

      //defer_update_render_target_view();

      //on_new_frame();

      //auto pframe = beginFrame();

      //on_begin_render(pframe);

      //_copy_image(vkimage, rectangle, false);

      //on_end_render(pframe);

      //endFrame();

      ////vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

      ////vkQueueWaitIdle(m_pgpucontext->presentQueue());

   }


} // namespace gpu_directx12



