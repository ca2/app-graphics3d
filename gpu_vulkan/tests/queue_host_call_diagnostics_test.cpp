#include "../queue_host_call_diagnostics.h"

#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>


int main()
{

   gpu_vulkan::queue_host_call_diagnostic_registry registry;

   auto pstateGraphics = registry.state_for(0x1000);
   auto pstateTransfer = registry.state_for(0x1000);
   auto pstatePresent = registry.state_for(0x2000);

   assert(pstateGraphics == pstateTransfer);
   assert(pstateGraphics != pstatePresent);

   auto entryGraphics = gpu_vulkan::enter_queue_host_call(*pstateGraphics);

   assert(entryGraphics.m_uSerial == 1);
   assert(entryGraphics.m_uActiveCallCount == 1);
   assert(!entryGraphics.m_bOverlap);

   auto entryTransfer = gpu_vulkan::enter_queue_host_call(*pstateTransfer);

   assert(entryTransfer.m_uSerial == 2);
   assert(entryTransfer.m_uActiveCallCount == 2);
   assert(entryTransfer.m_bOverlap);

   assert(gpu_vulkan::leave_queue_host_call(*pstateTransfer) == 1);
   assert(gpu_vulkan::leave_queue_host_call(*pstateGraphics) == 0);

   {

      gpu_vulkan::scoped_queue_host_call scopedGraphics(pstateGraphics);

      assert(!scopedGraphics.entry().m_bOverlap);

      {

         gpu_vulkan::scoped_queue_host_call scopedTransfer(pstateTransfer);

         assert(scopedTransfer.entry().m_bOverlap);
         assert(scopedTransfer.entry().m_uActiveCallCount == 2);

      }

      assert(pstateGraphics->m_uActiveCallCount.load() == 1);

   }

   assert(pstateGraphics->m_uActiveCallCount.load() == 0);

   std::atomic<bool> bWorkerStarted{false};
   std::atomic<bool> bWorkerEnteredQueueCall{false};
   std::thread worker;

   {

      gpu_vulkan::scoped_queue_host_call scopedGraphics(pstateGraphics);

      worker = std::thread(
         [&]()
         {

            bWorkerStarted = true;

            gpu_vulkan::scoped_queue_host_call scopedTransfer(pstateTransfer);

            bWorkerEnteredQueueCall = true;

         });

      while (!bWorkerStarted)
      {

         std::this_thread::yield();

      }

      std::this_thread::sleep_for(std::chrono::milliseconds(20));

      assert(!bWorkerEnteredQueueCall);

   }

   worker.join();

   assert(bWorkerEnteredQueueCall);

   bWorkerStarted = false;
   bWorkerEnteredQueueCall = false;

   {

      std::unique_lock<std::recursive_mutex> externalQueueOwner(
         pstateGraphics->m_mutexHostCall);

      worker = std::thread(
         [&]()
         {

            bWorkerStarted = true;

            gpu_vulkan::scoped_queue_host_call scopedPresent(pstateGraphics);

            bWorkerEnteredQueueCall = true;

         });

      while (!bWorkerStarted)
      {

         std::this_thread::yield();

      }

      std::this_thread::sleep_for(std::chrono::milliseconds(20));

      assert(!bWorkerEnteredQueueCall);

   }

   worker.join();

   assert(bWorkerEnteredQueueCall);

   return 0;

}
