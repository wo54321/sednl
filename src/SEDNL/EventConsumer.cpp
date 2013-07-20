// SEDNL - Copyright (c) 2013 Jeremy S. Cochoy
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//     1. The origin of this software must not be misrepresented; you must not
//        claim that you wrote the original software. If you use this software
//        in a product, an acknowledgment in the product documentation would
//        be appreciated but is not required.
//
//     2. Altered source versions must be plainly marked as such, and must not
//        be misrepresented as being the original software.
//
//     3. This notice may not be removed or altered from any source
//        distribution.

#include "SEDNL/EventConsumer.hpp"
#include "SEDNL/EventListener.hpp"
#include "SEDNL/Exception.hpp"

namespace SedNL
{

EventConsumer::EventConsumer()
    :m_producer(nullptr), m_running(false)
{}

EventConsumer::EventConsumer(EventListener &producer)
    :EventConsumer()
{
    set_producer(producer);
}

void EventConsumer::remove_producer() noexcept
{
    if (m_running)
        throw EventException(EventExceptionT::EventConsumerRunning);

    if (m_producer)
    {
        m_producer->remove_consumer(this);
        m_producer = nullptr;
    }
}

void EventConsumer::set_producer(EventListener &producer) throw(EventException)
{
    if (m_running)
        throw EventException(EventExceptionT::EventConsumerRunning);

    if (m_producer)
    {
        m_producer->remove_consumer(this);
        m_producer = nullptr;
    }
    m_producer->add_consumer(this);
    m_producer = &producer;
}

void EventConsumer::clean_slots()
{
    for (auto it = m_slots.begin();
         it != m_slots.end();)
    {
        //If slot is empty
        if (!it->second)
            it = m_slots.erase(it);
        else
            it++;
    }
}

void EventConsumer::run_init()
{
    //Remove empty slots
    clean_slots();

    //TODO
}

typedef std::pair<std::shared_ptr<Connection>, Event> CnEvent;
typedef SafeQueue<CnEvent> EventQueue;
typedef SafeQueue<std::shared_ptr<Connection>> ConnectionQueue;
typedef SafeQueue<TCPServer *> ServerQueue;

template<typename S>
static inline
void process(CnEvent& e, S& slot, EventQueue& queue)
{
    while(queue.pop(e))
        slot(*e.first.get(), e.second);
}

template<typename S>
static inline
void process(CnEvent&, S& slot, ConnectionQueue& queue)
{
    std::shared_ptr<Connection> ptr;
    while(queue.pop(ptr))
        slot(*ptr);
}

template<typename S>
static inline
void process(CnEvent&, S& slot, ServerQueue& queue)
{
    TCPServer* ptr;
    while(queue.pop(ptr))
        slot(*ptr);
}

#define PROCESS_MESSAGES(slot, queue) {    \
        if ((slot))                        \
            process(e, (slot), (queue));}

void EventConsumer::run_imp()
{
    //Sleep for 200ms
    auto rel_time = std::chrono::milliseconds(200);
    CnEvent e;

    while (m_running)
    {
        {
            std::unique_lock<std::mutex> lk(m_descriptor.mutex);
            //Continue if timed out and no work to do
            if (!m_descriptor.cv.wait_for(lk, rel_time,
                                         [&](){return m_descriptor.wake_up;}))
                if (!m_descriptor.wake_up)
                    continue;
            m_descriptor.wake_up = false;
        }

        PROCESS_MESSAGES(m_on_connect_slot, m_producer->m_connected_queue);
        PROCESS_MESSAGES(m_on_server_disconnect_slot,
                         m_producer->m_server_disconnected_queue);
        PROCESS_MESSAGES(m_on_disconnect_slot, m_producer->m_disconnected_queue);

        for (auto& pair : m_slots)
            PROCESS_MESSAGES(pair.second, m_producer->m_events[pair.first]);

        //TODO : Also handle m_on_event_slot

    }

    //TODO
}

void EventConsumer::run() throw(EventException)
{
    if (!m_running)
    {
        run_init();

        m_running = true;
        m_thread = std::thread(std::bind(&EventConsumer::run_imp, this));
    }
    else
        throw EventException(EventExceptionT::EventConsumerRunning);
}

void EventConsumer::join()
{
    if (m_running)
    {
        m_running = false;

        if (m_thread.joinable() == true)
            m_thread.join();
    }
}

} //namespace SedNL

