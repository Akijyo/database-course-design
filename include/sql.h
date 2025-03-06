#pragma once
#include<iostream>
#include<string>
#include<iomanip>
#include<chrono>
#include<vector>
#include<cstring>
#include<algorithm>
#include"/usr/local/opengauss/include/libpq-fe.h"

class SQL
{
friend class ConnectionPool;
private:
    std::string conninfo;//连接信息
    PGconn *conn;//连接实例
    int nFields;//字段数
    int row;//行数
    PGresult *res;//查询结果
    std::chrono::steady_clock::time_point aliveTime;//连接存活时间
public:
    SQL();
    ~SQL();
    SQL(const SQL &)=delete;
    SQL &operator=(const SQL &)=delete;   
    bool connect(std::string host,std::string port,std::string dbname,std::string user,std::string password);
    bool dml(std::string sql);//执行增删改
    bool dql(std::string sql);//执行查询
    void show();//显示查询结果
    bool isResEmpty();//判断查询结果是否为空
    void refreshTime();//刷新连接存活时间
    long long getAliveTime();//获取连接存活时间
};