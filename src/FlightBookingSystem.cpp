#include "FlightBookingSystem.h"
using namespace std;

FlightBookingSystem::FlightBookingSystem(shared_ptr<ConnectionPool> pool)
{
    this->pool = pool; // 导入连接池
    this->passenger = Passenger("", ""); // 初始化乘客对象
}

void FlightBookingSystem::isLoginMenu()
{
    cout << "欢迎使用航班订票管理系统" << endl;
    cout << "1. 登录" << endl;
    cout << "2. 注册" << endl;
    cout << "3. 退出" << endl;
}

bool FlightBookingSystem::login()
{
    string account,password;
     
    cout << "请输入账户名: ";
    cin >> account;
    this->passenger.account=account;
    cout << "请输入密码: ";
    cin >> password;
    this->passenger.password=password;

    if(this->passenger.loginIn(pool))
    {
        return true;
    }
    else
    {
        cout<<"登录失败，返回登录页面!"<<endl;
        return false;
    }
}

bool FlightBookingSystem::registerPassenger()
{
    if(this->passenger.registerPassenger(pool))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void FlightBookingSystem::printMenu()
{
    cout << "欢迎使用航班订票管理系统" << endl;
    cout << "1. 查询航班" << endl;
    cout << "2. 订票" << endl;
    cout << "3. 退票" << endl;
    cout << "4. 退出" << endl;
}

void FlightBookingSystem::queryFlight()
{
    Flight flight; // 创建航班对象，用于查询航班信息
    if (flight.queryAllFlight(pool))
    {
        cout << "查询成功!" << endl;
    }
    else
    {
        cout << "航班信息未找到!" << endl;
    }
}

void FlightBookingSystem::bookTicket()
{
    string flight_id, cabin_id, seat_num, cabin_level;

    //1.选择指定的航班
    cout << "请输入航班ID: ";
    cin >> flight_id;
    Flight flight(flight_id); // 创建航班对象，用于查询航班信息
    if (!flight.queryFlight(pool))
    {
        cout << "航班信息未找到!" << endl;
        return;
    }

    //2.选择已选定航班的舱位
    cout << "请选择舱位等级: ";
    cin >> cabin_id;
    Cabin cabin(flight_id, cabin_id); // 创建舱位对象，用于查询舱位信息
    if (!cabin.queryCabinInfo(pool))
    {
        cout << "舱位信息未找到!" << endl;
        return;
    }

    cout << "请输入座位号: ";
    cin >> seat_num;

    Ticket ticket(this->passenger.account, flight_id, cabin_id, seat_num); // 创建机票对象，用于订票
    if (ticket.bookTicket(pool))
    {
        cout << "订票成功!" << endl;
    }
    else
    {
        cout << "订票失败!" << endl;
    }
}

void FlightBookingSystem::cancelTicket()
{
    string order_id;
    cout << "请输入订单ID: ";
    cin >> order_id;

    Ticket ticket(this->passenger.account, "", "", ""); // 创建机票对象，用于退票
    if (ticket.cancelTicket(pool,order_id))
    {
        cout << "退票成功!" << endl;
    }
    else
    {
        cout << "退票失败!" << endl;
    }
}