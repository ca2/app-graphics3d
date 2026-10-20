// Enhanced Error Handling for DirectX 12
// Provides detailed error messages and debugging utilities
#pragma once

#include "_.h"
#include <string>
#include <functional>

namespace gpu_directx12
{

   /// @brief Enhanced error handler for DirectX 12 operations
   /// Provides detailed error messages, automatic retry logic, and debugging helpers
   class CLASS_DECL_GPU_DIRECTX12 error_handler
   {
   public:
      
      enum class error_severity
      {
         info,
         warning,
         error,
         fatal
      };
      
      struct error_info
      {
         HRESULT hresult;
         std::string message;
         std::string function;
         std::string file;
         int line;
         error_severity severity;
         std::chrono::steady_clock::time_point timestamp;
      };
      
      using error_callback = std::function<void(const error_info&)>;
      
   private:
      
      std::vector<error_info> m_error_history;
      size_t m_max_history_size;
      error_callback m_error_callback;
      bool m_break_on_error;
      
   public:
      
      error_handler();
      ~error_handler();
      
      /// @brief Handle HRESULT with detailed error information
      /// @return true if succeeded, false if failed
      bool handle_hresult(
         HRESULT hr,
         const char* function,
         const char* file,
         int line,
         error_severity severity = error_severity::error);
      
      /// @brief Get human-readable description of HRESULT
      static std::string get_error_description(HRESULT hr);
      
      /// @brief Get detailed error message including D3D12 specific information
      static std::string get_detailed_error_message(HRESULT hr);
      
      /// @brief Check for device removed and get removal reason
      static HRESULT check_device_removed(ID3D12Device* pdevice, std::string& reason);
      
      /// @brief Set custom error callback
      void set_error_callback(error_callback callback) { m_error_callback = callback; }
      
      /// @brief Enable/disable break on error (for debugging)
      void set_break_on_error(bool enable) { m_break_on_error = enable; }
      
      /// @brief Get error history
      const std::vector<error_info>& get_error_history() const { return m_error_history; }
      
      /// @brief Clear error history
      void clear_history() { m_error_history.clear(); }
      
      /// @brief Get error statistics
      struct error_stats
      {
         uint32_t total_errors;
         uint32_t fatal_errors;
         uint32_t errors;
         uint32_t warnings;
      };
      
      error_stats get_error_stats() const;
      
      /// @brief Generate error report
      std::string get_error_report() const;
   };
   
   // Helper macros for error handling
   #define DX12_CHECK(expr, handler) \
      do { \
         HRESULT __hr = (expr); \
         if (FAILED(__hr)) { \
            handler.handle_hresult(__hr, #expr, __FILE__, __LINE__); \
         } \
      } while(0)
   
   #define DX12_CHECK_THROW(expr, handler) \
      do { \
         HRESULT __hr = (expr); \
         if (FAILED(__hr)) { \
            if (!handler.handle_hresult(__hr, #expr, __FILE__, __LINE__, \
                error_handler::error_severity::fatal)) { \
               throw std::runtime_error("DirectX 12 fatal error"); \
            } \
         } \
      } while(0)

} // namespace gpu_directx12
