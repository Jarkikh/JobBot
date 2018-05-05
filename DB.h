#ifndef DB_H
#define DB_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


using namespace std;


/* Создание подключения с БД */
std::auto_ptr <sql::Connection> create_connection()
{

    /* Create a connection */
    sql::Driver* driver = get_driver_instance();
    std::auto_ptr <sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "adminbot", "botjobdb1234"));
    /* Connect to the MySQL database */
    con->setSchema("botjob");

    return std::move(con);

}

/* Нахождение строки по PK
 * в mySQL также создана хранимая процедура,
 * выполняющая SELECT по таблице */
std::string find_row(int chatID)
{

    std::string query;
    std::string id = std::to_string(chatID);

    std::auto_ptr <sql::Connection> con = create_connection();
    std::auto_ptr <sql::Statement> stmt(con->createStatement());

    query = "CALL getWords( " + id + ", @pop)";

    stmt->execute(query);
    std::auto_ptr <sql::ResultSet> res(stmt->executeQuery("SELECT @pop AS _reply"));

    res->next();
    con->close();
    return res->getString("_reply");
}

void add_tag(int chatID, const std::string& data)
{
    std::auto_ptr <sql::Connection> con = create_connection();
    std::auto_ptr <sql::Statement> stmt(con->createStatement());

    std::string query;

    query = "CALL addRow(NULL,'" + data + "' )";
    stmt->execute(query);
    con->close();

}

void delete_tag(string chat_id, string tag)
{

}

void delete_all_tags(int chatID)
{
    std::auto_ptr <sql::Connection> con = create_connection();
    std::auto_ptr <sql::Statement> stmt(con->createStatement());

    std::string query;
    std::string id = std::to_string(chatID);

    query = "CALL deleteRow(" + id + ")";
    stmt->execute(query);
    con->close();
}

string get_tags_by_subscriber(string chat_id)
{
    return string("nil");
}


/* Изменение строки в БД
 * На вход необходимо подавать РК - id чата, а также
 * данные, которые необходимо ДОБАВИТЬ к существующим */
void update_row(int chatID, const std::string& data)
{

    std::auto_ptr <sql::Connection> con = create_connection();
    std::auto_ptr <sql::Statement> stmt(con->createStatement());

    std::string query;
    std::string id = std::to_string(chatID);

    query = "CALL updateRow('" + id + "', '" + data + "')";
    stmt->execute(query);
    con->close();
}

string** get_all_subscribers() {
    std::auto_ptr<sql::Connection> con = create_connection();
    std::auto_ptr<sql::Statement> stmt(con->createStatement());
    int key = 0;

    stmt->execute("CALL get_data()");
    std::auto_ptr<sql::ResultSet> res;
    auto** result = new std::string*[255];
    for (int i = 0; i < 255; i++)
        result[i] = new std::string [255];

    do {
        res.reset(stmt->getResultSet());
        while (res->next()) {
//            std::cout << "chat: " << (std::string) res->getString("chatID") << " key: "
//                      << (std::string) res->getString("keyWords")
//                      << std::endl;
            result[key][0] = (std::string) res->getString("keyWords");
            key++;
        }
    } while (stmt->getMoreResults());

    con->close();
    return result;
}

/* Функция возврящает количество строк в таблице
 * используется хранимая процедура БД get_size() */
int get_size() {
    std::auto_ptr<sql::Connection> con = create_connection();
    std::auto_ptr<sql::Statement> stmt(con->createStatement());

    stmt->execute("CALL get_size()");

    std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());

    res->next();
    con->close();
    return res->getInt("count(*)");
}


#endif
