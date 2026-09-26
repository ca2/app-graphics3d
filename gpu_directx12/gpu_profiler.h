// GPU Performance Profiler for DirectX 12
// Provides frame timing, GPU memory stats, and performance metrics
#pragma once

#include "_.h"
#include <chrono>
#include <deque>
#include <string>

namespace gpu_directx12
{

   /// @brief GPU performance profiling utility
   /// Tracks frame times, GPU memory usage, and provides performance statistics
   class CLASS_DECL_GPU_DIRECTX12 gpu_profiler
   {
   public:
      
      struct frame_stats
      {
         double frame_time_ms;           // Frame time in milliseconds
         double gpu_time_ms;             // GPU execution time
         uint64_t vram_used_bytes;       // Video RAM used
         uint64_t vram_budget_bytes;     // Video RAM budget
         uint32_t draw_calls;            // Number of draw calls
         uint32_t triangles;             // Triangle count
         std::chrono::steady_clock::time_point timestamp;
      };

      struct performance_summary
      {
         double avg_frame_time_ms;
         double min_frame_time_ms;
         double max_frame_time_ms;
         double avg_fps;
         double avg_gpu_time_ms;
         uint64_t avg_vram_used_mb;
         uint32_t total_frames;
      };

   private:
      
      comptr<ID3D12Device> m_pdevice;
      comptr<IDXGIAdapter3> m_padapter;
      
      std::deque<frame_stats> m_frame_history;
      size_t m_max_history_size;
      
      std::chrono::steady_clock::time_point m_frame_start;
      std::chrono::steady_clock::time_point m_last_frame;
      
      uint64_t m_frame_count;
      bool m_is_profiling;
      
      // Current frame stats
      uint32_t m_current_draw_calls;
      uint32_t m_current_triangles;
      
   public:
      
      gpu_profiler();
      ~gpu_profiler();
      
      /// @brief Initialize profiler with D3D12 device
      void initialize(ID3D12Device* pdevice, IDXGIAdapter3* padapter);
      
      /// @brief Start profiling a new frame
      void begin_frame();
      
      /// @brief End frame profiling and record statistics
      void end_frame();
      
      /// @brief Record a draw call
      void record_draw_call(uint32_t triangle_count = 0);
      
      /// @brief Get current frame statistics
      const frame_stats& get_current_frame_stats() const;
      
      /// @brief Get performance summary over recent frames
      performance_summary get_performance_summary() const;
      
      /// @brief Get frame history for graphing
      const std::deque<frame_stats>& get_frame_history() const { return m_frame_history; }
      
      /// @brief Query current VRAM usage
      bool query_vram_usage(uint64_t& used_bytes, uint64_t& budget_bytes);
      
      /// @brief Set maximum frame history size (default: 120 frames)
      void set_history_size(size_t size) { m_max_history_size = size; }
      
      /// @brief Enable/disable profiling
      void set_profiling_enabled(bool enabled) { m_is_profiling = enabled; }
      
      /// @brief Check if profiling is active
      bool is_profiling_enabled() const { return m_is_profiling; }
      
      /// @brief Reset profiler statistics
      void reset();
      
      /// @brief Get formatted performance report
      std::string get_performance_report() const;
   };

} // namespace gpu_directx12
