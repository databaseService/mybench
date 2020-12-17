#ifndef MYBENCH_COUNT_DOWN_LATCH_H
#define MYBENCH_COUNT_DOWN_LATCH_H

#include <mutex>
#include <condition_variable>

namespace mybench{
    class Count_down_latch
    {
    public:
        Count_down_latch();
        Count_down_latch(uint32_t count);
        virtual ~Count_down_latch();
        virtual void await() noexcept;
        virtual void count_down() noexcept;
        virtual void count_up() noexcept;

    private:
        mutable std::mutex                          m_lock;
        mutable std::condition_variable             m_cond;
        uint32_t                                    m_count;
    };
}

#endif //MYBENCH_COUNT_DOWN_LATCH_H
