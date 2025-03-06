#pragma once
#include <string>
#include "ConnectionPool.h"

class Flight {
public:
    Flight();//默认构造函数
    Flight(const std::string& flight_id);//构造函数
    bool queryAllFlight(std::shared_ptr<ConnectionPool> pool);//查询全部航班
    bool queryFlight(std::shared_ptr<ConnectionPool> pool);//查询指定航班航班
private:
    std::string flight_id;//航班号
};