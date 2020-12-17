#include "concurrent/count_down_latch.h"

namespace mybench{

    Count_down_latch::Count_down_latch()
            :m_count(0)
    {

    }

    Count_down_latch::Count_down_latch(uint32_t count)
            :m_count(count)
    {

    }

    Count_down_latch::~Count_down_latch()
    {

    }

    /**
    * TODO test case:
    * count=2
    * thread1 await, thread2 await
    * thread3 count_down to 0
    * thread1 or thread2 wake up, could thread2 or thread1 wake up?
    *
    */
    void Count_down_latch::await() noexcept
    {
      std::unique_lock<std::mutex> lock(m_lock);
      m_cond.wait(lock, [this] { return 0 == m_count; });
    }

    void Count_down_latch::count_down() noexcept
    {
      std::lock_guard<std::mutex> guard(m_lock);
      if (0 == m_count) {
        return;
      }
      --m_count;
      if (0 == m_count) {
        m_cond.notify_all();
      }
    }

    void Count_down_latch::count_up() noexcept
    {
      std::lock_guard<std::mutex> guard(m_lock);
      ++m_count;
    }
}
