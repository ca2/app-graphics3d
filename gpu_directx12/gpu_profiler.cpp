// GPU Performance Profiler Implementation
#include "framework.h"
#include "gpu_profiler.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace gpu_directx12
{

   gpu_profiler::gpu_profiler()
      : m_max_history_size(120)
      , m_frame_count(0)
      , m_is_profiling(true)
      , m_current_draw_calls(0)
      , m_current_triangles(0)
   {
   }

   gpu_profiler::~gpu_profiler()
   {
   }

   void gpu_profiler::initialize(ID3D12Device* pdevice, IDXGIAdapter3* padapter)
   {
      m_pdevice = pdevice;
      m_padapter = padapter;
      reset();
   }

   void gpu_profiler::begin_frame()
   {
      if (!m_is_profiling) return;
      
      m_frame_start = std::chrono::steady_clock::now();
      m_current_draw_calls = 0;
      m_current_triangles = 0;
   }

   void gpu_profiler::end_frame()
   {
      if (!m_is_profiling) return;
      
      auto frame_end = std::chrono::steady_clock::now();
      
      frame_stats stats;
      stats.timestamp = frame_end;
      
      // Calculate frame time
      auto frame_duration = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - m_frame_start);
      stats.frame_time_ms = frame_duration.count() / 1000.0;
      
      // For now, GPU time equals frame time (can be refined with D3D12 queries)
      stats.gpu_time_ms = stats.frame_time_ms;
      
      // Query VRAM usage
      stats.vram_used_bytes = 0;
      stats.vram_budget_bytes = 0;
      query_vram_usage(stats.vram_used_bytes, stats.vram_budget_bytes);
      
      stats.draw_calls = m_current_draw_calls;
      stats.triangles = m_current_triangles;
      
      // Add to history
      m_frame_history.push_back(stats);
      if (m_frame_history.size() > m_max_history_size)
      {
         m_frame_history.pop_front();
      }
      
      m_last_frame = frame_end;
      m_frame_count++;
   }

   void gpu_profiler::record_draw_call(uint32_t triangle_count)
   {
      if (!m_is_profiling) return;
      
      m_current_draw_calls++;
      m_current_triangles += triangle_count;
   }

   const gpu_profiler::frame_stats& gpu_profiler::get_current_frame_stats() const
   {
      static frame_stats empty_stats{};
      if (m_frame_history.empty())
         return empty_stats;
      return m_frame_history.back();
   }

   gpu_profiler::performance_summary gpu_profiler::get_performance_summary() const
   {
      performance_summary summary{};
      
      if (m_frame_history.empty())
         return summary;
      
      double total_frame_time = 0.0;
      double total_gpu_time = 0.0;
      uint64_t total_vram = 0;
      double min_time = std::numeric_limits<double>::max();
      double max_time = 0.0;
      
      for (const auto& stats : m_frame_history)
      {
         total_frame_time += stats.frame_time_ms;
         total_gpu_time += stats.gpu_time_ms;
         total_vram += stats.vram_used_bytes;
         min_time = std::min(min_time, stats.frame_time_ms);
         max_time = std::max(max_time, stats.frame_time_ms);
      }
      
      size_t frame_count = m_frame_history.size();
      summary.avg_frame_time_ms = total_frame_time / frame_count;
      summary.avg_gpu_time_ms = total_gpu_time / frame_count;
      summary.avg_vram_used_mb = (total_vram / frame_count) / (1024 * 1024);
      summary.min_frame_time_ms = min_time;
      summary.max_frame_time_ms = max_time;
      summary.avg_fps = (summary.avg_frame_time_ms > 0.0) ? (1000.0 / summary.avg_frame_time_ms) : 0.0;
      summary.total_frames = static_cast<uint32_t>(m_frame_count);
      
      return summary;
   }

   bool gpu_profiler::query_vram_usage(uint64_t& used_bytes, uint64_t& budget_bytes)
   {
      if (!m_padapter)
         return false;
      
      DXGI_QUERY_VIDEO_MEMORY_INFO memInfo;
      HRESULT hr = m_padapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo);
      
      if (SUCCEEDED(hr))
      {
         used_bytes = memInfo.CurrentUsage;
         budget_bytes = memInfo.Budget;
         return true;
      }
      
      return false;
   }

   void gpu_profiler::reset()
   {
      m_frame_history.clear();
      m_frame_count = 0;
      m_current_draw_calls = 0;
      m_current_triangles = 0;
   }

   std::string gpu_profiler::get_performance_report() const
   {
      auto summary = get_performance_summary();
      
      std::ostringstream oss;
      oss << std::fixed << std::setprecision(2);
      oss << "=== GPU Performance Report ===\n";
      oss << "Total Frames: " << summary.total_frames << "\n";
      oss << "Average FPS: " << summary.avg_fps << "\n";
      oss << "Avg Frame Time: " << summary.avg_frame_time_ms << " ms\n";
      oss << "Min Frame Time: " << summary.min_frame_time_ms << " ms\n";
      oss << "Max Frame Time: " << summary.max_frame_time_ms << " ms\n";
      oss << "Avg GPU Time: " << summary.avg_gpu_time_ms << " ms\n";
      oss << "Avg VRAM Used: " << summary.avg_vram_used_mb << " MB\n";
      oss << "==============================\n";
      
      return oss.str();
   }

} // namespace gpu_directx12
