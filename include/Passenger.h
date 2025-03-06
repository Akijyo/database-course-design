#pragma once
#include <string>
#include <memory>
#include "ConnectionPool.h"

class Passenger {
friend class FlightBookingSystem;
public:
    Passenger();//默认构造
    Passenger(const std::string& account, const std::string& password);//构造函数
    void setAccount(const std::string& account);//设置账号
    void setPassword(const std::string& password);//设置密码
    bool authenticate(std::shared_ptr<ConnectionPool> pool);//验证函数
    std::string getAccount() const;//获取账号
    bool loginIn(std::shared_ptr<ConnectionPool> pool);//登录函数
    bool registerPassenger(std::shared_ptr<ConnectionPool> pool);//注册函数
private:
    std::string account;//账号
    std::string password;//密码
};
