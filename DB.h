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




/* Создание подключения с БД */
std::unique_ptr <sql::Connection> create_connection()
{

    /* Create a connection */
    sql::Driver* driver = get_driver_instance();
    std::unique_ptr <sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "admin", "123456789"));
    /* Connect to the MySQL database */
    con->setSchema("adminbot");

    return std::move(con);

}

/* Нахождение строки по PK
 * в mySQL также создана хранимая процедура,
 * выполняющая SELECT по таблице */
std::string get_tags_by_subscriber(const std::string& chat_id)
{
    std::string query;

    std::unique_ptr <sql::Connection> con = create_connection();
    std::unique_ptr <sql::Statement> stmt(con->createStatement());

    query = "CALL getWords( " + chat_id + ", @pop)";

    stmt->execute(query);
    std::unique_ptr <sql::ResultSet> res(stmt->executeQuery("SELECT @pop AS _reply"));

    res->next();
    con->close();
    return res->getString("_reply");
}

bool add_tags(const std::string& chat_id, const std::string& tag)
{
    if(get_tags_by_subscriber(chat_id).find(tag)!=std::string::npos)
        return false;
    std::unique_ptr <sql::Connection> con = create_connection();
    std::unique_ptr <sql::Statement> stmt(con->createStatement());

    std::string query;

    query = "CALL addRow("+chat_id+",'" + tag + "' )";
    stmt->execute(query);
    con->close();
    return true;
}

void delete_all_tags(const std::string& chat_id)
{
    std::unique_ptr <sql::Connection> con = create_connection();
    std::unique_ptr <sql::Statement> stmt(con->createStatement());

    std::string query;

    query = "CALL deleteRow(" + chat_id + ")";
    stmt->execute(query);
    con->close();
}

bool delete_tag(const std::string& chat_id,const std::string& tag)
{
    std::string previous=get_tags_by_subscriber(chat_id);
    unsigned long pos=0;
    if((pos=previous.find(tag))==std::string::npos)
        return false;
    std::string updated=previous.substr(0,pos)+previous.substr(pos+tag.length()+1);
    //std::cout << updated << std::endl;
    delete_all_tags(chat_id);
    add_tags(chat_id,updated);
    return true;
}


/* Функция возвращает двумерный массив стрингов с данными */
std::string** get_all_subscribers() {
    std::unique_ptr<sql::Connection> con = create_connection();
    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    int key = 0;

    stmt->execute("CALL get_data()");
    std::unique_ptr<sql::ResultSet> res;
    auto** result = new std::string*[255];
    for (int i = 0; i < 255; i++)
        result[i] = new std::string [2];

    do {
        res.reset(stmt->getResultSet());
        while (res->next()) {
//            std::cout << "chat: " << (std::string) res->getString("chatID") << " key: "
//                      << (std::string) res->getString("keyWords")
//                      << std::endl;
            result[key][1] = (std::string) res->getString("keyWords");
            result[key][0] = (std::string) res->getString("chatid");
            key++;
        }
    } while (stmt->getMoreResults());

    con->close();
    return result;
}

/* Функция возврящает количество строк в таблице
 * используется хранимая процедура БД get_size() */
int get_size() {
    std::unique_ptr<sql::Connection> con = create_connection();
    std::unique_ptr<sql::Statement> stmt(con->createStatement());

    stmt->execute("CALL get_size()");

    std::unique_ptr<sql::ResultSet> res(stmt->getResultSet());

    res->next();
    con->close();
    return res->getInt("count(*)");
}



#endif