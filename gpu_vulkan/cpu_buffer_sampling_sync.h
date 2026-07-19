#pragma once


namespace gpu_vulkan
{


   enum class enum_cpu_buffer_sampling_completion
   {

      queue_idle,
      render_finished_semaphore,

   };


   constexpr bool cpu_buffer_sampling_needs_render_finished_semaphore(
      enum_cpu_buffer_sampling_completion ecompletion) noexcept
   {

      return ecompletion == enum_cpu_buffer_sampling_completion::render_finished_semaphore;

   }


} // namespace gpu_vulkan
