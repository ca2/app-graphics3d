#include "../cpu_buffer_sampling_sync.h"

#include <cassert>


int main()
{

   using gpu_vulkan::enum_cpu_buffer_sampling_completion;

   assert(!gpu_vulkan::cpu_buffer_sampling_needs_render_finished_semaphore(
      enum_cpu_buffer_sampling_completion::queue_idle));

   assert(gpu_vulkan::cpu_buffer_sampling_needs_render_finished_semaphore(
      enum_cpu_buffer_sampling_completion::render_finished_semaphore));

   return 0;

}
