#include"Flight.h"
using namespace std;

Flight::Flight()
{
}

Flight::Flight(const string& flight_id)
{
    this->flight_id = flight_id;
}

bool Flight::queryAllFlight(shared_ptr<ConnectionPool> pool)
{
    shared_ptr<SQL> gs = pool->getConn();//获取连接池的连接
    //编织查询sql语句
    string sql = "select * from flight;";
    if(!gs->dql(sql))
    {
        return false;//sql执行失败
    }
    //检查查询结果是否为空
    if(gs->isResEmpty())
    {
        return false;//航班信息未找到
    }
    gs->show();//显示查询结果
    return true;//查询成功
}

bool Flight::queryFlight(shared_ptr<ConnectionPool> pool)
{
    shared_ptr<SQL> gs = pool->getConn();//获取连接池的连接
    //编织查询sql语句
    string sql = "select * from flight where flight_id = '" + this->flight_id + "';";
    if(!gs->dql(sql))
    {
        return false;//sql执行失败
    }
    //检查查询结果是否为空
    if(gs->isResEmpty())
    {
        return false;//航班信息未找到
    }
    //gs->show();//显示查询结果
    return true;//查询成功
}