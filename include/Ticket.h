#pragma once
#include <string>
#include "ConnectionPool.h"

class Ticket {
public:
    //构造函数
    Ticket(const std::string& passenger_account,const std::string& flight_id, 
            const std::string& cabin_level, const std::string& seat_num);
    //预定机票
    bool bookTicket(std::shared_ptr<ConnectionPool> pool);
    //取消机票
    bool cancelTicket(std::shared_ptr<ConnectionPool> pool,std::string order_id);
    //查询当前客户拥有的机票
    bool queryTicket(std::shared_ptr<ConnectionPool> pool);
private:
    std::string passenger_account;//乘客账号 外键
    std::string flight_id;//航班号 外键
    std::string cabin_id;//舱位等级
    std::string seat_num;//座位号
};