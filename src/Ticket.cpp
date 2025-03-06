#include"Ticket.h"
using namespace std;

Ticket::Ticket(const string& passenger_account,const string& flight_id, 
                const string& cabin_id, const string& seat_num)
{
    this->passenger_account = passenger_account;
    this->flight_id = flight_id;
    this->cabin_id = cabin_id;
    this->seat_num = seat_num;
}

bool Ticket::bookTicket(shared_ptr<ConnectionPool> pool)
{
    shared_ptr<SQL> gs = pool->getConn();//获取连接池的连接
    //编织插入sql语句
    string sql = "INSERT INTO ticket (passenger_account, flight_id, cabin_id, seat_num) VALUES ('" 
                 + this->passenger_account + "', '" + this->flight_id + "', '" + this->cabin_id + "', '" + this->seat_num + "');";
    return gs->dml(sql);//返回预定结果
}

bool Ticket::cancelTicket(shared_ptr<ConnectionPool> pool,string order_id)
{
    shared_ptr<SQL> gs = pool->getConn();//获取连接池的连接
    //编织删除sql语句
    string sql = "DELETE FROM ticket WHERE orders_id = '" + order_id 
                + "' AND passenger_account = '" + this->passenger_account + "'";
    return gs->dml(sql);//返回取消结果
}

bool Ticket::queryTicket(shared_ptr<ConnectionPool> pool)
{
    shared_ptr<SQL> gs = pool->getConn();//获取连接池的连接
    //编织查询sql语句
    string sql = "SELECT * FROM ticket WHERE passenger_account = '" + this->passenger_account + "';";
    if(!gs->dql(sql))
    {
        return false;//sql执行失败
    }
    //检查查询结果是否为空
    if(gs->isResEmpty())
    {
        return false;//机票信息未找到
    }
    gs->show();//显示查询结果
    return true;//查询成功
}