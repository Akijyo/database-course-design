#pragma once
#include <string>
#include "ConnectionPool.h"

class Cabin {
public:
    Cabin(const std::string& flight_id, const std::string& cabin_id);//构造函数
    bool queryCabinInfo(std::shared_ptr<ConnectionPool> pool);//查询舱位信息
private:
    std::string flight_id;//航班号 外键
    std::string cabin_id;//舱位号 主键
};