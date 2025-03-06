#include"Passenger.h"
using namespace std;

Passenger::Passenger()
{
    this->account = "";
    this->password = "";
}

Passenger::Passenger(const string& account, const string& password)
{
    this->account = account;
    this->password = password;
}

void Passenger::setAccount(const string& account)
{
    this->account = account;
}

void Passenger::setPassword(const string& password)
{
    this->password = password;
}


bool Passenger::authenticate(shared_ptr<ConnectionPool> pool)
{
    shared_ptr<SQL> gs = pool->getConn();//获取连接池的连接
    //编织查询sql语句
    string sql = "select * from passenger where account = '" + this->account + "' and password = '" + this->password + "';";
    if(!gs->dql(sql))
    {
        return false;//sql执行失败
    }
    //检查查询结果是否为空
    if(gs->isResEmpty())
    {
        return false;//账户或密码错误
    }
    gs->show();//显示查询结果
    return true;//验证成功
}

string Passenger::getAccount() const
{
    return this->account;
}

//验证用户的登录函数
bool Passenger::loginIn(shared_ptr<ConnectionPool> pool)
{
    shared_ptr<SQL> gs = pool->getConn();//获取连接池的连接
    string sql = "select account from passenger where account = '" + this->account + "'";
    //1.先检查用户名是否存在
    if(!gs->dql(sql))
    {
        cout<<"用户名不存在"<<endl;
        return false;//sql执行失败
    }

    if(gs->isResEmpty())
    {
        cout<<"用户名不存在"<<endl;
        return false;//用户名不存在
    }
    else
    {
    //2.再检查密码是否正确
        sql="select account,password from passenger where account = '"+this->account+"' and password = '"+this->password+"';";
        if(!gs->dql(sql))
        {
            return false;//sql执行失败
        }

        if(gs->isResEmpty())
        {
            cout<<"密码错误"<<endl;
            return false;//密码错误
        }
        else
        {
            cout<<"登录成功"<<endl;
            return true;//登录成功
        }
    }
}

//注册用户的函数
bool Passenger::registerPassenger(shared_ptr<ConnectionPool> pool)
{
    shared_ptr<SQL> gs = pool->getConn();//获取连接池的连接
    string account,password,name,tel_num,gender,birthday;

    //1.输入账号
    cout<<"请输入账号（小于30位字符）：";
    while (cin>>account)
    {
        if(account.size()<=30)
        {
            break;
        }
        cout<<"账号过长，请重新输入：";
    }

    //2.输入密码
    cout<<"请输入密码（小于30位字符）：";
    while (cin>>password)
    {
        if(password.size()<=30)
        {
            break;
        }
        cout<<"密码过长，请重新输入：";
    }

    //3.输入姓名
    cout<<"请输入姓名：";
    while (cin>>name)
    {
        if(name.size()<=30)
        {
            break;
        }
        cout<<"姓名过长，请重新输入：";
    }

    //4.输入电话号码
    cout<<"请输入电话号码（小于20位数字）：";
    while (cin>>tel_num)
    {
        if(tel_num.size()<=20)
        {
            break;
        }
        cout<<"电话号码过长，请重新输入：";
    }

    //5.输入性别
    cout<<"请输入性别（1.男/2.女/3.未知）:";
    int genderChoice;
    while (cin>>genderChoice)
    {
        if(genderChoice==1)
        {
            gender="男";
            break;
        }
        else if(genderChoice==2)
        {
            gender="女";
            break;
        }
        else if(genderChoice==3)
        {
            gender="未知";
            break;
        }
        else
        {
            cout<<"输入错误，请重新输入：";
        }
    }

    //6.输入生日
    string year,month,day;
    cout<<"请输入生日（格式：yyyy-mm-dd）"<<endl;
    {
        cout<<"输入你的生日年份：";
        while (cin>>year)
        {
            if(year.size()==4)
            {
                break;
            }
            cout<<"输入错误，请重新输入：";
        }

        cout<<"输入你的生日月份：";
        while (cin>>month)
        {
            if(month.size()==2)
            {
                break;
            }
            cout<<"输入错误，请重新输入：";
        }

        cout<<"输入你的生日日期：";
        while (cin>>day)
        {
            if(day.size()==2)
            {
                break;
            }
            cout<<"输入错误，请重新输入：";
        }

        birthday=year+"-"+month+"-"+day;

    }

    string sql = "insert into passenger(name,gender,birthday,tel_num,account,password) values('"+
                 name + "','" + gender + "','" + birthday + "','" + tel_num + "','" + account + "','" + password + "')";
    if(!gs->dml(sql))
    {
        return false;//sql执行失败
    }
    else
    {
        cout<<"注册成功"<<endl;
        this->account=account;
        this->password=password;
        return true;//注册成功
    }
}