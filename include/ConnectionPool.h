#pragma once
#include "sql.h"
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <atomic>

class ConnectionPool
{
private:
    ConnectionPool();                       // 构造函数私有化构造单例
    std::string host;                       // 主机地址
    std::string port;                       // 端口号
    std::string dbname;                     // 数据库名
    std::string user;                       /// 用户名
    std::string password;                   // 用户密码
    unsigned int maxConnSize;               // 最大连接数
    unsigned int minConnSize;               // 最小连接数
    std::atomic<unsigned int> busyConnSize; // 繁忙连接数
    //std::queue<SQL *> connPool;             // 连接池
    std::queue<std::shared_ptr<SQL>> connPool; // 连接池
    std::mutex mtx;                         // 互斥锁
    std::condition_variable producer_cv;    // 生产者条件变量
    std::condition_variable consumer_cv;    // 消费者条件变量
    std::thread producerThread;             // 生产者线程
    std::thread destroyThread;              // 销毁线程
    std::atomic<bool> isStop;               // 停止标志
    long long timeOut;                      // 连接超时时间
    void producer();                        // 生产者函数
    void destroy();                         // 销毁函数
    void addConn();                         // 添加连接
public:
    ~ConnectionPool(); // 析构函数
    ConnectionPool(const ConnectionPool &) = delete;
    ConnectionPool &operator=(const ConnectionPool &) = delete;
    static std::shared_ptr<ConnectionPool> getConnPool(); // 获取连接池实例
    std::shared_ptr<SQL> getConn();       // 获取连接
};