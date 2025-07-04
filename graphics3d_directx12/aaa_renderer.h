#pragma once
 
////#include "AppCore/vk_window.h"
////#include "graphics3d/container.h"
//#include "context.h"
////#include "offscreen.h"
////#include "swapchain.h"
//#include "render_pass.h"
//#include "bred/graphics3d/renderer.h"
//
//
//#include <memory>
//#include <vector>
//#include <cassert>
// 
//constexpr unsigned int FRAME_OVERLAP = 2;
//
//
//namespace graphics3d_directx12
//{
//
//
//	class CLASS_DECL_GRAPHICS3D_DIRECTX12 renderer:
//	virtual public ::gpu-::renderer
//	{
//	public:
//		bool m_bOffScreen = true;
//		//renderer(VkWindow &window, ::gpu::context * pgpucontext);
//		renderer();
//		~renderer();
//
//
//		void initialize_renderer(::gpu::context * pgpucontext) override;
//
//		VkRenderPass getRenderPass() const 
//		{
//		
//			return m_pvkcrenderpass->getRenderPass();
//			//return m_bOffScreen ?
//				//m_pvkcoffscreen->getRenderPass():
//				//m_pvkcswapchain->getRenderPass(); 
//
//		}
//
//		void sample();
//
//		float getAspectRatio() const
//		{
//
//			//if (m_bOffScreen)
//			{
//
//				return m_pvkcrenderpass->extentAspectRatio();
//
//			}
//			//else
//			//{
//
//			//	return m_pvkcswapchain->extentAspectRatio();
//
//			//}
//
//		}
//
//
//		bool isFrameInProgress() const { return isFrameStarted; }
//
//
//		VkCommandBuffer getCurrentCommandBuffer() const {
//			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
//			return commandBuffers[currentFrameIndex];
//		}
//
//		int getFrameIndex() const {
//			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
//			return currentFrameIndex;
//		}
//
//		::pointer < ::graphics3d::frame> beginFrame() override;
//		void endFrame() override;
//		void on_begin_render(::graphics3d::frame * pframe) override;
//		void on_end_render(::graphics3d::frame* pframe) override;
//	public:
//		void create_command_buffers() override;
//
//		void free_command_buffers() override;
//		//void recreateSwapchain();
//		void defer_layout();
//
//		//void prepareOffScreen();
//
//		struct OffScreenSampler :
//			virtual public ::particle
//		{
//
//			VkExtent2D			m_vkextent2d;
//			VkDeviceMemory		m_vkdevicememory;
//			VkImage				m_vkimage;
//
//			::pointer < context > m_pgpucontext;
//			::pointer < renderer > m_prenderer;
//
//
//
//
//			OffScreenSampler();
//			~OffScreenSampler();
//
//
//			void initialize_offscreen_sampler(::gpu::context * pgpucontext);
//
//			void clear();
//			void update(VkExtent2D vkextent2d);
//			void destroy();
//
//			void sample(VkImage vkimage);
//
//			void send_sample();
//
//		};
//
//		::pointer < ::user::graphics3d >	m_pimpact;
//		::pointer < context >				m_pgpucontext;
//		::pointer < OffScreenSampler >	m_poffscreensampler;
//		//::pointer<swap_chain_render_pass>			m_pvkcswapchain;
//		//::pointer<offscreen_render_pass>			m_pvkcoffscreen;
//		::pointer<render_pass>			m_pvkcrenderpass;
//		::array<VkCommandBuffer>	commandBuffers;
//		VkExtent2D m_extentRenderer;
//		uint32_t currentImageIndex;
//		int currentFrameIndex = 0;
//		bool isFrameStarted = false;
//	
//	};
//}
//
