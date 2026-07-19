#pragma once


#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>


namespace gpu_vulkan
{


   struct queue_host_call_diagnostic_entry
   {


      std::uint64_t m_uSerial;
      std::uint32_t m_uActiveCallCount;
      bool m_bOverlap;


   };


   struct queue_host_call_diagnostic_state
   {


      std::atomic<std::uint64_t> m_uNextSerial{0};
      std::atomic<std::uint32_t> m_uActiveCallCount{0};


   };


   class queue_host_call_diagnostic_registry
   {
   public:


      std::shared_ptr<queue_host_call_diagnostic_state> state_for(std::uintptr_t uQueueIdentity)
      {

         std::scoped_lock lock(m_mutex);

         auto &pweakstate = m_map[uQueueIdentity];

         auto pstate = pweakstate.lock();

         if (!pstate)
         {

            pstate = std::make_shared<queue_host_call_diagnostic_state>();

            pweakstate = pstate;

         }

         return pstate;

      }


   private:


      std::mutex m_mutex;
      std::unordered_map<std::uintptr_t, std::weak_ptr<queue_host_call_diagnostic_state>> m_map;


   };


   inline queue_host_call_diagnostic_entry enter_queue_host_call(queue_host_call_diagnostic_state &state)
   {

      const auto uSerial = state.m_uNextSerial.fetch_add(1, std::memory_order_relaxed) + 1;
      const auto uActiveCallCount = state.m_uActiveCallCount.fetch_add(1, std::memory_order_acq_rel) + 1;

      return {uSerial, uActiveCallCount, uActiveCallCount > 1};

   }


   inline std::uint32_t leave_queue_host_call(queue_host_call_diagnostic_state &state)
   {

      return state.m_uActiveCallCount.fetch_sub(1, std::memory_order_acq_rel) - 1;

   }


   class scoped_queue_host_call
   {
   public:


      explicit scoped_queue_host_call(const std::shared_ptr<queue_host_call_diagnostic_state> &pstate) :
         m_pstate(pstate),
         m_entry(enter_queue_host_call(*m_pstate))
      {

      }


      ~scoped_queue_host_call()
      {

         leave_queue_host_call(*m_pstate);

      }


      scoped_queue_host_call(const scoped_queue_host_call &) = delete;
      scoped_queue_host_call &operator=(const scoped_queue_host_call &) = delete;


      const queue_host_call_diagnostic_entry &entry() const
      {

         return m_entry;

      }


   private:


      std::shared_ptr<queue_host_call_diagnostic_state> m_pstate;
      queue_host_call_diagnostic_entry m_entry;


   };


} // namespace gpu_vulkan
