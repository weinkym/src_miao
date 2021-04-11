#include "zproduceconsumer.h"

#include <cstdlib>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include <QString>

#include <thread>
#include <mutex>
#include <condition_variable>
#include "zwlog.h"

int g_data_size = 10;
int g_data[10];

std::mutex g_mutex_data;
std::condition_variable g_not_empty;
std::condition_variable g_not_full;

size_t read_position = 0; // 消费者读取产品位置.
size_t write_position = 0; // 生产者写入产品位置.

static void outLog(const QString &text)
{
    static std::mutex s_mutex;
    std::lock_guard<std::mutex> lock(s_mutex);
    ZW_LOG_WARNING(text);
}

static void produce()
{
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::unique_lock<std::mutex> lock(g_mutex_data);
        if (((write_position + 1) % g_data_size)
               == read_position)
        { // item buffer is full, just wait here.
            {
                outLog("缓冲区满，等待缓冲区不满\n");
            }
            g_not_full.wait(lock); // 生产者等待"产品库缓冲区不为满"这一条件发生.
        }
        g_data[write_position] = write_position;
        outLog(QString("produce %1").arg(write_position));

        write_position++;
        write_position = write_position % g_data_size;
        g_not_empty.notify_one();
        lock.unlock();
    }
}

static void consumer()
{
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds((qrand() % 10 + 1) * 100));
        std::unique_lock<std::mutex> lock(g_mutex_data);
        if (read_position == write_position)
        {
            outLog("缓冲区空，等待生产者生成产品\n");
            g_not_empty.wait(lock);
        }
        outLog(QString("consumer %1").arg(read_position));
        read_position++;
        read_position = read_position % g_data_size;
        g_not_full.notify_one();
        lock.unlock();
    }
}

void ZProduceConsumer::test()
{
    std::thread produce1(produce);
    std::thread produce2(produce);
    std::thread consumer1(consumer);
    std::thread consumer2(consumer);
    std::thread consumer3(consumer);

    produce1.join();
    produce2.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();
}
