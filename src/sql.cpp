#include "../include/sql.h"
using namespace std;

SQL::SQL() // 初始化opengauss实例
{
    conninfo = "";
    conn = nullptr;
    res = nullptr;
    nFields = 0;
    row = 0;
    this->aliveTime = chrono::steady_clock::now();
}

SQL::~SQL() // 释放资源
{
    if (this->res != nullptr) // 释放查询结果
    {
        PQclear(this->res);
    }
    if (this->conn != nullptr) // 释放连接实例
    {
        PQfinish(this->conn);
    }
}

/**
 * @brief 连接函数
 *
 * @param host 主机地址
 * @param port 端口号
 * @param dbname 数据库名
 * @param user 用户名
 * @param password 用户密码
 * @return true
 * @return false
 */
bool SQL::connect(string host, string port, string dbname, string user, string password)
{
    this->conninfo = "host=" + host + " port=" + port + " dbname=" + dbname + " user=" + user + " password=" + password; // 对外部传入的连接信息进行拼接
    this->conn = PQconnectdb(this->conninfo.c_str());                                                                    // 连接数据库
    if (PQstatus(this->conn) != CONNECTION_OK)                                                                           // 判断连接是否成功
    {
        cerr << "连接失败： " << PQerrorMessage(this->conn) << endl;
        return false;
    }
    return true;
}

bool SQL::dml(string sql) // 增删改
{
    this->res = PQexec(this->conn, sql.c_str());
    if (PQresultStatus(this->res) != PGRES_COMMAND_OK)
    {
        cout << "执行失败" << endl;
        return false;
    }
    return true;
}

bool SQL::dql(string sql) // 查询
{
    if (this->res != nullptr) // 提前释放上一次查询的结果
    {
        PQclear(this->res);
    }
    this->res = PQexec(this->conn, sql.c_str());
    if (PQresultStatus(this->res) != PGRES_TUPLES_OK)
    {
        cout << "查询失败" << endl;
        cerr << "Query failed: " << PQerrorMessage(this->conn) << endl;
        PQclear(this->res);
        return false;
    }
    this->nFields = PQnfields(this->res); // 保存查询的相关信息
    this->row = PQntuples(this->res);
    return true;
}

// void SQL::show()
// {
//     if (this->res == nullptr)
//     {
//         cout << "查询结果为空" << endl;
//         return ;
//     }
//     for (int i = 0; i < this->nFields; i++)
//     {
//         cout << left << setw(10) << PQfname(this->res, i);
//     }
//     cout << endl;
//     for (int i = 0; i < this->row; i++)
//     {
//         for (int j = 0; j < this->nFields; j++)
//         {
//             cout << left << setw(10) << PQgetvalue(this->res, i, j); // 获取查询结果
//         }
//     }
//     cout << endl;
// }

void SQL::show()
{
    if (this->res == nullptr)
    {
        cout << "查询结果为空" << endl;
        return;
    }

    // 计算每列的最大宽度
    vector<int> colWidths(this->nFields, 0);
    for (int i = 0; i < this->nFields; i++)
    {
        colWidths[i] = max(colWidths[i], (int)strlen(PQfname(this->res, i)));
    }
    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < this->nFields; j++)
        {
            colWidths[j] = max(colWidths[j], (int)strlen(PQgetvalue(this->res, i, j)));
        }
    }

    // 打印列名
    for (int i = 0; i < this->nFields; i++)
    {
        cout << left << setw(colWidths[i] + 2) << PQfname(this->res, i);
    }
    cout << endl;

    // 打印行数据
    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < this->nFields; j++)
        {
            cout << left << setw(colWidths[j] + 2) << PQgetvalue(this->res, i, j); // 获取查询结果
        }
        cout << endl;
    }
}

bool SQL::isResEmpty() // 判断查询结果是否为空
{
    return PQntuples(this->res) == 0;
}

void SQL::refreshTime() // 刷新连接存活时间，用于连接池
{
    this->aliveTime = chrono::steady_clock::now();
}

long long SQL::getAliveTime() // 获取连接存活时间，用于连接池
{
    chrono::nanoseconds ns = chrono::steady_clock::now() - this->aliveTime;
    chrono::milliseconds result = chrono::duration_cast<chrono::milliseconds>(ns);
    return result.count();
}