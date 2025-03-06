#include <iostream>
#include <memory>
#include "FlightBookingSystem.h"
#include "./include/ConnectionPool.h"

int main()
{
    std::string host = "127.0.0.1";
    std::string port = "7654";
    std::string dbname = "last";
    std::string user = "opengauss";
    std::string password = "13690919281qq*";

    // 创建连接池
    auto pool = ConnectionPool::getConnPool();

    // 创建航班订票管理系统
    FlightBookingSystem system(pool);

    Passenger passenger("", "");
    // 键入登录注册选择界面
    while (true)
    {
        system.isLoginMenu();

        int choice;
        std::cin >> choice;

        if(choice == 1)
        {
            if(system.login())
            {
                break;
            }
        }
        else if(choice == 2)
        {
            if(system.registerPassenger())
            {
                break;
            }
            else
            {
                std::cout << "注册失败,返回登录界面!" << std::endl;
            }
        }
        else if(choice == 3)
        {
            std::cout << "退出系统..." << std::endl;
            return 0;
        }
        else
        {
            std::cout << "无效选择，请重新输入!" << std::endl;
        }
    }

    // 登录成功后进入系统主菜单
    while (true)
    {
        system.printMenu();
        int choice;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            system.queryFlight();
            break;
        case 2:
            system.bookTicket();
            break;
        case 3:
            system.cancelTicket();
            break;
        case 4:
            std::cout << "退出系统..." << std::endl;
            return 0;
        default:
            std::cout << "无效选择，请重新输入!" << std::endl;
        }
    }

    return 0;
}