#include "framework.h"
#include "context.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "acme/platform/application.h"
#include "acme/prototype/mathematics/mathematics.h"
#include "app-graphics3d/gpu_vulkan/descriptors.h"
#include "approach.h"
#include "aura/graphics/image/image.h"
#include "bred/gpu/compositor.h"
#include "bred/gpu/frame.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/pixmap.h"
#include "bred/gpu/types.h"
#include "buffer.h"
#include "command_buffer.h"
#include "debug.h"
#include "device.h"
#include "gltf_model.h"
#include "memory_buffer.h"
#include "physical_device.h"
#include "program.h"
#include "render_target.h"
#include "renderer.h"
#include "shader.h"
#include "swap_chain.h"
#include "texture.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
// #define GLM_FORCE_LEFT_HANDED  // Optional — depends on your conventions
#include <chrono>
#include "glm/mat4x4.hpp"
#include "gltf_model.h"
#include "initializers.h"
#include "vk_init.h"

#include "pipeline.h"
#include "queue.h"
#include "gpu_vulkan/ibl/cubemap_framebuffer.h"

using namespace vulkan;


namespace gpu_vulkan
{


   context::context()
   {

#ifdef WINDOWS_DESKTOP
      m_formatImageDefault = VK_FORMAT_B8G8R8A8_UNORM;
#else
      m_formatImageDefault = VK_FORMAT_R8G8B8A8_UNORM;
#endif

      m_vksampler001 = nullptr;
      // m_bOffscreen = true;
      //       m_emode = e_mode_none;
      // m_itaskGpu = 0;
      // m_iLastBitmap1Scan = -1;

      // m_gluTextureBitmap1 = 0;
      // m_VAO = 0;
      // m_VBO = 0;

      m_bMesa = false;

      // m_emode = e_mode_system;

      m_estatus = error_not_initialized;

      // m_physicaldevicefeaturesCreate = {};
      // m_physicaldevicefeaturesEnabled = {};
      // this->logicalDevice() = VK_NULL_HANDLE;
      // m_vkcommandpool = VK_NULL_HANDLE;

      // m_vkqueuePresent = nullptr;
      // m_vkqueueGraphics = nullptr;

      m_vkcommandpoolGraphics = VK_NULL_HANDLE;
      //m_vkcommandpoolTransfer = VK_NULL_HANDLE;
      //m_vkcommandpoolPresent = VK_NULL_HANDLE;

   }


   context::~context() {}


   // void context::initialize(::particle * pparticle)
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
      ////      float vertexes[] = {f
      //         // positions         // colors
      //  //        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
      //    //     -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
      //      //    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
      //      //};
      //      float vertexes[] = {
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
      // #if defined(__APPLE__)
      //
      //      glGenVertexArrays(1, &m_VAO);
      //      glGenBuffers(1, &m_VBO);
      //      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex
      //      attributes(s). glBindVertexArray(m_VAO);
      //
      //      //#elif defined(__ANDROID__)
      //      //
      //      //      glGenVertexArrays(1, &m_VAO);
      //      //      glGenBuffers(1, &m_VBO);
      //      //      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure
      //      vertex attributes(s).
      //      //      glBindVertexArray(m_VAO);
      //
      // #else
      //
      ////      glGenVertexArrays(1, &m_VAO);
      ////      glGenBuffers(1, &m_VBO);
      ////      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex
      ///attributes(s). /      glBindVertexArray(m_VAO);
      ////
      // #endif
      ////
      ////      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
      ////      glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
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


      // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_elements);
      // int iError16 = glGetError();

      // int size = 0;
      // glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
      // int iError17 = glGetError();

      // glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
      // int iError18 = glGetError();


      // ASSERT(is_current_task());

      ////      glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

      //      // Clear the screen
      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      ////glClear(GL_COLOR_BUFFER_BIT);

      // if (m_pprogram && m_pprogram->m_pshader)
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

      // if (m_iMatrixUniform >= 0)
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


   void context::set_bitmap_1(::image::image *pimage)
   {

      ASSERT(is_current_task());

      //      if (!m_gluTextureBitmap1)
      //      {
      //
      ////         glGenTextures(1, &m_gluTextureBitmap1); // generate texture id for your texture (can skip this line)
      ////         glEnable(GL_TEXTURE_2D);
      ////         glBindTexture(GL_TEXTURE_2D, m_gluTextureBitmap1);
      ////
      ////         glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // set alignment of data in memory (a good thing to do before
      ///glTexImage)
      ////
      ////#if defined(__APPLE__) || defined(__ANDROID__)
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // set clamp (GL_CLAMP_TO_EDGE
      ///would be better)
      ////#else
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // set clamp (GL_CLAMP_TO_EDGE would be
      ///better)
      ////#endif
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // set linear filtering (so you can
      ///scale your image)
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


   // void context::swap_buffers()
   //{


   //}


   VkSampler context::_001VkSampler()
   {

      if (!m_vksampler001)
      {

         VkSamplerCreateInfo samplerInfo = {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter = VK_FILTER_LINEAR,
            .minFilter = VK_FILTER_LINEAR,
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,

            .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .mipLodBias = 0.0f,
            .anisotropyEnable = VK_FALSE,
            .maxAnisotropy = 1.0f,
            .compareEnable = VK_FALSE,
            .compareOp = VK_COMPARE_OP_ALWAYS,
            .minLod = 0.0f,
            .maxLod = 0.0f,
            .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
            .unnormalizedCoordinates = VK_FALSE,

         };


         if (vkCreateSampler(this->logicalDevice(), &samplerInfo, NULL, &m_vksampler001) != VK_SUCCESS)
         {
            // Handle error
         }
      }

      return m_vksampler001;
   }


   // void context::create_offscreen_buffer(const ::int_size& size)
   //{

   //   auto pgpu = application()->get_gpu();

   //   if (::is_null(pgpu))
   //   {

   //      return ::error_failed;

   //   }

   //   ::e_status estatus = øconstruct(m_pbuffer);

   //   if (!estatus)
   //   {

   //      return false;

   //   }

   //   m_pbuffer->m_pimage = create_image(size);

   //   if (!::is_ok(m_pbuffer->m_pimage))
   //   {

   //      return false;

   //   }

   //   estatus = _create_offscreen_buffer(size);

   //   if(!estatus)
   //   {

   //      return estatus;

   //   }

   //   return ::success_none;

   //}


   // void context::_create_offscreen_buffer(const ::int_size& size)
   //{

   //   return ::success_none;

   //}


   // void context::resize_offscreen_buffer(const ::int_size& size)
   //{

   //   return ::success_none;

   //}


   // void context::make_current()
   //{

   //   return ::success_none;

   //}


   // void context::destroy_offscreen_buffer()
   //{

   //   return ::success_none;

   //}


   string context::_001GetIntroProjection()
   {

      string strVersion = get_shader_version_text();


      string strProjection = "layout(location = 0) in vec3 aPos;\n"
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

      string strFragment = "uniform vec2 resolution;\n"
                           "uniform float time;\n"
                           "uniform vec2 mouse;\n"
                           "uniform sampler2D backbuffer;\n"
                           "\n"
                           "void main(void) {\n"
                           "float base_res = min(resolution.x(), resolution.y());\n"
                           "vec2 uv = (gl_FragCoord.xy * 2.0 - resolution.xy) / base_res;\n"
                           "\n"
                           //"gl_FragColor = vec4(uv, (uv.x() * uv.x()) / 2.0, ((uv.x() + (base_res - uv.y())) *(uv.x()
                           //+ (base_res - uv.y()))) / 2.0);\n"
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


   // string context::get_shader_version_text()
   //{

   //   return "version 330 core";

   //}

   // void context::translate_shader(string& strFragment)
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


   // void context::_translate_shader(string_array& stra)
   //{

   //}


   // bool context::defer_construct_new(::pointer < ::gpu_vulkan::memory_buffer >& pmemorybuffer, memsize size)
   //{

   //   if (ødefer_construct_new(pmemorybuffer))
   //   {

   //      pmemorybuffer->initialize_memory_buffer(this, size);

   //      return true;

   //   }

   //   return false;

   //}


   // bool context::defer_construct_new(::pointer < ::gpu_vulkan::memory_buffer >& pmemorybuffer, const ::block& block)
   //{

   //   if (defer_construct_new(pmemorybuffer, block.size()))
   //   {

   //      pmemorybuffer->assign(block.data(), block.size());

   //      return true;

   //   }

   //   return false;

   //}


   void context::set_matrix_uniform(const ::gpu::payload &uniformMatrix)
   {

      // m_iMatrixUniform = uniformMatrix.m_iUniform;
   }


   ::pointer<::gpu::context> allocate_system_context(::particle *pparticle)
   {

      return pparticle->øcreate_new<context>();
   }


   // context::context()
   //{


   //}


   // context::~context()
   //{

   //}


   bool context::is_mesa() { return m_bMesa; }


   void context::_create_offscreen_window(const ::int_size &size)
   {
      // if (::IsWindow(m_hwnd))
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
      // else
      //{

      //   LPCTSTR lpClassName = L"draw2d_vulkan_offscreen_buffer_window";
      //   LPCTSTR lpWindowName = L"draw2d_vulkan_offscreen_buffer_window";
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
      //   HINSTANCE hInstance = ::GetModuleHandleW(L"gpu_vulkan.dll");
      //   LPVOID lpParam = nullptr;

      //   m_hwnd = CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent,
      //   hMenu, hInstance, lpParam);

      //   if (!m_hwnd)
      //   {

      //      informationf("MS GDI - CreateWindow failed");

      //      informationf("last-error code: %d\n", GetLastError());

      //      throw ::exception(error_failed);

      //   }

      //   //return m_hwnd;

      //}
   }


   void context::_create_context_win32(::gpu::device *pgpudevice, const ::gpu::enum_output &eoutput,
                                       ::windowing::window *pwindow, const ::int_size &size)
   {

      if (!pgpudevice)
      {

         throw ::exception(error_failed);
      }
   }


         /**
    * Create a command pool for allocation command buffers from
    *
    * @param queueFamilyIndex Family index of the queue to create the command pool for
    * @param createFlags (Optional) Command pool creation flags (Defaults to
    * VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
    *
    * @note Command buffers allocated from the created pool can only be submitted to a queue with the same family index
    *
    * @return A handle to the created command buffer
    */
   VkCommandPool context::createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags)
   {
      
      VkCommandPoolCreateInfo cmdPoolInfo = {};
      
      cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
      cmdPoolInfo.flags = createFlags;
      
      VkCommandPool cmdPool;
      
      VK_CHECK_RESULT(vkCreateCommandPool(this->logicalDevice(), &cmdPoolInfo, nullptr, &cmdPool));

      return cmdPool;

   }


   void context::on_create_context(::gpu::device *pgpudevice, const ::gpu::enum_output &eoutput,
                                   ::windowing::window *pwindow, const ::int_size &size)
   {

      // m_itaskGpu = ::current_itask();
      m_pgpudevice = pgpudevice;

      ::cast<::gpu_vulkan::device> pdevice = pgpudevice;

      auto graphicsFamilyIndex = pdevice->m_queuefamilyindexes.graphicsFamily;

      // Create a default command pool for graphics command buffers
      m_vkcommandpoolGraphics = createCommandPool(graphicsFamilyIndex);

      auto transferFamilyIndex = pdevice->m_queuefamilyindexes.graphicsFamily;

      if (transferFamilyIndex >= 0)
      {

         m_vkcommandpoolTransfer = createCommandPool(transferFamilyIndex, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);

      }
      else
      {

         m_vkcommandpoolTransfer = VK_NULL_HANDLE;

      }

      auto presentFamilyIndex = pdevice->m_queuefamilyindexes.presentFamily;

      if (presentFamilyIndex >= 0)
      {

         m_vkcommandpoolPresent = createCommandPool(presentFamilyIndex);

      }
      else
      {

         m_vkcommandpoolPresent = VK_NULL_HANDLE;

      }

      _create_context_win32(pgpudevice, eoutput, pwindow, size);

   }


   void context::endSingleTimeCommands(::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      VkCommandBuffer commandbuffers[] = {pcommandbuffer->m_vkcommandbuffer};

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = commandbuffers;

      endSingleTimeCommands(pcommandbuffer, 1, &submitInfo);

   }


   void context::endSingleTimeCommands(command_buffer *pcommandbuffer, int iSubmitCount, VkSubmitInfo *psubmitinfo)
   {

      if (vkEndCommandBuffer(pcommandbuffer->m_vkcommandbuffer) != VK_SUCCESS)
      {
         
         throw ::exception(error_failed);

      }

      VkFence fence;

      VkFenceCreateInfo fenceInfo = {
         .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
         .pNext = NULL,
         .flags = 0 // 0 = fence starts in unsignaled state
      };

      VkResult result = vkCreateFence(this->logicalDevice(), &fenceInfo, NULL, &fence);

      if (result != VK_SUCCESS)
      {

         fprintf(stderr, "Failed to create fence\n");
         
      }

      ::cast<::gpu_vulkan::queue> pqueue = pcommandbuffer->m_pgpuqueue;

      VkQueue vkqueue = pqueue->m_vkqueue;

      // if (pcommandbuffer->m_ecommandbuffer == ::gpu::e_command_buffer_present)
      // {
      //
      //    vkqueue = m_vkqueuePresent;
      //
      // }
      // else
      // {
      //
      //    vkqueue = m_vkqueueGraphics;
      //
      // }

      vkQueueSubmit(vkqueue, 1, psubmitinfo, fence);

      vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

      vkQueueWaitIdle(vkqueue);

      vkDestroyFence(this->logicalDevice(), fence, NULL);

   }


   VkDevice context::logicalDevice()
   {

      ::cast<device> pgpudevice = m_pgpudevice;

      return pgpudevice->logicalDevice();
   }


   //   VkResult context::createLogicalDevice(
   //   VkPhysicalDeviceFeatures enabledFeatures,
   //   ::array<const char *> enabledExtensions,
   //   void * pNextChain,
   //   bool useSwapChain,
   //   VkQueueFlags requestedQueueTypes)
   //   {
   //
   //      ::cast < approach > pgpuapproach = application()->get_gpu();
   //
   //      ::cast < physical_device > pphysicaldevice = pgpuapproach->m_pphysicaldevice;
   //
   //      // Desired queues need to be requested upon logical device creation
   //      // Due to differing queue family configurations of Vulkan implementations this can be a bit tricky,
   //      especially if the application
   //      // requests different queue types
   //
   //      ::array<VkDeviceQueueCreateInfo> queueCreateInfos{};
   //
   //      // Get queue family indexes for the requested queue family types
   //      // Note that the indexes may overlap depending on the implementation
   //
   //      const float defaultQueuePriority(0.0f);
   //
   //      m_queuefamilyindexes = pphysicaldevice->findQueueFamilies();
   //
   //      // Graphics queue
   //      if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT
   //         && m_queuefamilyindexes.graphicsFamilyHasValue)
   //      {
   //         //m_queuefamilyindexes.graphics = pphysicaldevice->getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
   //         VkDeviceQueueCreateInfo queueInfo{};
   //         queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
   //         queueInfo.queueFamilyIndex = m_queuefamilyindexes.graphicsFamily;
   //         queueInfo.queueCount = 1;
   //         queueInfo.pQueuePriorities = &defaultQueuePriority;
   //         queueCreateInfos.add(queueInfo);
   //      }
   //      else
   //      {
   //         m_queuefamilyindexes.graphicsFamily = 0;
   //      }
   //
   //      // Dedicated compute queue
   //      if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT
   //         && m_queuefamilyindexes.computeFamilyHasValue)
   //      {
   //         //m_queuefamilyindexes.compute = pphysicaldevice->getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
   //         if (m_queuefamilyindexes.computeFamily != m_queuefamilyindexes.graphicsFamily)
   //         {
   //            // If compute family index differs, we need an additional queue create info for the compute queue
   //            VkDeviceQueueCreateInfo queueInfo{};
   //            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
   //            queueInfo.queueFamilyIndex = m_queuefamilyindexes.computeFamily;
   //            queueInfo.queueCount = 1;
   //            queueInfo.pQueuePriorities = &defaultQueuePriority;
   //            queueCreateInfos.add(queueInfo);
   //         }
   //      }
   //      else
   //      {
   //         // Else we use the same queue
   //         m_queuefamilyindexes.computeFamily = m_queuefamilyindexes.graphicsFamily;
   //      }
   //
   //      // Dedicated transfer queue
   //      if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT
   //         && m_queuefamilyindexes.transferFamilyHasValue)
   //      {
   //         //m_queuefamilyindexes.transfer = pphysicaldevice->getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
   //         if ((m_queuefamilyindexes.transferFamily != m_queuefamilyindexes.graphicsFamily)
   //            && (m_queuefamilyindexes.transferFamily != m_queuefamilyindexes.computeFamily))
   //         {
   //            // If transfer family index differs, we need an additional queue create info for the transfer queue
   //            VkDeviceQueueCreateInfo queueInfo{};
   //            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
   //            queueInfo.queueFamilyIndex = m_queuefamilyindexes.transferFamily;
   //            queueInfo.queueCount = 1;
   //            queueInfo.pQueuePriorities = &defaultQueuePriority;
   //            queueCreateInfos.add(queueInfo);
   //         }
   //      }
   //      else
   //      {
   //         // Else we use the same queue
   //         m_queuefamilyindexes.transferFamily = m_queuefamilyindexes.graphicsFamily;
   //      }
   //
   //      // Create the logical device representation
   //      ::array<const char *> deviceExtensions(enabledExtensions);
   //      if (useSwapChain)
   //      {
   //
   //         // If the device will be used for presenting to a display via a swapchain we need to request the swapchain
   //         extension deviceExtensions.add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
   //
   //      }
   //
   //      VkDeviceCreateInfo deviceCreateInfo = {};
   //      deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
   //      deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
   //      deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
   //      deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
   //
   //      // If a pNext(Chain) has been passed, we need to add it to the device creation info
   //      VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{};
   //      if (pNextChain) {
   //         physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
   //         physicalDeviceFeatures2.features = enabledFeatures;
   //         physicalDeviceFeatures2.pNext = pNextChain;
   //         deviceCreateInfo.pEnabledFeatures = nullptr;
   //         deviceCreateInfo.pNext = &physicalDeviceFeatures2;
   //      }
   //
   // #if (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT))
   // && defined(VK_KHR_portability_subset)
   //      // SRS - When running on iOS/macOS with MoltenVK and VK_KHR_portability_subset is defined and supported by
   //      the device, enable the extension if (extensionSupported(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
   //      {
   //         deviceExtensions.add(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
   //      }
   // #endif
   //
   //      if (deviceExtensions.size() > 0)
   //      {
   //         for (const char * enabledExtension : deviceExtensions)
   //         {
   //            if (!isExtensionSupported(enabledExtension)) {
   //               information() << "Enabled device extension \"" << enabledExtension << "\" is not present at device
   //               level\n";
   //            }
   //         }
   //
   //         deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
   //         deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
   //      }
   //
   //      this->m_physicaldevicefeaturesEnabled = enabledFeatures;
   //
   //      auto physicaldevice = pphysicaldevice->m_physicaldevice;
   //
   //      VkResult result = vkCreateDevice(physicaldevice, &deviceCreateInfo, nullptr, &this->logicalDevice());
   //      if (result != VK_SUCCESS)
   //      {
   //         return result;
   //      }
   //
   //      if (m_queuefamilyindexes.graphicsFamily >= 0)
   //      {
   //         vkGetDeviceQueue(this->logicalDevice(), m_queuefamilyindexes.graphicsFamily, 0, &m_vkqueueGraphics);
   //      }
   //      if (m_queuefamilyindexes.presentFamily >= 0)
   //      {
   //         vkGetDeviceQueue(this->logicalDevice(), m_queuefamilyindexes.presentFamily, 0, &m_vkqueuePresent);
   //      }
   //
   //
   //      // Create a default command pool for graphics command buffers
   //      m_vkcommandpool = createCommandPool(m_queuefamilyindexes.graphicsFamily);
   //
   //      return result;
   //
   //   }
   //
   //
   //   /**
   //   * Create a command pool for allocation command buffers from
   //   *
   //   * @param queueFamilyIndex Family index of the queue to create the command pool for
   //   * @param createFlags (Optional) Command pool creation flags (Defaults to
   //   VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
   //   *
   //   * @note Command buffers allocated from the created pool can only be submitted to a queue with the same family
   //   index
   //   *
   //   * @return A handle to the created command buffer
   //   */
   //   VkCommandPool context::createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags)
   //   {
   //      VkCommandPoolCreateInfo cmdPoolInfo = {};
   //      cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
   //      cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
   //      cmdPoolInfo.flags = createFlags;
   //      VkCommandPool cmdPool;
   //      VK_CHECK_RESULT(vkCreateCommandPool(this->logicalDevice(), &cmdPoolInfo, nullptr, &cmdPool));
   //      return cmdPool;
   //   }
   //
   //
   //   /**
   //   * Check if an extension is supported by the (physical device)
   //   *
   //   * @param extension Name of the extension to check
   //   *
   //   * @return True if the extension is supported (present in the list read at device creation time)
   //   */
   //   bool context::isExtensionSupported(const ::scoped_string & scopedstrExtension)
   //   {
   //
   //      return m_straSupportedExtensions.contains(scopedstrExtension);
   //
   //   }


   // void context::_create_window_buffer()
   //{

   //   if (!m_hdc || !m_hrc)
   //   {

   //      auto psystem = system();

   //      auto pgpu = application()->get_gpu();

   //      ::pointer < ::vulkan::vulkan > pvulkan = pgpu;

   //      if (!pvulkan->m_atomClass)
   //      {

   //         informationf("MS GDI - RegisterClass failed");

   //         informationf("last-error code: %d\n", GetLastError());

   //         throw ::exception(error_failed);

   //      }

   //      // create WGL context, make current

   //      //xxxopengl>>>>vulkan PIXELFORMATDESCRIPTOR pixformat;

   //      int chosenformat;

   //      HDC hdc = GetDC(m_hwnd);

   //      if (!hdc)
   //      {

   //         informationf("MS GDI - GetDC failed");

   //         informationf("last-error code: %d\n", GetLastError());

   //         throw ::exception(error_failed);

   //      }

   //      ////xxxopengl>>>>vulkan  zero(pixformat);
   //      //pixformat.nSize = sizeof(pixformat);
   //      //pixformat.nVersion = 1;
   //      //pixformat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_VULKAN | PFD_DOUBLEBUFFER;
   //      //pixformat.iPixelType = PFD_TYPE_RGBA;
   //      //pixformat.cColorBits = 32;
   //      //pixformat.cRedShift = 16;
   //      //pixformat.cGreenShift = 8;
   //      //pixformat.cBlueShift = 0;
   //      //pixformat.cAlphaShift = 24;
   //      //pixformat.cAlphaBits = 8;
   //      //pixformat.cDepthBits = 24;
   //      //pixformat.cStencilBits = 8;

   //      //xxxopengl>>>>vulkan chosenformat = ChoosePixelFormat(hdc, &pixformat);

   //      //if (chosenformat == 0)
   //      //{

   //      //   informationf("MS GDI - ChoosePixelFormat failed");

   //      //   informationf("last-error code: %d\n", GetLastError());

   //      //   ReleaseDC(m_hwnd, hdc);

   //      //   throw ::exception(error_failed);

   //      //}

   //      //bool spfok = SetPixelFormat(hdc, chosenformat, &pixformat);

   //      //if (!spfok)
   //      //{

   //      //   informationf("MS GDI - SetPixelFormat failed");

   //      //   informationf("last-error code: %d\n", GetLastError());

   //      //   ReleaseDC(m_hwnd, hdc);

   //      //   throw ::exception(error_failed);

   //      //}

   //      //HGLRC hglrc = wglCreateContext(hdc);

   //      //if (!hglrc)
   //      //{

   //      //   informationf("MS WGL - wglCreateContext failed");

   //      //   informationf("last-error code: %d\n", GetLastError());

   //      //   ReleaseDC(m_hwnd, hdc);

   //      //   throw ::exception(error_failed);

   //      //}

   //      //bool bMakeCurrentOk = wglMakeCurrent(hdc, hglrc);

   //      //if (!bMakeCurrentOk)
   //      //{

   //      //   informationf("MS WGL - wglMakeCurrent failed");

   //      //   informationf("last-error code: %d\n", GetLastError());

   //      //   ReleaseDC(m_hwnd, hdc);

   //      //   throw ::exception(error_failed);

   //      //}


   //      pvulkan->defer_init_gpu_library();

   //      //auto pszVersion = (const char *)glGetString(GL_VERSION);
   //      ////::e_status estatus =

   //      //::string strVersion(pszVersion);

   //      //if (strVersion.case_insensitive_contains("mesa"))
   //      //{

   //      //   m_bMesa = true;

   //      //}

   //      ////if (!estatus)
   //      ////{

   //      ////   ReleaseDC(window, hdc);

   //      ////   return estatus;

   //      ////}

   //      m_hwnd = m_hwnd;
   //      m_hdc = hdc;
   //      //xxxopengl>>>>vulkan m_hrc = hglrc;

   //   }

   //   RECT rectClient;

   //   ::GetClientRect(m_hwnd, &rectClient);

   //   m_size = { rectClient.right - rectClient.left,
   //      rectClient.bottom - rectClient.top };

   //   m_itaskGpu = ::current_itask();

   //   m_estatus = ::success;

   //   set_ok_flag();
   //
   //}


   void context::_create_window_context(::windowing::window *pwindowParam)
   {

      // m_itaskGpu = ::current_itask();

      //   ::cast < ::windowing_win32::window > pwindow = pwindowParam;

      //   if (!m_hdc || !m_hrc)
      //   {

      //      auto psystem = system();

      //      auto pgpu = application()->get_gpu();

      //      ::pointer < ::vulkan::vulkan > pvulkan = pgpu;

      //      //if (!pvulkan->m_atomClass)
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
      //         //pixformat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_VULKAN | PFD_DOUBLEBUFFER;
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


      ////         pvulkan->defer_init_gpu_library();

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
      // rectClient.bottom - rectClient.top };
      //
      //   if (m_size != sizeNew)
      //   {
      //      m_size = sizeNew;


      //      //HDC pdcDIB;                      // контекст устройства в памяти
      //      //HBITMAP hbmpDIB;                 // и его текущий битмапvoid *pBitsDIB(NULL);            // содержимое
      //      битмапаint cxDIB(200); int cyDIB(300);  // его размеры (например для окна 200х300)
      //      //auto &BIH=pwindow->m_bitmapinfoheaderProto;            // и заголовок// …// создаем DIB section//
      //      создаем структуру BITMAPINFOHEADER, описывающую наш DIBint iSize = sizeof(BITMAPINFOHEADER);  // размер
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


   void context::_create_cpu_buffer(const ::int_size &size)
   {

      _create_offscreen_window(size);

      //_create_window_buffer();

      // m_itaskGpu = ::current_itask();
   }


   void context::defer_create_window_context(::windowing::window *pwindow)
   {

      // if (m_hrc)
      //{

      //   return;

      //}

      //::vulkan::context::defer_create_window_context(pwindow);
   }


   void context::_defer_create_window_context(::windowing::window *pwindow) { _create_window_context(pwindow); }


   void context::resize_cpu_buffer(const ::int_size &sizeParam)
   {

      if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      {

         return;
      }

      auto size(sizeParam);

      send(
         [this, size]()
         {
            // if (!m_pcpubuffer)

            create_cpu_buffer(size);


            /// m_pcpubuffer->m_pixmap.create(m_pcpubuffer->m_memory, size);

            // #ifdef WINDOWS_DESKTOP
            //
            //       ::SetWindowPos(m_hwnd, 0, 0, 0, size.cx(), size.cy(), SWP_NOZORDER | SWP_NOMOVE | SWP_HIDEWINDOW);
            //
            // #else

            //      destroy_offscreen_buffer();
            //
            //      if(!create_offscreen_buffer(size))
            //      {
            //
            //         return error_failed;
            //
            //      }

            // #endif

            // make_current();

            // glViewport(0, 0, size.cx(), size.cy());
            // glMatrixMode(GL_PROJECTION);
            // glLoadIdentity();
            // glOrtho(0, size.cx(), 0, size.cy(), -10, 10);
            // glMatrixMode(GL_MODELVIEW);
            // glutPostRedisplay();

            // return ::success;
         });
   }


   // void context::make_current()
   //{

   //   ::gpu::context::make_current();

   //   //if (!is_current_task())
   //   //{

   //   //   ASSERT(FALSE);

   //   //}

   //   //::e_status estatus = ::success;

   //   ////bool bMakeCurrentOk = wglMakeCurrent(m_hdc, m_hrc);

   //   ////if (!bMakeCurrentOk)
   //   ////{

   //   ////   informationf("MS WGL - wglMakeCurrent failed");

   //   ////   informationf("last-error code: %d\n", GetLastError());

   //   ////   throw ::exception(error_failed);

   //   ////}

   //   ////return estatus;

   //}


   void context::destroy_cpu_buffer()
   {

      // ASSERT(is_current_task());

      //::e_status estatus = ::success;

      // if (m_hrc == NULL && m_hdc == NULL && m_hwnd == NULL)
      //{

      //   return;

      //}

      ////wglMakeCurrent(nullptr, nullptr);
      ////wglDeleteContext(m_hrc);
      //::ReleaseDC(m_hwnd, m_hdc);
      //::DestroyWindow(m_hwnd);
      // m_size.set(0, 0);
      // m_hrc = NULL;
      // m_hwnd = NULL;
      // m_hdc = NULL;

      ////return estatus;
   }


   // void context::render()
   //{

   //   ::gpu_vulkan::context::render();


   //}


   // void context::swap_buffers()
   //{

   //   //SwapBuffers(m_hdc);

   //}


   ::memory context::white_to_color_sampler_vert()
   {

      unsigned int pvertexshader[] = {
#include "shader/white_to_color.vert.spv.inl"
      };

      return ::as_memory_block(pvertexshader);
   }


   ::memory context::white_to_color_sampler_frag()
   {

      unsigned int pfragmentshader[] = {
#include "shader/white_to_color.frag.spv.inl"
      };

      return ::as_memory_block(pfragmentshader);
   }


   string context::get_shader_version_text() { return "#version 330 core"; }


   void context::_translate_shader(string_array_base &stra)
   {

      gpu::context::_translate_shader(stra);

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


   //// class member functions
   ////context::context(::graphics3d_vulkan::VulkanDevice* pgpudevice) :
   ///this->logicalDevice(){pgpudevice->logicalDevice} {
   // context::context()
   //{

   //   m_vksurfacekhr = nullptr;
   //   m_vkqueuePresent = nullptr;
   //   m_vkqueueGraphics = nullptr;

   //   createInstance();
   //   setupDebugMessenger();
   //   createSurface();
   //   pickPhysicalDevice();
   //   createLogicalDevice();
   //   createCommandPool();
   //}

   // context::~context()
   //{
   //    vkDestroyCommandPool(this->logicalDevice(), m_vkcommandpool, nullptr);
   //    vkDestroyDevice(this->logicalDevice(), nullptr);

   //   if (enableValidationLayers) {
   //      DestroyDebugUtilsMessengerEXT(m_vkinstance, debugMessenger, nullptr);
   //   }

   //   if (m_vksurfacekhr)
   //   {
   //      vkDestroySurfaceKHR(m_vkinstance, m_vksurfacekhr, nullptr);
   //   }
   //   vkDestroyInstance(m_vkinstance, nullptr);

   //}


   // void context::initialize_context(::user::graphics3d * pimpact)
   //{

   //   m_pimpact = pimpact;


   //   validationLayers.add("VK_LAYER_KHRONOS_validation");

   //   if (m_papplication->m_bUseSwapChainWindow)
   //   {

   //      deviceExtensions.add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

   //   }


   //}


   // void context::createInstance()
   //{

   //   if (enableValidationLayers && !checkValidationLayerSupport()) {
   //      throw ::exception(error_failed,"validation layers requested, but not available!");
   //   }

   //   VkApplicationInfo appInfo = {};
   //   appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   //   appInfo.pApplicationName = "LittleVulkanEngine App";
   //   appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
   //   appInfo.pEngineName = "No Engine";
   //   appInfo.approachVersion = VK_MAKE_VERSION(1, 0, 0);
   //   appInfo.apiVersion = VK_API_VERSION_1_0;

   //   VkInstanceCreateInfo createInfo = {};
   //   createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   //   createInfo.pApplicationInfo = &appInfo;

   //   auto extensions = getRequiredExtensions();
   //   createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
   //   createInfo.ppEnabledExtensionNames = extensions.data();

   //   VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
   //   if (enableValidationLayers) {
   //      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
   //      createInfo.ppEnabledLayerNames = validationLayers.data();

   //      populateDebugMessengerCreateInfo(debugCreateInfo);
   //      createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
   //   }
   //   else {
   //      createInfo.enabledLayerCount = 0;
   //      createInfo.pNext = nullptr;
   //   }

   //   if (vkCreateInstance(&createInfo, nullptr, &m_vkinstance) != VK_SUCCESS) {
   //      throw ::exception(error_failed,"failed to create m_vkinstance!");
   //   }

   //   hasGflwRequiredInstanceExtensions();
   //}


   // void context::pickPhysicalDevice()
   //{
   //    uint32_t deviceCount = 0;
   //    vkEnumeratePhysicalDevices(m_vkinstance, &deviceCount, nullptr);
   //    if (deviceCount == 0) {
   //       throw ::exception(error_failed,"failed to find GPUs with Vulkan support!");
   //    }
   //    information() << "Device count: " << deviceCount;
   //    ::array<VkPhysicalDevice> devices(deviceCount);
   //    vkEnumeratePhysicalDevices(m_vkinstance, &deviceCount, devices.data());

   //   for (const auto & pvkcdevice : devices) {
   //      if (isDeviceSuitable(pvkcdevice)) {
   //         physicalDevice = pvkcdevice;
   //         break;
   //      }
   //   }

   //   if (physicalDevice == VK_NULL_HANDLE) {
   //      throw ::exception(error_failed,"failed to find a suitable GPU!");
   //   }

   //   vkGetPhysicalDeviceProperties(physicalDevice, &properties);
   //   information() << "physical pvkcdevice: " << properties.deviceName;
   //}


   // void context::createLogicalDevice()
   //{

   //   QueueFamilyIndices indexes = findQueueFamilies(physicalDevice);

   //   ::array<VkDeviceQueueCreateInfo> queueCreateInfos;
   //   std::set<uint32_t> uniqueQueueFamilies;
   //   if (indexes.graphicsFamilyHasValue)
   //   {
   //      uniqueQueueFamilies.insert(indexes.graphicsFamily);
   //   }
   //   if (indexes.presentFamilyHasValue)
   //   {
   //      uniqueQueueFamilies.insert(indexes.presentFamily);
   //   }

   //   float queuePriority = 1.0f;
   //   for (uint32_t queueFamily : uniqueQueueFamilies)
   //   {
   //      VkDeviceQueueCreateInfo queueCreateInfo = {};
   //      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
   //      queueCreateInfo.queueFamilyIndex = queueFamily;
   //      queueCreateInfo.queueCount = 1;
   //      queueCreateInfo.pQueuePriorities = &queuePriority;
   //      queueCreateInfos.add(queueCreateInfo);
   //   }

   //   VkPhysicalDeviceFeatures deviceFeatures = {};
   //   deviceFeatures.samplerAnisotropy = VK_TRUE;

   //   VkDeviceCreateInfo createInfo = {};
   //   createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

   //   createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
   //   createInfo.pQueueCreateInfos = queueCreateInfos.data();

   //   createInfo.pEnabledFeatures = &deviceFeatures;
   //   createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
   //   createInfo.ppEnabledExtensionNames = deviceExtensions.data();

   //   // might not really be necessary anymore because pvkcdevice specific validation layers
   //   // have been deprecated
   //   if (enableValidationLayers)
   //   {
   //      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
   //      createInfo.ppEnabledLayerNames = validationLayers.data();
   //   }
   //   else
   //   {
   //      createInfo.enabledLayerCount = 0;
   //   }

   //   if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &this->logicalDevice()) != VK_SUCCESS)
   //   {
   //      throw ::exception(error_failed,"failed to create logical pvkcdevice!");
   //   }
   //   if (indexes.graphicsFamilyHasValue)
   //   {
   //      vkGetDeviceQueue(this->logicalDevice(), indexes.graphicsFamily, 0, &m_vkqueueGraphics);
   //   }
   //   if (indexes.presentFamilyHasValue)
   //   {
   //      vkGetDeviceQueue(this->logicalDevice(), indexes.presentFamily, 0, &m_vkqueuePresent);
   //   }
   //}


   // void context::createCommandPool()
   //{
   //    QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

   //   VkCommandPoolCreateInfo poolInfo = {};
   //   poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
   //   poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
   //   poolInfo.flags =
   //      VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

   //   if (vkCreateCommandPool(this->logicalDevice(), &poolInfo, nullptr, &m_vkcommandpool) != VK_SUCCESS) {
   //      throw ::exception(error_failed,"failed to create command pool!");
   //   }
   //}


   // VkCommandBuffer context::createCommandBuffer(VkCommandBufferLevel level, bool begin)
   // {
   //
   //    ::cast < device > pdevice = m_pgpudevice;
   //
   //    auto vkcommandpool = pdevice->getCommandPool();
   //
   //    return createCommandBuffer(level, vkcommandpool, begin);
   //
   // }
   //
   //
   // VkCommandBuffer context::createCommandBuffer(VkCommandBufferLevel level, VkCommandPool pool, bool begin)
   // {
   //
   //    VkCommandBufferAllocateInfo pcommandbufferCmd->m_vkcommandbufferAllocateInfo =
   //    vkinit::commandBufferAllocateInfo(pool, level, 1);
   //
   //    VkCommandBuffer pcommandbufferCmd->m_vkcommandbufferfer;
   //
   //    vkAllocateCommandBuffers(this->logicalDevice(), &pcommandbufferCmd->m_vkcommandbufferAllocateInfo,
   //    &pcommandbufferCmd->m_vkcommandbufferfer);
   //
   //    // If requested, also start recording for the new command buffer
   //    if (begin)
   //    {
   //
   //       VkCommandBufferBeginInfo pcommandbufferCmd->m_vkcommandbufferInfo = vkinit::commandBufferBeginInfo();
   //
   //       vkBeginCommandBuffer(pcommandbufferCmd->m_vkcommandbufferfer, &pcommandbufferCmd->m_vkcommandbufferInfo);
   //
   //    }
   //
   //    return pcommandbufferCmd->m_vkcommandbufferfer;
   //
   // }
   //
   //
   // void context::flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, VkCommandPool pool, bool free)
   // {
   //
   //    if (commandBuffer == VK_NULL_HANDLE)
   //    {
   //
   //       return;
   //
   //    }
   //
   //    VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));
   //
   //    VkSubmitInfo submitInfo = vkinit::submitInfo();
   //
   //    submitInfo.commandBufferCount = 1;
   //
   //    submitInfo.pCommandBuffers = &commandBuffer;
   //
   //    // Create fence to ensure that the command buffer has finished executing
   //    VkFenceCreateInfo fenceInfo = vkinit::fenceCreateInfo(VK_FLAGS_NONE);
   //
   //    VkFence fence;
   //
   //    VK_CHECK_RESULT(vkCreateFence(this->logicalDevice(), &fenceInfo, nullptr, &fence));
   //
   //    // Submit to the queue
   //    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
   //
   //    // Wait for the fence to signal that command buffer has finished executing
   //    VK_CHECK_RESULT(vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT));
   //
   //    vkDestroyFence(this->logicalDevice(), fence, nullptr);
   //
   //    if (free)
   //    {
   //
   //       vkFreeCommandBuffers(this->logicalDevice(), pool, 1, &commandBuffer);
   //
   //    }
   //
   // }
   //
   //
   // void context::flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, bool free)
   // {
   //
   //    ::cast < device > pdevice = m_pgpudevice;
   //
   //    auto vkcommandpool = pdevice->getCommandPool();
   //
   //    return flushCommandBuffer(commandBuffer, queue, vkcommandpool, free);
   //
   // }


   // // 1) Query swapchain support for our chosen physical pdevice + surface:
   // SwapChainSupportDetails device::querySwapchainSupport(VkSurfaceKHR surface) const {
   //    SwapChainSupportDetails details;
   //
   //    // Capabilities:
   //    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
   //       m_physicalDevice, surface, &details.capabilities);
   //
   //    // Surface formats:
   //    uint32_t count = 0;
   //    vkGetPhysicalDeviceSurfaceFormatsKHR(
   //       m_physicalDevice, surface, &count, nullptr);
   //    if (count > 0) {
   //       details.formats.resize(count);
   //       vkGetPhysicalDeviceSurfaceFormatsKHR(
   //          m_physicalDevice, surface, &count,
   //          details.formats.data());
   //    }
   //
   //    // Present modes:
   //    vkGetPhysicalDeviceSurfacePresentModesKHR(
   //       m_physicalDevice, surface, &count, nullptr);
   //    if (count > 0) {
   //       details.presentModes.resize(count);
   //       vkGetPhysicalDeviceSurfacePresentModesKHR(
   //          m_physicalDevice, surface, &count,
   //          details.presentModes.data());
   //    }
   //
   //    return details;
   // }


   // void context::createSurface() { window.createWindowSurface(m_vkinstance, &m_vksurfacekhr); }

   // void context::_createSurface() {}


   // bool context::isDeviceSuitable(VkPhysicalDevice pvkcdevice)
   //{

   //   QueueFamilyIndices indexes = findQueueFamilies(pvkcdevice);

   //   bool extensionsSupported = checkDeviceExtensionSupport(pvkcdevice);

   //   bool swapChainAdequate = false;
   //   if (m_vksurfacekhr)
   //   {
   //      if (extensionsSupported)
   //      {
   //         SwapChainSupportDetails swapChainSupport = querySwapChainSupport(pvkcdevice);
   //         swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
   //      }
   //   }
   //   else
   //   {

   //      swapChainAdequate = true;

   //   }

   //   VkPhysicalDeviceFeatures supportedFeatures;
   //   vkGetPhysicalDeviceFeatures(pvkcdevice, &supportedFeatures);

   //   return (!m_vksurfacekhr || indexes.isComplete()) && extensionsSupported && swapChainAdequate &&
   //      supportedFeatures.samplerAnisotropy;

   //}


   // void context::populateDebugMessengerCreateInfo(
   //    VkDebugUtilsMessengerCreateInfoEXT & createInfo)
   //{

   //   createInfo = {};
   //   createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
   //   createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
   //      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
   //   createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
   //      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
   //      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
   //   createInfo.pfnUserCallback = debugCallback;
   //   createInfo.pUserData = nullptr;  // Optional

   //}


   // void context::setupDebugMessenger()
   //{

   //   if (!enableValidationLayers)
   //   {

   //      return;

   //   }

   //   VkDebugUtilsMessengerCreateInfoEXT createInfo;

   //   populateDebugMessengerCreateInfo(createInfo);

   //   if (CreateDebugUtilsMessengerEXT(m_vkinstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed,"failed to set up debug messenger!");

   //   }

   //}


   // bool context::checkValidationLayerSupport()
   //{
   //    uint32_t layerCount;
   //    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

   //   ::array<VkLayerProperties> availableLayers(layerCount);
   //   vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

   //   for (const char * layerName : validationLayers)
   //   {
   //      bool layerFound = false;


   //      printf_line("checking for validation layer : %s", layerName);
   //      for (const auto & layerProperties : availableLayers)
   //      {
   //         printf_line("an available layer : %s", layerProperties.layerName);
   //         if (strcmp(layerName, layerProperties.layerName) == 0)
   //         {
   //            layerFound = true;
   //            break;
   //         }
   //      }

   //      if (!layerFound) {
   //         return false;
   //      }
   //   }

   //   return true;

   //}


   //::array<const char *> context::getRequiredExtensions()
   //{
   //   uint32_t glfwExtensionCount = 0;
   //   const char ** glfwExtensions;
   //   //glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

   //   //::array<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
   //   ::array<const char *> extensions;

   //   if (enableValidationLayers)
   //   {
   //      extensions.add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
   //   }

   //   return extensions;
   //   return {};
   //}

   // void context::gpu_debug_message(const ::scoped_string& scopedstrMessage)
   void context::start_debug_happening(const ::scoped_string &scopedstrMessage)
   {

      //::string strMessage(scopedstrMessage);

      ::color::color color(::color::black);

      // VkDebugUtilsLabelEXT debugLabel = {
      //     .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
      //     .pNext = NULL,
      //     .pLabelName = strMessage.c_str(),
      //     .color = {
      //       color.f32_red(),
      //       color.f32_green(),
      //       color.f32_blue(),
      //       color.f32_opacity() }
      // };

      ::cast<command_buffer> pcommandbuffer = ::gpu::current_frame()->m_pgpucommandbuffer;

      ::vulkan::debugutils::insertDebugLabel(pcommandbuffer->m_vkcommandbuffer, scopedstrMessage, color);
   }


   // void context::hasGflwRequiredInstanceExtensions()
   //{
   //    uint32_t extensionCount = 0;
   //    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
   //    ::array<VkExtensionProperties> extensions(extensionCount);
   //    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

   //   information() << "available extensions:";
   //   set<::string> available;
   //   for (const auto & extension : extensions) {
   //      information() << "\t" << extension.extensionName;
   //      available.set_at(extension.extensionName);
   //   }

   //   information() << "required extensions:";
   //   auto requiredExtensions = getRequiredExtensions();
   //   for (const auto & required : requiredExtensions)
   //   {
   //      information() << "\t" << required;
   //      if (!available.plookup(required)) {
   //         throw ::exception(error_failed,"Missing required glfw extension");
   //      }
   //   }

   //}


   // bool context::checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice)
   //{

   //   uint32_t extensionCount;
   //   vkEnumerateDeviceExtensionProperties(pvkcdevice, nullptr, &extensionCount, nullptr);

   //   ::array<VkExtensionProperties> availableExtensions(extensionCount);
   //   vkEnumerateDeviceExtensionProperties(
   //      pvkcdevice,
   //      nullptr,
   //      &extensionCount,
   //      availableExtensions.data());

   //   set<::string> requiredExtensions(deviceExtensions);

   //   for (const auto & extension : availableExtensions) {
   //      requiredExtensions.erase(extension.extensionName);
   //   }

   //   return requiredExtensions.empty();
   //}


   ::pointer<buffer> context::create_buffer(VkDeviceSize size, VkBufferUsageFlags usage,
                                            VkMemoryPropertyFlags properties)
   {

      VkBufferCreateInfo bufferInfo{};
      bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufferInfo.size = size;
      bufferInfo.usage = usage;
      bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      auto pbuffer = øcreate_new<buffer>();

      pbuffer->m_pgpucontext = this;

      pbuffer->m_size = size;

      if (vkCreateBuffer(this->logicalDevice(), &bufferInfo, nullptr, &pbuffer->m_vkbuffer) != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to create vertex buffer!");
      }

      VkMemoryRequirements memRequirements;
      vkGetBufferMemoryRequirements(this->logicalDevice(), pbuffer->m_vkbuffer, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex =
         m_pgpudevice->m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(this->logicalDevice(), &allocInfo, nullptr, &pbuffer->m_vkdevicememory) != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to allocate vertex buffer memory!");
      }

      vkBindBufferMemory(this->logicalDevice(), pbuffer->m_vkbuffer, pbuffer->m_vkdevicememory, 0);

      return pbuffer;
   }


   // void context::do_on_context(const ::function< void(::gpu::context*) >& functionFrame)
   //{

   //   ::gpu::rear_guard rear_guard(this);

   //   auto function = functionFrame;

   //   m_pgpucontext->send([this, function]()
   //      {


   //      });


   //}


   ::pointer<::gpu::command_buffer> context::beginSingleTimeCommands(::gpu::queue *pqueue,
                                                                     ::gpu::enum_command_buffer ecommandbuffer)
   {

      return ::gpu::context::beginSingleTimeCommands(pqueue, ecommandbuffer);

      //::pointer < command_buffer > pcommandbuffer;

      // ødefer_construct_new(pcommandbuffer);

      // pcommandbuffer->initialize_command_buffer(m_pgpurenderer->m_pgpurendertarget, ecommandbuffer);

      // pcommandbuffer->begin_command_buffer(true);

      // return pcommandbuffer;
   }


   // void context::endSingleTimeCommands(auto pcommandbuffer)
   //{

   //   vkEndCommandBuffer(pcommandbuffer->m_vkcommandbuffer);

   //   VkSubmitInfo submitInfo{};
   //   submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
   //   submitInfo.commandBufferCount = 1;
   //   submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;

   //   vkQueueSubmit(m_vkqueueGraphics, 1, &submitInfo, VK_NULL_HANDLE);
   //   vkQueueWaitIdle(m_vkqueueGraphics);

   //   vkFreeCommandBuffers(this->logicalDevice(), m_pgpudevice->getCommandPool(), 1,
   //   &pcommandbuffer->m_vkcommandbuffer);

   //}


   void context::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
   {

      ::pointer<command_buffer> pcommandbuffer = beginSingleTimeCommands(m_pgpudevice->graphics_queue());

      VkBufferCopy copyRegion{};
      copyRegion.srcOffset = 0; // Optional
      copyRegion.dstOffset = 0; // Optional
      copyRegion.size = size;

      vkCmdCopyBuffer(pcommandbuffer->m_vkcommandbuffer, srcBuffer, dstBuffer, 1, &copyRegion);

      endSingleTimeCommands(pcommandbuffer);
   }


   void context::copyBufferToImage(::gpu::command_buffer *pgpucommandbuffer, ::gpu_vulkan::texture *ptexture,
                                   ::gpu_vulkan::buffer *pbuffer, const ::int_rectangle &rectangleSubImage)
   // VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount)
   {

      // auto pcommandbuffer = beginSingleTimeCommands();

      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      ptexture->_set_state(pcommandbuffer, {

                                              VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                              VK_PIPELINE_STAGE_TRANSFER_BIT

                                           });

      VkBufferImageCopy regions[6];

      int iRegionCount;

      if (ptexture->m_etype == ::gpu::texture::e_type_cube_map)
      {

         iRegionCount = 6;

         auto texWidth = (uint32_t)ptexture->width();

         auto texHeight = (uint32_t)ptexture->height();

         auto layerarea = texWidth * texHeight * 4;

         for (uint32_t face = 0; face < 6; face++)
         {

            auto &region = regions[face];

            memset(&region, 0, sizeof(region));

            region.bufferOffset = layerarea * face;
            region.bufferRowLength = 0;
            region.bufferImageHeight = 0;
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = 0;
            region.imageSubresource.baseArrayLayer = face;
            region.imageSubresource.layerCount = 1;
            region.imageOffset = {0, 0, 0};
            region.imageExtent = {texWidth, texHeight, 1};
         }
      }
      else
      {

         iRegionCount = 1;

         auto &region = regions[0];

         memset(&region, 0, sizeof(region));

         region.bufferOffset = 0;
         region.bufferRowLength = 0;
         region.bufferImageHeight = 0;

         region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         region.imageSubresource.mipLevel = 0;
         region.imageSubresource.baseArrayLayer = 0;
         region.imageSubresource.layerCount = 1;

         if (rectangleSubImage.is_empty())
         {

            region.imageOffset = {0, 0, 0};
            region.imageExtent = {(uint32_t)ptexture->width(), (uint32_t)ptexture->height(), 1};
         }
         else
         {
            region.imageOffset = {(int32_t)rectangleSubImage.left(), (int32_t)rectangleSubImage.top(), 0};
            region.imageExtent = {(uint32_t)rectangleSubImage.width(), (uint32_t)rectangleSubImage.height(), 1};
         }
      }

      vkCmdCopyBufferToImage(pcommandbuffer->m_vkcommandbuffer, pbuffer->m_vkbuffer, ptexture->m_vkimage,
                             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, iRegionCount, regions);

      pcommandbuffer->m_particleaHold.add(pbuffer);
   }


   // void context::copyBufferToImage(
   //    ::gpu::command_buffer* pgpucommandbuffer,
   //    ::gpu::pixmap* pgpupixmap, ::gpu_vulkan::buffer* pbuffer)
   //    //VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount)
   //{

   //   //auto pcommandbuffer = beginSingleTimeCommands();

   //   ::cast < command_buffer > pcommandbuffer = pgpucommandbuffer;

   //   ::cast < ::gpu_vulkan::texture > ptexture = pgpupixmap->m_pgputexture;

   //   ptexture->_set_state(pcommandbuffer,
   //      {

   //         VK_ACCESS_TRANSFER_WRITE_BIT,
   //         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
   //         VK_PIPELINE_STAGE_TRANSFER_BIT

   //      });

   //   VkBufferImageCopy region;

   //   int iRegionCount = 1;

   //   //if (ptexture->m_etype == ::gpu::texture::e_type_cube_map)
   //   //{

   //   //   iRegionCount = 6;

   //   //   auto texWidth = (uint32_t)ptexture->width();

   //   //   auto texHeight = (uint32_t)ptexture->height();

   //   //   auto layerarea = texWidth * texHeight * 4;

   //   //   for (uint32_t face = 0; face < 6; face++)
   //   //   {

   //   //      auto& region = regions[face];

   //   //      memset(&region, 0, sizeof(region));

   //   //      region.bufferOffset = layerarea * face;
   //   //      region.bufferRowLength = 0;
   //   //      region.bufferImageHeight = 0;
   //   //      region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //   //      region.imageSubresource.mipLevel = 0;
   //   //      region.imageSubresource.baseArrayLayer = face;
   //   //      region.imageSubresource.layerCount = 1;
   //   //      region.imageOffset = { 0, 0, 0 };
   //   //      region.imageExtent = { texWidth, texHeight, 1 };

   //   //   }

   //   //}
   //   //else
   //   {

   //      //iRegionCount = 1;

   //      //auto& region = regions[0];

   //      memset(&region, 0, sizeof(region));

   //      region.bufferOffset = 0;
   //      region.bufferRowLength = 0;
   //      region.bufferImageHeight = 0;

   //      region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //      region.imageSubresource.mipLevel = 0;
   //      region.imageSubresource.baseArrayLayer = 0;
   //      region.imageSubresource.layerCount = 1;

   //      region.imageOffset = {
   //         (int32_t)pgpupixmap->m_rectangle.left(),
   //         (int32_t)pgpupixmap->m_rectangle.top(), 0 };
   //      region.imageExtent = {
   //         (uint32_t)pgpupixmap->m_rectangle.width(),
   //         (uint32_t)pgpupixmap->m_rectangle.height(), 1 };

   //   }

   //   vkCmdCopyBufferToImage(
   //      pcommandbuffer->m_vkcommandbuffer,
   //      pbuffer->m_vkbuffer,
   //      ptexture->m_vkimage,
   //      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
   //      iRegionCount,
   //      &region);

   //}


   void context::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
                                       uint32_t mizLevels, uint32_t layerCount)
   {


      auto pgpucommandbuffer = beginSingleTimeCommands(nullptr);

      VkImageMemoryBarrier barrier{};
      barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      barrier.oldLayout = oldLayout;
      barrier.newLayout = newLayout;

      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

      barrier.image = image;

      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
      {
         barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
         if (hasStencilComponent(format))
         {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
         }
      }

      barrier.subresourceRange.baseMipLevel = 0;
      barrier.subresourceRange.levelCount = mizLevels;
      barrier.subresourceRange.baseArrayLayer = 0;
      barrier.subresourceRange.layerCount = layerCount;

      VkPipelineStageFlags sourceStage;
      VkPipelineStageFlags destinationStage;

      if ((oldLayout == VK_IMAGE_LAYOUT_UNDEFINED) && (newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL))
      {
         barrier.srcAccessMask = 0;
         barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
         sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
         destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      }
      else if ((oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) &&
               (newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL))
      {
         barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
         barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
         sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
         destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
      }
      else if ((oldLayout == VK_IMAGE_LAYOUT_UNDEFINED) &&
               (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL))
      {
         barrier.srcAccessMask = 0;
         barrier.dstAccessMask =
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
         sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
         destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
      }
      else
      {
         throw ::exception(error_bad_argument, "unsupported layout transition!");
      }
      ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pgpucommandbuffer;

      vkCmdPipelineBarrier(pcommandbuffer->m_vkcommandbuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr,
                           1, &barrier);

      endSingleTimeCommands(pcommandbuffer);
   }

   bool context::hasStencilComponent(VkFormat format)
   {
      return (format == VK_FORMAT_D32_SFLOAT_S8_UINT) || (format == VK_FORMAT_D24_UNORM_S8_UINT);
   }


   void context::createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties,
                                     VkImage &image, VkDeviceMemory &imageMemory)
   {

      if (vkCreateImage(this->logicalDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to create image!");
      }

      VkMemoryRequirements memRequirements;
      vkGetImageMemoryRequirements(this->logicalDevice(), image, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex =
         m_pgpudevice->m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(this->logicalDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
      {
         throw ::exception(error_failed, "failed to allocate image memory!");
      }

      if (vkBindImageMemory(this->logicalDevice(), image, imageMemory, 0) != VK_SUCCESS)
      {
         throw ::exception(error_failed, "failed to bind image memory!");
      }
   }


   void context::submitWork(command_buffer *pcommandbuffer, VkQueue queue)
   {

      VkSubmitInfo submitInfo = initializers::submitInfo();
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
      // m_submitInfo.commandBufferCount = 1;
      // m_submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
      VkFenceCreateInfo fenceInfo = initializers::fenceCreateInfo();
      VkFence fence;
      VK_CHECK_RESULT(vkCreateFence(this->logicalDevice(), &fenceInfo, nullptr, &fence));
      VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
      VK_CHECK_RESULT(vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX));
      vkDestroyFence(this->logicalDevice(), fence, nullptr);
   }


   // void context::submitSamplingWork(VkCommandBuffer pcommandbuffer->m_vkcommandbuffer, VkQueue queue)
   //{
   //    VkSubmitInfo submitInfo = initializers::submit_info();
   //    submitInfo.commandBufferCount = 1;
   //    submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
   //    //m_submitInfo.commandBufferCount = 1;
   //    //m_submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
   //    VkFenceCreateInfo fenceInfo = initializers::fence_create_info();
   //    VkFence fence;
   //    VK_CHECK_RESULT(vkCreateFence(this->logicalDevice(), &fenceInfo, nullptr, &fence));
   //    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
   //    VK_CHECK_RESULT(vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX));
   //    vkDestroyFence(this->logicalDevice(), fence, nullptr);
   // }

   ::gpu_vulkan::descriptor_pool *context::get_global_pool(int iFrameCount) { return m_pdescriptorpoolGlobal; }


   // bool context::hasStencilComponent(VkFormat format)
   //{
   //    return (format == VK_FORMAT_D32_SFLOAT_S8_UINT) || (format == VK_FORMAT_D24_UNORM_S8_UINT);
   // }


   // void context::transitionImageLayout(
   //    VkImage image,
   //    VkFormat format,
   //    VkImageLayout oldLayout,
   //    VkImageLayout newLayout,
   //    uint32_t mizLevels,
   //    uint32_t layerCount)
   //{

   //   if (oldLayout == newLayout)
   //   {

   //      return;

   //   }
   //   auto pcommandbuffer = beginSingleTimeCommands();

   //   VkImageMemoryBarrier barrier{};
   //   barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
   //   barrier.oldLayout = oldLayout;
   //   barrier.newLayout = newLayout;

   //   barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
   //   barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

   //   barrier.image = image;

   //   barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //   if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
   //      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
   //      if (hasStencilComponent(format)) {
   //         barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
   //      }
   //   }

   //   barrier.subresourceRange.baseMipLevel = 0;
   //   barrier.subresourceRange.levelCount = mizLevels;
   //   barrier.subresourceRange.baseArrayLayer = 0;
   //   barrier.subresourceRange.layerCount = layerCount;

   //   VkPipelineStageFlags sourceStage;
   //   VkPipelineStageFlags destinationStage;

   //   if ((oldLayout == VK_IMAGE_LAYOUT_UNDEFINED) && (newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)) {
   //      barrier.srcAccessMask = 0;
   //      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
   //      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
   //      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
   //   }
   //   else if ((oldLayout == VK_IMAGE_LAYOUT_UNDEFINED) && (newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
   //      barrier.srcAccessMask = 0;
   //      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
   //      sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
   //      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
   //   }
   //   else if ((oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) && (newLayout ==
   //   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
   //      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
   //      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
   //      sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
   //      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
   //   }
   //   else if ((oldLayout == VK_IMAGE_LAYOUT_UNDEFINED) && (newLayout ==
   //   VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)) {
   //      barrier.srcAccessMask = 0;
   //      barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
   //      VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT; sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
   //      destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
   //   }
   //   else {
   //      throw ::exception(error_bad_argument, "unsupported layout transition!");
   //   }

   //   vkCmdPipelineBarrier(
   //      pcommandbuffer->m_vkcommandbuffer,
   //      sourceStage, destinationStage,
   //      0,
   //      0, nullptr,
   //      0, nullptr,
   //      1, &barrier
   //   );

   //   endSingleTimeCommands(pcommandbuffer);
   //}


   void context::create_global_ubo(int iGlobalUboSize, int iFrameCount)
   {

      m_uboBuffers.set_size(iFrameCount);

      for (int i = 0; i < m_uboBuffers.size(); i++)
      {

         ødefer_construct_new(m_uboBuffers[i]);

         m_uboBuffers[i]->_initialize_buffer(this, iGlobalUboSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

         m_uboBuffers[i]->map(0, m_uboBuffers[i]->m_size);

         auto bufferInfo = m_uboBuffers[i]->descriptorInfo();

         descriptor_writer(*m_psetdescriptorlayoutGlobal, *m_pdescriptorpoolGlobal)
            .writeBuffer(0, &bufferInfo)
            .build(m_descriptorsetsGlobal[i]);

         m_uboBuffers[i]->unmap();
      }

      // auto globalSetLayout = m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
   }


   void context::update_global_ubo(const ::block &block)
   {

      auto iFrameIndex = m_pgpurenderer->m_pgpurendertarget->get_frame_index();

      if (iFrameIndex < 0 || iFrameIndex >= m_uboBuffers.size())
      {

         return;
      }

      if (!m_uboBuffers[iFrameIndex])
      {

         return;
      }

      m_uboBuffers[iFrameIndex]->map();

      m_uboBuffers[iFrameIndex]->writeToBuffer(block.data());

      m_uboBuffers[iFrameIndex]->flush();

      m_uboBuffers[iFrameIndex]->unmap();
   }


   void context::engine_on_frame_context_initialization()
   {

   }


   void context::onBeforePreloadGlobalAssets()
   {

      // Global UBO descriptors
      if (!m_psetdescriptorlayoutGlobal)
      {

         auto pgpurenderer = get_gpu_renderer();

         m_psetdescriptorlayoutGlobal =
            descriptor_set_layout::Builder(this)
               .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
               .build();

         auto iFrameCount = pgpurenderer->m_pgpurendertarget->get_frame_count();

         m_descriptorsetsGlobal.resize(iFrameCount);

         auto pdescriptorpoolbuilder = øallocate::gpu_vulkan::descriptor_pool::Builder();

         pdescriptorpoolbuilder->initialize_builder(this);
         pdescriptorpoolbuilder->setMaxSets(iFrameCount);
         pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, iFrameCount);

         m_pdescriptorpoolGlobal = pdescriptorpoolbuilder->build();
      }


   }


   VkDescriptorSet context::getGlobalDescriptorSet(::gpu_vulkan::renderer *prenderer, ::collection::index iFrameIndex)
   {

      // if (m_globalDescriptorSets.is_empty())
      //{


      //}

      if (iFrameIndex < 0)
      {

         iFrameIndex = prenderer->m_pgpurendertarget->get_frame_index();

      }

      return m_descriptorsetsGlobal[iFrameIndex];
   }


   // void context::copy(::gpu::texture* ptexture)
   //{

   //   ::cast < renderer > prenderer = get_gpu_renderer();

   //   prenderer->_copy_image(ptexture, ptexture->m_rectangleTarget, true);

   //}


   void context::merge_layers(::gpu::texture *ptextureTarget, ::pointer_array<::gpu::layer> *playera)
   {

      ::cast<renderer> pgpurenderer = m_pgpurenderer;

      ::cast<render_target> prendertarget = pgpurenderer->m_pgpurendertarget;

      ::cast<::gpu_vulkan::texture> ptexture = prendertarget->current_texture(::gpu::current_frame());

      ::cast<swap_chain> pswapchain = m_pgpuswapchain;

      auto psynchronization = ptexture->synchronization();

      auto fence = psynchronization->in_flight_fence();

      pgpurenderer->wait_swap_chain_command_buffer_ready();

      /*if (fence)
      {

         vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

      }*/

      /// vkResetFences(this->logicalDevice(), 1, &fence);

      ::pointer<command_buffer> pcommandbuffer = this->beginSingleTimeCommands(m_pgpudevice->graphics_queue());

      ::gpu::scoped_command_buffer scopedcommanbuffer(pcommandbuffer);
      // pcommandbuffer->begin_command_buffer(false);

      if (::nok(m_pshaderBlend3))
      {

         unsigned int full_screen_triangle_vertex_shader[] = {
#include "shader/blend2.vert.spv.inl"
         };

         unsigned int full_screen_triangle_fragment_shader[] = {
#include "shader/blend2.frag.spv.inl"
         };

         øconstruct(m_pshaderBlend3);

         m_pshaderBlend3->m_bEnableBlend = true;
         m_pshaderBlend3->m_bindingSampler.set();
         m_pshaderBlend3->m_bDisableDepthTest = true;
         // m_pshaderBlend3->m_bT
         // m_pshaderBlend3->m_pgpurenderer = this;
         m_pshaderBlend3->m_bindingSampler.set();
         // Image Blend descriptors
         // if (!m_psetdescriptorlayoutImageBlend)

         // m_pshaderBlend3->m_bClearColor = true;
         // m_pshaderBlend3->m_colorClear = ::color::transparent;

         m_pshaderBlend3->m_bClearColor = false;
         // m_pshaderBlend3->m_colorClear = ::color::transparent;

         m_pshaderBlend3->initialize_shader_with_block(
            m_pgpurenderer, ::as_memory_block(full_screen_triangle_vertex_shader),
            ::as_memory_block(full_screen_triangle_fragment_shader), {}, {}, {}, {},
            // this means the vertex input layout will be null/empty
            // the full screen shader is embed in the shader code
            ::gpu::shader::e_flag_clear_default_bindings_and_attributes_descriptions

         );
      }


      // if (!m_pd3d11blendstateBlend3)
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

      ::cast<renderer> prenderer = m_pgpurenderer;

      auto vkcommandbuffer = pcommandbuffer->m_vkcommandbuffer;

      ::cast<::gpu_vulkan::texture> ptextureDst = ptextureTarget;

      ptextureDst->_set_state(pcommandbuffer, {VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                               VK_PIPELINE_STAGE_TRANSFER_BIT});


      pcommandbuffer->set_viewport(m_rectangle.size());

      pcommandbuffer->set_scissor(m_rectangle.size());

      ::pointer_array<::gpu::texture> textureaSrc;
      {

         // 2. Clear
         VkClearColorValue clearColor = {.float32 = {0.0f, 0.0f, 0.0f, 0.0f}};
         VkImageSubresourceRange range = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
         };

         vkCmdClearColorImage(vkcommandbuffer, ptextureDst->m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              &clearColor, 1, &range);
      }

      {

         ////float clearColor[4] = { 0.95f * 0.5f, 0.95f * 0.5f, 0.25f * 0.5f, 0.5f }; // Clear to transparent
         ////m_pcontext->ClearRenderTargetView(ptextureDst->m_prendertargetview, clearColor);
         // float clearColor[4] = { 0.f, 0.f, 0.f, 0.f }; // Clear to transparent
         // pcommandlist->ClearRenderTargetView(ptextureDst->m_handleRenderTargetView, clearColor, 0, nullptr);
         auto scopedstate = ptextureDst->_scoped_state(pcommandbuffer, {VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                                                                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                                        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT});

         int iH = ptextureDst->m_pgpurenderer->m_pgpucontext->m_rectangle.height();

         // if (!ptextureDst->m_handleRenderTargetView.ptr)
         //{

         // ptextureDst->create_render_target();

         //}


         // m_pshaderBlend3->bind(ptextureTarget);


         // int iDescriptorSize = ptextureDst->m_rtvDescriptorSize;
         // int iFrameIndex = m_pgpurendertarget->get_frame_index();
         // auto hRtv = pgpurendertargetview->m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
         // auto hRtv = ptextureDst->m_handleRenderTargetView;
         // CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
         //  hRtv,
         // iFrameIndex,
         // iDescriptorSize);

         // float clearColor[4] = { 0.5f * 0.5f, 0.75f * 0.5f, 0.9f * 0.5f, 0.5f };
         // float clearColor[4] = { 0.f, 0.f , 0.f, 0.f };
         // pcommandlist->ClearRenderTargetView(hRtv, clearColor, 0, nullptr);

         //{
         //   float blendFactor[4] = { 0, 0, 0, 0 }; // Ignored with this blend mode
         //   UINT sampleMask = 0xFFFFFFFF;
         //   m_pcontext->OMSetBlendState(m_pd3d11blendstateBlend3, blendFactor, sampleMask);
         //}


         // ID3D11RenderTargetView* rendertargetview[] = { ptextureDst->m_prendertargetview };

         // m_p(1, rendertargetview, nullptr);

         // m_pcontext->OMSetBlendState(g_blendState, nullptr, 0xffffffff);
         // g_context->VSSetShader(g_vs, nullptr, 0);
         // g_context->PSSetShader(g_ps, nullptr, 0);
         // g_context->PSSetSamplers(0, 1, &g_sampler);

                     for (auto player: *playera)
         {

            // if (iLayer == 2)
            {

               ::cast<::gpu_vulkan::texture> ptextureSrc = player->texture();
               ptextureSrc->_set_state(
                  pcommandbuffer, {0, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT});
            }
         }


         _001BeginRenderPass(pcommandbuffer, ptextureDst);

         if (1)
         {
            int iLayer = 0;
            for (auto player: *playera)
            {

               // if (iLayer == 2)
               {

                  ::cast<::gpu_vulkan::texture> ptextureSrc = player->texture();
                  textureaSrc.add(ptextureSrc.m_p);
                  //auto scopedstateLayer =
                  //   ptextureSrc->_scoped_state(pcommandbuffer, {0, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                  //                                               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT});

                  m_pshaderBlend3->bind(pcommandbuffer, ptextureDst, ptextureSrc);

                  // pcommandbuffer->set_viewport(ptextureSrc->m_rectangleTarget);

                  // pcommandbuffer->set_scissor(ptextureSrc->m_rectangleTarget.size());

                  auto r = ptextureSrc->m_rectangleTarget;

                  int h = r.height();

                  r.top() = iH - r.bottom();

                  r.bottom() = r.top() + h;

                  pcommandbuffer->set_viewport(r);

                  pcommandbuffer->set_scissor(r);


                  //{

                  //   // 2. Clear
                  //   VkClearColorValue clearColor = { .float32 = { 0.95f * 0.5f, 0.95f * 0.5f, 0.50f * 0.5f, 0.5f } };
                  //   VkImageSubresourceRange range = {
                  //       .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                  //       .baseMipLevel = 0,
                  //       .levelCount = 1,
                  //       .baseArrayLayer = 0,
                  //       .layerCount = 1,
                  //   };

                  //   vkCmdClearColorImage(
                  //      vkcommandbuffer,
                  //      ptextureDst->m_vkimage,
                  //      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                  //      &clearColor,
                  //      1,
                  //      &range);

                  //}


                  // ID3D11SamplerState* samplerstatea[] =
                  //{ ptexture->m_psamplerstate };
                  // ID3D11ShaderResourceView* sharedresourceviewa[] =
                  //{ ptexture->m_pshaderresourceview };

                  // 1. Define viewport and scissor rectangle
                  // D3D12_VIEWPORT viewport = {};
                  // viewport.TopLeftX = ptextureSrc->m_rectangleTarget.left();
                  // viewport.TopLeftY = ptextureSrc->m_rectangleTarget.top();
                  // viewport.Width = static_cast<float>(ptextureSrc->m_rectangleTarget.width());
                  // viewport.Height = static_cast<float>(ptextureSrc->m_rectangleTarget.height());
                  // viewport.MinDepth = 0.0f;
                  // viewport.MaxDepth = 1.0f;

                  // D3D12_RECT scissorRect = {};
                  // scissorRect.left = ptextureSrc->m_rectangleTarget.left();
                  // scissorRect.top = ptextureSrc->m_rectangleTarget.top();
                  // scissorRect.right = ptextureSrc->m_rectangleTarget.right();
                  // scissorRect.bottom = ptextureSrc->m_rectangleTarget.bottom();


                  ////// 4. Set the viewport and scissor
                  // pcommandlist->RSSetViewports(1, &viewport);
                  // pcommandlist->RSSetScissorRects(1, &scissorRect);
                  // D3D11_VIEWPORT vp = {};
                  // vp.TopLeftX = ptexture->m_rectangleTarget.left();
                  // vp.TopLeftY = ptexture->m_rectangleTarget.top();
                  // vp.Width = static_cast<float>(ptexture->m_rectangleTarget.width());
                  // vp.Height = static_cast<float>(ptexture->m_rectangleTarget.height());
                  // vp.MinDepth = 0.0f;
                  // vp.MaxDepth = 1.0f;
                  // m_pcontext->RSSetViewports(1, &vp);

                  // m_pcontext->PSSetSamplers(0, 1, samplerstatea);
                  // m_pcontext->PSSetShaderResources(0, 1, sharedresourceviewa);


                  // pcommandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                  vkCmdDraw(vkcommandbuffer, 3, 1, 0, 0);


                  m_pshaderBlend3->unbind(pcommandbuffer);
               }

               iLayer++;
            }
         }
         //}
      }

      _001EndRenderPass(pcommandbuffer);

      
                     for (auto player: *playera)
      {

         // if (iLayer == 2)
         {

            ::cast<::gpu_vulkan::texture> ptextureSrc = player->texture();
            ptextureSrc->_set_state(
               pcommandbuffer, 
               {VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                     VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT});
         }
      }

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
      // if (!m_pgpurenderer->isFrameStarted)
      {

         if (vkEndCommandBuffer(pcommandbuffer->m_vkcommandbuffer) != VK_SUCCESS)
         {

            throw ::exception(error_failed);
         }
         VkFence fence = VK_NULL_HANDLE;
         pcommandbuffer->submitCommandBuffers(ptextureTarget, textureaSrc, {}, {}, {}, &fence);

         if (fence)
         {

            vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
         }

         ::cast<::gpu_vulkan::queue> pqueue = pcommandbuffer->m_pgpuqueue;

         vkQueueWaitIdle(pqueue->m_vkqueue);
      }
   }


   void context::on_start_layer(::gpu::layer *player)
   {

     

      ::cast<::gpu_vulkan::texture> ptexture =
         m_pgpurenderer->m_pgpurendertarget->current_texture(::gpu::current_frame());

      ::cast<command_buffer> pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_frame());

      if (pcommandbuffer->m_estate != command_buffer::e_state_recording)
      {

         ::cast<gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

         ::cast<render_target> prendertarget = m_pgpurenderer->m_pgpurendertarget;

         //::cast < render_pass > prenderpass = prendertarget->render_pass();

         auto psynchronization = ptexture->synchronization();

         VkFence fence = psynchronization->in_flight_fence();

         if (fence)
         {

            vkWaitForFences(pcontext->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
         }

         pcommandbuffer->begin_command_buffer(false);
      }

      ptexture->_set_state(pcommandbuffer,
                           {VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT});

       
      {

         if (m_pgpucompositor && m_etype == e_type_draw2d)
         {

            m_pgpucompositor->on_start_layer(player);
         }
      }
   }


void context::on_end_layer(::gpu::layer *player)
{
   ::gpu::context::on_end_layer(player);
   {
      ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast<renderer> prenderer = m_pgpurenderer;

      ::cast<command_buffer> pcommandbuffer = ::gpu::current_frame()->m_pgpucommandbuffer;
      //
      //      {
      //
      //   {
      //
      //      if (pgpucontext->m_pgpudevice->m_iLayer == 0)
      //      {
      //
      //         VkClearAttachment clearAttachment = {
      //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //            .colorAttachment = 0,
      //            .clearValue={.color = {0.5f, 0.0f, 0.0f, 0.5f}} // Red
      //         };
      //
      //         VkClearRect clearRect = {
      //             .rect = {
      //                 .offset = {100, 100},
      //                 .extent = {100, 100}
      //             },
      //             .baseArrayLayer = 0,
      //             .layerCount = 1
      //         };
      //
      //         vkCmdClearAttachments(
      //            pcommandbuffer->m_vkcommandbuffer,
      //            1,
      //            &clearAttachment,
      //            1,
      //            &clearRect);
      //
      //      }
      //      else if (pgpucontext->m_pgpudevice->m_iLayer == 1)
      //      {
      //
      //         VkClearAttachment clearAttachment = {
      //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //            .colorAttachment = 0,
      //            .clearValue = {.color = {0.0f, 0.5f, 0.0f, 0.5f} } // Green
      //         };
      //
      //         VkClearRect clearRect = {
      //             .rect = {
      //                 .offset = {200, 100},
      //                 .extent = {100, 100}
      //             },
      //             .baseArrayLayer = 0,
      //             .layerCount = 1
      //         };
      //
      //         vkCmdClearAttachments(
      //            pcommandbuffer->m_vkcommandbuffer,
      //            1,
      //            &clearAttachment,
      //            1,
      //            &clearRect);
      //
      //
      //      }
      //      else if (pgpucontext->m_pgpudevice->m_iLayer == 2)
      //      {
      //
      //         VkClearAttachment clearAttachment = {
      //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //            .colorAttachment = 0,
      //            .clearValue = {.color = {0.0f, 0.0f, 0.5f, 0.5f} } // Blue
      //         };
      //
      //         VkClearRect clearRect = {
      //             .rect = {
      //                 .offset = {300, 100},
      //                 .extent = {100, 100}
      //             },
      //             .baseArrayLayer = 0,
      //             .layerCount = 1
      //         };
      //
      //         vkCmdClearAttachments(
      //            pcommandbuffer->m_vkcommandbuffer,
      //            1,
      //            &clearAttachment,
      //            1,
      //            &clearRect);
      //
      //      }
      //
      //   }
      //
      //}


      vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);
   }
}


void context::copy(::gpu::texture *ptextureTarget, ::gpu::texture *ptextureSource)
{

   int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

   int iFrameIndex = m_pgpurenderer->m_pgpurendertarget->get_frame_index();

   ::pointer<command_buffer> pcommandbuffer;

   if (m_pgpurenderer->isFrameStarted)
   {

      pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_frame());
   }
   else
   {

      pcommandbuffer = this->beginSingleTimeCommands(m_pgpudevice->graphics_queue());
   }

   {

      ::cast<::gpu_vulkan::texture> ptextureDst = ptextureTarget;

      ::cast<::gpu_vulkan::texture> ptextureSrc = ptextureSource;

      auto scopedstateDst =
         ptextureDst->_scoped_state(pcommandbuffer, {VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                     VK_PIPELINE_STAGE_TRANSFER_BIT});

      auto scopedstateSrc =
         ptextureSrc->_scoped_state(pcommandbuffer, {VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                     VK_PIPELINE_STAGE_TRANSFER_BIT});

      auto srcImage = ptextureSrc->m_vkimage;

      auto dstImage = ptextureDst->m_vkimage;

      // Copy region
      VkImageCopy copyRegion = {
         .srcSubresource =
            {
               .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
               .mipLevel = 0,
               .baseArrayLayer = 0,
               .layerCount = 1,
            },
         .srcOffset = {0, 0, 0},
         .dstSubresource =
            {
               .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
               .mipLevel = 0,
               .baseArrayLayer = 0,
               .layerCount = 1,
            },
         .dstOffset = {0, 0, 0},
         .extent = {(uint32_t)m_rectangle.width(), (uint32_t)m_rectangle.height(), 1},
      };

      vkCmdCopyImage(pcommandbuffer->m_vkcommandbuffer, srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage,
                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
   }

   if (!m_pgpurenderer->isFrameStarted)
   {

      if (vkEndCommandBuffer(pcommandbuffer->m_vkcommandbuffer) != VK_SUCCESS)
      {

         throw ::exception(error_failed);
      }
      VkFence fence = VK_NULL_HANDLE;
      pcommandbuffer->submitCommandBuffers(ptextureTarget, {ptextureSource}, {}, {}, {}, &fence);

      if (fence)
      {

         vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
      }

      ::cast<::gpu_vulkan::queue> pqueue = pcommandbuffer->m_pgpuqueue;

      vkQueueWaitIdle(pqueue->m_vkqueue);
   }
}


VkFormat context::findDepthFormat()
{

   return m_pgpudevice->m_pphysicaldevice->findSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL,
      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}


::memory context::rectangle_shader_vert()
{

   unsigned int uaRectangleVert[] = {
#include "shader/rectangle.vert.spv.inl"
   };

   return as_memory_block(uaRectangleVert);
}


::memory context::rectangle_shader_frag()
{

   unsigned int uaRectangleFrag[] = {
#include "shader/rectangle.frag.spv.inl"
   };

   return as_memory_block(uaRectangleFrag);
}


// void context::initialize_rectangle_shader(::gpu::shader* pshader)
//{

//   auto pcontext = gpu_context();

//   //::cast < ::gpu_vulkan::device > pgpudevice = pgpucontext->m_pgpudevice;
//   pshaderRectangle->initialize_shader_with_block(
//      pcontext->m_pgpurenderer,
//      as_memory_block(g_uaRectangleVertexShader),
//      //as_memory_block(g_uaAccumulationFragmentShader),
//      as_memory_block(g_uaRectangleFragmentShader),
//      {},
//      m_psetdescriptorlayoutRectangle,
//      {},
//      pcontext->input_layout<::graphics3d::sequence2_color>());


//}


::pointer<::gpu::texture> context::loadCubemap(const ::scoped_string &name, const ::scoped_string &scopedstrFileName,
                                               VkFormat format, VkQueue vkqueueCopy, VkImageUsageFlags usageFlags,
                                               VkImageLayout initialLayout)
{

   auto pgputexture = øcreate<::gpu::texture>();

   ::cast<::gpu_vulkan::texture> ptexture = pgputexture;

   ptexture->m_pgpurenderer = m_pgpurenderer;

   try
   {

      if (scopedstrFileName.case_insensitive_ends(".ktx"))
      {

         ::cast<::gpu_vulkan::queue> pqueueGraphics = m_pgpudevice->graphics_queue();

         ptexture->KtxLoadCubemapFromFile(
            name, 
            scopedstrFileName, 
            format, 
            pqueueGraphics->m_vkqueue, 
            usageFlags,
            initialLayout);

      }
      else if (scopedstrFileName.case_insensitive_ends(".hdr"))
      {

         try
         {

            auto ptexture = cubemap_from_hdr(scopedstrFileName);

            return ptexture;

         }
         catch (const ::exception &e)
         {

            throw ::exception(e.m_estatus, "Failed to load HDR cubemap '" + name + "': " + e.get_message());

         }

      }
      else
      {

         warning() << "not implemented loadCubemap case";

      }

   }
   catch (const ::exception &e)
   {

      throw ::exception(e.m_estatus, "Failed to load HDR cubemap '" + name + "': " + e.get_message());

   }

   return pgputexture;

}

// void AssetManager::generatePrefilteredEnvMap()
//{
//   auto tStart = std::chrono::high_resolution_clock::now();

//   const VkFormat format = VK_FORMAT_R16G16B16A16_SFLOAT;
//   const int32_t dim = 512;
//   const uint32_t numMips = static_cast<uint32_t>(floor(log2(dim))) + 1;

//   // Pre-filtered cube map
//   // Image
//   VkImageCreateInfo imageCI = vkinit::imageCreateInfo();
//   imageCI.imageType = VK_IMAGE_TYPE_2D;
//   imageCI.format = format;
//   imageCI.extent.width = dim;
//   imageCI.extent.height = dim;
//   imageCI.extent.depth = 1;
//   imageCI.mipLevels = numMips;
//   imageCI.arrayLayers = 6;
//   imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
//   imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
//   imageCI.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
//   imageCI.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
//   VK_CHECK_RESULT(vkCreateImage(this->logicalDevice(), &imageCI, nullptr, &prefilteredCube->m_vkimage));
//   VkMemoryAllocateInfo memAlloc = vkinit::memoryAllocateInfo();
//   VkMemoryRequirements memReqs;
//   vkGetImageMemoryRequirements(this->logicalDevice(), prefilteredCube->m_vkimage, &memReqs);
//   memAlloc.allocationSize = memReqs.size;
//   memAlloc.memoryTypeIndex = pphysicaldevice->findMemoryType(memReqs.memoryTypeBits,
//   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT); VK_CHECK_RESULT(vkAllocateMemory(this->logicalDevice(), &memAlloc, nullptr,
//   &prefilteredCube->m_vkdevicememory)); VK_CHECK_RESULT(
//      vkBindImageMemory(this->logicalDevice(), prefilteredCube->m_vkimage, prefilteredCube->m_vkdevicememory, 0));
//   // Image view
//   VkImageViewCreateInfo viewCI = vkinit::imageViewCreateInfo();
//   viewCI.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
//   viewCI.format = format;
//   viewCI.subresourceRange = {};
//   viewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//   viewCI.subresourceRange.levelCount = numMips;
//   viewCI.subresourceRange.layerCount = 6;
//   viewCI.image = prefilteredCube->m_vkimage;
//   VK_CHECK_RESULT(vkCreateImageView(this->logicalDevice(), &viewCI, nullptr, &prefilteredCube->m_vkimageview));
//   // Sampler
//   VkSamplerCreateInfo samplerCI = vkinit::samplerCreateInfo();
//   samplerCI.magFilter = VK_FILTER_LINEAR;
//   samplerCI.minFilter = VK_FILTER_LINEAR;
//   samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
//   samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//   samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//   samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//   samplerCI.minLod = 0.0f;
//   samplerCI.maxLod = static_cast<float>(numMips);
//   samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
//   VK_CHECK_RESULT(vkCreateSampler(this->logicalDevice(), &samplerCI, nullptr, &prefilteredCube->m_sampler));

//   prefilteredCube->m_descriptor.imageView = prefilteredCube->m_vkimageview;
//   prefilteredCube->m_descriptor.sampler = prefilteredCube->m_sampler;
//   prefilteredCube->m_descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//   prefilteredCube->m_pDevice = &m_device;

//   // FB, Att, RP, Pipe, etc.
//   VkAttachmentDescription attDesc = {};
//   // Color attachment
//   attDesc.format = format;
//   attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
//   attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//   attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//   attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//   attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//   attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//   attDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//   VkAttachmentReference colorReference = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

//   VkSubpassDescription subpassDescription = {};
//   subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//   subpassDescription.colorAttachmentCount = 1;
//   subpassDescription.pColorAttachments = &colorReference;

//   // Use subpass dependencies for layout transitions
//   std::array<VkSubpassDependency, 2> dependencies;
//   dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
//   dependencies[0].dstSubpass = 0;
//   dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//   dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//   dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//   dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//   dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//   dependencies[1].srcSubpass = 0;
//   dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
//   dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//   dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//   dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//   dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//   dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

//   // Renderpass
//   VkRenderPassCreateInfo renderPassCI = vkinit::renderPassCreateInfo();
//   renderPassCI.attachmentCount = 1;
//   renderPassCI.pAttachments = &attDesc;
//   renderPassCI.subpassCount = 1;
//   renderPassCI.pSubpasses = &subpassDescription;
//   renderPassCI.dependencyCount = 2;
//   renderPassCI.pDependencies = dependencies.data();
//   VkRenderPass renderpass;
//   VK_CHECK_RESULT(vkCreateRenderPass(this->logicalDevice(), &renderPassCI, nullptr, &renderpass));

//   struct
//   {
//      VkImage image;
//      VkImageView view;
//      VkDeviceMemory memory;
//      VkFramebuffer framebuffer;
//   } offscreen;

//   // Offfscreen framebuffer
//   {
//      // Color attachment
//      VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
//      imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
//      imageCreateInfo.format = format;
//      imageCreateInfo.extent.width = dim;
//      imageCreateInfo.extent.height = dim;
//      imageCreateInfo.extent.depth = 1;
//      imageCreateInfo.mipLevels = 1;
//      imageCreateInfo.arrayLayers = 1;
//      imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
//      imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
//      imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//      imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
//      imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//      VK_CHECK_RESULT(vkCreateImage(this->logicalDevice(), &imageCreateInfo, nullptr, &offscreen.image));

//      VkMemoryAllocateInfo memAlloc = vkinit::memoryAllocateInfo();
//      VkMemoryRequirements memReqs;
//      vkGetImageMemoryRequirements(this->logicalDevice(), offscreen.image, &memReqs);
//      memAlloc.allocationSize = memReqs.size;
//      memAlloc.memoryTypeIndex =
//         pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//      VK_CHECK_RESULT(vkAllocateMemory(this->logicalDevice(), &memAlloc, nullptr, &offscreen.memory));
//      VK_CHECK_RESULT(vkBindImageMemory(this->logicalDevice(), offscreen.image, offscreen.memory, 0));

//      VkImageViewCreateInfo colorImageView = vkinit::imageViewCreateInfo();
//      colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
//      colorImageView.format = format;
//      colorImageView.flags = 0;
//      colorImageView.subresourceRange = {};
//      colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//      colorImageView.subresourceRange.baseMipLevel = 0;
//      colorImageView.subresourceRange.levelCount = 1;
//      colorImageView.subresourceRange.baseArrayLayer = 0;
//      colorImageView.subresourceRange.layerCount = 1;
//      colorImageView.image = offscreen.image;
//      VK_CHECK_RESULT(vkCreateImageView(this->logicalDevice(), &colorImageView, nullptr, &offscreen.view));

//      VkFramebufferCreateInfo fbufCreateInfo = vkinit::framebufferCreateInfo();
//      fbufCreateInfo.renderPass = renderpass;
//      fbufCreateInfo.attachmentCount = 1;
//      fbufCreateInfo.pAttachments = &offscreen.view;
//      fbufCreateInfo.width = dim;
//      fbufCreateInfo.height = dim;
//      fbufCreateInfo.layers = 1;
//      VK_CHECK_RESULT(vkCreateFramebuffer(this->logicalDevice(), &fbufCreateInfo, nullptr, &offscreen.framebuffer));

//      VkCommandBuffer layoutCmd = m_device.createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
//      tools::setImageLayout(layoutCmd, offscreen.image, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED,
//                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//      m_device.flushCommandBuffer(layoutCmd, m_transferQueue, true);
//   }

//   // --- Descriptor layout / pool / set ---
//   VkDescriptorSetLayout descriptorsetlayout = VK_NULL_HANDLE;
//   ::array_base<VkDescriptorSetLayoutBinding> setLayoutBindings = {
//      vkinit::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT,
//                                         0),
//   };
//   VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI =
//   vkinit::descriptorSetLayoutCreateInfo(setLayoutBindings); VK_CHECK_RESULT(
//      vkCreateDescriptorSetLayout(this->logicalDevice(), &descriptorsetlayoutCI, nullptr, &descriptorsetlayout));

//   // Descriptor Pool
//   VkDescriptorPool descriptorpool = VK_NULL_HANDLE;
//   ::array_base<VkDescriptorPoolSize> poolSizes = {
//      vkinit::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)};
//   VkDescriptorPoolCreateInfo descriptorPoolCI = vkinit::descriptorPoolCreateInfo(poolSizes, 2);
//   VK_CHECK_RESULT(vkCreateDescriptorPool(this->logicalDevice(), &descriptorPoolCI, nullptr, &descriptorpool));

//   // Allocate descriptor set
//   VkDescriptorSet descriptorset = VK_NULL_HANDLE;
//   VkDescriptorSetAllocateInfo allocInfo =
//      vkinit::descriptorSetAllocateInfo(descriptorpool, &descriptorsetlayout, 1);
//   VK_CHECK_RESULT(vkAllocateDescriptorSets(this->logicalDevice(), &allocInfo, &descriptorset));

//   // Write the environment cubemap descriptor (make sure environmentCube is valid)
//   if (!environmentCube)
//   {
//      throw std::runtime_error("generatePrefilteredEnvMap: environmentCube is null");
//   }
//   VkWriteDescriptorSet writeDescriptorSet = vkinit::writeDescriptorSet(
//      descriptorset, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &environmentCube->m_descriptor);
//   vkUpdateDescriptorSets(this->logicalDevice(), 1, &writeDescriptorSet, 0, nullptr);

//   // --- Pipeline layout & push constants ---
//   struct PushBlock
//   {
//      glm::mat4 mvp;
//      float roughness;
//      uint32_t numSamples = 32u;
//   } pushBlock;

//   VkPushConstantRange pushRange{};
//   pushRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
//   pushRange.offset = 0;
//   pushRange.size = sizeof(PushBlock);

//   VkPipelineLayout pipelineLayoutLocal = VK_NULL_HANDLE;
//   VkPipelineLayoutCreateInfo pipelineLayoutCI = vkinit::pipelineLayoutCreateInfo(&descriptorsetlayout, 1);
//   pipelineLayoutCI.pushConstantRangeCount = 1;
//   pipelineLayoutCI.pPushConstantRanges = &pushRange;
//   VK_CHECK_RESULT(vkCreatePipelineLayout(this->logicalDevice(), &pipelineLayoutCI, nullptr, &pipelineLayoutLocal));

//   // --- Pipeline creation using your VkSandboxPipeline wrapper (vertex pos only) ---
//   ::vulkan::pipeline_configuration cfg{};
//   this->default_pipeline_configuration(cfg);

//   // Vertex input: vec3 position only (location 0)
//   VkVertexInputBindingDescription bindingDesc{};
//   bindingDesc.binding = 0;
//   bindingDesc.stride = sizeof(::gpu_vulkan::gltf::Vertex);
//   bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

//   VkVertexInputAttributeDescription attrDesc{};
//   attrDesc.binding = 0;
//   attrDesc.location = 0;
//   attrDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
//   attrDesc.offset = 0;

//   cfg.bindingDescriptions = {bindingDesc};
//   cfg.attributeDescriptions = {attrDesc};
//   cfg.renderPass = renderpass;
//   cfg.pipelineLayout = pipelineLayoutLocal;
//   cfg.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
//   cfg.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//   cfg.dynamicStateInfo.pDynamicStates = cfg.dynamicStateEnables.data();
//   cfg.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(cfg.dynamicStateEnables.size());
//   cfg.descriptorSetLayouts = {descriptorsetlayout};
//   cfg.pushConstantRanges = {pushRange};

//   // shader paths (match your project layout)
//   std::string vert = std::string(PROJECT_ROOT_DIR) + "/res/shaders/spirV/filtered_cube.vert.spv";
//   std::string frag = std::string(PROJECT_ROOT_DIR) + "/res/shaders/spirV/prefiltered_env_map.spv";

//   if (frag.empty())
//   {
//      // cleanup minimal resources
//      if (pipelineLayoutLocal != VK_NULL_HANDLE)
//         vkDestroyPipelineLayout(this->logicalDevice(), pipelineLayoutLocal, nullptr);
//      if (descriptorsetlayout != VK_NULL_HANDLE)
//         vkDestroyDescriptorSetLayout(this->logicalDevice(), descriptorsetlayout, nullptr);
//      if (descriptorpool != VK_NULL_HANDLE)
//         vkDestroyDescriptorPool(this->logicalDevice(), descriptorpool, nullptr);
//      throw std::runtime_error("Prefilter fragment shader SPIR-V not found");
//   }

//   VkSandboxPipeline prefilterPipeline{m_device, vert, frag, cfg};

//   // --- Command buffer & initial transitions (use m_device helpers) ---
//   VkCommandBuffer pcommandbufferCmd->m_vkcommandbuffer =
//   m_device.createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

//   VkImageSubresourceRange subresourceRange = {};
//   subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//   subresourceRange.baseMipLevel = 0;
//   subresourceRange.levelCount = numMips;
//   subresourceRange.layerCount = 6;

//   // Transition target cubemap to transfer dst
//   tools::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, prefilteredCube->m_vkimage, VK_IMAGE_LAYOUT_UNDEFINED,
//                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);

//   // Setup matrices and viewports
//   ::array_base<glm::mat4> matrices = {
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // +X
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // -X
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)), // +Y
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // -Y
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // +Z
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // -Z
//   };

//   VkViewport viewport = vkinit::viewport((float)dim, (float)dim, 0.0f, 1.0f);
//   VkRect2D scissor = vkinit::rect2D(dim, dim, 0, 0);

//   vkCmdSetViewport(pcommandbufferCmd->m_vkcommandbuffer, 0, 1, &viewport);
//   vkCmdSetScissor(pcommandbufferCmd->m_vkcommandbuffer, 0, 1, &scissor);

//   // --- Main render loop (mips + faces) ---
//   for (uint32_t m = 0; m < numMips; m++)
//   {
//      pushBlock.roughness = static_cast<float>(m) / static_cast<float>(numMips - 1);
//      uint32_t mipDim = static_cast<uint32_t>(dim * std::pow(0.5f, (float)m));
//      viewport.width = static_cast<float>(mipDim);
//      viewport.height = static_cast<float>(mipDim);

//      for (uint32_t f = 0; f < 6; f++)
//      {
//         // Update render area for this mip
//         VkRenderPassBeginInfo rpBI = vkinit::renderPassBeginInfo();
//         rpBI.renderPass = renderpass;
//         rpBI.framebuffer = offscreen.framebuffer;
//         rpBI.renderArea.extent.width = mipDim;
//         rpBI.renderArea.extent.height = mipDim;
//         VkClearValue clear{{{0.0f, 0.0f, 0.0f, 0.0f}}};
//         rpBI.clearValueCount = 1;
//         rpBI.pClearValues = &clear;

//         vkCmdBeginRenderPass(pcommandbufferCmd->m_vkcommandbuffer, &rpBI, VK_SUBPASS_CONTENTS_INLINE);

//         vkCmdSetViewport(pcommandbufferCmd->m_vkcommandbuffer, 0, 1, &viewport);
//         vkCmdSetScissor(pcommandbufferCmd->m_vkcommandbuffer, 0, 1, &scissor);

//         // push constants (projection * view)
//         pushBlock.mvp = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 512.0f) * matrices[f];
//         pushBlock.mvp[1][1] *= -1.0f; // flip y

//         vkCmdPushConstants(pcommandbufferCmd->m_vkcommandbuffer, prefilterPipeline.getPipelineLayout(),
//                            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushBlock),
//                            &pushBlock);

//         // bind pipeline and descriptor set (environment cubemap sampler)
//         prefilterPipeline.bind(pcommandbufferCmd->m_vkcommandbuffer);
//         vkCmdBindDescriptorSets(pcommandbufferCmd->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
//         prefilterPipeline.getPipelineLayout(), 0,
//                                 1, &descriptorset, 0, nullptr);

//         // draw the skybox mesh (ensure it binds position vertex at location 0)
//         if (!m_skyboxModel)
//         {
//            spdlog::error("[AssetManager] No skybox model loaded - skipping draw in generatePrefilteredEnvMap()");
//         }
//         else
//         {
//            m_skyboxModel->bind(pcommandbufferCmd->m_vkcommandbuffer);
//            m_skyboxModel->gltfDraw(pcommandbufferCmd->m_vkcommandbuffer);
//         }

//         vkCmdEndRenderPass(pcommandbufferCmd->m_vkcommandbuffer);

//         // copy from offscreen -> prefilteredCube mip/face
//         tools::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, offscreen.image, VK_IMAGE_ASPECT_COLOR_BIT,
//                               VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

//         VkImageCopy copyRegion{};
//         copyRegion.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
//         copyRegion.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, m, f, 1};
//         copyRegion.extent = {mipDim, mipDim, 1};

//         vkCmdCopyImage(pcommandbufferCmd->m_vkcommandbuffer, offscreen.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
//         prefilteredCube->m_vkimage,
//                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

//         // restore offscreen layout
//         tools::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, offscreen.image, VK_IMAGE_ASPECT_COLOR_BIT,
//                               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//      }
//   }

//   // final transition: prefiltered cubemap -> shader read
//   tools::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, prefilteredCube->m_vkimage,
//   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
//                         VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);

//   m_device.flushCommandBuffer(pcommandbufferCmd->m_vkcommandbuffer, m_transferQueue);
//   vkQueueWaitIdle(m_transferQueue);


//   // --- Cleanup: destroy only resources we created here (do NOT destroy pipeline layout; wrapper owns pipeline)
//   if (offscreen.framebuffer != VK_NULL_HANDLE)
//      vkDestroyFramebuffer(this->logicalDevice(), offscreen.framebuffer, nullptr);
//   if (renderpass != VK_NULL_HANDLE)
//      vkDestroyRenderPass(this->logicalDevice(), renderpass, nullptr);
//   if (offscreen.memory != VK_NULL_HANDLE)
//      vkFreeMemory(this->logicalDevice(), offscreen.memory, nullptr);
//   if (offscreen.view != VK_NULL_HANDLE)
//      vkDestroyImageView(this->logicalDevice(), offscreen.view, nullptr);
//   if (offscreen.image != VK_NULL_HANDLE)
//      vkDestroyImage(this->logicalDevice(), offscreen.image, nullptr);
//   if (descriptorpool != VK_NULL_HANDLE)
//      vkDestroyDescriptorPool(this->logicalDevice(), descriptorpool, nullptr);
//   if (descriptorsetlayout != VK_NULL_HANDLE)
//      vkDestroyDescriptorSetLayout(this->logicalDevice(), descriptorsetlayout, nullptr);

//   auto tEnd = std::chrono::high_resolution_clock::now();
//   auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
//   spdlog::info("Generating pre-filtered environment cube with {} mip levels took {} ms", numMips, tDiff);
//}


//::pointer<::gpu::texture> context::generate_ibl_prefiltered_env_map(::gpu::texture *environmentCubeExisting,
//                                                             ::graphics3d::renderable *prenderableSkybox)
//{
//
//   ::pointer<::gpu::texture> prefilteredCubeNew;
//
//   øconstruct(prefilteredCubeNew);
//
//   ::cast<::gpu_vulkan::texture> prefilteredCube = prefilteredCubeNew;
//
//   ::cast<::gpu_vulkan::texture> environmentCube = environmentCubeExisting;
//
//   if (!prefilteredCube || !environmentCube || !prenderableSkybox)
//   {
//
//      error("bad argument");
//
//      throw ::exception(error_failed);
//   }
//
//   ::cast<device> pdevice = m_pgpudevice;
//
//   auto pphysicaldevice = pdevice->m_pphysicaldevice;
//
//
//   auto tStart = std::chrono::high_resolution_clock::now();
//
//   const VkFormat format = VK_FORMAT_R16G16B16A16_SFLOAT;
//   const int32_t dim = 512;
//   const uint32_t numMips = static_cast<uint32_t>(floor(log2(dim))) + 1;
//
//   // Pre-filtered cube map
//   // Image
//   VkImageCreateInfo imageCI = vkinit::imageCreateInfo();
//   imageCI.imageType = VK_IMAGE_TYPE_2D;
//   imageCI.format = format;
//   imageCI.extent.width = dim;
//   imageCI.extent.height = dim;
//   imageCI.extent.depth = 1;
//   imageCI.mipLevels = numMips;
//   imageCI.arrayLayers = 6;
//   imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
//   imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
//   imageCI.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
//   imageCI.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
//   VK_CHECK_RESULT(vkCreateImage(this->logicalDevice(), &imageCI, nullptr, &prefilteredCube->m_vkimage));
//   VkMemoryAllocateInfo memAlloc = vkinit::memoryAllocateInfo();
//   VkMemoryRequirements memReqs;
//   vkGetImageMemoryRequirements(this->logicalDevice(), prefilteredCube->m_vkimage, &memReqs);
//   memAlloc.allocationSize = memReqs.size;
//   memAlloc.memoryTypeIndex =
//      pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//   VK_CHECK_RESULT(vkAllocateMemory(this->logicalDevice(), &memAlloc, nullptr, &prefilteredCube->m_vkdevicememory));
//   VK_CHECK_RESULT(
//      vkBindImageMemory(this->logicalDevice(), prefilteredCube->m_vkimage, prefilteredCube->m_vkdevicememory, 0));
//   // Image view
//   VkImageViewCreateInfo viewCI = vkinit::imageViewCreateInfo();
//   viewCI.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
//   viewCI.format = format;
//   viewCI.subresourceRange = {};
//   viewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//   viewCI.subresourceRange.levelCount = numMips;
//   viewCI.subresourceRange.layerCount = 6;
//   viewCI.image = prefilteredCube->m_vkimage;
//   VK_CHECK_RESULT(vkCreateImageView(this->logicalDevice(), &viewCI, nullptr, &prefilteredCube->m_vkimageview));
//   // Sampler
//   VkSamplerCreateInfo samplerCI = vkinit::samplerCreateInfo();
//   samplerCI.magFilter = VK_FILTER_LINEAR;
//   samplerCI.minFilter = VK_FILTER_LINEAR;
//   samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
//   samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//   samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//   samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//   samplerCI.minLod = 0.0f;
//   samplerCI.maxLod = static_cast<float>(numMips);
//   samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
//   VK_CHECK_RESULT(vkCreateSampler(this->logicalDevice(), &samplerCI, nullptr, &prefilteredCube->m_vksamplerDedicated));
//
//   prefilteredCube->m_descriptor3.imageView = prefilteredCube->m_vkimageview;
//   prefilteredCube->m_descriptor3.sampler = prefilteredCube->m_vksamplerDedicated;
//   prefilteredCube->m_descriptor3.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//   // prefilteredCube->m_pDevice = &m_device;
//
//   // FB, Att, RP, Pipe, etc.
//   VkAttachmentDescription attDesc = {};
//   // Color attachment
//   attDesc.format = format;
//   attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
//   attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//   attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//   attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//   attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//   attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//   attDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//   VkAttachmentReference colorReference = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
//
//   VkSubpassDescription subpassDescription = {};
//   subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//   subpassDescription.colorAttachmentCount = 1;
//   subpassDescription.pColorAttachments = &colorReference;
//
//   // Use subpass dependencies for layout transitions
//   std::array<VkSubpassDependency, 2> dependencies;
//   dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
//   dependencies[0].dstSubpass = 0;
//   dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//   dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//   dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//   dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//   dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//   dependencies[1].srcSubpass = 0;
//   dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
//   dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//   dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//   dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//   dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//   dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//   // Renderpass
//   VkRenderPassCreateInfo renderPassCI = vkinit::renderPassCreateInfo();
//   renderPassCI.attachmentCount = 1;
//   renderPassCI.pAttachments = &attDesc;
//   renderPassCI.subpassCount = 1;
//   renderPassCI.pSubpasses = &subpassDescription;
//   renderPassCI.dependencyCount = 2;
//   renderPassCI.pDependencies = dependencies.data();
//   VkRenderPass renderpass;
//   VK_CHECK_RESULT(vkCreateRenderPass(this->logicalDevice(), &renderPassCI, nullptr, &renderpass));
//
//   struct
//   {
//      VkImage image;
//      VkImageView view;
//      VkDeviceMemory memory;
//      VkFramebuffer framebuffer;
//   } offscreen;
//
//   // Offfscreen framebuffer
//   {
//      // Color attachment
//      VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
//      imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
//      imageCreateInfo.format = format;
//      imageCreateInfo.extent.width = dim;
//      imageCreateInfo.extent.height = dim;
//      imageCreateInfo.extent.depth = 1;
//      imageCreateInfo.mipLevels = 1;
//      imageCreateInfo.arrayLayers = 1;
//      imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
//      imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
//      imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//      imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
//      imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//      VK_CHECK_RESULT(vkCreateImage(this->logicalDevice(), &imageCreateInfo, nullptr, &offscreen.image));
//
//      VkMemoryAllocateInfo memAlloc = vkinit::memoryAllocateInfo();
//      VkMemoryRequirements memReqs;
//      vkGetImageMemoryRequirements(this->logicalDevice(), offscreen.image, &memReqs);
//      memAlloc.allocationSize = memReqs.size;
//      memAlloc.memoryTypeIndex =
//         pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//      VK_CHECK_RESULT(vkAllocateMemory(this->logicalDevice(), &memAlloc, nullptr, &offscreen.memory));
//      VK_CHECK_RESULT(vkBindImageMemory(this->logicalDevice(), offscreen.image, offscreen.memory, 0));
//
//      VkImageViewCreateInfo colorImageView = vkinit::imageViewCreateInfo();
//      colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
//      colorImageView.format = format;
//      colorImageView.flags = 0;
//      colorImageView.subresourceRange = {};
//      colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//      colorImageView.subresourceRange.baseMipLevel = 0;
//      colorImageView.subresourceRange.levelCount = 1;
//      colorImageView.subresourceRange.baseArrayLayer = 0;
//      colorImageView.subresourceRange.layerCount = 1;
//      colorImageView.image = offscreen.image;
//      VK_CHECK_RESULT(vkCreateImageView(this->logicalDevice(), &colorImageView, nullptr, &offscreen.view));
//
//      VkFramebufferCreateInfo fbufCreateInfo = vkinit::framebufferCreateInfo();
//      fbufCreateInfo.renderPass = renderpass;
//      fbufCreateInfo.attachmentCount = 1;
//      fbufCreateInfo.pAttachments = &offscreen.view;
//      fbufCreateInfo.width = dim;
//      fbufCreateInfo.height = dim;
//      fbufCreateInfo.layers = 1;
//      VK_CHECK_RESULT(vkCreateFramebuffer(this->logicalDevice(), &fbufCreateInfo, nullptr, &offscreen.framebuffer));
//
//      // VkCommandBuffer layoutCmd = m_device.createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
//      auto pgpucommandbufferLayoutCmd = this->beginSingleTimeCommands(transfer_queue());
//      ::cast<command_buffer> pcommandbufferLayoutCmd = pgpucommandbufferLayoutCmd;
//      ::vulkan::setImageLayout(pcommandbufferLayoutCmd->m_vkcommandbuffer, offscreen.image, VK_IMAGE_ASPECT_COLOR_BIT,
//                               VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//      // m_device.flushCommandBuffer(pcommandbufferLayoutCmd->m_vkcommandbuffer, m_transferQueue, true);          ;
//      this->endSingleTimeCommands(pcommandbufferLayoutCmd);
//   }
//
//   // --- Descriptor layout / pool / set ---
//   VkDescriptorSetLayout descriptorsetlayout = VK_NULL_HANDLE;
//   ::array_base<VkDescriptorSetLayoutBinding> setLayoutBindings = {
//      vkinit::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0),
//   };
//   VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI = vkinit::descriptorSetLayoutCreateInfo(setLayoutBindings);
//   VK_CHECK_RESULT(
//      vkCreateDescriptorSetLayout(this->logicalDevice(), &descriptorsetlayoutCI, nullptr, &descriptorsetlayout));
//
//   // Descriptor Pool
//   VkDescriptorPool descriptorpool = VK_NULL_HANDLE;
//   ::array_base<VkDescriptorPoolSize> poolSizes = {
//      vkinit::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)};
//   VkDescriptorPoolCreateInfo descriptorPoolCI = vkinit::descriptorPoolCreateInfo(poolSizes, 2);
//   VK_CHECK_RESULT(vkCreateDescriptorPool(this->logicalDevice(), &descriptorPoolCI, nullptr, &descriptorpool));
//
//   // Allocate descriptor set
//   VkDescriptorSet descriptorset = VK_NULL_HANDLE;
//   VkDescriptorSetAllocateInfo allocInfo = vkinit::descriptorSetAllocateInfo(descriptorpool, &descriptorsetlayout, 1);
//   VK_CHECK_RESULT(vkAllocateDescriptorSets(this->logicalDevice(), &allocInfo, &descriptorset));
//
//   // Write the environment cubemap descriptor (make sure environmentCube is valid)
//   if (!environmentCube)
//   {
//      throw ::exception(error_failed, "generatePrefilteredEnvMap: environmentCube is null");
//   }
//   VkWriteDescriptorSet writeDescriptorSet = vkinit::writeDescriptorSet(
//      descriptorset, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &environmentCube->m_descriptor3);
//   vkUpdateDescriptorSets(this->logicalDevice(), 1, &writeDescriptorSet, 0, nullptr);
//
//   // --- Pipeline layout & push constants ---
//   struct PushBlock
//   {
//      glm::mat4 mvp;
//      float roughness;
//      uint32_t numSamples = 32u;
//   } pushBlock;
//
//   VkPushConstantRange pushRange{};
//   pushRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
//   pushRange.offset = 0;
//   pushRange.size = sizeof(PushBlock);
//
//   VkPipelineLayout pipelineLayoutLocal = VK_NULL_HANDLE;
//   VkPipelineLayoutCreateInfo pipelineLayoutCI = vkinit::pipelineLayoutCreateInfo(&descriptorsetlayout, 1);
//   pipelineLayoutCI.pushConstantRangeCount = 1;
//   pipelineLayoutCI.pPushConstantRanges = &pushRange;
//   VK_CHECK_RESULT(vkCreatePipelineLayout(this->logicalDevice(), &pipelineLayoutCI, nullptr, &pipelineLayoutLocal));
//
//   // --- Pipeline creation using your VkSandboxPipeline wrapper (vertex pos only) ---
//   ::vulkan::pipeline_configuration cfg{};
//   ::vulkan::defaultPipelineConfigInfo2(cfg);
//
//   // Vertex input: vec3 position only (location 0)
//   VkVertexInputBindingDescription bindingDesc{};
//   bindingDesc.binding = 0;
//   bindingDesc.stride = sizeof(::gpu_vulkan::gltf::Vertex);
//   bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
//
//   VkVertexInputAttributeDescription attrDesc{};
//   attrDesc.binding = 0;
//   attrDesc.location = 0;
//   attrDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
//   attrDesc.offset = 0;
//
//   cfg.bindingDescriptions = {bindingDesc};
//   cfg.attributeDescriptions = {attrDesc};
//   cfg.renderPass = renderpass;
//   cfg.pipelineLayout = pipelineLayoutLocal;
//   cfg.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
//   cfg.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//   cfg.dynamicStateInfo.pDynamicStates = cfg.dynamicStateEnables.data();
//   cfg.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(cfg.dynamicStateEnables.size());
//   cfg.descriptorSetLayouts = {descriptorsetlayout};
//   cfg.pushConstantRanges = {pushRange};
//
//   // shader paths (match your project layout)
//   ::memory vert;
//   ::memory frag;
//   pdevice->defer_shader_memory(vert, "matter://shaders/filtered_cube.vert");
//   pdevice->defer_shader_memory(frag, "matter://shaders/prefiltered_env_map.frag");
//   // auto vert = file()->as_memory("matter://shaders/filtered_cube.vert");
//   // auto frag = file()->as_memory("matter://shaders/prefiltered_env_map.frag");
//
//   if (frag.is_empty())
//   {
//      // cleanup minimal resources
//      if (pipelineLayoutLocal != VK_NULL_HANDLE)
//         vkDestroyPipelineLayout(this->logicalDevice(), pipelineLayoutLocal, nullptr);
//      if (descriptorsetlayout != VK_NULL_HANDLE)
//         vkDestroyDescriptorSetLayout(this->logicalDevice(), descriptorsetlayout, nullptr);
//      if (descriptorpool != VK_NULL_HANDLE)
//         vkDestroyDescriptorPool(this->logicalDevice(), descriptorpool, nullptr);
//      throw ::exception(error_failed, "Prefilter fragment shader SPIR-V not found");
//   }
//
//   auto prefilterPipeline = øcreate<::gpu_vulkan::pipeline>();
//
//
//   prefilterPipeline->initialize_graphics_pipeline(m_pgpurenderer, vert, frag, cfg);
//
//   // --- Command buffer & initial transitions (use m_device helpers) ---
//   // VkCommandBuffer pcommandbufferCmd->m_vkcommandbuffer =
//   // m_device.createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
//
//   auto pgpucommandbufferCmd = this->beginSingleTimeCommands(transfer_queue());
//
//   ::cast<::gpu_vulkan::command_buffer> pcommandbufferCmd = pgpucommandbufferCmd;
//
//   VkImageSubresourceRange subresourceRange = {};
//   subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//   subresourceRange.baseMipLevel = 0;
//   subresourceRange.levelCount = numMips;
//   subresourceRange.layerCount = 6;
//
//   // Transition target cubemap to transfer dst
//   ::vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, prefilteredCube->m_vkimage, VK_IMAGE_LAYOUT_UNDEFINED,
//                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);
//
//   // Setup matrices and viewports
//   ::array_base<glm::mat4> matrices = {
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // +X
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // -X
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)), // +Y
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // -Y
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // +Z
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // -Z
//   };
//
//   VkViewport viewport = vkinit::viewport((float)dim, (float)dim, 0.0f, 1.0f);
//   VkRect2D scissor = vkinit::rect2D(dim, dim, 0, 0);
//
//   vkCmdSetViewport(pcommandbufferCmd->m_vkcommandbuffer, 0, 1, &viewport);
//   vkCmdSetScissor(pcommandbufferCmd->m_vkcommandbuffer, 0, 1, &scissor);
//
//   // --- Main render loop (mips + faces) ---
//   for (uint32_t m = 0; m < numMips; m++)
//   {
//      pushBlock.roughness = static_cast<float>(m) / static_cast<float>(numMips - 1);
//      uint32_t mipDim = static_cast<uint32_t>(dim * std::pow(0.5f, (float)m));
//      viewport.width = static_cast<float>(mipDim);
//      viewport.height = static_cast<float>(mipDim);
//
//      for (uint32_t f = 0; f < 6; f++)
//      {
//         // Update render area for this mip
//         VkRenderPassBeginInfo rpBI = vkinit::renderPassBeginInfo();
//         rpBI.renderPass = renderpass;
//         rpBI.framebuffer = offscreen.framebuffer;
//         rpBI.renderArea.extent.width = mipDim;
//         rpBI.renderArea.extent.height = mipDim;
//         VkClearValue clear{{{0.0f, 0.0f, 0.0f, 0.0f}}};
//         rpBI.clearValueCount = 1;
//         rpBI.pClearValues = &clear;
//
//         vkCmdBeginRenderPass(pcommandbufferCmd->m_vkcommandbuffer, &rpBI, VK_SUBPASS_CONTENTS_INLINE);
//
//         vkCmdSetViewport(pcommandbufferCmd->m_vkcommandbuffer, 0, 1, &viewport);
//         vkCmdSetScissor(pcommandbufferCmd->m_vkcommandbuffer, 0, 1, &scissor);
//
//         // push constants (projection * view)
//         pushBlock.mvp = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 512.0f) * matrices[f];
//         pushBlock.mvp[1][1] *= -1.0f; // flip y
//
//         vkCmdPushConstants(pcommandbufferCmd->m_vkcommandbuffer, prefilterPipeline->m_vkpipelinelayout,
//                            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushBlock),
//                            &pushBlock);
//
//         // bind pipeline and descriptor set (environment cubemap sampler)
//         prefilterPipeline->bind(pcommandbufferCmd);
//         vkCmdBindDescriptorSets(pcommandbufferCmd->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
//                                 prefilterPipeline->m_vkpipelinelayout, 0, 1, &descriptorset, 0, nullptr);
//
//         // draw the skybox mesh (ensure it binds position vertex at location 0)
//         // if (!m_skyboxModel)
//         //{
//         //   spdlog::error("[AssetManager] No skybox model loaded - skipping draw in generatePrefilteredEnvMap()");
//         //}
//         // else
//         //{
//         prenderableSkybox->bind(pcommandbufferCmd);
//         // prenderableSkybox->gltfDraw(pcommandbufferCmd);
//         prenderableSkybox->draw(pcommandbufferCmd);
//         //}
//
//         vkCmdEndRenderPass(pcommandbufferCmd->m_vkcommandbuffer);
//
//         // copy from offscreen -> prefilteredCube mip/face
//         ::vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, offscreen.image, VK_IMAGE_ASPECT_COLOR_BIT,
//                                  VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
//
//         VkImageCopy copyRegion{};
//         copyRegion.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
//         copyRegion.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, m, f, 1};
//         copyRegion.extent = {mipDim, mipDim, 1};
//
//         vkCmdCopyImage(pcommandbufferCmd->m_vkcommandbuffer, offscreen.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
//                        prefilteredCube->m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
//
//         // restore offscreen layout
//         ::vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, offscreen.image, VK_IMAGE_ASPECT_COLOR_BIT,
//                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//      }
//   }
//
//   // final transition: prefiltered cubemap -> shader read
//   ::vulkan::setImageLayout(pcommandbufferCmd->m_vkcommandbuffer, prefilteredCube->m_vkimage,
//                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
//                            subresourceRange);
//
//   this->endSingleTimeCommands(pcommandbufferCmd);
//   ::cast<::gpu_vulkan::queue> pqueue = pcommandbufferCmd->m_pgpuqueue;
//   vkQueueWaitIdle(pqueue->m_vkqueue);
//
//
//   // --- Cleanup: destroy only resources we created here (do NOT destroy pipeline layout; wrapper owns pipeline)
//   if (offscreen.framebuffer != VK_NULL_HANDLE)
//      vkDestroyFramebuffer(this->logicalDevice(), offscreen.framebuffer, nullptr);
//   if (renderpass != VK_NULL_HANDLE)
//      vkDestroyRenderPass(this->logicalDevice(), renderpass, nullptr);
//   if (offscreen.memory != VK_NULL_HANDLE)
//      vkFreeMemory(this->logicalDevice(), offscreen.memory, nullptr);
//   if (offscreen.view != VK_NULL_HANDLE)
//      vkDestroyImageView(this->logicalDevice(), offscreen.view, nullptr);
//   if (offscreen.image != VK_NULL_HANDLE)
//      vkDestroyImage(this->logicalDevice(), offscreen.image, nullptr);
//   if (descriptorpool != VK_NULL_HANDLE)
//      vkDestroyDescriptorPool(this->logicalDevice(), descriptorpool, nullptr);
//   if (descriptorsetlayout != VK_NULL_HANDLE)
//      vkDestroyDescriptorSetLayout(this->logicalDevice(), descriptorsetlayout, nullptr);
//
//   auto tEnd = std::chrono::high_resolution_clock::now();
//   auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
//   information("Generating pre-filtered environment cube with {} mip levels took {} ms", numMips, tDiff);
//
//
//   return prefilteredCubeNew;
//}
//
//
//::pointer<::gpu::texture> context::generateIrradianceMap(
//   //::gpu::texture * ptextureIrradianceCubeNewlyAllocatedPointer,
//   ::gpu::texture *environmentCubeExisting, ::graphics3d::renderable *prenderableSkybox)
//{
//
//   ::cast<device> pgpudevice = m_pgpudevice;
//
//   ::pointer<::gpu::texture> irradianceCubeNew;
//
//   øconstruct(irradianceCubeNew);
//
//   ::cast<::gpu_vulkan::texture> irradianceCube = irradianceCubeNew;
//
//   ::cast<::gpu_vulkan::texture> environmentCube = environmentCubeExisting;
//
//   if (!irradianceCube || !environmentCube || !prenderableSkybox)
//   {
//
//      error("bad argument");
//
//      throw ::exception(error_failed);
//   }
//
//   irradianceCube->m_vksampler3 = _001VkSampler();
//
//   // m_vkqueueTransfer3 = m_vkqueueGraphics;
//
//   auto tStart = std::chrono::high_resolution_clock::now();
//
//   ::cast<device> pdevice = m_pgpudevice;
//
//   auto pphysicaldevice = pdevice->m_pphysicaldevice;
//
//   const VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;
//   const int32_t dim = 64;
//   const uint32_t numMips = static_cast<uint32_t>(floor(log2(dim))) + 1;
//
//   // create irradiance cubemap (same as before)
//   VkImageCreateInfo imageCI = vkinit::imageCreateInfo();
//   imageCI.imageType = VK_IMAGE_TYPE_2D;
//   imageCI.format = format;
//   imageCI.extent.width = dim;
//   imageCI.extent.height = dim;
//   imageCI.extent.depth = 1;
//   imageCI.mipLevels = numMips;
//   imageCI.arrayLayers = 6;
//   imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
//   imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
//   imageCI.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
//   imageCI.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
//   VK_CHECK_RESULT(vkCreateImage(this->logicalDevice(), &imageCI, nullptr, &irradianceCube->m_vkimage));
//
//   VkMemoryRequirements memReqs;
//   vkGetImageMemoryRequirements(this->logicalDevice(), irradianceCube->m_vkimage, &memReqs);
//   VkMemoryAllocateInfo memAlloc = vkinit::memoryAllocateInfo();
//   memAlloc.allocationSize = memReqs.size;
//   memAlloc.memoryTypeIndex =
//      pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//   VK_CHECK_RESULT(vkAllocateMemory(this->logicalDevice(), &memAlloc, nullptr, &irradianceCube->m_vkdevicememory));
//   VK_CHECK_RESULT(
//      vkBindImageMemory(this->logicalDevice(), irradianceCube->m_vkimage, irradianceCube->m_vkdevicememory, 0));
//
//   // view & sampler
//   VkImageViewCreateInfo viewCI = vkinit::imageViewCreateInfo();
//   viewCI.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
//   viewCI.format = format;
//   viewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//   viewCI.subresourceRange.baseMipLevel = 0;
//   viewCI.subresourceRange.levelCount = numMips;
//   viewCI.subresourceRange.baseArrayLayer = 0;
//   viewCI.subresourceRange.layerCount = 6;
//   viewCI.image = irradianceCube->m_vkimage;
//   VK_CHECK_RESULT(vkCreateImageView(this->logicalDevice(), &viewCI, nullptr, &irradianceCube->m_vkimageview));
//
//   VkSamplerCreateInfo samplerCI = vkinit::samplerCreateInfo();
//   samplerCI.magFilter = VK_FILTER_LINEAR;
//   samplerCI.minFilter = VK_FILTER_LINEAR;
//   samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
//   samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//   samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//   samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//   samplerCI.minLod = 0.0f;
//   samplerCI.maxLod = static_cast<float>(numMips);
//   samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
//   VK_CHECK_RESULT(vkCreateSampler(this->logicalDevice(), &samplerCI, nullptr, &irradianceCube->m_vksamplerDedicated));
//
//   irradianceCube->m_descriptor3.imageView = irradianceCube->m_vkimageview;
//   irradianceCube->m_descriptor3.sampler = irradianceCube->m_vksamplerDedicated;
//   irradianceCube->m_descriptor3.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//   // irradianceCube->m_pDevice = m_pgpudevice;
//
//   // --- create offscreen renderpass/framebuffer (unchanged) ---
//   VkAttachmentDescription attDesc = {};
//   attDesc.format = format;
//   attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
//   attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//   attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//   attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//   attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//   attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//   attDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//   VkAttachmentReference colorReference = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
//
//   VkSubpassDescription subpassDescription = {};
//   subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//   subpassDescription.colorAttachmentCount = 1;
//   subpassDescription.pColorAttachments = &colorReference;
//
//   ::block_array<VkSubpassDependency, 2> dependencies;
//   dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
//   dependencies[0].dstSubpass = 0;
//   dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//   dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//   dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//   dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//   dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//   dependencies[1].srcSubpass = 0;
//   dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
//   dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//   dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//   dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//   dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//   dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//   VkRenderPassCreateInfo renderPassCI = vkinit::renderPassCreateInfo();
//   renderPassCI.attachmentCount = 1;
//   renderPassCI.pAttachments = &attDesc;
//   renderPassCI.subpassCount = 1;
//   renderPassCI.pSubpasses = &subpassDescription;
//   renderPassCI.dependencyCount = static_cast<uint32_t>(dependencies.size());
//   renderPassCI.pDependencies = dependencies.data();
//   VkRenderPass renderpass;
//   VK_CHECK_RESULT(vkCreateRenderPass(this->logicalDevice(), &renderPassCI, nullptr, &renderpass));
//
//   // offscreen color image (1 mip, reused for all mips/faces)
//   struct
//   {
//      VkImage image;
//      VkImageView view;
//      VkDeviceMemory memory;
//      VkFramebuffer framebuffer;
//   } offscreen;
//
//   {
//      VkImageCreateInfo imageCreateInfo = vkinit::imageCreateInfo();
//      imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
//      imageCreateInfo.format = format;
//      imageCreateInfo.extent.width = dim;
//      imageCreateInfo.extent.height = dim;
//      imageCreateInfo.extent.depth = 1;
//      imageCreateInfo.mipLevels = 1;
//      imageCreateInfo.arrayLayers = 1;
//      imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
//      imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
//      imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//      imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
//      imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//      VK_CHECK_RESULT(vkCreateImage(this->logicalDevice(), &imageCreateInfo, nullptr, &offscreen.image));
//
//      vkGetImageMemoryRequirements(this->logicalDevice(), offscreen.image, &memReqs);
//      memAlloc.allocationSize = memReqs.size;
//      memAlloc.memoryTypeIndex =
//         pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//      VK_CHECK_RESULT(vkAllocateMemory(this->logicalDevice(), &memAlloc, nullptr, &offscreen.memory));
//      VK_CHECK_RESULT(vkBindImageMemory(this->logicalDevice(), offscreen.image, offscreen.memory, 0));
//
//      VkImageViewCreateInfo colorImageView = vkinit::imageViewCreateInfo();
//      colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
//      colorImageView.format = format;
//      colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//      colorImageView.subresourceRange.baseMipLevel = 0;
//      colorImageView.subresourceRange.levelCount = 1;
//      colorImageView.subresourceRange.baseArrayLayer = 0;
//      colorImageView.subresourceRange.layerCount = 1;
//      colorImageView.image = offscreen.image;
//      VK_CHECK_RESULT(vkCreateImageView(this->logicalDevice(), &colorImageView, nullptr, &offscreen.view));
//
//      VkFramebufferCreateInfo fbufCreateInfo = vkinit::framebufferCreateInfo();
//      fbufCreateInfo.renderPass = renderpass;
//      fbufCreateInfo.attachmentCount = 1;
//      fbufCreateInfo.pAttachments = &offscreen.view;
//      fbufCreateInfo.width = dim;
//      fbufCreateInfo.height = dim;
//      fbufCreateInfo.layers = 1;
//      VK_CHECK_RESULT(vkCreateFramebuffer(this->logicalDevice(), &fbufCreateInfo, nullptr, &offscreen.framebuffer));
//
//      // VkCommandBuffer layoutCmd = this->beginSingleTimeCommands((VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
//      ::pointer<::gpu_vulkan::command_buffer> pcommandbuffer = this->beginSingleTimeCommands(transfer_queue());
//      vulkan::setImageLayout(pcommandbuffer->m_vkcommandbuffer, offscreen.image, VK_IMAGE_ASPECT_COLOR_BIT,
//                             VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//      // this->flushCommandBuffer(layoutCmd, m_vkqueueTransfer3, true);
//      this->endSingleTimeCommands(pcommandbuffer);
//   }
//
//   // Descriptor layout/pool/set (same as before)
//   VkDescriptorSetLayout descriptorsetlayout;
//   ::array_base<VkDescriptorSetLayoutBinding> setLayoutBindings = {
//      vkinit::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0),
//   };
//   VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI = vkinit::descriptorSetLayoutCreateInfo(setLayoutBindings);
//   VK_CHECK_RESULT(
//      vkCreateDescriptorSetLayout(this->logicalDevice(), &descriptorsetlayoutCI, nullptr, &descriptorsetlayout));
//
//   ::array_base<VkDescriptorPoolSize> poolSizes = {
//      vkinit::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)};
//   VkDescriptorPoolCreateInfo descriptorPoolCI = vkinit::descriptorPoolCreateInfo(poolSizes, 2);
//   VkDescriptorPool descriptorpool;
//   VK_CHECK_RESULT(vkCreateDescriptorPool(this->logicalDevice(), &descriptorPoolCI, nullptr, &descriptorpool));
//
//   VkDescriptorSet descriptorset;
//   VkDescriptorSetAllocateInfo allocInfo = vkinit::descriptorSetAllocateInfo(descriptorpool, &descriptorsetlayout, 1);
//   VK_CHECK_RESULT(vkAllocateDescriptorSets(this->logicalDevice(), &allocInfo, &descriptorset));
//   VkWriteDescriptorSet writeDescriptorSet = vkinit::writeDescriptorSet(
//      descriptorset, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &environmentCube->m_descriptor3);
//   vkUpdateDescriptorSets(this->logicalDevice(), 1, &writeDescriptorSet, 0, nullptr);
//
//   // Push block
//   struct PushBlock
//   {
//      glm::mat4 mvp;
//      float deltaPhi;
//      float deltaTheta;
//   } pushBlock;
//
//
//   pushBlock.deltaPhi = (2.0f * float(this->mathematics()->π())) / 180.0f;
//   pushBlock.deltaTheta = (0.5f * float(this->mathematics()->π())) / 64.0f;
//
//
//   // Pipeline config — IMPORTANT: provide vertex input descriptions to match shader (location 0)
//   pipeline_configuration pipelineconfiguration{};
//   ::vulkan::defaultPipelineConfigInfo2(pipelineconfiguration);
//
//   // Vertex input: location 0 is a vec3 position (adjust if your skybox vertex layout differs)
//   VkVertexInputBindingDescription bindingDesc{};
//   bindingDesc.binding = 0;
//   bindingDesc.stride = sizeof(gltf::Vertex);
//   bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
//   VkVertexInputAttributeDescription attrDesc{};
//   attrDesc.binding = 0;
//   attrDesc.location = 0;
//   attrDesc.format = VK_FORMAT_R32G32B32_SFLOAT; // vec3
//   attrDesc.offset = 0;
//
//   pipelineconfiguration.bindingDescriptions = {bindingDesc};
//   pipelineconfiguration.attributeDescriptions = {attrDesc};
//
//   pipelineconfiguration.renderPass = renderpass;
//   pipelineconfiguration.pipelineLayout = VK_NULL_HANDLE;
//   pipelineconfiguration.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
//   pipelineconfiguration.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//   pipelineconfiguration.dynamicStateInfo.pDynamicStates = pipelineconfiguration.dynamicStateEnables.data();
//   pipelineconfiguration.dynamicStateInfo.dynamicStateCount =
//      uint32_t(pipelineconfiguration.dynamicStateEnables.size());
//   pipelineconfiguration.descriptorSetLayouts = {descriptorsetlayout};
//
//   VkPushConstantRange pushRange{};
//   pushRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
//   pushRange.offset = 0;
//   pushRange.size = sizeof(PushBlock);
//   pipelineconfiguration.pushConstantRanges = {pushRange};
//
//   pipelineconfiguration.pipelineLayout = VK_NULL_HANDLE;
//
//   // std::string vert = std::string(PROJECT_ROOT_DIR) + "/res/shaders/spirV/filtered_cube.vert.spv";
//   // std::string frag = std::string(PROJECT_ROOT_DIR) + "/res/shaders/spirV/irradiance_cube.frag.spv";
//   auto ppipelineIrradiance = øcreate<pipeline>();
//
//   ::memory vert;
//   ::memory frag;
//   pgpudevice->defer_shader_memory(vert, "matter://shaders/filtered_cube.vert");
//   pgpudevice->defer_shader_memory(frag, "matter://shaders/irradiance_cube.frag");
//
//
//   ppipelineIrradiance->initialize_graphics_pipeline(m_pgpurenderer, vert, frag, pipelineconfiguration);
//
//   // COMMAND RECORDING
//   // VkCommandBuffer vkcommandbuffer = this->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
//   ::pointer<::gpu_vulkan::command_buffer> pcommandbuffer = this->beginSingleTimeCommands(transfer_queue());
//
//   // Transition irradiance cubemap to TRANSFER_DST (outside any renderpass)
//   VkImageSubresourceRange cubemapRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, numMips, 0, 6};
//   ::vulkan::setImageLayout(pcommandbuffer->m_vkcommandbuffer, irradianceCube->m_vkimage, VK_IMAGE_LAYOUT_UNDEFINED,
//                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, cubemapRange);
//
//   // Setup matrices (same as Sascha)
//   ::array_base<glm::mat4> matrices = {
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // +X
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // -X
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)), // +Y
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // -Y
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // +Z
//      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // -Z
//   };
//
//   // Main loop: mips and faces (matches Sascha's approach)
//   for (uint32_t m = 0; m < numMips; ++m)
//   {
//      uint32_t mipDim = static_cast<uint32_t>(dim * std::pow(0.5f, (float)m));
//      VkViewport vp = vkinit::viewport((float)mipDim, (float)mipDim, 0.0f, 1.0f);
//      VkRect2D sc = vkinit::rect2D(mipDim, mipDim, 0, 0);
//
//      for (uint32_t face = 0; face < 6; ++face)
//      {
//         // Begin render pass into offscreen framebuffer
//         VkClearValue clear{{{0.0f, 0.0f, 0.0f, 0.0f}}};
//         VkRenderPassBeginInfo rpBI = vkinit::renderPassBeginInfo();
//         rpBI.renderPass = renderpass;
//         rpBI.framebuffer = offscreen.framebuffer;
//         rpBI.renderArea.extent = {mipDim, mipDim};
//         rpBI.clearValueCount = 1;
//         rpBI.pClearValues = &clear;
//         vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &rpBI, VK_SUBPASS_CONTENTS_INLINE);
//
//         vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &vp);
//         vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &sc);
//
//         // push constants
//         pushBlock.mvp = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 512.0f) * matrices[face];
//         vkCmdPushConstants(pcommandbuffer->m_vkcommandbuffer, ppipelineIrradiance->_pipeline_layout(),
//                            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushBlock),
//                            &pushBlock);
//
//         // bind pipeline and descriptor set (USE the allocated VkDescriptorSet)
//         ppipelineIrradiance->bind(pcommandbuffer);
//         vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
//                                 ppipelineIrradiance->_pipeline_layout(), 0, 1, &descriptorset, 0, nullptr);
//
//         // draw skybox — ensure your skybox.draw binds the vertex buffer that matches location 0 vec3 pos
//         // if (!m_skyboxModel) {
//         //     spdlog::error("[AssetManager] No skybox model loaded - skipping draw in generateIrradianceMap()");
//         // }
//         // else {
//
//
//         prenderableSkybox->bind(pcommandbuffer);
//         prenderableSkybox->draw(pcommandbuffer);
//         // pmodelbufferSkybox->gltfDraw(pcommandbuffer->m_vkcommandbuffer);
//         // }
//
//
//         // END render pass BEFORE any barriers/copies
//         vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);
//
//         // Transition offscreen image -> TRANSFER_SRC and copy to target cubemap mip/face
//         ::vulkan::setImageLayout(pcommandbuffer->m_vkcommandbuffer, offscreen.image, VK_IMAGE_ASPECT_COLOR_BIT,
//                                  VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
//
//         VkImageCopy copyRegion{};
//         copyRegion.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
//         copyRegion.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, m, face, 1};
//         copyRegion.extent = {mipDim, mipDim, 1};
//
//         vkCmdCopyImage(pcommandbuffer->m_vkcommandbuffer, offscreen.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
//                        irradianceCube->m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
//
//         // restore offscreen layout for next render
//         ::vulkan::setImageLayout(pcommandbuffer->m_vkcommandbuffer, offscreen.image, VK_IMAGE_ASPECT_COLOR_BIT,
//                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//      }
//   }
//
//   // final transition for cubemap to shader read layout
//   ::vulkan::setImageLayout(pcommandbuffer->m_vkcommandbuffer, irradianceCube->m_vkimage,
//                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
//                            cubemapRange);
//
//   // this->flushCommandBuffer(vkcommandbuffer, m_vkqueueTransfer3);
//   this->endSingleTimeCommands(pcommandbuffer);
//   ::cast<::gpu_vulkan::queue> pqueue = pcommandbuffer->m_pgpuqueue;
//   vkQueueWaitIdle(pqueue->m_vkqueue);
//
//   // cleanup (destroy created renderpass/framebuffer)
//   vkDestroyFramebuffer(this->logicalDevice(), offscreen.framebuffer, nullptr);
//   vkDestroyRenderPass(this->logicalDevice(), renderpass, nullptr);
//
//   auto tEnd = std::chrono::high_resolution_clock::now();
//   auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
//   information("Generating irradiance cube took {} ms", tDiff);
//
//   return irradianceCubeNew;
//}



void context::_001BeginRenderPass(::gpu::command_buffer * pgpucommandbuffer, ::gpu::texture * pgputexture)
{

   
      {

      //////////////////////////////////////////


      ::cast<context> pgpucontext = this;

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast<renderer> prenderer = m_pgpurenderer;
      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      //::cast<command_buffer> pcommandbuffer = ::gpu::current_frame()->m_pgpucommandbuffer;

      VkRenderPassBeginInfo renderPassBeginInfo{};

      renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

      // if (has_sampler())
      //{

      //   throw ::exception(error_wrong_state, "use bind(txtDst, txtDsr)");

      //}

      ::cast<render_target> prendertarget = prenderer->m_pgpurendertarget;
      //::cast<renderer> prenderer = m_pgpurenderer;

      ::cast<render_pass> prenderpass = prenderer->render_pass2();


      renderPassBeginInfo.renderPass = prenderpass->getRenderPass();
      // if (prenderer->m_pgpulayer)
      {
         ::cast<::gpu_vulkan::texture> ptexture;
         
         if (pgputexture) 
         {
            
            ptexture = pgputexture; 
         
         }
         else
         {
            
            ptexture = prendertarget->current_texture(::gpu::current_frame());

         }

         if (ptexture->m_state.m_vkimagelayout == VK_IMAGE_LAYOUT_UNDEFINED)
         {

            warning() << "what?";
         }

         renderPassBeginInfo.framebuffer = ptexture->framebuffer(prenderpass);
      }
      // else
      //{

      //   renderPassBeginInfo.framebuffer =
      //   prenderpass->getFrameBuffer(prenderer->m_pgpurendertarget->get_frame_index());

      //}

      VkClearValue clearValues[2]{};
      // clearValues[0].color = { 0.5f* 0.5f, 0.75f*0.5f, 0.95f* 0.5f, 0.5f };
      bool bClearColor = true;
      // if (m_bClearColor)
      if (bClearColor)
      {

         ::color::color colorClear(color::transparent);

         auto fR = colorClear.f32_red();
         auto fG = colorClear.f32_green();
         auto fB = colorClear.f32_blue();
         auto fA = colorClear.f32_opacity();

         clearValues[0].color = {fR * fA, fG * fA, fB * fA, fA};
         clearValues[1].depthStencil = {1.0f, 0};
         renderPassBeginInfo.clearValueCount = 2;
         renderPassBeginInfo.pClearValues = clearValues;
      }
      else
      {

         renderPassBeginInfo.clearValueCount = 0;
         renderPassBeginInfo.pClearValues = nullptr;
      }


      renderPassBeginInfo.renderArea.offset = {0, 0};
      renderPassBeginInfo.renderArea.extent = {(uint32_t)pgpucontext->m_rectangle.width(),
                                               (uint32_t)pgpucontext->m_rectangle.height()};


      vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);


      //////////////////////////////////////////
   }

}


void context::_001BeginRenderPass(::gpu::command_buffer *pgpucommandbuffer,
                                  ::gpu::ibl::cubemap_framebuffer *pgpucubemapframebuffer, int iFace,
                                  ::gpu::enum_scene escene)
{


   {

      //////////////////////////////////////////


      ::cast<context> pgpucontext = this;

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast<renderer> prenderer = m_pgpurenderer;
      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      //::cast<command_buffer> pcommandbuffer = ::gpu::current_frame()->m_pgpucommandbuffer;

      VkRenderPassBeginInfo renderPassBeginInfo{};

      renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

      // if (has_sampler())
      //{

      //   throw ::exception(error_wrong_state, "use bind(txtDst, txtDsr)");

      //}

      ::cast<render_target> prendertarget = prenderer->m_pgpurendertarget;
      //::cast<renderer> prenderer = m_pgpurenderer;

      ::cast<render_pass> prenderpass = prenderer->render_pass2(escene);


      renderPassBeginInfo.renderPass = prenderpass->getRenderPass();
      // if (prenderer->m_pgpulayer)
      {

         ::cast<::gpu_vulkan::ibl::cubemap_framebuffer> pcubemapframebuffer;

         pcubemapframebuffer = pgpucubemapframebuffer;

         //if (pgputexture)
         //{

         //   ptexture = pgputexture;
         //}
         //else
         //{

         //   ptexture = prendertarget->current_texture(::gpu::current_frame());
         //}

         //if (ptexture->m_state.m_vkimagelayout == VK_IMAGE_LAYOUT_UNDEFINED)
         //{

         //   warning() << "what?";
         //}

         renderPassBeginInfo.framebuffer = pcubemapframebuffer->framebuffer(prenderpass, iFace);

      }
      // else
      //{

      //   renderPassBeginInfo.framebuffer =
      //   prenderpass->getFrameBuffer(prenderer->m_pgpurendertarget->get_frame_index());

      //}

      VkClearValue clearValues[2]{};
      // clearValues[0].color = { 0.5f* 0.5f, 0.75f*0.5f, 0.95f* 0.5f, 0.5f };
      bool bClearColor = true;
      // if (m_bClearColor)
      if (bClearColor)
      {

         ::color::color colorClear(color::transparent);

         auto fR = colorClear.f32_red();
         auto fG = colorClear.f32_green();
         auto fB = colorClear.f32_blue();
         auto fA = colorClear.f32_opacity();



         clearValues[0].color = {fR * fA, fG * fA, fB * fA, fA};

         if (escene == ::gpu::e_scene_3d)
         {
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassBeginInfo.clearValueCount = 2;
         }
         else
         {
            renderPassBeginInfo.clearValueCount = 1;
         }
         renderPassBeginInfo.pClearValues = clearValues;
      }
      else
      {

         renderPassBeginInfo.clearValueCount = 0;
         renderPassBeginInfo.pClearValues = nullptr;
      }


      renderPassBeginInfo.renderArea.offset = {0, 0};
      renderPassBeginInfo.renderArea.extent = {(uint32_t)pgpucontext->m_rectangle.width(),
                                               (uint32_t)pgpucontext->m_rectangle.height()};


      vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);


      //////////////////////////////////////////
   }
}


void context::_001EndRenderPass(::gpu::command_buffer *pgpucommandbuffer)
{

    //::gpu::context::on_end_layer(player);
   {
      ::cast<context> pgpucontext = this;

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast<renderer> prenderer = m_pgpurenderer;

      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      //::cast<command_buffer> pcommandbuffer = ::gpu::current_frame()->m_pgpucommandbuffer;
      //
      //      {
      //
      //   {
      //
      //      if (pgpucontext->m_pgpudevice->m_iLayer == 0)
      //      {
      //
      //         VkClearAttachment clearAttachment = {
      //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //            .colorAttachment = 0,
      //            .clearValue={.color = {0.5f, 0.0f, 0.0f, 0.5f}} // Red
      //         };
      //
      //         VkClearRect clearRect = {
      //             .rect = {
      //                 .offset = {100, 100},
      //                 .extent = {100, 100}
      //             },
      //             .baseArrayLayer = 0,
      //             .layerCount = 1
      //         };
      //
      //         vkCmdClearAttachments(
      //            pcommandbuffer->m_vkcommandbuffer,
      //            1,
      //            &clearAttachment,
      //            1,
      //            &clearRect);
      //
      //      }
      //      else if (pgpucontext->m_pgpudevice->m_iLayer == 1)
      //      {
      //
      //         VkClearAttachment clearAttachment = {
      //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //            .colorAttachment = 0,
      //            .clearValue = {.color = {0.0f, 0.5f, 0.0f, 0.5f} } // Green
      //         };
      //
      //         VkClearRect clearRect = {
      //             .rect = {
      //                 .offset = {200, 100},
      //                 .extent = {100, 100}
      //             },
      //             .baseArrayLayer = 0,
      //             .layerCount = 1
      //         };
      //
      //         vkCmdClearAttachments(
      //            pcommandbuffer->m_vkcommandbuffer,
      //            1,
      //            &clearAttachment,
      //            1,
      //            &clearRect);
      //
      //
      //      }
      //      else if (pgpucontext->m_pgpudevice->m_iLayer == 2)
      //      {
      //
      //         VkClearAttachment clearAttachment = {
      //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //            .colorAttachment = 0,
      //            .clearValue = {.color = {0.0f, 0.0f, 0.5f, 0.5f} } // Blue
      //         };
      //
      //         VkClearRect clearRect = {
      //             .rect = {
      //                 .offset = {300, 100},
      //                 .extent = {100, 100}
      //             },
      //             .baseArrayLayer = 0,
      //             .layerCount = 1
      //         };
      //
      //         vkCmdClearAttachments(
      //            pcommandbuffer->m_vkcommandbuffer,
      //            1,
      //            &clearAttachment,
      //            1,
      //            &clearRect);
      //
      //      }
      //
      //   }
      //
      //}


      vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);
   }

}


//::pointer<::gpu::texture> context::generateBRDFlut()
//{
//
//   ::pointer<::gpu::texture> lutBrdfNew;
//
//   øconstruct(lutBrdfNew);
//
//   ::cast<::gpu_vulkan::texture> lutBrdf = lutBrdfNew;
//
//   ::cast<::gpu_vulkan::device> pgpudevice = m_pgpudevice;
//
//   auto pphysicaldevice = pgpudevice->m_pphysicaldevice;
//
//   auto tStart = std::chrono::high_resolution_clock::now();
//
//   const VkFormat format = VK_FORMAT_R16G16_SFLOAT; // R16G16 is supported pretty much everywhere
//   const int32_t dim = 512;
//
//   // Image
//   VkImageCreateInfo imageCI = vkinit::imageCreateInfo();
//   imageCI.imageType = VK_IMAGE_TYPE_2D;
//   imageCI.format = format;
//   imageCI.extent.width = dim;
//   imageCI.extent.height = dim;
//   imageCI.extent.depth = 1;
//   imageCI.mipLevels = 1;
//   imageCI.arrayLayers = 1;
//   imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
//   imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
//   imageCI.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
//   VK_CHECK_RESULT(vkCreateImage(this->logicalDevice(), &imageCI, nullptr, &lutBrdf->m_vkimage));
//   VkMemoryAllocateInfo memAlloc = vkinit::memoryAllocateInfo();
//   VkMemoryRequirements memReqs;
//   vkGetImageMemoryRequirements(this->logicalDevice(), lutBrdf->m_vkimage, &memReqs);
//   memAlloc.allocationSize = memReqs.size;
//   memAlloc.memoryTypeIndex =
//      pphysicaldevice->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//   VK_CHECK_RESULT(vkAllocateMemory(this->logicalDevice(), &memAlloc, nullptr, &lutBrdf->m_vkdevicememory));
//   VK_CHECK_RESULT(vkBindImageMemory(this->logicalDevice(), lutBrdf->m_vkimage, lutBrdf->m_vkdevicememory, 0));
//
//
//   // Image view
//   VkImageViewCreateInfo viewCI = vkinit::imageViewCreateInfo();
//   viewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
//   viewCI.format = format;
//   viewCI.subresourceRange = {};
//   viewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//   viewCI.subresourceRange.levelCount = 1;
//   viewCI.subresourceRange.layerCount = 1;
//   viewCI.image = lutBrdf->m_vkimage;
//   VK_CHECK_RESULT(vkCreateImageView(this->logicalDevice(), &viewCI, nullptr, &lutBrdf->m_vkimageview));
//
//   //lutBrdf->m_vksampler3 = _001VkSampler();
//
//   // Sampler
//   VkSamplerCreateInfo samplerCI = vkinit::samplerCreateInfo();
//   samplerCI.magFilter = VK_FILTER_LINEAR;
//   samplerCI.minFilter = VK_FILTER_LINEAR;
//   samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
//   samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//   samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//   samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//   samplerCI.minLod = 0.0f;
//   samplerCI.maxLod = 1.0f;
//   samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
//   VK_CHECK_RESULT(vkCreateSampler(this->logicalDevice(), &samplerCI, nullptr, &lutBrdf->m_vksamplerDedicated));
//
//   lutBrdf->m_descriptor3.imageView = lutBrdf->m_vkimageview;
//   lutBrdf->m_descriptor3.sampler = lutBrdf->m_vksamplerDedicated;
//   lutBrdf->m_descriptor3.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//   // lutBrdf->m_pDevice = m_pgpudevice;
//
//   // FB, Att, RP, Pipe, etc.
//   VkAttachmentDescription attDesc = {};
//   // Color attachment
//   attDesc.format = format;
//   attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
//   attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//   attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//   attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//   attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//   attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//   attDesc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//   VkAttachmentReference colorReference = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
//
//   VkSubpassDescription subpassDescription = {};
//   subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//   subpassDescription.colorAttachmentCount = 1;
//   subpassDescription.pColorAttachments = &colorReference;
//
//   // Use subpass dependencies for layout transitions
//   ::block_array<VkSubpassDependency, 2> dependencies;
//   dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
//   dependencies[0].dstSubpass = 0;
//   dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//   dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//   dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//   dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//   dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//   dependencies[1].srcSubpass = 0;
//   dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
//   dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//   dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//   dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//   dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//   dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//   // Create the actual renderpass
//   VkRenderPassCreateInfo renderPassCI{};
//   renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//   renderPassCI.attachmentCount = 1;
//   renderPassCI.pAttachments = &attDesc;
//   renderPassCI.subpassCount = 1;
//   renderPassCI.pSubpasses = &subpassDescription;
//   renderPassCI.dependencyCount = static_cast<uint32_t>(dependencies.size());
//   renderPassCI.pDependencies = dependencies.data();
//
//   VkRenderPass renderpass = VK_NULL_HANDLE;
//   VK_CHECK_RESULT(vkCreateRenderPass(this->logicalDevice(), &renderPassCI, nullptr, &renderpass));
//
//   VkFramebufferCreateInfo framebufferCI = vkinit::framebufferCreateInfo();
//   framebufferCI.renderPass = renderpass;
//   framebufferCI.attachmentCount = 1;
//   framebufferCI.pAttachments = &lutBrdf->m_vkimageview;
//   framebufferCI.width = dim;
//   framebufferCI.height = dim;
//   framebufferCI.layers = 1;
//
//   VkFramebuffer framebuffer;
//   VK_CHECK_RESULT(vkCreateFramebuffer(this->logicalDevice(), &framebufferCI, nullptr, &framebuffer));
//
//   // Descriptors
//   VkDescriptorSetLayout descriptorsetlayout;
//   ::array_base<VkDescriptorSetLayoutBinding> setLayoutBindings = {};
//   VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI = vkinit::descriptorSetLayoutCreateInfo(setLayoutBindings);
//   VK_CHECK_RESULT(
//      vkCreateDescriptorSetLayout(this->logicalDevice(), &descriptorsetlayoutCI, nullptr, &descriptorsetlayout));
//
//   // Descriptor Pool
//   ::array_base<VkDescriptorPoolSize> poolSizes = {
//      vkinit::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)};
//   VkDescriptorPoolCreateInfo descriptorPoolCI = vkinit::descriptorPoolCreateInfo(poolSizes, 2);
//   VkDescriptorPool descriptorpool;
//   VK_CHECK_RESULT(vkCreateDescriptorPool(this->logicalDevice(), &descriptorPoolCI, nullptr, &descriptorpool));
//
//   // Descriptor sets
//   VkDescriptorSet descriptorset;
//   VkDescriptorSetAllocateInfo allocInfo = vkinit::descriptorSetAllocateInfo(descriptorpool, &descriptorsetlayout, 1);
//   VK_CHECK_RESULT(vkAllocateDescriptorSets(this->logicalDevice(), &allocInfo, &descriptorset));
//
//   // Pipeline layout
//   VkPipelineLayout pipelinelayout;
//   VkPipelineLayoutCreateInfo pipelineLayoutCI = vkinit::pipelineLayoutCreateInfo(&descriptorsetlayout, 1);
//   VK_CHECK_RESULT(vkCreatePipelineLayout(this->logicalDevice(), &pipelineLayoutCI, nullptr, &pipelinelayout));
//
//   // Pipeline
//   VkPipelineInputAssemblyStateCreateInfo inputAssemblyState =
//      vkinit::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
//   VkPipelineRasterizationStateCreateInfo rasterizationState = vkinit::pipelineRasterizationStateCreateInfo(
//      VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
//   VkPipelineColorBlendAttachmentState blendAttachmentState = vkinit::pipelineColorBlendAttachmentState(0xf, VK_FALSE);
//   VkPipelineColorBlendStateCreateInfo colorBlendState =
//      vkinit::pipelineColorBlendStateCreateInfo(1, &blendAttachmentState);
//   VkPipelineDepthStencilStateCreateInfo depthStencilState =
//      vkinit::pipelineDepthStencilStateCreateInfo(VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS_OR_EQUAL);
//   VkPipelineViewportStateCreateInfo viewportState = vkinit::pipelineViewportStateCreateInfo(1, 1);
//   VkPipelineMultisampleStateCreateInfo multisampleState =
//      vkinit::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT);
//   ::array_base<VkDynamicState> dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
//   VkPipelineDynamicStateCreateInfo dynamicState = vkinit::pipelineDynamicStateCreateInfo(dynamicStateEnables);
//   VkPipelineVertexInputStateCreateInfo emptyInputState = vkinit::pipelineVertexInputStateCreateInfo();
//   ::block_array<VkPipelineShaderStageCreateInfo, 2> shaderStages;
//
//   VkGraphicsPipelineCreateInfo pipelineCI = vkinit::pipelineCreateInfo(pipelinelayout, renderpass);
//   pipelineCI.pInputAssemblyState = &inputAssemblyState;
//   pipelineCI.pRasterizationState = &rasterizationState;
//   pipelineCI.pColorBlendState = &colorBlendState;
//   pipelineCI.pMultisampleState = &multisampleState;
//   pipelineCI.pViewportState = &viewportState;
//   pipelineCI.pDepthStencilState = &depthStencilState;
//   pipelineCI.pDynamicState = &dynamicState;
//   pipelineCI.stageCount = 2;
//   pipelineCI.pStages = shaderStages.data();
//   pipelineCI.pVertexInputState = &emptyInputState;
//
//
//   // 4) Fill your pipeline_configuration_information
//   ::vulkan::pipeline_configuration pipelineconfiguration{};
//   ::vulkan::defaultPipelineConfigInfo2(pipelineconfiguration);
//
//   pipelineconfiguration.bindingDescriptions.clear();
//   pipelineconfiguration.attributeDescriptions.clear();
//   pipelineconfiguration.renderPass = renderpass;
//   pipelineconfiguration.pipelineLayout = pipelinelayout;
//   //  viewport & scissor will be dynamic
//   pipelineconfiguration.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
//   pipelineconfiguration.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//   pipelineconfiguration.dynamicStateInfo.pDynamicStates = pipelineconfiguration.dynamicStateEnables.data();
//   pipelineconfiguration.dynamicStateInfo.dynamicStateCount =
//      (uint32_t)pipelineconfiguration.dynamicStateEnables.size();
//
//   // Look-up-table (from BRDF) pipeline
//
//   auto ppipelineBrdf = øcreate_new<pipeline>();
//
//   ::memory vert;
//   ::memory frag;
//   pgpudevice->defer_shader_memory(vert, "matter://shaders/gen_brdflut.vert");
//   pgpudevice->defer_shader_memory(frag, "matter://shaders/gen_brdflut.frag");
//
//   ppipelineBrdf->initialize_graphics_pipeline(m_pgpurenderer, vert, frag, pipelineconfiguration);
//
//   // COMMAND RECORDING
//   // VkCommandBuffer vkcommandbuffer = this->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
//   ::pointer<::gpu_vulkan::command_buffer> pcommandbuffer = this->beginSingleTimeCommands(transfer_queue());
//
//   // Render
//   VkClearValue clearValues[1];
//   clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
//
//   VkRenderPassBeginInfo renderPassBeginInfo = vkinit::renderPassBeginInfo();
//   renderPassBeginInfo.renderPass = renderpass;
//   renderPassBeginInfo.renderArea.extent.width = dim;
//   renderPassBeginInfo.renderArea.extent.height = dim;
//   renderPassBeginInfo.clearValueCount = 1;
//   renderPassBeginInfo.pClearValues = clearValues;
//   renderPassBeginInfo.framebuffer = framebuffer;
//
//   // VkCommandBuffer vkcommandbuffer = m_pgpudevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
//   vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
//   VkViewport viewport = vkinit::viewport((float)dim, (float)dim, 0.0f, 1.0f);
//   VkRect2D scissor = vkinit::rect2D(dim, dim, 0, 0);
//   vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &viewport);
//   vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &scissor);
//
//   ppipelineBrdf->bind(pcommandbuffer);
//   vkCmdDraw(pcommandbuffer->m_vkcommandbuffer, 3, 1, 0, 0);
//   vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);
//   // m_pgpudevice->flushCommandBuffer(pcommandbuffer->m_vkcommandbuffer, m_vkqueueTransfer3);
//   this->endSingleTimeCommands(pcommandbuffer);
//
//   // vkQueueWaitIdle(m_vkqueueTransfer3);
//   ::cast<::gpu_vulkan::queue> pqueue = pcommandbuffer->m_pgpuqueue;
//   vkQueueWaitIdle(pqueue->m_vkqueue);
//
//   vkDestroyFramebuffer(this->logicalDevice(), framebuffer, nullptr);
//   vkDestroyRenderPass(this->logicalDevice(), renderpass, nullptr);
//
//   auto tEnd = std::chrono::high_resolution_clock::now();
//   auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
//   information() << "Generating BRDF LUT took " << tDiff << " ms";
//
//
//   return lutBrdfNew;
//}

//
// ::pointer<::graphics3d::renderable> context::loadObjModel(
//    const ::scoped_string &name,
//    const ::scoped_string &filepath,
//    bool isSkybox
// )
// {
//    // 1) cache check
//    if (auto it = m_mapObjectModel.find(name); it != m_mapObjectModel.end())
//       return it->element2();
//
//    // 2) load
//    auto model = sandbox_object_model::createModelFromFile(m_pgpudevice, filepath, isSkybox);
//
//    // 3) cache & return
//    m_mapObjectModel[name] = model;
//    return model;
// }


::pointer<::graphics3d::renderable> context::_load_gltf_model(const ::gpu::renderable_t &model)
{
   // if (auto it = m_mapgltfModel.find(name); it != m_mapgltfModel.end())
   //  return it->element2();

   auto pmodel = øcreate_new<::gpu_vulkan::gltf::Model>();

   *((::gpu::renderable_t *)pmodel) = model;

   ::cast<::gpu_vulkan::queue> pqueueGraphics = m_pgpudevice->graphics_queue();

   pmodel->loadFromFile(model.m_pathRenderable.c_str(), this, pqueueGraphics->m_vkqueue, model.m_iFlags, model.m_fScale);

   // m_mapgltfModel[name] = model;
   return pmodel;
}


::pointer<::gpu::texture> context::load_cube_map(const ::scoped_string &scopedstrName, const ::file::path &path,
                                                 bool b32)
{

   VkFormat vkformat;

   if (!b32)
   {

      vkformat = VK_FORMAT_R16G16B16A16_SFLOAT;

   }
   else
   {

      vkformat = VK_FORMAT_R32G32B32A32_SFLOAT;

   }

   ::cast<gpu_vulkan::context> pcontext = m_pgpurenderer->m_pgpucontext;

   ::cast<gpu_vulkan::queue> pqueueCopy = pcontext->m_pgpudevice->transfer_queue();

   auto vkqueueCopy = pqueueCopy->m_vkqueue;

   VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

   VkImageLayout initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

   auto ptexture = loadCubemap(scopedstrName, path, vkformat, vkqueueCopy, usageFlags, initialLayout);

   return ptexture;

}


::pointer<::gpu::texture> context::load_sandbox_texture(const ::scoped_string &scopedstrName, const ::file::path &path,
                                                        const ::scoped_string &scopedstrImageFormat)
{


   VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
   if (scopedstrImageFormat == "VK_FORMAT_R8_UNORM")
      format = VK_FORMAT_R8_UNORM;
   else if (scopedstrImageFormat == "VK_FORMAT_R16G16B16A16_SFLOAT")
      format = VK_FORMAT_R16G16B16A16_SFLOAT;
   else if (scopedstrImageFormat == "VK_FORMAT_R32G32B32A32_SFLOAT")
      format = VK_FORMAT_R32G32B32A32_SFLOAT;

   // candidate prefixes to try (order matters — most likely first)
   string_array_base candidates;

   candidates.add("matter://textures");
   candidates.add("matter://models");
   candidates.add("matter://");

   //{std::string(PROJECT_ROOT_DIR) + "/res/textures",
   // std::string(PROJECT_ROOT_DIR) + "/res/models",
   // std::string(PROJECT_ROOT_DIR) + "/res", std::string(PROJECT_ROOT_DIR)};

   ::file::path pathFound;

   for (auto &strFolder: candidates)
   {

      ::file::path pathFolder = strFolder;

      ::file::path pathCandidate = pathFolder / path;

      if (file()->exists(pathCandidate))
      {
         pathFound = pathCandidate;
         break;
      }
   }

   // auto resolved = find_existing_path(relPath, candidates);
   if (pathFound.is_empty())
   {
      error("[AssetManager] texture '{}' not found, tried candidates. JSON path='{}'", scopedstrName, path);

      for (const auto &c: candidates)
      {
         debug("[AssetManager] tried: {}", c / path);
      }
      return nullptr;
   }

   information("[AssetManager] Loading texture '{}' from resolved path '{}'", scopedstrName, pathFound);

   try
   {
      auto ptexture =
         _loadTexture(scopedstrName, pathFound, format, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
      if (!ptexture)
      {
         error("[AssetManager] loadTexture returned nullptr for '{}'", scopedstrName);
         return nullptr;
      }
      // registerTextureIfNeeded(name, tex, m_textures, m_textureIndexMap, m_textureList);
      information("[AssetManager] Loaded texture '{}' from '{}'", scopedstrName, path);
      return ptexture;
   }
   catch (const ::exception &e)
   {
      error("[AssetManager] Failed to load texture '{}': {}", scopedstrName, e.get_message());
   }
   return nullptr;
}


::pointer<::gpu::texture> context::_loadTexture(const ::scoped_string &name, const ::file::path &path, VkFormat format,
                                                VkImageUsageFlags usageFlags, VkImageLayout imageLayout)
{


   information("[AssetManager] Loading texture '{}' from '{}'", name, path);

   // Create the texture wrapper tied to this device
   auto pgputexture = øcreate<::gpu::texture>();
   ::cast<::gpu_vulkan::texture> ptexture = pgputexture;
   ptexture->m_state.m_vkimagelayout = imageLayout;
   ptexture->m_vkformat = format;
   ptexture->m_pgpurenderer = m_pgpurenderer;

   ::string strExtension = path.final_extension();

   strExtension.make_lower();

   try
   {
      bool ok = false;

      if (strExtension == "ktx" || strExtension == "ktx2")
      {
         ::cast<::gpu_vulkan::queue> pqueueTransfer = m_pgpudevice->transfer_queue();
         ok = ptexture->KTXLoadFromFile(path, format, pqueueTransfer->m_vkqueue, usageFlags, imageLayout,
                                        /*forceLinear=*/false);

         if (!ok)
         {
            error("[AssetManager] KTXLoadFromFile failed for '{}'", path);
            return nullptr;
         }
      }
      else
      {

         ok = ptexture->imaging_load_from_file(path);
         if (!ok)
         {
            error("[AssetManager] STBLoadFromFile failed for '{}'", path);
            return nullptr;
         }
      }


      VkSampler sampler = ptexture->m_vksamplerDedicated;
      VkImageView view = ptexture->m_vkimageview;

      if (sampler == VK_NULL_HANDLE || view == VK_NULL_HANDLE)
      {
         warning("[AssetManager] Texture '{}' loaded but sampler/view are null (sampler: {}, view: {})", name,
                 static_cast<uint64_t>(reinterpret_cast<uintptr_t>(sampler)),
                 static_cast<uint64_t>(reinterpret_cast<uintptr_t>(view)));
      }

      ptexture->m_descriptor3.sampler = sampler;
      ptexture->m_descriptor3.imageView = view;
      ptexture->m_descriptor3.imageLayout = imageLayout;

      information("[AssetManager] Texture '{}' loaded OK (view: {}, sampler: {})", name,
                  (view != VK_NULL_HANDLE ? "valid" : "null"), (sampler != VK_NULL_HANDLE ? "valid" : "null"));

      return ptexture.m_p;
   }
   catch (const ::exception &e)
   {
      error("[AssetManager] Exception while loading texture '{}': {}", path, e.get_message());
      return nullptr;
   }
   catch (...)
   {
      error("[AssetManager] Unknown error while loading texture '{}'", path);
      return nullptr;
   }
}
void context::load_generic_texture(::pointer<::gpu::texture> &ptexture, const ::file::path &path,
                                   int iAssimpTextureType)
{

   if (ødefer_construct(ptexture))
   {
   
       ptexture->initialize_image_texture(m_pgpurenderer, path, false);
   
    }

}




} // namespace gpu_vulkan
