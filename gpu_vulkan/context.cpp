#include "framework.h"
#include "approach.h"
#include "buffer.h"
#include "command_buffer.h"
#include "context.h"
#include "device.h"
#include "memory_buffer.h"
#include "physical_device.h"
#include "program.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "acme/platform/application.h"
#include "aura/graphics/image/image.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/types.h"
#include "app-graphics3d/gpu_vulkan/descriptors.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#define GLM_FORCE_LEFT_HANDED  // Optional — depends on your conventions
#include "glm/mat4x4.hpp"
#include "initializers.h"


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

      m_vkqueuePresent = nullptr;
      m_vkqueueGraphics = nullptr;



   }


   context::~context()
   {

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


   //void context::swap_buffers()
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


         if (vkCreateSampler(this->logicalDevice(), &samplerInfo, NULL, &m_vksampler001) != VK_SUCCESS) {
            // Handle error
         }

      }

      return m_vksampler001;

   }





   //void context::create_offscreen_buffer(const ::int_size& size)
   //{

   //   auto pgpu = application()->get_gpu();

   //   if (::is_null(pgpu))
   //   {

   //      return ::error_failed;

   //   }

   //   ::e_status estatus = __øconstruct(m_pbuffer);

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


   //void context::_create_offscreen_buffer(const ::int_size& size)
   //{

   //   return ::success_none;

   //}


   //void context::resize_offscreen_buffer(const ::int_size& size)
   //{

   //   return ::success_none;

   //}



   //void context::make_current()
   //{

   //   return ::success_none;

   //}


   //void context::destroy_offscreen_buffer()
   //{

   //   return ::success_none;

   //}


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


   //bool context::defer_construct_new(::pointer < ::gpu_vulkan::memory_buffer >& pmemorybuffer, memsize size)
   //{

   //   if (__defer_construct_new(pmemorybuffer))
   //   {

   //      pmemorybuffer->initialize_memory_buffer(this, size);

   //      return true;

   //   }

   //   return false;

   //}


   //bool context::defer_construct_new(::pointer < ::gpu_vulkan::memory_buffer >& pmemorybuffer, const ::block& block)
   //{

   //   if (defer_construct_new(pmemorybuffer, block.size()))
   //   {

   //      pmemorybuffer->assign(block.data(), block.size());

   //      return true;

   //   }

   //   return false;

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


   void context::_create_context_win32(::gpu::device* pgpudevice, const ::gpu::enum_output& eoutput, ::windowing::window* pwindow, const ::int_size& size)
   {

      //createInstance();
      //setupDebugMessenger();
      //createSurface();
      //pickPhysicalDevice();
      //createLogicalDevice();
      //createCommandPool();

      if (!pgpudevice)
      {

         throw ::exception(error_failed);

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
      //      VkResult result = createLogicalDevice(
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
      //         exitFatal("Could not create Vulkan device: \n" + errorString(result) + " VkResult=" + ::as_string(result), result);
      //
      //         throw ::exception(error_failed);
      //
      //      }


            //device = vulkanDevice->logicalDevice;

   }


   void context::on_create_context(::gpu::device* pgpudevice, const ::gpu::enum_output& eoutput, ::windowing::window* pwindow, const ::int_size& size)
   {

      //m_itaskGpu = ::current_itask();

      m_pgpudevice = pgpudevice;

      if (m_pgpudevice->m_queuefamilyindices.graphicsFamily >= 0)
      {

         vkGetDeviceQueue(this->logicalDevice(), m_pgpudevice->m_queuefamilyindices.graphicsFamily, 0, &m_vkqueueGraphics);

      }

      if (m_pgpudevice->m_queuefamilyindices.presentFamily >= 0)
      {

         vkGetDeviceQueue(this->logicalDevice(), m_pgpudevice->m_queuefamilyindices.presentFamily, 0, &m_vkqueuePresent);

      }

      _create_context_win32(pgpudevice, eoutput, pwindow, size);

   }



   void context::endSingleTimeCommands(command_buffer* pcommandbuffer)
   {

      VkCommandBuffer commandbuffers[] = { pcommandbuffer->m_vkcommandbuffer };

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = commandbuffers;

      endSingleTimeCommands(pcommandbuffer, 1, &submitInfo);

   }


   void context::endSingleTimeCommands(command_buffer* pcommandbuffer, int iSubmitCount, VkSubmitInfo* psubmitinfo)
   {

      vkEndCommandBuffer(pcommandbuffer->m_vkcommandbuffer);

      VkFence fence;

      VkFenceCreateInfo fenceInfo = {
          .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
          .pNext = NULL,
          .flags = 0  // 0 = fence starts in unsignaled state
      };

      VkResult result = vkCreateFence(this->logicalDevice(), &fenceInfo, NULL, &fence);
      if (result != VK_SUCCESS) {
         fprintf(stderr, "Failed to create fence\n");
         // handle error
      }

      vkQueueSubmit(m_vkqueueGraphics, 1, psubmitinfo, fence);

      vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

      vkQueueWaitIdle(m_vkqueueGraphics);


      vkDestroyFence(this->logicalDevice(), fence, NULL);
   }



   VkDevice context::logicalDevice()
   {

      ::cast < device > pgpudevice = m_pgpudevice;

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
   //      // Due to differing queue family configurations of Vulkan implementations this can be a bit tricky, especially if the application
   //      // requests different queue types
   //
   //      ::array<VkDeviceQueueCreateInfo> queueCreateInfos{};
   //
   //      // Get queue family indices for the requested queue family types
   //      // Note that the indices may overlap depending on the implementation
   //
   //      const float defaultQueuePriority(0.0f);
   //
   //      m_queuefamilyindices = pphysicaldevice->findQueueFamilies();
   //
   //      // Graphics queue
   //      if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT
   //         && m_queuefamilyindices.graphicsFamilyHasValue)
   //      {
   //         //m_queuefamilyindices.graphics = pphysicaldevice->getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
   //         VkDeviceQueueCreateInfo queueInfo{};
   //         queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
   //         queueInfo.queueFamilyIndex = m_queuefamilyindices.graphicsFamily;
   //         queueInfo.queueCount = 1;
   //         queueInfo.pQueuePriorities = &defaultQueuePriority;
   //         queueCreateInfos.add(queueInfo);
   //      }
   //      else
   //      {
   //         m_queuefamilyindices.graphicsFamily = 0;
   //      }
   //
   //      // Dedicated compute queue
   //      if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT
   //         && m_queuefamilyindices.computeFamilyHasValue)
   //      {
   //         //m_queuefamilyindices.compute = pphysicaldevice->getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
   //         if (m_queuefamilyindices.computeFamily != m_queuefamilyindices.graphicsFamily)
   //         {
   //            // If compute family index differs, we need an additional queue create info for the compute queue
   //            VkDeviceQueueCreateInfo queueInfo{};
   //            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
   //            queueInfo.queueFamilyIndex = m_queuefamilyindices.computeFamily;
   //            queueInfo.queueCount = 1;
   //            queueInfo.pQueuePriorities = &defaultQueuePriority;
   //            queueCreateInfos.add(queueInfo);
   //         }
   //      }
   //      else
   //      {
   //         // Else we use the same queue
   //         m_queuefamilyindices.computeFamily = m_queuefamilyindices.graphicsFamily;
   //      }
   //
   //      // Dedicated transfer queue
   //      if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT
   //         && m_queuefamilyindices.transferFamilyHasValue)
   //      {
   //         //m_queuefamilyindices.transfer = pphysicaldevice->getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
   //         if ((m_queuefamilyindices.transferFamily != m_queuefamilyindices.graphicsFamily)
   //            && (m_queuefamilyindices.transferFamily != m_queuefamilyindices.computeFamily))
   //         {
   //            // If transfer family index differs, we need an additional queue create info for the transfer queue
   //            VkDeviceQueueCreateInfo queueInfo{};
   //            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
   //            queueInfo.queueFamilyIndex = m_queuefamilyindices.transferFamily;
   //            queueInfo.queueCount = 1;
   //            queueInfo.pQueuePriorities = &defaultQueuePriority;
   //            queueCreateInfos.add(queueInfo);
   //         }
   //      }
   //      else
   //      {
   //         // Else we use the same queue
   //         m_queuefamilyindices.transferFamily = m_queuefamilyindices.graphicsFamily;
   //      }
   //
   //      // Create the logical device representation
   //      ::array<const char *> deviceExtensions(enabledExtensions);
   //      if (useSwapChain)
   //      {
   //
   //         // If the device will be used for presenting to a display via a swapchain we need to request the swapchain extension
   //         deviceExtensions.add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
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
   //#if (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT)) && defined(VK_KHR_portability_subset)
   //      // SRS - When running on iOS/macOS with MoltenVK and VK_KHR_portability_subset is defined and supported by the device, enable the extension
   //      if (extensionSupported(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
   //      {
   //         deviceExtensions.add(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
   //      }
   //#endif
   //
   //      if (deviceExtensions.size() > 0)
   //      {
   //         for (const char * enabledExtension : deviceExtensions)
   //         {
   //            if (!isExtensionSupported(enabledExtension)) {
   //               information() << "Enabled device extension \"" << enabledExtension << "\" is not present at device level\n";
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
   //      if (m_queuefamilyindices.graphicsFamily >= 0)
   //      {
   //         vkGetDeviceQueue(this->logicalDevice(), m_queuefamilyindices.graphicsFamily, 0, &m_vkqueueGraphics);
   //      }
   //      if (m_queuefamilyindices.presentFamily >= 0)
   //      {
   //         vkGetDeviceQueue(this->logicalDevice(), m_queuefamilyindices.presentFamily, 0, &m_vkqueuePresent);
   //      }
   //
   //
   //      // Create a default command pool for graphics command buffers
   //      m_vkcommandpool = createCommandPool(m_queuefamilyindices.graphicsFamily);
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
   //   * @param createFlags (Optional) Command pool creation flags (Defaults to VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
   //   *
   //   * @note Command buffers allocated from the created pool can only be submitted to a queue with the same family index
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


      //void context::_create_window_buffer()
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


   void context::_create_window_context(::windowing::window* pwindowParam)
   {

      //m_itaskGpu = ::current_itask();

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

      //::vulkan::context::defer_create_window_context(pwindow);

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


   //void context::make_current()
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

   //void context::render()
   //{

   //   ::gpu_vulkan::context::render();



   //}


   //void context::swap_buffers()
   //{

   //   //SwapBuffers(m_hdc);

   //}


   string context::get_shader_version_text()
   {

      return "#version 330 core";

   }


   void context::_translate_shader(string_array& stra)
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
   ////context::context(::graphics3d_vulkan::VulkanDevice* pgpudevice) : this->logicalDevice(){pgpudevice->logicalDevice} {
   //context::context()
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

   //context::~context()
   //{
   //   vkDestroyCommandPool(this->logicalDevice(), m_vkcommandpool, nullptr);
   //   vkDestroyDevice(this->logicalDevice(), nullptr);

   //   if (enableValidationLayers) {
   //      DestroyDebugUtilsMessengerEXT(m_vkinstance, debugMessenger, nullptr);
   //   }

   //   if (m_vksurfacekhr)
   //   {
   //      vkDestroySurfaceKHR(m_vkinstance, m_vksurfacekhr, nullptr);
   //   }
   //   vkDestroyInstance(m_vkinstance, nullptr);

   //}


   //void context::initialize_context(::user::graphics3d * pimpact)
   //{

   //   m_pimpact = pimpact;


   //   validationLayers.add("VK_LAYER_KHRONOS_validation");

   //   if (m_papplication->m_bUseSwapChainWindow)
   //   {

   //      deviceExtensions.add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

   //   }


   //}


   //void context::createInstance()
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


   //void context::pickPhysicalDevice()
   //{
   //   uint32_t deviceCount = 0;
   //   vkEnumeratePhysicalDevices(m_vkinstance, &deviceCount, nullptr);
   //   if (deviceCount == 0) {
   //      throw ::exception(error_failed,"failed to find GPUs with Vulkan support!");
   //   }
   //   information() << "Device count: " << deviceCount;
   //   ::array<VkPhysicalDevice> devices(deviceCount);
   //   vkEnumeratePhysicalDevices(m_vkinstance, &deviceCount, devices.data());

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


   //void context::createLogicalDevice()
   //{

   //   QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

   //   ::array<VkDeviceQueueCreateInfo> queueCreateInfos;
   //   std::set<uint32_t> uniqueQueueFamilies;
   //   if (indices.graphicsFamilyHasValue)
   //   {
   //      uniqueQueueFamilies.insert(indices.graphicsFamily);
   //   }
   //   if (indices.presentFamilyHasValue)
   //   {
   //      uniqueQueueFamilies.insert(indices.presentFamily);
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
   //   if (indices.graphicsFamilyHasValue)
   //   {
   //      vkGetDeviceQueue(this->logicalDevice(), indices.graphicsFamily, 0, &m_vkqueueGraphics);
   //   }
   //   if (indices.presentFamilyHasValue)
   //   {
   //      vkGetDeviceQueue(this->logicalDevice(), indices.presentFamily, 0, &m_vkqueuePresent);
   //   }
   //}


   //void context::createCommandPool()
   //{
   //   QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

   //   VkCommandPoolCreateInfo poolInfo = {};
   //   poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
   //   poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
   //   poolInfo.flags =
   //      VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

   //   if (vkCreateCommandPool(this->logicalDevice(), &poolInfo, nullptr, &m_vkcommandpool) != VK_SUCCESS) {
   //      throw ::exception(error_failed,"failed to create command pool!");
   //   }
   //}

   //void context::createSurface() { window.createWindowSurface(m_vkinstance, &m_vksurfacekhr); }

   //void context::_createSurface() {}


   //bool context::isDeviceSuitable(VkPhysicalDevice pvkcdevice)
   //{

   //   QueueFamilyIndices indices = findQueueFamilies(pvkcdevice);

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

   //   return (!m_vksurfacekhr || indices.isComplete()) && extensionsSupported && swapChainAdequate &&
   //      supportedFeatures.samplerAnisotropy;

   //}


   //void context::populateDebugMessengerCreateInfo(
   //   VkDebugUtilsMessengerCreateInfoEXT & createInfo)
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


   //void context::setupDebugMessenger()
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


   //bool context::checkValidationLayerSupport()
   //{
   //   uint32_t layerCount;
   //   vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

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


   //void context::hasGflwRequiredInstanceExtensions()
   //{
   //   uint32_t extensionCount = 0;
   //   vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
   //   ::array<VkExtensionProperties> extensions(extensionCount);
   //   vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

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


   //bool context::checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice)
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


   void context::createBuffer(
      VkDeviceSize size,
      VkBufferUsageFlags usage,
      VkMemoryPropertyFlags properties,
      VkBuffer& buffer,
      VkDeviceMemory& bufferMemory)
   {

      VkBufferCreateInfo bufferInfo{};
      bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufferInfo.size = size;
      bufferInfo.usage = usage;
      bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      if (vkCreateBuffer(this->logicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to create vertex buffer!");

      }

      VkMemoryRequirements memRequirements;
      vkGetBufferMemoryRequirements(this->logicalDevice(), buffer, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = m_pgpudevice->m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(this->logicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to allocate vertex buffer memory!");

      }

      vkBindBufferMemory(this->logicalDevice(), buffer, bufferMemory, 0);

   }


   //void context::do_on_context(const ::function< void(::gpu::context*) >& functionFrame)
   //{

   //   ::gpu::rear_guard rear_guard(this);

   //   auto function = functionFrame;

   //   m_pgpucontext->send([this, function]()
   //      {


   //      });


   //}


   ::pointer < command_buffer > context::beginSingleTimeCommands()
   {

      ::pointer < command_buffer > pcommandbuffer;

      __defer_construct_new(pcommandbuffer);

      pcommandbuffer->initialize_command_buffer(m_pgpurenderer->m_pgpurendertarget);

      pcommandbuffer->begin_command_buffer(true);

      return pcommandbuffer;

   }


   //void context::endSingleTimeCommands(auto pcommandbuffer)
   //{

   //   vkEndCommandBuffer(pcommandbuffer->m_vkcommandbuffer);

   //   VkSubmitInfo submitInfo{};
   //   submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
   //   submitInfo.commandBufferCount = 1;
   //   submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;

   //   vkQueueSubmit(m_vkqueueGraphics, 1, &submitInfo, VK_NULL_HANDLE);
   //   vkQueueWaitIdle(m_vkqueueGraphics);

   //   vkFreeCommandBuffers(this->logicalDevice(), m_pgpudevice->getCommandPool(), 1, &pcommandbuffer->m_vkcommandbuffer);

   //}


   void context::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
   {

      auto pcommandbuffer = beginSingleTimeCommands();

      VkBufferCopy copyRegion{};
      copyRegion.srcOffset = 0;  // Optional
      copyRegion.dstOffset = 0;  // Optional
      copyRegion.size = size;

      vkCmdCopyBuffer(pcommandbuffer->m_vkcommandbuffer, srcBuffer, dstBuffer, 1, &copyRegion);

      endSingleTimeCommands(pcommandbuffer);

   }


   void context::copyBufferToImage(
      VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount)
   {

      auto pcommandbuffer = beginSingleTimeCommands();

      VkBufferImageCopy region{};

      region.bufferOffset = 0;
      region.bufferRowLength = 0;
      region.bufferImageHeight = 0;

      region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      region.imageSubresource.mipLevel = 0;
      region.imageSubresource.baseArrayLayer = 0;
      region.imageSubresource.layerCount = layerCount;

      region.imageOffset = { 0, 0, 0 };
      region.imageExtent = { width, height, 1 };

      vkCmdCopyBufferToImage(
         pcommandbuffer->m_vkcommandbuffer,
         buffer,
         image,
         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         1,
         &region);

      endSingleTimeCommands(pcommandbuffer);

   }


   void context::createImageWithInfo(
      const VkImageCreateInfo& imageInfo,
      VkMemoryPropertyFlags properties,
      VkImage& image,
      VkDeviceMemory& imageMemory)
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
      allocInfo.memoryTypeIndex = m_pgpudevice->m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(this->logicalDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
         throw ::exception(error_failed, "failed to allocate image memory!");
      }

      if (vkBindImageMemory(this->logicalDevice(), image, imageMemory, 0) != VK_SUCCESS) {
         throw ::exception(error_failed, "failed to bind image memory!");
      }
   }


   void context::submitWork(command_buffer* pcommandbuffer, VkQueue queue)
   {

      VkSubmitInfo submitInfo = initializers::submitInfo();
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
      //m_submitInfo.commandBufferCount = 1;
      //m_submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
      VkFenceCreateInfo fenceInfo = initializers::fenceCreateInfo();
      VkFence fence;
      VK_CHECK_RESULT(vkCreateFence(this->logicalDevice(), &fenceInfo, nullptr, &fence));
      VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
      VK_CHECK_RESULT(vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX));
      vkDestroyFence(this->logicalDevice(), fence, nullptr);
   }


   //void context::submitSamplingWork(VkCommandBuffer pcommandbuffer->m_vkcommandbuffer, VkQueue queue)
   //{
   //   VkSubmitInfo submitInfo = initializers::submit_info();
   //   submitInfo.commandBufferCount = 1;
   //   submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
   //   //m_submitInfo.commandBufferCount = 1;
   //   //m_submitInfo.pCommandBuffers = &pcommandbuffer->m_vkcommandbuffer;
   //   VkFenceCreateInfo fenceInfo = initializers::fence_create_info();
   //   VkFence fence;
   //   VK_CHECK_RESULT(vkCreateFence(this->logicalDevice(), &fenceInfo, nullptr, &fence));
   //   VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
   //   VK_CHECK_RESULT(vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX));
   //   vkDestroyFence(this->logicalDevice(), fence, nullptr);
   //}

   ::gpu_vulkan::descriptor_pool* context::get_global_pool(int iFrameCount)
   {

      return m_pdescriptorpoolGlobal;

   }


   void context::create_global_ubo(int iGlobalUboSize, int iFrameCount)
   {

      m_uboBuffers.set_size(iFrameCount);

      for (int i = 0; i < m_uboBuffers.size(); i++)
      {

         m_uboBuffers[i] = __allocate buffer();

         m_uboBuffers[i]->initialize_buffer(
            this,
            iGlobalUboSize,
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

         m_uboBuffers[i]->map();

         auto bufferInfo = m_uboBuffers[i]->descriptorInfo();

         descriptor_writer(*m_psetdescriptorlayoutGlobal, *m_pdescriptorpoolGlobal)
            .writeBuffer(0, &bufferInfo)
            .build(m_descriptorsetsGlobal[i]);

      }

      //auto globalSetLayout = m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();

   }


   void context::update_global_ubo(const ::block& block)
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

      m_uboBuffers[iFrameIndex]->writeToBuffer(block.data());

      m_uboBuffers[iFrameIndex]->flush();

   }


   void context::engine_on_frame_context_initialization()
   {

      // Global UBO descriptors
      if (!m_psetdescriptorlayoutGlobal)
      {

         auto pgpurenderer = get_gpu_renderer();

         m_psetdescriptorlayoutGlobal = set_descriptor_layout::Builder(this)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

         auto iFrameCount = pgpurenderer->m_pgpurendertarget->get_frame_count();

         m_descriptorsetsGlobal.resize(iFrameCount);

         auto pdescriptorpoolbuilder = __allocate::gpu_vulkan::descriptor_pool::Builder();

         pdescriptorpoolbuilder->initialize_builder(this);
         pdescriptorpoolbuilder->setMaxSets(iFrameCount);
         pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, iFrameCount);

         m_pdescriptorpoolGlobal = pdescriptorpoolbuilder->build();

      }



   }


   VkDescriptorSet context::getGlobalDescriptorSet(::gpu_vulkan::renderer* prenderer)
   {

      //if (m_globalDescriptorSets.is_empty())
      //{


      //}

      return m_descriptorsetsGlobal[prenderer->m_pgpurendertarget->get_frame_index()];

   }


   //void context::copy(::gpu::texture* ptexture)
   //{

   //   ::cast < renderer > prenderer = get_gpu_renderer();

   //   prenderer->_copy_image(ptexture, ptexture->m_rectangleTarget, true);

   //}


   void context::merge_layers(::gpu::texture* ptextureTarget, ::pointer_array < ::gpu::layer >* playera)
   {

      if (!m_pshaderBlend3)
      {

         unsigned int full_screen_triangle_vertex_shader[] = {
#include "shader/blend2.vert.spv.inl"
         };

         unsigned int full_screen_triangle_fragment_shader[] = {
#include "shader/blend2.frag.spv.inl"
         };

         __defer_construct_new(m_pshaderBlend3);

         m_pshaderBlend3->m_bEnableBlend = true;
         m_pshaderBlend3->m_bTextureAndSampler = true;
         m_pshaderBlend3->m_bDisableDepthTest = true;
         //m_pshaderBlend3->m_bT
         //m_pshaderBlend3->m_pgpurenderer = this;
         m_pshaderBlend3->m_iSamplerSlot = 0;
         // Image Blend descriptors
//if (!m_psetdescriptorlayoutImageBlend)

         //m_pshaderBlend3->m_bClearColor = true;
         //m_pshaderBlend3->m_colorClear = ::color::transparent;

         m_pshaderBlend3->m_bClearColor = false;
         //m_pshaderBlend3->m_colorClear = ::color::transparent;

         m_pshaderBlend3->initialize_shader_with_block(
            m_pgpurenderer,
            ::as_memory_block(full_screen_triangle_vertex_shader),
            ::as_memory_block(full_screen_triangle_fragment_shader),
            {},
            {},
            {},
            {},
            // this means the vertex input layout will be null/empty
            // the full screen shader is embed in the shader code
            ::gpu::shader::e_flag_clear_default_bindings_and_attributes_descriptions

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

      auto vkcommandbuffer = pcommandbuffer->m_vkcommandbuffer;

      ::cast <texture > ptextureDst = ptextureTarget;


      ptextureDst->_new_state(
         pcommandbuffer,
         VK_ACCESS_TRANSFER_WRITE_BIT,
         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         VK_PIPELINE_STAGE_TRANSFER_BIT
      );



      pcommandbuffer->set_viewport(m_rectangle.size());

      pcommandbuffer->set_scissor(m_rectangle.size());


      // 2. Clear
      VkClearColorValue clearColor = { .float32 = { 0.0f, 0.0f, 0.0f, 0.0f } };
      VkImageSubresourceRange range = {
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .baseMipLevel = 0,
          .levelCount = 1,
          .baseArrayLayer = 0,
          .layerCount = 1,
      };

      vkCmdClearColorImage(
         vkcommandbuffer,
         ptextureDst->m_vkimage,
         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         &clearColor,
         1,
         &range);


      ////float clearColor[4] = { 0.95f * 0.5f, 0.95f * 0.5f, 0.25f * 0.5f, 0.5f }; // Clear to transparent
      ////m_pcontext->ClearRenderTargetView(ptextureDst->m_prendertargetview, clearColor);
      //float clearColor[4] = { 0.f, 0.f, 0.f, 0.f }; // Clear to transparent
      //pcommandlist->ClearRenderTargetView(ptextureDst->m_handleRenderTargetView, clearColor, 0, nullptr);
      ptextureDst->_new_state(pcommandbuffer,
         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
      );

      //if (!ptextureDst->m_handleRenderTargetView.ptr)
      //{

        // ptextureDst->create_render_target();

      //}


      //m_pshaderBlend3->bind(ptextureTarget);


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

               ptextureSrc->_new_state(
                  pcommandbuffer,
                  0,
                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
               );

               m_pshaderBlend3->bind(ptextureDst, ptextureSrc);

               pcommandbuffer->set_viewport(ptextureSrc->m_rectangleTarget);

               pcommandbuffer->set_scissor(ptextureSrc->m_rectangleTarget);

               //ID3D11SamplerState* samplerstatea[] =
               //{ ptexture->m_psamplerstate };
               //ID3D11ShaderResourceView* sharedresourceviewa[] =
               //{ ptexture->m_pshaderresourceview };

                              // 1. Define viewport and scissor rectangle
               //D3D12_VIEWPORT viewport = {};
               //viewport.TopLeftX = ptextureSrc->m_rectangleTarget.left();
               //viewport.TopLeftY = ptextureSrc->m_rectangleTarget.top();
               //viewport.Width = static_cast<float>(ptextureSrc->m_rectangleTarget.width());
               //viewport.Height = static_cast<float>(ptextureSrc->m_rectangleTarget.height());
               //viewport.MinDepth = 0.0f;
               //viewport.MaxDepth = 1.0f;

               //D3D12_RECT scissorRect = {};
               //scissorRect.left = ptextureSrc->m_rectangleTarget.left();
               //scissorRect.top = ptextureSrc->m_rectangleTarget.top();
               //scissorRect.right = ptextureSrc->m_rectangleTarget.right();
               //scissorRect.bottom = ptextureSrc->m_rectangleTarget.bottom();


               ////// 4. Set the viewport and scissor
               //pcommandlist->RSSetViewports(1, &viewport);
               //pcommandlist->RSSetScissorRects(1, &scissorRect);
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


               //pcommandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
               vkCmdDraw(vkcommandbuffer, 3, 1, 0, 0);
               m_pshaderBlend3->unbind();
            }
            iLayer++;

         }

      }
      //}



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

      ::cast < texture > ptexture = m_pgpurenderer->m_pgpurendertarget->current_texture();

      ::cast < command_buffer > pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2();

      if (pcommandbuffer->m_estate != command_buffer::e_state_recording)
      {

         ::cast < gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;

         ::cast < render_pass > prenderpass = m_pgpurenderer->m_pgpurendertarget;

         VkFence& fence = prenderpass->inFlightFences.element_at_grow(prenderpass->get_frame_index());

         if (fence)
         {

            vkWaitForFences(pcontext->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

         }

         pcommandbuffer->begin_command_buffer(false);

      }

      ptexture->_new_state(
         pcommandbuffer,
         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
      );


   }


   void context::copy(::gpu::texture* ptextureTarget, ::gpu::texture* ptextureSource)
   {

      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

      int iFrameIndex = m_pgpurenderer->m_pgpurendertarget->get_frame_index();

      ::cast < command_buffer > pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2();

      ::cast < texture > ptextureDst = ptextureTarget;

      ::cast < texture > ptextureSrc = ptextureSource;

      ptextureDst->_new_state(
         pcommandbuffer,
         VK_ACCESS_TRANSFER_WRITE_BIT,
         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         VK_PIPELINE_STAGE_TRANSFER_BIT);

      ptextureSrc->_new_state(
         pcommandbuffer,
         VK_ACCESS_TRANSFER_READ_BIT,
         VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         VK_PIPELINE_STAGE_TRANSFER_BIT);

      auto srcImage = ptextureSrc->m_vkimage;

      auto dstImage = ptextureDst->m_vkimage;

      // Copy region
      VkImageCopy copyRegion = {
          .srcSubresource = {
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .mipLevel = 0,
              .baseArrayLayer = 0,
              .layerCount = 1,
          },
          .srcOffset = { 0, 0, 0 },
          .dstSubresource = {
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .mipLevel = 0,
              .baseArrayLayer = 0,
              .layerCount = 1,
          },
          .dstOffset = { 0, 0, 0 },
          .extent =
         {
            (uint32_t)m_rectangle.width(),
            (uint32_t)m_rectangle.height(),
            1},
      };

      vkCmdCopyImage(pcommandbuffer->m_vkcommandbuffer,
         srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         1, &copyRegion);

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

  
   //void context::initialize_rectangle_shader(::gpu::shader* pshader)
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
   //      pcontext->input_layout(::graphics3d::sequence2_color_properties()));


   //}





} // namespace gpu_vulkan



