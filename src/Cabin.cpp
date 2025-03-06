#include"Cabin.h"
using namespace std;

Cabin::Cabin(const string& flight_id, const string& cabin_id)
{
    this->flight_id = flight_id;
    this->cabin_id = cabin_id;
}

bool Cabin::queryCabinInfo(shared_ptr<ConnectionPool> pool)
{
    shared_ptr<SQL> gs = pool->getConn();//获取连接池的连接
    //编织查询sql语句
    string sql = "select * from cabin where flight_id = '" + this->flight_id + "' and cabin_id = '" + this->cabin_id + "';";
    if(!gs->dql(sql))
    {
        return false;//sql执行失败
    }
    //检查查询结果是否为空
    if(gs->isResEmpty())
    {
        return false;//舱位信息未找到
    }
    //gs->show();//显示查询结果
    return true;//查询成功
}