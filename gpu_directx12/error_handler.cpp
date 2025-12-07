// Enhanced Error Handler Implementation
#include "framework.h"
#include "error_handler.h"
#include <sstream>
#include <iomanip>

namespace gpu_directx12
{

   error_handler::error_handler()
      : m_max_history_size(100)
      , m_break_on_error(false)
   {
   }

   error_handler::~error_handler()
   {
   }

   bool error_handler::handle_hresult(
      HRESULT hr,
      const char* function,
      const char* file,
      int line,
      error_severity severity)
   {
      if (SUCCEEDED(hr))
         return true;
      
      error_info info;
      info.hresult = hr;
      info.function = function;
      info.file = file;
      info.line = line;
      info.severity = severity;
      info.timestamp = std::chrono::steady_clock::now();
      info.message = get_detailed_error_message(hr);
      
      // Add to history
      m_error_history.push_back(info);
      if (m_error_history.size() > m_max_history_size)
      {
         m_error_history.erase(m_error_history.begin());
      }
      
      // Call user callback if set
      if (m_error_callback)
      {
         m_error_callback(info);
      }
      
      // Break on error if enabled (useful for debugging)
      if (m_break_on_error && severity >= error_severity::error)
      {
         #ifdef _DEBUG
         __debugbreak();
         #endif
      }
      
      return false;
   }

   std::string error_handler::get_error_description(HRESULT hr)
   {
      // Get system error message
      LPSTR messageBuffer = nullptr;
      size_t size = FormatMessageA(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL,
         hr,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
         (LPSTR)&messageBuffer,
         0,
         NULL);
      
      std::string message;
      if (size > 0)
      {
         message = std::string(messageBuffer, size);
         LocalFree(messageBuffer);
      }
      else
      {
         std::ostringstream oss;
         oss << "Unknown error: 0x" << std::hex << std::setw(8) << std::setfill('0') << hr;
         message = oss.str();
      }
      
      return message;
   }

   std::string error_handler::get_detailed_error_message(HRESULT hr)
   {
      std::ostringstream oss;
      oss << "HRESULT: 0x" << std::hex << std::setw(8) << std::setfill('0') << hr << "\n";
      
      // Add common DirectX error codes with explanations
      switch (hr)
      {
      case DXGI_ERROR_DEVICE_REMOVED:
         oss << "Device Removed: The GPU device has been physically removed or driver has crashed.\n";
         oss << "Possible causes: GPU overheating, driver crash, power issues.\n";
         break;
      case DXGI_ERROR_DEVICE_RESET:
         oss << "Device Reset: The GPU device has been reset.\n";
         oss << "Possible causes: Driver update, TDR (Timeout Detection and Recovery).\n";
         break;
      case E_OUTOFMEMORY:
         oss << "Out of Memory: Insufficient GPU or system memory.\n";
         oss << "Consider reducing texture quality or resource usage.\n";
         break;
      case DXGI_ERROR_INVALID_CALL:
         oss << "Invalid Call: The method call is invalid.\n";
         oss << "Check API usage and parameter validity.\n";
         break;
      case D3D12_ERROR_ADAPTER_NOT_FOUND:
         oss << "Adapter Not Found: Could not find a suitable GPU adapter.\n";
         break;
      case D3D12_ERROR_DRIVER_VERSION_MISMATCH:
         oss << "Driver Version Mismatch: Graphics driver is incompatible.\n";
         oss << "Update your graphics drivers.\n";
         break;
      default:
         oss << get_error_description(hr);
         break;
      }
      
      return oss.str();
   }

   HRESULT error_handler::check_device_removed(ID3D12Device* pdevice, std::string& reason)
   {
      if (!pdevice)
      {
         reason = "Device pointer is null";
         return E_POINTER;
      }
      
      HRESULT hr = pdevice->GetDeviceRemovedReason();
      
      if (hr != S_OK)
      {
         reason = get_detailed_error_message(hr);
      }
      else
      {
         reason = "Device is functioning normally";
      }
      
      return hr;
   }

   error_handler::error_stats error_handler::get_error_stats() const
   {
      error_stats stats{};
      
      for (const auto& error : m_error_history)
      {
         stats.total_errors++;
         
         switch (error.severity)
         {
         case error_severity::fatal:
            stats.fatal_errors++;
            break;
         case error_severity::error:
            stats.errors++;
            break;
         case error_severity::warning:
            stats.warnings++;
            break;
         default:
            break;
         }
      }
      
      return stats;
   }

   std::string error_handler::get_error_report() const
   {
      auto stats = get_error_stats();
      
      std::ostringstream oss;
      oss << "=== DirectX 12 Error Report ===\n";
      oss << "Total Errors: " << stats.total_errors << "\n";
      oss << "Fatal Errors: " << stats.fatal_errors << "\n";
      oss << "Errors: " << stats.errors << "\n";
      oss << "Warnings: " << stats.warnings << "\n";
      oss << "\nRecent Errors:\n";
      
      // Show last 10 errors
      size_t start = (m_error_history.size() > 10) ? m_error_history.size() - 10 : 0;
      for (size_t i = start; i < m_error_history.size(); ++i)
      {
         const auto& error = m_error_history[i];
         oss << "[" << i + 1 << "] ";
         oss << error.function << " at " << error.file << ":" << error.line << "\n";
         oss << "    " << error.message << "\n";
      }
      
      oss << "================================\n";
      
      return oss.str();
   }

} // namespace gpu_directx12
