#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Passenger.h"
#include "Flight.h"
#include "Cabin.h"
#include "Ticket.h"
#include "ConnectionPool.h"

class FlightBookingSystem
{
public:
    FlightBookingSystem(std::shared_ptr<ConnectionPool> pool);

    void isLoginMenu();       // 登录注册选择界面
    bool login();             // 登录
    bool registerPassenger(); // 注册

    void printMenu();    // 菜单界面
    void queryFlight();  // 查询航班
    void bookTicket();   // 预定机票
    void cancelTicket(); // 取消机票
private:
    Passenger passenger;
    std::shared_ptr<ConnectionPool> pool;
};