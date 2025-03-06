#include "../include/ConnectionPool.h"
using namespace std;

void ConnectionPool::addConn()//对添加连接的封装
{
    //新建一个连接
    shared_ptr<SQL> gs = make_shared<SQL>();
    //连接数据库，并将连接放入连接池
    if (gs->connect(this->host, this->port, this->dbname, this->user, this->password))
    {
        unique_lock<mutex> lock(this->mtx);
        this->connPool.push(gs);
    }
}

ConnectionPool::ConnectionPool()
{
    //这里定义先将连接池内容写死
    this->host = "127.0.0.1";
    this->port = "7654";
    this->dbname = "last";
    this->user = "opengauss";
    this->password = "13690919281qq*";
    this->maxConnSize = 20;
    this->minConnSize = 5;
    this->busyConnSize = 0;
    this->isStop = false;
    this->timeOut = 5000;
    //初始化连接池
    for (int i = 0; i < this->minConnSize; i++)
    {
        this->addConn();
    }
    //创建生产者线程和销毁线程
    this->producerThread = thread(bind(&ConnectionPool::producer, this));
    this->destroyThread = thread(bind(&ConnectionPool::destroy, this));
}

ConnectionPool::~ConnectionPool()
{
    //将连接池停止
    this->isStop = true;
    //唤醒所有线程,去触发停止的判断
    this->producer_cv.notify_all();
    this->consumer_cv.notify_all();
    //等待线程结束
    if(this->producerThread.joinable())
    {
        this->producerThread.join();
    }
    if(this->destroyThread.joinable())
    {
        this->destroyThread.join();
    }
    //清空连接池
    while (!this->connPool.empty())
    {
        this->connPool.pop();
    }
}

shared_ptr<ConnectionPool> ConnectionPool::getConnPool()
{
    //单例模式
    static shared_ptr<ConnectionPool> connPool(new ConnectionPool());
    return connPool;
}

void ConnectionPool::producer()
{
    while (!this->isStop)
    {
        unique_lock<mutex> lock(this->mtx);
        //定义一个生产者工作的规则
        while (!this->isStop || this->connPool.size() + this->busyConnSize >= this->maxConnSize || this->connPool.size() > this->minConnSize)
        {
            this->producer_cv.wait(lock);
            //如果停止标志为真，跳出循环
            if (this->isStop)
            {
                break;
            }
        }
        //二重判断，防止线程终止不掉继续运行
        if (this->isStop)
        {
            break;
        }
        //添加连接
        this->addConn();
        //唤醒消费者
        this->consumer_cv.notify_one();
    }
}

void ConnectionPool::destroy()
{
    while (!this->isStop)
    {
        //销毁线程每500ms检查一次连接池
        this_thread::sleep_for(chrono::milliseconds(500));
        unique_lock<mutex> lock(this->mtx);
        //定义一个销毁连接的规则
        while (!this->isStop&&this->connPool.size() > this->minConnSize)
        {
            shared_ptr<SQL> gs = this->connPool.front();
            if (gs->getAliveTime() >= this->timeOut)
            {
                this->connPool.pop();
            }
            else
            {
                break;
            }
        }
    }
}

shared_ptr<SQL> ConnectionPool::getConn()
{
    unique_lock<mutex> lock(this->mtx);
    while (!this->isStop && this->connPool.empty())
    {
        //等待连接池中有连接
        this->consumer_cv.wait(lock);
        //如果停止标志为真，返回空指针
        if(this->isStop)
        {
            return nullptr;
        }
    }
    //二重判断，防止连接池关闭后继续运行下面的代码
    if(this->isStop)
    {
        return nullptr;
    }
    //获取连接
    shared_ptr<SQL> gs = this->connPool.front();
    this->connPool.pop();
    this->busyConnSize++;
    //使用智能指针与自定义连接器管理连接，这里需要捕获指针指针，也就是lambda捕获gs，不然gs会被销毁，导致连接器使用原始指针gs.get()去创建新的智能指针
    //但是由于这个指针先被释放，所以会导致连接器使用一个已经被释放的指针去创建新的智能指针，导致程序崩溃
    shared_ptr<SQL> gsWithBack(gs.get(), [this, gs](SQL* ptr)
    {
        unique_lock<mutex> lock(this->mtx);
        this->busyConnSize--;
        gs->refreshTime();
        PQclear(gs->res);
        gs->res=nullptr;
        gs->row=0;
        gs->nFields=0;
        this->connPool.push(gs);
        this->producer_cv.notify_one();
    });
    return gsWithBack;
}








// #include "../include/ConnectionPool.h"
// using namespace std;

// void ConnectionPool::addConn()
// {
//     SQL *gs=new SQL();
//     if (gs->connect(this->host, this->port, this->dbname, this->user, this->password))
//     {
//         unique_lock<mutex> lock(this->mtx);
//         this->connPool.push(gs);
//     }
// }

// ConnectionPool::ConnectionPool()
// {
//     this->host = "127.0.0.1";
//     this->port = "7654";
//     this->dbname = "last";
//     this->user = "opengauss";
//     this->password = "13690919281qq*";
//     this->maxConnSize = 20;
//     this->minConnSize = 3;
//     this->busyConnSize = 0;
//     this->isStop = false;
//     this->timeOut = 5000;
//     for (int i = 0; i < this->minConnSize; i++)
//     {
//         this->addConn();
//     }
//     this->producerThread = thread(bind(&ConnectionPool::producer, this));
//     this->destroyThread = thread(bind(&ConnectionPool::destroy, this));
// }

// ConnectionPool::~ConnectionPool()
// {
//     this->isStop = true;
//     this->producer_cv.notify_all();
//     this->consumer_cv.notify_all();
//     if(this->producerThread.joinable())
//     {
//         this->producerThread.join();
//     }
//     if(this->destroyThread.joinable())
//     {
//         this->destroyThread.join();
//     }
//     while (!this->connPool.empty())
//     {
//         SQL *gs = this->connPool.front();
//         this->connPool.pop();
//         delete gs;
//         gs = nullptr;
//     }
//     cout<<"连接池析构"<<endl;
// }

// ConnectionPool *ConnectionPool::getConnPool()
// {
//     static ConnectionPool pool;
//     return &pool;
// }

// void ConnectionPool::producer()
// {
//     while (!this->isStop)
//     {
//         unique_lock<mutex> lock(this->mtx);
//         while (!this->isStop || this->connPool.size() + this->busyConnSize >= this->maxConnSize || this->connPool.size() > this->minConnSize)
//         {
//             this->producer_cv.wait(lock);
//             if (this->isStop)
//             {
//                 break;
//             }
//         }
//         if (this->isStop)
//         {
//             break;
//         }
//         this->addConn();
//         this->consumer_cv.notify_one();
//     }
// }

// void ConnectionPool::destroy()
// {
//     while (!this->isStop)
//     {
//         this_thread::sleep_for(chrono::milliseconds(500));
//         unique_lock<mutex> lock(this->mtx);
//         while (!this->isStop&&this->connPool.size() > this->minConnSize)
//         {
//             SQL *gs = this->connPool.front();
//             if (gs->getAliveTime() >= this->timeOut)
//             {
//                 this->connPool.pop();
//                 delete gs;
//                 gs = nullptr;
//             }
//             else
//             {
//                 break;
//             }
//         }
//     }
// }

// shared_ptr<SQL> ConnectionPool::getConn()
// {
//     unique_lock<mutex> lock(this->mtx);
//     while (!this->isStop && this->connPool.empty())
//     {
//         this->consumer_cv.wait(lock);
//         if(this->isStop)
//         {
//             return nullptr;
//         }
//     }
//     if(this->isStop)
//     {
//         return nullptr;
//     }
//     shared_ptr<SQL> gs(this->connPool.front(),[this](SQL *gs)
//     {
//         unique_lock<mutex> lock(this->mtx);
//         gs->refreshTime();
//         this->busyConnSize--;
//         this->connPool.push(gs);
//         this->producer_cv.notify_one();
//         cout<<"连接回收"<<endl;
//     });
//     this->connPool.pop();
//     this->busyConnSize++;
//     return gs;
// }