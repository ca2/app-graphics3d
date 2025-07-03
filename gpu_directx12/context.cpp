#include "framework.h"
#include "acme/platform/application.h"
#include "approach.h"
#include "aura/graphics/image/image.h"
#include "bred/gpu/compositor.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/types.h"
#include "buffer.h"
#include "command_buffer.h"
#include "context.h"
#include "device.h"
#include "gpu_directx12/descriptors.h"
#include "offscreen_render_target_view.h"
#include "physical_device.h"
#include "program.h"
#include "renderer.h"
#include "shader.h"
#include "swap_chain.h"
#include "texture.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/mat4x4.hpp>
#include "initializers.h"
#include "acme_windows_common/dxgi_surface_bindable.h"
#include "windowing_win32/window.h"

using namespace directx12;


namespace gpu_directx12
{


   extern thread_local device* t_pgpudevice;

   context::context()
   {


      //m_vksampler001 = nullptr;
      //m_bOffscreen = true;
      //      m_emode = e_mode_none;
            //m_itaskGpu = 0;
            //m_iLastBitmap1Scan = -1;

            //m_gluTextureBitmap1 = 0;
            //m_VAO = 0;
            //m_VBO = 0;

      m_bMesa = false;

      //m_emode = e_mode_system;

      m_estatus = error_not_initialized;

      //m_physicaldevicefeaturesCreate = {};
      //m_physicaldevicefeaturesEnabled = {};
      //this->logicalDevice() = VK_NULL_HANDLE;
      //m_vkcommandpool = VK_NULL_HANDLE;

      //m_vkqueuePresent = nullptr;
      //m_vkqueueGraphics = nullptr;



   }


   context::~context()
   {

   }



   void context::on_create_context(::gpu::device* pgpudevice, const ::gpu::enum_output& eoutput, ::windowing::window* pwindow, const ::int_size& size)
   {

      m_pgpudevice = pgpudevice;

      if (m_bD3D11On12Shared)
      {

         ::cast < device > pdevice = m_pgpudevice;

         ::cast < context > pcontext = pdevice->main_context();

         auto pdxgidevice = pcontext->_get_dxgi_device();

      }

      ::gpu::context::on_create_context(pgpudevice, eoutput, pwindow, size);




      //// Create an empty root signature.
      //CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
      //rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

      //::comptr<ID3DBlob> signature;
      //::comptr<ID3DBlob> error;

      //::cast < ::gpu_directx12::device > pdevice = m_pgpudevice;
      //
      //::defer_throw_hresult(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
      //::defer_throw_hresult(pdevice->m_pdevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
      //   
      //   __interface_of(&m_prootsignature)));


   }

   //void context::initialize(::particle * pparticle)
   //{

   //   ::e_status estatus = ::object::initialize(pparticle);

   //   if (!estatus)
   //   {

   //      return estatus;

   //   }

   //   return estatus;

   //}


   void context::draw()
   {

      ASSERT(is_current_task());

      //      if (m_VAO != 0)
      //      {
      //
      //         //return ::success;
      //         return;
      //
      //      }
      //      //glGenBuffers(1, &VAO);
      ////      float vertices[] = {f
      //         // positions         // colors
      //  //        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
      //    //     -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
      //      //    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
      //      //};
      //      float vertices[] = {
      //         // first triangle
      //          1.f,  1.f, 0.0f,  // top right
      //          1.f, -1.f, 0.0f,  // bottom right
      //         -1.f,  1.f, 0.0f,  // top left
      //         // second triangle
      //          1.f, -1.f, 0.0f,  // bottom right
      //         -1.f, -1.f, 0.0f,  // bottom left
      //         -1.f,  1.f, 0.0f   // top left
      //      };
      //
      //#if defined(__APPLE__)
      //
      //      glGenVertexArrays(1, &m_VAO);
      //      glGenBuffers(1, &m_VBO);
      //      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
      //      glBindVertexArray(m_VAO);
      //
      //      //#elif defined(__ANDROID__)
      //      //
      //      //      glGenVertexArrays(1, &m_VAO);
      //      //      glGenBuffers(1, &m_VBO);
      //      //      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
      //      //      glBindVertexArray(m_VAO);
      //
      //#else
      //
      ////      glGenVertexArrays(1, &m_VAO);
      ////      glGenBuffers(1, &m_VBO);
      ////      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
      ////      glBindVertexArray(m_VAO);
      ////
      //#endif
      ////
      ////      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
      ////      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      ////
      ////      // position attribute
      ////      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
      ////      glEnableVertexAttribArray(0);
      ////      // color attribute
      ////      //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
      ////      //glEnableVertexAttribArray(1);
      ////
      ////      //return ::success;
   }


   void context::start_drawing()
   {


      //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_elements);
//int iError16 = glGetError();

//int size = 0; 
//glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
//int iError17 = glGetError();

//glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
//int iError18 = glGetError();




      //ASSERT(is_current_task());

      ////      glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

      //      // Clear the screen
      //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      ////glClear(GL_COLOR_BUFFER_BIT);

      //if (m_pprogram && m_pprogram->m_pshader)
      //{

      //   m_pprogram->m_pshader->use();

      //}
      //// Use our shader
      ////glUseProgram(programID);

      //// be sure to activate the shader
      ////glUseProgram(shaderProgram);

      //// update the uniform color
      ////float timeValue = glfwGetTime();
      ////float greenValue = sin(timeValue) / 2.0f + 0.5f;
      ////int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
      ////glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

      ////return ::success;

   }



   void context::global_transform()
   {


      //// Get a handle for our "MVP" uniform
      ////GLint MatrixID = glGetUniformLocation(programID, "MVP");

      //if (m_iMatrixUniform >= 0)
      //{

      //   // Compute the MVP matrix from keyboard and mouse input
      //   //computeMatricesFromInputs();
      //   ::glm::mat4 matrixProjection = (::glm::mat4 &)projection_matrix();
      //   ::glm::mat4 matrixView = (::glm::mat4 &)view_matrix();
      //   ::glm::mat4 matrixModel = glm::mat4(1.0);
      //   ::glm::mat4 matrixMVP = matrixProjection * matrixView * matrixModel;

      //   // Send our transformation to the currently bound shader, 
      //   // in the "MVP" uniform
      //   //glUniformMatrix4fv(m_iMatrixUniform, 1, GL_FALSE, &matrixMVP[0][0]);

      //}

      ////glm::mat4 getViewMatrix() {
      ////   return ViewMatrix;
      ////}
      ////glm::mat4 getProjectionMatrix() {
      ////   return ProjectionMatrix;
      ////}


      ////// Initial position : on +Z
      ////glm::vec3 position = glm::vec3(0, 0, 5);
      ////// Initial horizontal angle : toward -Z
      ////float horizontalAngle = 3.14f;
      ////// Initial vertical angle : none
      ////float verticalAngle = 0.0f;
      ////// Initial Field of View
      ////float initialFoV = 45.0f;

      ////float speed = 3.0f; // 3 units / second
      ////float mouseSpeed = 0.005f;



   }


   void context::render()
   {

      ASSERT(is_current_task());

      //      //#ifdef __APPLE__
      //
      //            //glBindVertexArrayAPPLE(m_VAO);
      //
      //      //#else
      //
      //      if (m_gluTextureBitmap1)
      //      {
      //
      //         //glMatrixMode(GL_PROJECTION);
      //         //glOrtho(0, 800, 0, 600, -1, 1);
      //         // glMatrixMode(GL_MODELVIEW);
      //
      //         //clear and draw quad with texture (could be in display callback)
      //         //glClear(GL_COLOR_BUFFER_BIT);
      //         //glBindTexture(GL_TEXTURE_2D, m_gluTextureBitmap1);
      //         ////glEnable(GL_TEXTURE_2D);
      //         //glBegin(GL_QUADS);
      //         //glTexCoord2i(0, 0); glVertex2f(-1.0, -1.0);
      //         //glTexCoord2i(0, 1); glVertex2f(-1.0, 1.0);
      //         //glTexCoord2i(1, 1); glVertex2f(1.0, 1.0);
      //         //glTexCoord2i(1, 0); glVertex2f(1.0, -1.0);
      //         //glEnd();
      //         //glDisable(GL_TEXTURE_2D);
      //         //glBindTexture(GL_TEXTURE_2D, 0);
      //         //glFlush();
      //
      //         /*GLuint readFboId = 0;
      //         glGenFramebuffers(1, &readFboId);
      //         glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);
      //         glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      //            GL_TEXTURE_2D, m_gluTextureBitmap1, 0);
      //         glBlitFramebuffer(0, 0, m_sizeBitmap1.cx(), m_sizeBitmap1.cy(),
      //            0, 0, m_size.cx(), m_size.cy(),
      //            GL_COLOR_BUFFER_BIT, GL_LINEAR);
      //         glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
      //         glDeleteFramebuffers(1, &readFboId);*/
      //      }
      //      else
      //      {
      //
      ////#ifdef __APPLE__
      ////         glBindVertexArrayAPPLE(m_VAO);
      ////#else
      ////         glBindVertexArray(m_VAO);
      ////#endif
      ////
      ////         glDrawArrays(GL_TRIANGLES, 0, 6);
      //
      //   }
      //
      //      //return ::success;

   }


   class context::d3d11on12* context::d3d11on12()
   {

      if (this != m_pgpudevice->m_pgpucontextMain)
      {

         ::cast < context > pcontextMain = m_pgpudevice->m_pgpucontextMain;

         return pcontextMain->d3d11on12();

      }

      {

         __defer_construct_new(m_pd3d11on12);

         if (!m_pd3d11on12->m_pdxgidevice)
         {

            //          {

          //      ::gpu_directx12::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);

                //m_pgpudevice = ::gpu::swap_chain::m_pgpudevice;

                ///::cast < ::gpu_directx12::device > pdevice = m_pgpudevice;

            assert(command_queue() && "Command queue must be initialized before D3D11On12CreateDevice");

            ::cast < device> pdevice = m_pgpudevice;

            ::cast < context > pcontextMainDraw2d = pdevice->main_draw2d_context();

            D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

            UINT numFeatureLevels = _countof(featureLevels);

            IUnknown* unknowna[] =
            {
               pcontextMainDraw2d->command_queue()
            };

            HRESULT hrD3D11On12 = D3D11On12CreateDevice(
               pdevice->m_pdevice,
               D3D11_CREATE_DEVICE_BGRA_SUPPORT,
               featureLevels,
               numFeatureLevels,
               unknowna,
               1,
               0,
               &m_pd3d11on12->m_pd3d11device,
               &m_pd3d11on12->m_pd3d11context,
               nullptr
            );

            ::defer_throw_hresult(hrD3D11On12);

            ::defer_throw_hresult(m_pd3d11on12->m_pd3d11device.as(m_pd3d11on12->m_pd3d11on12)); // Query interface

            ::defer_throw_hresult(m_pd3d11on12->m_pd3d11device.as(m_pd3d11on12->m_pdxgidevice));

            //::defer_throw_hresult(m_pdxgiswapchain3.as(m_pdxgiswapchain1));

            //DXGI_SWAP_CHAIN_DESC swapchaindesc1{};

            //int FrameCount = 2;

            //if (SUCCEEDED(m_pdxgiswapchain3->GetDesc(&swapchaindesc1)))
            //{

            //   FrameCount = swapchaindesc1.BufferCount;

            //}

            //m_frameIndex = m_pdxgiswapchain3->GetCurrentBackBufferIndex();

            //// Create synchronization objects and wait until assets have been uploaded to the GPU.
            //{
            //   ::defer_throw_hresult(pd3d12device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __interface_of(m_fence)));
            //   m_fenceValue = 1;

            //   // Create an event handle to use for frame synchronization.
            //   m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            //   if (m_fenceEvent == nullptr)
            //   {
            //      ::defer_throw_hresult(HRESULT_FROM_WIN32(GetLastError()));
            //   }

            //}

            ////_defer_d3d11on12_wrapped_resources();
            //// Create descriptor heaps.

            //{
            //   // Describe and create a render target view (RTV) descriptor heap.
            //   D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
            //   rtvHeapDesc.NumDescriptors = FrameCount;
            //   rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            //   rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            //   ::defer_throw_hresult(pd3d12device->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(m_rtvHeap)));

            //   m_rtvDescriptorSize = pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

            //}

            //// Create frame resources.
            //{

            //   CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

            //   // Create a RTV for each frame.
            //   for (UINT n = 0; n < FrameCount; n++)
            //   {

            //      auto & prendertarget = m_renderTargets[n];

            //      ::defer_throw_hresult(
            //         m_pdxgiswapchain1->GetBuffer(
            //            n, __interface_of(prendertarget)));

            //      pd3d12device->CreateRenderTargetView(prendertarget, nullptr, rtvHandle);

            //      rtvHandle.Offset(1, m_rtvDescriptorSize);

            //   }

            //}

            //::draw2d_direct2d::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);



         }

         

      }
      return m_pd3d11on12;

   }

   //::gpu_directx12::texture* context::_layer_source_texture(::gpu::layer* player)
   //{

   //   get_gpu_renderer()->m_pgpurendertarget->current_texture();

   //   

   //}
   


   ID3D12CommandQueue* context::command_queue()
   {

      if (!m_pcommandqueue)
      {

         D3D12_COMMAND_QUEUE_DESC queueDesc = {};
         queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // or COMPUTE, COPY
         queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
         queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; // Use NONE or D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT
         queueDesc.NodeMask = 0; // For single-GPU systems

         ::cast < device > pdevice = m_pgpudevice;

         HRESULT hr = pdevice->m_pdevice->CreateCommandQueue(
            &queueDesc, __interface_of(m_pcommandqueue));
         
         ::defer_throw_hresult(hr);

      }

      return m_pcommandqueue;

   }


   void context::set_bitmap_1(::image::image* pimage)
   {

      ASSERT(is_current_task());

      //      if (!m_gluTextureBitmap1)
      //      {
      //
      ////         glGenTextures(1, &m_gluTextureBitmap1); // generate texture id for your texture (can skip this line)
      ////         glEnable(GL_TEXTURE_2D);
      ////         glBindTexture(GL_TEXTURE_2D, m_gluTextureBitmap1);
      ////
      ////         glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // set alignment of data in memory (a good thing to do before glTexImage)
      ////
      ////#if defined(__APPLE__) || defined(__ANDROID__)
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // set clamp (GL_CLAMP_TO_EDGE would be better)
      ////#else
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // set clamp (GL_CLAMP_TO_EDGE would be better)
      ////#endif
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // set linear filtering (so you can scale your image)
      //
      //      }
      //
      //      if (pimage->is_ok())
      //      {
      //
      //         pimage->map();
      //
      //         //glBindTexture(GL_TEXTURE_2D, m_gluTextureBitmap1);
      //
      //         //if (m_iLastBitmap1Scan != pimage->m_iScan)
      //         //{
      //
      //         //   m_iLastBitmap1Scan = pimage->m_iScan;
      //
      //         //   glPixelStorei(GL_UNPACK_ROW_LENGTH, pimage->m_iScan / 4);
      //
      //         //}
      //
      //         m_sizeBitmap1 = pimage->size();
      //
      //
      //         m_memorySwap.set_size(m_sizeBitmap1.area() * 4);
      //
      //         //vertical_swap_copy_image32_swap_red_blue(
      //            ((image32_t *)m_memorySwap.data())->vertical_swap_copy_swap_red_blue(
      //            m_sizeBitmap1.cx(),
      //            m_sizeBitmap1.cy(),
      //            m_sizeBitmap1.cx() * 4,
      //            pimage->get_data(),
      //            pimage->m_iScan);
      //
      //         //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
      //         //   m_sizeBitmap1.cx(),
      //         //   m_sizeBitmap1.cy(),
      //         //   0, GL_RGBA, GL_UNSIGNED_BYTE,
      //         //   m_memorySwap.data()); // upload image data to the textur
      //
      //
      //
      //
      //
      //      }

   }


   void context::copy(::gpu::texture* ptextureTarget, ::gpu::texture* ptextureSource)
   {

      ::cast < texture > ptextureDst = ptextureTarget;

      ::cast < texture > ptextureSrc = ptextureSource;

      ::cast < renderer > prenderer = ptextureSrc->m_pgpurenderer;

      //auto pcommandbuffer = prenderer->beginSingleTimeCommands(D3D12_COMMAND_LIST_TYPE_DIRECT);

      ::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2();

      pcommandbuffer->wait_commands_to_execute();

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      //texture_guard guard1(pcommandlist, ptextureDst, D3D12_RESOURCE_STATE_COPY_DEST);

      //texture_guard guard2(pcommandlist, ptextureSrc, D3D12_RESOURCE_STATE_COPY_SOURCE);

      ptextureDst->_new_state(pcommandlist, D3D12_RESOURCE_STATE_COPY_DEST);
      ptextureSrc->_new_state(pcommandlist, D3D12_RESOURCE_STATE_COPY_SOURCE);

      //// Transition source to COPY_SOURCE
      //D3D12_RESOURCE_BARRIER barrier1 = {};
      //barrier1.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
      //barrier1.Transition.pResource = ptextureSrc->m_presource;
      //barrier1.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
      //barrier1.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
      //barrier1.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
      //pcommandlist->ResourceBarrier(1, &barrier1);

      //// Transition dest to COPY_DEST
      //D3D12_RESOURCE_BARRIER barrier2 = {};
      //barrier2.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
      //barrier2.Transition.pResource = ptextureDst->m_presource;
      //barrier2.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
      //barrier2.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
      //barrier2.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
      //pcommandlist->ResourceBarrier(1, &barrier2);

      pcommandlist->CopyResource(ptextureDst->m_presource, ptextureSrc->m_presource);

      //pcommandbuffer->submit_command_buffer();

      //// Restore states
      //::swap(barrier2.Transition.StateBefore, barrier2.Transition.StateAfter);
      //pcommandlist->ResourceBarrier(1, &barrier2);

      //::swap(barrier1.Transition.StateBefore, barrier1.Transition.StateAfter);
      //pcommandlist->ResourceBarrier(1, &barrier1);

      //return pcommandbuffer;

   }



   void context::initialize_gpu_context_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow)
   {

      ::cast < ::gpu_directx12::swap_chain > pswapchain = get_swap_chain();

      if (!pswapchain->m_pdxgiswapchain)
      {

         ::cast < device > pdevice = m_pgpudevice;

         ::cast < ::windowing_win32::window > pwin32window = pwindow;

         DXGI_SWAP_CHAIN_DESC1 dxgiswapchaindesc1 = {};
         dxgiswapchaindesc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
         dxgiswapchaindesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
         dxgiswapchaindesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
         dxgiswapchaindesc1.BufferCount = 2;
         dxgiswapchaindesc1.SampleDesc.Count = 1;
         dxgiswapchaindesc1.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;

         RECT rect = {};

         GetWindowRect(pwin32window->m_hwnd, &rect);

         dxgiswapchaindesc1.Width = rect.right - rect.left;
         dxgiswapchaindesc1.Height = rect.bottom - rect.top;

         //D3D12_COMMAND_QUEUE_DESC queueDesc = {};
         //queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
         //m_pdevice->CreateCommandQueue(&queueDesc, __interface_of(m_pcommandqueue));

         ::comptr < IDXGISwapChain1 > swapchain1;

         HRESULT hrCreateSwapChainForComposition =
            pdevice->m_pdxgifactory4->CreateSwapChainForComposition(
               command_queue(),
               &dxgiswapchaindesc1,
               nullptr, // Don’t restrict
               &swapchain1);

         ::defer_throw_hresult(hrCreateSwapChainForComposition);

         //::cast < ::gpu_directx12::swap_chain > pswapchain = get_swap_chain();
         HRESULT hrQueryDxgiSwapChain3 = swapchain1.as(pswapchain->m_pdxgiswapchain);


         //HRESULT hrQueryDxgiSwapChain3 = swapchain1.as(pswapchain->m_pdxgiswapchain);

         pswapchain->get_new_swap_chain_index();

         pswapchain->initialize_swap_chain_window(this, pwindow);

         ::defer_throw_hresult(hrQueryDxgiSwapChain3);

         ::comptr<ID3D12DescriptorHeap> rtvHeap;

         UINT rtvDescriptorSize = 0;

         //int iFrameCount = 2;

         //// Describe and create an RTV descriptor heap.
         //D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
         //rtvHeapDesc.NumDescriptors = iFrameCount; // One per back buffer (typically 2)
         //rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
         //rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // Must be NONE for RTV/DSV heaps

         //HRESULT hr = m_pdevice->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(rtvHeap));
         //::defer_throw_hresult(hr);

         //// Store the descriptor size (used for handle incrementing)
         //rtvDescriptorSize = m_pdevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
         //m_resourceaBackBufferTexture.set_size(iFrameCount);

         //for (int i = 0; i < iFrameCount; i++)
         //{

         //   auto& presource = m_resourceaBackBufferTexture[i];

         //   HRESULT hrGetBuffer = pswapchain->m_pdxgiswapchain3->GetBuffer(i, __interface_of(presource));

         //   ::defer_throw_hresult(hrGetBuffer);
         //   
         //   m_handleaBackBufferRenderTargetView.element_at_grow(i)
         //      = rtvHeap->GetCPUDescriptorHandleForHeapStart(); // RTV descriptor heap assumed created

         //   m_pdevice->CreateRenderTargetView(presource, nullptr, m_handleaBackBufferRenderTargetView[i]);

         //}

      }


   }

   string context::_001GetIntroProjection()
   {

      string strVersion = get_shader_version_text();


      string strProjection =
         "layout(location = 0) in vec3 aPos;\n"
         "out vec3 ourPosition;\n"
         "\n"
         "void main()\n"
         "{\n"
         "   gl_Position = vec4(aPos, 1.0);\n"
         "   ourPosition = aPos;\n"
         "}\n";

      return strProjection;
   }


   string context::_001GetIntroFragment()
   {

      string strVersion = get_shader_version_text();

      string strFragment =
         "uniform vec2 resolution;\n"
         "uniform float time;\n"
         "uniform vec2 mouse;\n"
         "uniform sampler2D backbuffer;\n"
         "\n"
         "void main(void) {\n"
         "float base_res = min(resolution.x(), resolution.y());\n"
         "vec2 uv = (gl_FragCoord.xy * 2.0 - resolution.xy) / base_res;\n"
         "\n"
         //"gl_FragColor = vec4(uv, (uv.x() * uv.x()) / 2.0, ((uv.x() + (base_res - uv.y())) *(uv.x() + (base_res - uv.y()))) / 2.0);\n"
         "float posx = max(0.f, uv.x());\n"
         "float posy = max(0.f, uv.y());\n"
         "gl_FragColor = vec4(uv, (posx * posx) / 4.0, ((posx + posy) * (posx + posy)) / 4.0);\n"
         "}\n";

      return strFragment;

   }

   //   string context::load_fragment(const ::string & pszPath, enum_shader & eshader)
   //   {
   //
   //      ::file::path path(pszPath);
   //
   //      auto & app = papp;
   //
   //      auto & file = app.file();
   //
   //      string strFragment = file.as_string(path);
   //
   //      string strExtension = path.extension();
   //
   //      string strVersion = get_shader_version_text();
   //
   //      if (strExtension.case_insensitive_begins("shadertoy"))
   //      {
   //
   //         eshader = e_shader_shadertoy;
   //
   //         strFragment =
   //            //"#" + strVersion + "\n"
   //            //"\n"
   //            //"precision highp float;\n"
   //            "\n"
   //            "uniform vec2 iResolution;\n"
   //            "uniform float iTime;\n"
   //            "uniform vec2 iMouse;\n"
   //            "uniform sampler2D backbuffer;\n"
   //            "\n"
   //            "\n"
   //            + strFragment;
   //
   //
   //         strFragment +=
   //            "\n\n"
   //            "void main(void)\n"
   //            "{\n"
   //            "   mainImage(gl_FragColor, gl_FragCoord.xy);\n"
   //            "}\n";
   //
   //      }
   //      else
   //      {
   //
   //         strFragment =
   ////            "#" + strVersion + "\n"
   ////            "\n"
   //            + strFragment;
   //
   //         eshader = e_shader_neort;
   //
   //      }
   //
   //      return strFragment;
   //
   //   }


      //string context::get_shader_version_text()
      //{

      //   return "version 330 core";

      //}

      //void context::translate_shader(string& strFragment)
      //{

      //   string_array stra;

      //   stra.add_lines(strFragment);

      //   auto pFind = stra.case_insensitive_find_first_begins("#version ");

      //   if (::is_set(pFind))
      //   {

      //      stra[iFind] = get_shader_version_text();

      //   }
      //   else
      //   {

      //      stra.insert_at(0, get_shader_version_text());

      //   }

      //   _translate_shader(stra);

      //   strFragment = stra.implode("\n");

      //}


      //void context::_translate_shader(string_array& stra)
      //{

      //}


   void context::set_matrix_uniform(const ::gpu::payload& uniformMatrix)
   {

      //m_iMatrixUniform = uniformMatrix.m_iUniform;

   }

   ::pointer < ::gpu::context > allocate_system_context(::particle* pparticle)
   {

      return pparticle->__create_new <context>();

   }


   //context::context()
   //{



   //}


   //context::~context()
   //{

   //}


   bool context::is_mesa()
   {

      return m_bMesa;

   }


   void context::_create_offscreen_window(const ::int_size& size)
   {
      //if (::IsWindow(m_hwnd))
      //{

      //   if (!::SetWindowPos(m_hwnd,
      //      nullptr, 0, 0,
      //      size.cx()
      //      , size.cy(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
      //      | SWP_NOCOPYBITS | SWP_NOSENDCHANGING
      //      | SWP_NOREPOSITION | SWP_NOREDRAW))
      //   {

      //      information() << "SetWindowPos Failed";

      //   }


      //   //return m_hwnd;

      //}
      //else
      //{

      //   LPCTSTR lpClassName = L"draw2d_directx12_offscreen_buffer_window";
      //   LPCTSTR lpWindowName = L"draw2d_directx12_offscreen_buffer_window";
      //   //unsigned int dwStyle = WS_CAPTION | WS_POPUPWINDOW; // | WS_VISIBLE
      //   unsigned int dwExStyle = 0;
      //   unsigned int dwStyle = WS_OVERLAPPEDWINDOW;
      //   dwStyle |= WS_POPUP;
      //   //dwStyle |= WS_VISIBLE;
      //   //dwStyle |= WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
      //   dwStyle &= ~WS_CAPTION;
      //   //dwStyle = 0;
      //   dwStyle &= ~WS_THICKFRAME;
      //   dwStyle &= ~WS_BORDER;
      //   int x = 0;
      //   int y = 0;
      //   int nWidth = size.cx();
      //   int nHeight = size.cy();
      //   HWND hWndParent = nullptr;
      //   HMENU hMenu = nullptr;
      //   HINSTANCE hInstance = ::GetModuleHandleW(L"gpu_directx12.dll");
      //   LPVOID lpParam = nullptr;

      //   m_hwnd = CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

      //   if (!m_hwnd)
      //   {

      //      informationf("MS GDI - CreateWindow failed");

      //      informationf("last-error code: %d\n", GetLastError());

      //      throw ::exception(error_failed);

      //   }

      //   //return m_hwnd;

      //}


   }





   void context::_create_context_directx12(::gpu::device* pgpudeviceParam, const ::gpu::enum_output& eoutput, ::windowing::window* pwindow, const ::int_size& size)
   {

      //createInstance();
      //setupDebugMessenger();
      //createSurface();
      //pickPhysicalDevice();
      //createLogicalDevice();
      //createCommandPool();

      ::cast < device > pgpudevice = pgpudeviceParam;

      if (!pgpudevice)
      {

         throw ::exception(error_failed);

      }


      //::defer_throw_hresult(pgpudevice->m_pdevicecontext.as(m_pcontext));

      //::defer_throw_hresult(pgpudevice->m_pdevicecontext.as(m_pcontext1));

      if (m_eoutput == ::gpu::e_output_cpu_buffer)
      {


      }

      //      auto pphysicaldevice = pgpudevice->m_pphysicaldevice;
      //
      //      assert(pphysicaldevice && pphysicaldevice->m_physicaldevice);
      //      
      //      m_pphysicaldevice = pphysicaldevice;
      //
      //      if (startcontext.m_eoutput == ::gpu::e_output_swap_chain)
      //      {
      //
      //         m_pphysicaldevice->createWindowSurface(startcontext.m_pwindow);
      //
      //      }
      //   
      //      auto physicaldevice = pphysicaldevice->m_physicaldevice;
      //
      //      // Get list of supported extensions
      //      uint32_t extCount = 0;
      //      vkEnumerateDeviceExtensionProperties(physicaldevice, nullptr, &extCount, nullptr);
      //      if (extCount > 0)
      //      {
      //         ::array<VkExtensionProperties> extensions(extCount);
      //         if (vkEnumerateDeviceExtensionProperties(physicaldevice, nullptr, &extCount, extensions.data()) == VK_SUCCESS)
      //         {
      //            for (auto & ext : extensions)
      //            {
      //               m_straSupportedExtensions.add(ext.extensionName);
      //            }
      //         }
      //      }
      //
      //      // Derived examples can enable extensions based on the list of supported extensions read from the physical device
      //      //getEnabledExtensions();
      //
      //      bool useSwapChain = m_eoutput == ::gpu::e_output_swap_chain;
      //
      //      m_itaskGpu = ::current_itask();
      //
      //      VkPhysicalDeviceScalarBlockLayoutFeatures scalarBlockLayoutSupport = {
      //.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES,
      //      .scalarBlockLayout = TRUE};
      //      pgpuapproach->m_pDeviceCreatepNextChain = &scalarBlockLayoutSupport;
      //      m_physicaldevicefeaturesCreate.logicOp = TRUE;
      //
      //      HRESULT result = createLogicalDevice(
      //         m_physicaldevicefeaturesCreate,
      //         pgpuapproach->m_pszaEnabledDeviceExtensions,
      //         pgpuapproach->m_pDeviceCreatepNextChain,
      //         useSwapChain);
      //
      //      if (result != VK_SUCCESS)
      //      {
      //
      //         m_itaskGpu = {};
      //
      //         exitFatal("Could not create DirectX12 device: \n" + errorString(result) + " HRESULT=" + ::as_string(result), result);
      //
      //         throw ::exception(error_failed);
      //
      //      }


            //device = directx12Device->logicalDevice;

   }






   void context::_create_window_context(::windowing::window* pwindowParam)
   {

      //m_itaskGpu = ::current_itask();

      //   ::cast < ::windowing_win32::window > pwindow = pwindowParam;

      //   if (!m_hdc || !m_hrc)
      //   {

      //      auto psystem = system();

      //      auto pgpu = application()->get_gpu();

      //      ::pointer < ::directx12::directx12 > pdirectx12 = pgpu;

      //      //if (!pdirectx12->m_atomClass)
      //      //{

      //      //   informationf("MS GDI - RegisterClass failed");

      //      //   informationf("last-error code: %d\n", GetLastError());

      //      //   throw ::exception(error_failed);

      //      //}

      //      if (!m_hwnd)
      //      {

      //         auto hwnd = pwindow->m_hwnd;


      //         m_hwnd = hwnd;


      //         //// create WGL context, make current

      //         //PIXELFORMATDESCRIPTOR pixformat;

      //         //int chosenformat;

      //         HDC hdc = GetDC(m_hwnd);

      //         //if (!hdc)
      //         //{

      //         //   informationf("MS GDI - GetDC failed");

      //         //   informationf("last-error code: %d\n", GetLastError());

      //         //   throw ::exception(error_failed);

      //         //}

      //         //zero(pixformat);
      //         //pixformat.nSize = sizeof(pixformat);
      //         //pixformat.nVersion = 1;
      //         //pixformat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_DIRECTX12 | PFD_DOUBLEBUFFER;
      //         //pixformat.iPixelType = PFD_TYPE_RGBA;
      //         //pixformat.cColorBits = 32;
      //         //pixformat.cRedShift = 16;
      //         //pixformat.cGreenShift = 8;
      //         //pixformat.cBlueShift = 0;
      //         //pixformat.cAlphaShift = 24;
      //         //pixformat.cAlphaBits = 8;
      //         //pixformat.cDepthBits = 24;
      //         //pixformat.cStencilBits = 8;

      //         //chosenformat = ChoosePixelFormat(hdc, &pixformat);

      //         //if (chosenformat == 0)
      //         //{

      //         //   informationf("MS GDI - ChoosePixelFormat failed");

      //         //   informationf("last-error code: %d\n", GetLastError());

      //         //   ReleaseDC(m_hwnd, hdc);

      //         //   throw ::exception(error_failed);

      //         //}

      //         //bool spfok = SetPixelFormat(hdc, chosenformat, &pixformat);

      //         //if (!spfok)
      //         //{

      //         //   informationf("MS GDI - SetPixelFormat failed");

      //         //   informationf("last-error code: %d\n", GetLastError());

      //         //   ReleaseDC(m_hwnd, hdc);

      //         //   throw ::exception(error_failed);

      //         //}

      ////         auto hglrc = wglCreateContext(hdc);

      ////         pwindow->m_hglrcProto = hglrc;

      //////         int context_attribs[] = {
      //////WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
      //////WGL_CONTEXT_MINOR_VERSION_ARB, 1,
      //////0, 0
      //////         };
      //////         auto hglrc = wglCreateContextAttribsARB(hdc, NULL, context_attribs);
      //////         if (!hglrc) {
      //////            //ReleaseDC(hWnd, hDC);
      //////            //DestroyWindow(hWnd);

      //////            throw ::exception(error_failed);
      //////         }
      //////         //ReleaseDC(hWnd, hDC);

      ////         if (!pwindow->m_hglrcProto)
      ////         {

      ////            informationf("MS WGL - wglCreateContext failed");

      ////            informationf("last-error code: %d\n", GetLastError());

      ////            ReleaseDC(m_hwnd, hdc);

      ////            throw ::exception(error_failed);

      ////         }

      ////         bool bMakeCurrentOk = wglMakeCurrent(hdc, pwindow->m_hglrcProto);

      ////         if (!bMakeCurrentOk)
      ////         {

      ////            informationf("MS WGL - wglMakeCurrent failed");

      ////            informationf("last-error code: %d\n", GetLastError());

      ////            ReleaseDC(m_hwnd, hdc);

      ////            throw ::exception(error_failed);

      ////         }


      ////         pdirectx12->defer_init_gpu_library();

      //         //auto pszVersion = (const char *)glGetString(GL_VERSION);
      //         ////::e_status estatus = 

      //         //::string strVersion(pszVersion);

      //         //if (strVersion.case_insensitive_contains("mesa"))
      //         //{

      //         //   m_bMesa = true;

      //         //}

      //         ////if (!estatus)
      //         ////{

      //         ////   ReleaseDC(window, hdc);

      //         ////   return estatus;

      //         ////}

      //         m_hwnd = m_hwnd;
      //         m_hdc = hdc;
      //         m_hrc = pwindow->m_hglrcProto;

      //      }

      //   }

      //   RECT rectClient;

      //   ::GetClientRect(m_hwnd, &rectClient);

      //   ::int_size sizeNew = { rectClient.right - rectClient.left,
      //rectClient.bottom - rectClient.top };
      //   
      //   if (m_size != sizeNew)
      //   {
      //      m_size = sizeNew;


      //      //HDC pdcDIB;                      // контекст устройства в памяти
      //      //HBITMAP hbmpDIB;                 // и его текущий битмапvoid *pBitsDIB(NULL);            // содержимое битмапаint cxDIB(200); int cyDIB(300);  // его размеры (например для окна 200х300)
      //      //auto &BIH=pwindow->m_bitmapinfoheaderProto;            // и заголовок// …// создаем DIB section// создаем структуру BITMAPINFOHEADER, описывающую наш DIBint iSize = sizeof(BITMAPINFOHEADER);  // размер
      //      //memset(&BIH, 0, sizeof(pwindow->m_bitmapinfoheaderProto));

      //      //BIH.biSize = sizeof(pwindow->m_bitmapinfoheaderProto);        // размер структуры
      //      //BIH.biWidth = m_size.cx();       // геометрия
      //      //BIH.biHeight = m_size.cy();      // битмапа
      //      //BIH.biPlanes = 1;          // один план
      //      //BIH.biBitCount = 32;       // 24 bits per pixel
      //      //BIH.biCompression = BI_RGB;// без сжатия// создаем новый DC в памяти
      //      ////pdcDIB = CreateCompatibleDC(NULL);
      //      ////void * pBits = nullptr;
      //      //// создаем DIB-секцию
      //      //pwindow->m_hbitmapProto = CreateDIBSection(
      //      //  m_hdc,                  // контекст устройства
      //      //  (BITMAPINFO *)&BIH,       // информация о битмапе
      //      //  DIB_RGB_COLORS,          // параметры цвета
      //      //  &pwindow->m_pbitsProto,               // местоположение буфера (память выделяет система)
      //      //  NULL,                    // не привязываемся к отображаемым в память файлам
      //      //  0);

      //      //// выберем новый битмап (DIB section) для контекста устройства в памяти
      //      //SelectObject(m_hdc, pwindow->m_hbitmapProto);
      //      //pwindow->m_hdcProto = m_hdc;
      //   }

      //   m_itaskGpu = ::current_itask();

      //   m_estatus = ::success;

      //   set_ok_flag();

   }


   void context::_create_cpu_buffer(const ::int_size& size)
   {

      _create_offscreen_window(size);

      //_create_window_buffer();

      //m_itaskGpu = ::current_itask();

   }


   void context::defer_create_window_context(::windowing::window* pwindow)
   {

      //if (m_hrc)
      //{

      //   return;

      //}

      //::directx12::context::defer_create_window_context(pwindow);

   }


   void context::_defer_create_window_context(::windowing::window* pwindow)
   {

      _create_window_context(pwindow);

   }


   void context::resize_cpu_buffer(const ::int_size& sizeParam)
   {

      if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      {

         return;

      }

      auto size(sizeParam);

      send([this, size]()
         {
            //if (!m_pcpubuffer)

            create_cpu_buffer(size);


            ///m_pcpubuffer->m_pixmap.create(m_pcpubuffer->m_memory, size);

      //#ifdef WINDOWS_DESKTOP
      //
      //      ::SetWindowPos(m_hwnd, 0, 0, 0, size.cx(), size.cy(), SWP_NOZORDER | SWP_NOMOVE | SWP_HIDEWINDOW);
      //
      //#else

      //      destroy_offscreen_buffer();
      //
      //      if(!create_offscreen_buffer(size))
      //      {
      //
      //         return error_failed;
      //
      //      }

      //#endif

            //make_current();

            //glViewport(0, 0, size.cx(), size.cy());
            //glMatrixMode(GL_PROJECTION);
            //glLoadIdentity();
            //glOrtho(0, size.cx(), 0, size.cy(), -10, 10);
            //glMatrixMode(GL_MODELVIEW);
            //glutPostRedisplay();

            //return ::success;
         });


   }



   void context::destroy_cpu_buffer()
   {

      //ASSERT(is_current_task());

      //::e_status estatus = ::success;

      //if (m_hrc == NULL && m_hdc == NULL && m_hwnd == NULL)
      //{

      //   return;

      //}

      ////wglMakeCurrent(nullptr, nullptr);
      ////wglDeleteContext(m_hrc);
      //::ReleaseDC(m_hwnd, m_hdc);
      //::DestroyWindow(m_hwnd);
      //m_size.set(0, 0);
      //m_hrc = NULL;
      //m_hwnd = NULL;
      //m_hdc = NULL;

      ////return estatus;

   }



   string context::get_shader_version_text()
   {

      return "#version 330 core";

   }


   void context::_translate_shader(string_array& stra)
   {

      ::gpu::context::_translate_shader(stra);

      character_count iFindPrecision = stra.case_insensitive_find_first_begins("precision ");

      if (iFindPrecision >= 0)
      {

         stra[iFindPrecision] = "precision highp float;";

      }
      else
      {

         stra.insert_at(1, "precision highp float;");

         iFindPrecision = 1;

      }

   }





   //::gpu_directx12::descriptor_pool* context::get_global_pool(int iFrameCount)
   //{

   //   return m_pdescriptorpoolGlobal;

   //}


   void context::create_global_ubo(int iGlobalUboSize, int iFrameCount)
   {

      ::cast < renderer > prenderer = m_pgpurenderer;

      ::cast < device > pgpudevice = m_pgpudevice;

      CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

      CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(
         ::directx12::Align256(iGlobalUboSize));

      pgpudevice->m_pdevice->CreateCommittedResource(
         &heapProps,
         D3D12_HEAP_FLAG_NONE,
         &bufferDesc,
         D3D12_RESOURCE_STATE_COMMON,
         nullptr,
         __interface_of(prenderer->m_presourceGlobalUBO));

      D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};

      cbvDesc.BufferLocation = prenderer->m_presourceGlobalUBO->GetGPUVirtualAddress();

      cbvDesc.SizeInBytes = ::directx12::Align256(iGlobalUboSize); // must be 256-byte aligned

      auto handle = prenderer->m_pheapCbv->GetCPUDescriptorHandleForHeapStart();

      pgpudevice->m_pdevice->CreateConstantBufferView(&cbvDesc, handle);

      CD3DX12_RANGE readRange(0, 0);

      prenderer->m_presourceGlobalUBO->Map(
         0, &readRange,
         &prenderer->m_pGlobalUBO);

   }


   void context::update_global_ubo(const ::block& block)
   {

      auto iFrameIndex = m_pgpurenderer->m_pgpurendertarget->get_frame_index();


      //MyGlobalData globalData = { /* your values */ };
      ::cast < renderer > prenderer = m_pgpurenderer;
      //      UINT8* mappedPtr = nullptr;
        //    D3D12_RANGE readRange = {}; // no read access
          //  m_uboBuffers[iFrameIndex]->m_presourceBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mappedPtr));
            //memcpy(mappedPtr, block.data(), block.size());
      auto dataTarget = prenderer->m_pGlobalUBO;
      memcpy(dataTarget, block.data(), block.size());


      //m_uboBuffers[iFrameIndex]->m_presourceBuffer->Unmap(0, nullptr);

      //m_uboBuffers[iFrameIndex]->writeToBuffer(block.data());

      //m_uboBuffers[iFrameIndex]->flush();

      //ID3D11Buffer* globalUBOBuffer = nullptr;
      //D3D11_BUFFER_DESC cbd = {};
      //cbd.Usage = D3D11_USAGE_DYNAMIC;
      //cbd.ByteWidth = block;
      //cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      //cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      //device->CreateBuffer(&cbd, nullptr, &globalUBOBuffer);
      //if (m_pbufferGlobalUbo)
      //{

      //   D3D11_MAPPED_SUBRESOURCE mapped;
      //   m_pcontext->Map(m_pbufferGlobalUbo, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
      //   memcpy(mapped.pData, block.data(), block.size());
      //   m_pcontext->Unmap(m_pbufferGlobalUbo, 0);


      //}

      //m_pbufferGlobalUbo
      //m_pbufferGlobalUbo

   }


   void context::engine_on_frame_context_initialization()
   {

      //// Global UBO descriptors
      //if (!m_psetdescriptorlayoutGlobal)
      //{

      //   auto pgpurenderer = get_renderer(::gpu::e_scene_3d);

      //   m_psetdescriptorlayoutGlobal = set_descriptor_layout::Builder(this)
      //      .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
      //      .build();

      //   auto iFrameCount = pgpurenderer->get_frame_count();

      //   m_descriptorsetsGlobal.resize(iFrameCount);

      //   auto pdescriptorpoolbuilder = __allocate::gpu_directx12::descriptor_pool::Builder();

      //   pdescriptorpoolbuilder->initialize_builder(this);
      //   pdescriptorpoolbuilder->setMaxSets(iFrameCount);
      //   pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, iFrameCount);

      //   m_pdescriptorpoolGlobal = pdescriptorpoolbuilder->build();

      //}



   }


   //VkDescriptorSet context::getGlobalDescriptorSet(::gpu_directx12::renderer* prenderer)
   //{

   //   //if (m_globalDescriptorSets.is_empty())
   //   //{


   //   //}

   //   return m_descriptorsetsGlobal[prenderer->get_frame_index()];

   //}
   //   


   //ID3D11DeviceContext* context::draw_get_d3d11_device_context()
   //{

   //   return m_pcontext;

   //}


   //ID3D11DeviceContext1* context::draw_get_d3d11_device_context1()
   //{

   //   return m_pcontext1;

   //}


   void context::__bind_draw2d_compositor(::gpu::compositor* pgpucompositor, ::gpu::layer* player)
   {

      ASSERT(m_etype == e_type_draw2d);
      
      ::cast < ::dxgi_surface_bindable > pdxgisurfacebindable = pgpucompositor;

      ::cast < texture > ptexture = player->source_texture();

      auto pdxgidevice = _get_dxgi_device();

      auto iFrameIndex = m_pgpurenderer->m_pgpurendertarget->get_frame_index();

      auto etypeRenderer = m_pgpurenderer->m_pgpucontext->m_etype;

      auto etypeCompositor = pgpucompositor->gpu_context()->m_etype;

      auto & pdxgisurface = ptexture->d3d11()->dxgiSurface;

      ::cast < context > pcontextMain = m_pgpudevice->main_context();

      if (!ptexture->d3d11()->wrappedResource)
      {

         assert(!ptexture->m_pheapDepthStencilView);
         //assert(!ptexture->m_pheapRenderTargetView);
         //assert(!ptexture->m_pheapShaderResourceView);
         //assert(!ptexture->m_pheapSampler);

         //auto & sharedHandle= ptexture->d3d11()->sharedHandle;

         //::defer_throw_hresult(pdevice->m_pdevice->CreateSharedHandle(
         //   ptexture->m_presource, nullptr, GENERIC_ALL, nullptr, 
         //   &sharedHandle));

         D3D11_RESOURCE_FLAGS flags = {};
         //flags.BindFlags = D3D11_BIND_RENDER_TARGET;
         flags.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
         assert(ptexture->m_presource); // Confirm it’s non-null
         HRESULT hrCreateWrappedResource = d3d11on12()->m_pd3d11on12->CreateWrappedResource(
            ptexture->m_presource,
            &flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            __interface_of(ptexture->d3d11()->wrappedResource)
         );

         ::defer_throw_hresult(hrCreateWrappedResource);

      }

      ptexture->d3d11()->m_d3d11wrappedresources[0] = { ptexture->d3d11()->wrappedResource.m_p };

      ::cast < renderer > prenderer = m_pgpurenderer;



      //::cast < texture > ptexture = m_pgpurenderer->m_pgpurendertarget->current_texture();

      //ptexture->_new_state(prenderer->getCurrentCommandBuffer2()->m_pcommandlist, D3D12_RESOURCE_STATE_RENDER_TARGET);


      d3d11on12()->m_pd3d11on12->AcquireWrappedResources(
         ptexture->d3d11()->m_d3d11wrappedresources,
         _countof(ptexture->d3d11()->m_d3d11wrappedresources));

      m_iResourceWrappingCount++;

      ASSERT(m_iResourceWrappingCount == 1);

      //::defer_throw_hresult(m_pd3d11device.as(m_pd3d11on12)); // Query interface

      ::defer_throw_hresult(ptexture->d3d11()->wrappedResource.as(pdxgisurface)); // Get IDXGISurface

      pdxgisurfacebindable->_bind(iFrameIndex, player->m_iLayerIndex, pdxgisurface);

   }


   void context::__soft_unbind_draw2d_compositor(::gpu::compositor* pgpucompositor, ::gpu::layer* player)
   {

      ::cast < ::dxgi_surface_bindable > pdxgisurfacebindable = pgpucompositor;

      ::cast < texture > ptexture = player->source_texture();

      ::cast < device > pdevice = m_pgpudevice;

      auto pdxgidevice = _get_dxgi_device();

      auto iFrameIndex = m_pgpurenderer->m_pgpurendertarget->get_frame_index();

      auto& pdxgisurface = ptexture->d3d11()->dxgiSurface;

      d3d11on12()->m_pd3d11context->Flush();

      // Assume you already have a ID3D12Fence*

      auto& context4 = d3d11on12()->dx11Context4;
      auto& dx11Fence = d3d11on12()->dx11Fence;
      auto& dx12Fence = d3d11on12()->dx12Fence;
      auto& fenceValue = d3d11on12()->fenceValue;
      auto& fenceEvent = d3d11on12()->fenceEvent;

      if (!dx12Fence)
      {

         HRESULT hrCreateFence = pdevice->m_pdevice->CreateFence(
            fenceValue,                // Initial value
            D3D12_FENCE_FLAG_SHARED,     // Flags (can be SHARED or NONE)
            __interface_of(dx12Fence)       // Out pointer
         );
         ::defer_throw_hresult(hrCreateFence);

         
         auto &sharedFenceHandle = d3d11on12()->sharedFenceHandle;
         HRESULT hrCreateSharedHandle = pdevice->m_pdevice->CreateSharedHandle(
            dx12Fence,
            nullptr, // Security attributes
            GENERIC_ALL,
            nullptr, // Optional name
            &sharedFenceHandle);
         ::defer_throw_hresult(hrCreateSharedHandle);

         auto& pdevice5 = d3d11on12()->dx11Device5;
         // Query ID3D11Device5 from the D3D11On12 device
         HRESULT hrDev5 = d3d11on12()->m_pd3d11device.as(pdevice5);
         ::defer_throw_hresult(hrDev5);
         // Create an ID3D11Fence from the ID3D12Fence
         HRESULT hrOpenSharedFence = pdevice5->OpenSharedFence(
            sharedFenceHandle, __interface_of(dx11Fence));
         
         auto& pcontext = d3d11on12()->m_pd3d11context;
         HRESULT hrCtx4 = pcontext.as(context4);
         ::defer_throw_hresult(hrCtx4);
      }

      fenceValue++;
      context4->Signal(
         dx11Fence,
         fenceValue);

      if (dx12Fence->GetCompletedValue() < fenceValue)
      {

         if (!fenceEvent)
         {

            fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

         }
         ::ResetEvent(fenceEvent);
         dx12Fence->SetEventOnCompletion(
            fenceValue, fenceEvent);
         WaitForSingleObject(fenceEvent, INFINITE);
         //CloseHandle(fenceEvent);
      }

      if (ptexture->d3d11()->wrappedResource)
      {

         d3d11on12()->m_pd3d11on12->ReleaseWrappedResources(
            ptexture->d3d11()->m_d3d11wrappedresources, 1);

         m_iResourceWrappingCount--;

         //ptexture->m_estate = D3D12_RESOURCE_STATE_COPY_SOURCE;

      }

      ASSERT(m_iResourceWrappingCount == 0);



   }


   void context::__bind_graphics3d_compositor(::gpu::compositor* pgpucompositor, ::gpu::layer* player)
   {

      ASSERT(m_etype != e_type_draw2d);

      //::cast < texture > ptexture = player->source_texture();

      //auto iFrameIndex = m_pgpurenderer->m_pgpurendertarget->get_frame_index();

      //auto etypeRenderer = m_pgpurenderer->m_pgpucontext->m_etype;

      //auto etypeCompositor = pgpucompositor->m_pgpucontextCompositor->m_etype;

      //::cast < command_buffer > pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2();

      //if (!ptexture->m_handleRenderTargetView.ptr)
      //{

      //   ptexture->create_render_target();

      //}

      //D3D12_CPU_DESCRIPTOR_HANDLE handleaRTV[] = {
      //   ptexture->m_handleRenderTargetView
      //};

      //if (!ptexture->m_handleDepthStencilView.ptr)
      //{

      //   ptexture->create_depth_resources();

      //}

      //D3D12_CPU_DESCRIPTOR_HANDLE handleaDSV[] = {
      //   ptexture->m_handleDepthStencilView
      //};

      //pcommandbuffer->m_pcommandlist->OMSetRenderTargets(1, handleaRTV, FALSE, handleaDSV);

   }


   ::gpu::enum_output context::get_eoutput()
   {

      if (m_pgpudevice->m_edevicetarget == ::gpu::e_device_target_swap_chain)
      {

         return ::gpu::e_output_gpu_buffer_to_swap_chain;

      }
      else
      {

         return ::gpu::e_output_gpu_buffer;

      }

   }


   void context::on_create_texture(::gpu::texture * pgputexture)
   {

      ::gpu::context::on_create_texture(pgputexture);

      //::cast < texture > ptexture = pgputexture;

      //DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
      //// 1. Create the texture resource
      //D3D12_RESOURCE_DESC textureDesc = {};
      //textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
      //textureDesc.Width = size.width();
      //textureDesc.Height = size.height();
      //textureDesc.MipLevels = 1;
      //textureDesc.DepthOrArraySize = 1;
      //textureDesc.Format = format;
      //textureDesc.SampleDesc.Count = 1;
      //textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
      //textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

      //D3D12_CLEAR_VALUE clearValue = {};
      //clearValue.Format = format;
      //clearValue.Color[0] = 0.5f * 0.5f;
      //clearValue.Color[1] = 0.75f * 0.5f;
      //clearValue.Color[2] = 0.9f * 0.5f;
      //clearValue.Color[3] = 0.5f;

      ////clearValue.Color = { 0.5f, 0.75f, 0.9f, 0.5f };

      //::cast < ::gpu_directx12::device > pdevice = m_pgpudevice;

      //CD3DX12_HEAP_PROPERTIES heapproperties(D3D12_HEAP_TYPE_DEFAULT);

      //HRESULT hrCreateCommittedResource = pdevice->m_pdevice->CreateCommittedResource(
      //   &heapproperties,
      //   D3D12_HEAP_FLAG_NONE,
      //   &textureDesc,
      //   D3D12_RESOURCE_STATE_RENDER_TARGET,
      //   &clearValue,
      //   __interface_of(presource));

      //pdevice->defer_throw_hresult(hrCreateCommittedResource);

      //return presource;

   }


   
   //void context::composition_store()
   //{

   //   ID3D12Resource* presourceSource = nullptr;
   //   ::cast < renderer > prenderer = m_pgpurenderer;
   //   ::cast < render_target_view > prendertargetview = prenderer->m_pgpurendertarget;
   //   auto ptexture = prendertargetview->current_texture();
   //   if (m_edrawing == e_drawing_draw2d)
   //   {
   //      if (!ptexture->m_pd3d1)
   //      {

   //         throw ::exception(error_wrong_state);

   //      }
   //      presourceSource = ptexture->m_presource;

   //   }
   //   else
   //   {

   //      throw ::exception(error_wrong_state);
   //   }
   //   auto psnapshot = __allocate snapshot(_take_snapshot(presourceSource,)
   //   m_resourceaSnapshot.add();


   //}


   void context::on_begin_draw_attach(::gpu::graphics* pgpugraphics)
   {

      ::gpu::context::on_begin_draw_attach(pgpugraphics);

   }


   void context::draw2d_on_begin_draw(::gpu::graphics* pgpugraphics)
   {

      ::gpu::context::draw2d_on_begin_draw(pgpugraphics);

   }



   void context::merge_layers(::gpu::texture* ptextureTarget, ::pointer_array < ::gpu::layer >* playera)
   {

      if (!m_pshaderBlend3)
      {

         const char* full_screen_triangle_vertex_shader = R"hlsl(
// vertex.hlsl
struct VSOut {
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

VSOut main(uint id : SV_VertexID)
{
    float2 verts[3] = {
        float2(-1, -1),
        float2(-1,  3),
        float2( 3, -1)
    };

    float2 uv[3] = {
        float2(0, 1),
        float2(0, -1),
        float2(2, 1)
    };

    VSOut o;
    o.pos = float4(verts[id], 0, 1);
    o.uv  = uv[id];
    return o;
}
)hlsl";

         const char* full_screen_triangle_fragment_shader = R"hlsl(  
// pixel.hlsl
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_TARGET
{
    //return tex.Sample(samp, uv); // Assumes premultiplied alpha
//if(uv.y >0.5)
//{
  // return float4(0.7*0.5, 0.5*0.5, 0.98*0.5, 0.5); // test if the shader pipeline is running
//}
//else
//{
return tex.Sample(samp, uv);
//}
}
)hlsl";

         __defer_construct_new(m_pshaderBlend3);

         m_pshaderBlend3->m_bEnableBlend = true;
         m_pshaderBlend3->m_bTextureAndSampler = true;
         m_pshaderBlend3->m_bDisableDepthTest = true;

         m_pshaderBlend3->initialize_shader_with_block(
            m_pgpurenderer,
            ::as_block(full_screen_triangle_vertex_shader),
            ::as_block(full_screen_triangle_fragment_shader)
            //,
            //{},
            //{},
            //{},
            //{},
            //copy_using_shader_input_layout_properties()
         );

      }


      //if (!m_pd3d11blendstateBlend3)
      //{

      //   D3D12_BLEND_DESC blendDesc = { 0 };
      //   blendDesc.RenderTarget[0].BlendEnable = TRUE;
      //   blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;              // Premultiplied alpha
      //   blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;   // Use inverse of alpha
      //   blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

      //   blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;         // Alpha blending (optional)
      //   blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
      //   blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

      //   blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

      //   ::cast < ::gpu_directx11::device > pgpudevice = m_pgpudevice;

      //   HRESULT hr = pgpudevice->m_pdevice->CreateBlendState(&blendDesc, &m_pd3d11blendstateBlend3);
      //   ::defer_throw_hresult(hr);

      //}

      ::cast < renderer > prenderer = m_pgpurenderer;

      ::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2();

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      ::cast <texture > ptextureDst = ptextureTarget;
      ////float clearColor[4] = { 0.95f * 0.5f, 0.95f * 0.5f, 0.25f * 0.5f, 0.5f }; // Clear to transparent
      ////m_pcontext->ClearRenderTargetView(ptextureDst->m_prendertargetview, clearColor);
      float clearColor[4] = { 0.f, 0.f, 0.f, 0.f }; // Clear to transparent
      pcommandlist->ClearRenderTargetView(ptextureDst->m_handleRenderTargetView, clearColor, 0, nullptr);
      ptextureDst->_new_state(pcommandlist, D3D12_RESOURCE_STATE_RENDER_TARGET);

      if(!ptextureDst->m_handleRenderTargetView.ptr)
      {

         ptextureDst->create_render_target();

      }


      m_pshaderBlend3->bind(ptextureTarget);


      //int iDescriptorSize = ptextureDst->m_rtvDescriptorSize;
      //int iFrameIndex = m_pgpurendertarget->get_frame_index();
      //auto hRtv = pgpurendertargetview->m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
      //auto hRtv = ptextureDst->m_handleRenderTargetView;
      //CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        // hRtv,
         //iFrameIndex,
         //iDescriptorSize);

      //float clearColor[4] = { 0.5f * 0.5f, 0.75f * 0.5f, 0.9f * 0.5f, 0.5f };
      //float clearColor[4] = { 0.f, 0.f , 0.f, 0.f };
      //pcommandlist->ClearRenderTargetView(hRtv, clearColor, 0, nullptr);

      //{
      //   float blendFactor[4] = { 0, 0, 0, 0 }; // Ignored with this blend mode
      //   UINT sampleMask = 0xFFFFFFFF;
      //   m_pcontext->OMSetBlendState(m_pd3d11blendstateBlend3, blendFactor, sampleMask);
      //}



      //ID3D11RenderTargetView* rendertargetview[] = { ptextureDst->m_prendertargetview };

      //m_p(1, rendertargetview, nullptr);

      //m_pcontext->OMSetBlendState(g_blendState, nullptr, 0xffffffff);
      //g_context->VSSetShader(g_vs, nullptr, 0);
      //g_context->PSSetShader(g_ps, nullptr, 0);
      //g_context->PSSetSamplers(0, 1, &g_sampler);

      if (1)
      {
         int iLayer = 0;
         for (auto player : *playera)
         {

            //if (iLayer == 2)
            {


               ::cast <texture > ptextureSrc = player->texture();

               ptextureSrc->_new_state(pcommandlist, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

               m_pshaderBlend3->bind_source(ptextureSrc);

               //ID3D11SamplerState* samplerstatea[] =
               //{ ptexture->m_psamplerstate };
               //ID3D11ShaderResourceView* sharedresourceviewa[] =
               //{ ptexture->m_pshaderresourceview };

                              // 1. Define viewport and scissor rectangle
               D3D12_VIEWPORT viewport = {};
               viewport.TopLeftX = (FLOAT) ptextureSrc->m_rectangleTarget.left();
               viewport.TopLeftY = (FLOAT) ptextureSrc->m_rectangleTarget.top();
               viewport.Width = static_cast<float>(ptextureSrc->m_rectangleTarget.width());
               viewport.Height = static_cast<float>(ptextureSrc->m_rectangleTarget.height());
               viewport.MinDepth = 0.0f;
               viewport.MaxDepth = 1.0f;

               D3D12_RECT scissorRect = {};
               scissorRect.left = ptextureSrc->m_rectangleTarget.left();
               scissorRect.top = ptextureSrc->m_rectangleTarget.top();
               scissorRect.right = ptextureSrc->m_rectangleTarget.right();
               scissorRect.bottom = ptextureSrc->m_rectangleTarget.bottom();


               //// 4. Set the viewport and scissor
               pcommandlist->RSSetViewports(1, &viewport);
               pcommandlist->RSSetScissorRects(1, &scissorRect);
               //D3D11_VIEWPORT vp = {};
               //vp.TopLeftX = ptexture->m_rectangleTarget.left();
               //vp.TopLeftY = ptexture->m_rectangleTarget.top();
               //vp.Width = static_cast<float>(ptexture->m_rectangleTarget.width());
               //vp.Height = static_cast<float>(ptexture->m_rectangleTarget.height());
               //vp.MinDepth = 0.0f;
               //vp.MaxDepth = 1.0f;
               //m_pcontext->RSSetViewports(1, &vp);

               //m_pcontext->PSSetSamplers(0, 1, samplerstatea);
               //m_pcontext->PSSetShaderResources(0, 1, sharedresourceviewa);


               pcommandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
               //pcommandlist->Draw(3, 0); // Fullscreen triangle
               pcommandlist->DrawInstanced(3, 1, 0, 0);
            }
            iLayer++;

         }
         
      }
      //}

      m_pshaderBlend3->unbind();

      ////::cast <texture > ptextureDst = ptextureTarget;
      //{
      //   
      //   float clearColor2[4] = { 0.95f * 0.5f, 0.75f * 0.5f, 0.95f * 0.5f, 0.5f }; // Clear to transparent
      //   
      //   D3D12_RECT r[1];
      //   
      //   r[0].left = 100;
      //   r[0].top = 200;
      //   r[0].right = 200;
      //   r[0].bottom = 300;
      //   
      //   pcommandlist->ClearRenderTargetView(
      //      ptextureDst->m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart(),
      //      clearColor2,
      //      1, r);

      //}

   }


   void context::on_start_layer(::gpu::layer* player)
   {

      ::cast < ::gpu_directx12::renderer > prenderer = m_pgpurenderer;

      if (player->m_pcommandbufferLayer != prenderer->getCurrentCommandBuffer2())
      {

         player->m_pcommandbufferLayer = prenderer->getCurrentCommandBuffer2();

         //auto pcommanbuffer=__create_new < ::gpu_directx12::command_buffer>();

         //::comptr < ID3D12CommandQueue > pcommandqueue;

         //::cast < device > pdevice = m_pgpudevice;

         //D3D12_COMMAND_QUEUE_DESC queueDesc = {};
         //queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
         //HRESULT hrCreateCommandQueue = 
         //   pdevice->m_pdevice->CreateCommandQueue(
         //      &queueDesc, __interface_of(pcommandqueue));

         //::defer_throw_hresult(hrCreateCommandQueue);

         //::cast < ::gpu_directx12::renderer > prenderer = m_pgpurenderer;

         //pcommanbuffer->initialize_command_buffer(
         //   pcommandqueue, D3D12_COMMAND_LIST_TYPE_DIRECT, prenderer);

         //player->m_pcommandbuffer = pcommanbuffer;

      }

      if (m_pgpurenderer->m_iSentLayerCount)
      {

         player->m_pcommandbufferLayer->wait_commands_to_execute();

      }

      //player->m_pcommandbufferLayer->reset();

      if (m_pgpucompositor)
      {

         if (m_etype == e_type_draw2d)
         {

            //::cast < device > pdevice = m_pgpudevice;
            //::cast < renderer > prenderer = m_pgpurenderer;
            //::cast < texture > ptexture = m_pgpurenderer->m_pgpurendertarget->current_texture();

            //_get_dxgi_device();

            ////ptexture->_new_state(prenderer->getCurrentCommandBuffer2()->m_pcommandlist, D3D12_RESOURCE_STATE_RENDER_TARGET);

            //// 4. Release wrapped resource to allow access from D3D12

            //d3d11on12()->m_pd3d11on12->AcquireWrappedResources(
            //   d3d11on12()->m_d3d11wrappedresources, 1);

            //m_iResourceWrappingCount++;

            //ASSERT(m_iResourceWrappingCount == 1);
            //
            ////::cast < ::dxgi_surface_bindable > pdxgisurfacebindable = m_pgpucompositor;

            ////::cast < texture > ptexture = m_pgpurenderer->m_pgpurendertarget->current_texture();

            ////auto& pdxgisurface = ptexture->d3d11()->dxgiSurface;

            ////::defer_throw_hresult(ptexture->d3d11()->wrappedResource.as(pdxgisurface)); // Get IDXGISurface

            ////int iFrameIndex = m_pgpurenderer->m_pgpurendertarget->get_frame_index();

            ////pdxgisurfacebindable->_bind(iFrameIndex, pdxgisurface);

            __bind_draw2d_compositor(m_pgpucompositor, player);

         }
         else
         {

            __bind_graphics3d_compositor(m_pgpucompositor, player);

         }

         m_pgpucompositor->on_start_layer();

      }

   }


   void context::on_end_layer(::gpu::layer* player)
   {

      if (m_pgpucompositor)
      {

         m_pgpucompositor->on_end_layer();

         if (m_etype == e_type_draw2d)
         {

            __soft_unbind_draw2d_compositor(m_pgpucompositor, player);

            //::cast < device > pdevice = m_pgpudevice;

            //if (m_etype == e_type_draw2d)
            //{

            //   d3d11on12()->m_pd3d11context->Flush(); // ✅ Ensures D3D11 commands are issued

            //   // 4. Release wrapped resource to allow access from D3D12
            //   d3d11on12()->m_pd3d11on12->ReleaseWrappedResources(
            //      d3d11on12()->m_d3d11wrappedresources, 1);

            //   m_iResourceWrappingCount--;

            //   ASSERT(m_iResourceWrappingCount == 0);

            //   ::cast < texture > ptexture = get_gpu_renderer()->m_pgpurendertarget->current_texture();

            //   //ptexture->m_estate = D3D12_RESOURCE_STATE_COPY_SOURCE;

            //}

         }

      }

   }



   ::gpu::swap_chain* context::get_swap_chain()
   {

      if (m_etype != e_type_window)
      {

         throw ::exception(error_failed);

         return nullptr;

      }

      if (!m_pswapchain)
      {

         __defer_construct(m_pswapchain);

         ///m_pswapchain->initialize_gpu_swap_chain(this, m_pwindow);

      }

      return m_pswapchain;

   }


   IDXGIDevice* context::_get_dxgi_device()
   {

      if (m_pgpudevice->m_pgpucontextMain != this)
      {

         ::cast < context > pcontextMain = m_pgpudevice->m_pgpucontextMain;

         return pcontextMain->_get_dxgi_device();

      }

      ASSERT(d3d11on12()->m_pdxgidevice);

      //if (!d3d11on12()->m_pdxgidevice)
      //{

      //   //          {

      // //      ::gpu_directx12::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);

      //       //m_pgpudevice = ::gpu::swap_chain::m_pgpudevice;

      //       ///::cast < ::gpu_directx12::device > pdevice = m_pgpudevice;

      //   assert(command_queue() && "Command queue must be initialized before D3D11On12CreateDevice");

      //   ::cast < device> pdevice = m_pgpudevice;
      //   
      //   ::cast < context > pcontextMainDraw2d = pdevice->main_draw2d_context();
      //   
      //   D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
      //   
      //   UINT numFeatureLevels = _countof(featureLevels);
      //   
      //   IUnknown* unknowna[] = 
      //   { 
      //      pcontextMainDraw2d->command_queue() 
      //   };

      //   HRESULT hrD3D11On12 = D3D11On12CreateDevice(
      //      pdevice->m_pdevice,
      //      D3D11_CREATE_DEVICE_BGRA_SUPPORT,
      //      featureLevels,
      //      numFeatureLevels,
      //      unknowna,
      //      1,
      //      0,
      //      &d3d11on12()->m_pd3d11device,
      //      &d3d11on12()->m_pd3d11context,
      //      nullptr
      //   );

      //   ::defer_throw_hresult(hrD3D11On12);

      //   ::defer_throw_hresult(d3d11on12()->m_pd3d11device.as(d3d11on12()->m_pd3d11on12)); // Query interface

      //   ::defer_throw_hresult(d3d11on12()->m_pd3d11device.as(d3d11on12()->m_pdxgidevice));

      //   //::defer_throw_hresult(m_pdxgiswapchain3.as(m_pdxgiswapchain1));

      //   //DXGI_SWAP_CHAIN_DESC swapchaindesc1{};

      //   //int FrameCount = 2;

      //   //if (SUCCEEDED(m_pdxgiswapchain3->GetDesc(&swapchaindesc1)))
      //   //{

      //   //   FrameCount = swapchaindesc1.BufferCount;

      //   //}

      //   //m_frameIndex = m_pdxgiswapchain3->GetCurrentBackBufferIndex();

      //   //// Create synchronization objects and wait until assets have been uploaded to the GPU.
      //   //{
      //   //   ::defer_throw_hresult(pd3d12device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __interface_of(m_fence)));
      //   //   m_fenceValue = 1;

      //   //   // Create an event handle to use for frame synchronization.
      //   //   m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
      //   //   if (m_fenceEvent == nullptr)
      //   //   {
      //   //      ::defer_throw_hresult(HRESULT_FROM_WIN32(GetLastError()));
      //   //   }

      //   //}

      //   ////_defer_d3d11on12_wrapped_resources();
      //   //// Create descriptor heaps.

      //   //{
      //   //   // Describe and create a render target view (RTV) descriptor heap.
      //   //   D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
      //   //   rtvHeapDesc.NumDescriptors = FrameCount;
      //   //   rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
      //   //   rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
      //   //   ::defer_throw_hresult(pd3d12device->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(m_rtvHeap)));

      //   //   m_rtvDescriptorSize = pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

      //   //}

      //   //// Create frame resources.
      //   //{

      //   //   CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

      //   //   // Create a RTV for each frame.
      //   //   for (UINT n = 0; n < FrameCount; n++)
      //   //   {

      //   //      auto & prendertarget = m_renderTargets[n];

      //   //      ::defer_throw_hresult(
      //   //         m_pdxgiswapchain1->GetBuffer(
      //   //            n, __interface_of(prendertarget)));

      //   //      pd3d12device->CreateRenderTargetView(prendertarget, nullptr, rtvHandle);

      //   //      rtvHandle.Offset(1, m_rtvDescriptorSize);

      //   //   }

      //   //}

      //   //::draw2d_direct2d::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);



      //}

      return d3d11on12()->m_pdxgidevice;

   }

} // namespace gpu_directx12



