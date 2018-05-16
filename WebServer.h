#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/algorithm/string.hpp>
#include "Telegram.h"
#include "JsonParse.h"
#include "DB.h"

bool done;

class Message_Handler
{
public:
    std::string operator()(Message* mes);
private:
    std::string add(Message* mes);
    std::string remove(Message* mes);
    std::string unsubscribe(Message* mes);
    std::string show_all(Message* mes);
    std::string help();
    std::string do_not_recognize();
};

std::string Message_Handler::operator()(Message* mes)
{
    std::string result=mes->first_name+string(", ");
    boost::algorithm::to_lower(mes->text);
    if(mes->text.substr(0,10)=="добав")
        result+=this->add(mes);
    else if(mes->text.substr(0,8)=="удал")
        result+=this->remove(mes);
    else if(mes->text.substr(0,10)=="отпис")
        result+=this->unsubscribe(mes);
    else if(mes->text.substr(0,12)=="список")
        result+=this->show_all(mes);
    else if(mes->text.substr(0,6)=="епт")
        result=this->help();
    else
        result+=this->do_not_recognize();
    delete mes;
    result=encode_url(result);
    return result;
}
std::string Message_Handler::add(Message *mes)
{
    std::string message;
    unsigned long begin=mes->text.find(':',0);
    ++begin;
    unsigned long end = begin;
    unsigned long prend = end;
    while((end=mes->text.find(',',begin),prend)<=begin)
    {
        std::string tag;
        if(mes->text[begin]==' ')
            ++begin;
        tag=mes->text.substr(begin,end-begin);
        prend=end;
        begin=end+1;
        if(!add_tags(mes->chat_id,encode_url(tag)+','))
            message+="вы уже добавили тег '"+tag+"' \n";
        else
        {
            message+='\'';
            message+=(tag+std::string("' — тег добавлен \n"));
        }
    }
    return message;
}

std::string Message_Handler::remove(Message *mes)
{
    std::string message;
    unsigned long begin=mes->text.find(':',0);
    ++begin;
    unsigned long end = begin;
    unsigned long prend = end;
    while((end=mes->text.find(',',begin),prend)<=begin)
    {
        std::string tag;
        if(mes->text[begin]==' ')
            ++begin;
        tag=mes->text.substr(begin,end-begin);
        prend=end;
        begin=end+1;
        if(!delete_tag(mes->chat_id,encode_url(tag)))
            message+="у вас нет тега '"+tag+"' \n";
        else
        {
            message+='\'';
            message+=(tag+std::string("' — тег удалён \n"));
        }
    }
    return message;
}

std::string Message_Handler::unsubscribe(Message *mes)
{
    std::string message;
    delete_all_tags(mes->chat_id);
    return message+std::string("вы успешно отписались!");
}

std::string Message_Handler::show_all(Message *mes)
{
    std::string message;
    std::string tagggs=get_tags_by_subscriber(mes->chat_id);
    tagggs.pop_back();
    tagggs=decode_url(tagggs);
    return message+std::string("ваши теги: ")+tagggs;
}

std::string Message_Handler::help()
{
    return std::string("Для того, чтобы добавить теги, используйте \"добавить: тег1, тег2 ...\". \n Для удаления тега используйте команду \"Удалить\". \n Для отображения действующих тегов \"Список\". \n Для того, чтобы отписаться от рассылки \"Отписаться\". ");
}

std::string Message_Handler::do_not_recognize()
{
    return std::string("ваше сообщение не было распознано нашими алгоритмами.\n Попробуйте 'епт'");
}

class WebServer
{
public:
    WebServer(WebServer&)=delete;
    WebServer(WebServer&&)=delete;
    WebServer& operator=(WebServer&)=delete;
    WebServer& operator=(WebServer&&)=delete;
    void stop() {done=true; instance=nullptr; this->~WebServer();}
    ~WebServer() { thr.join(); /*delete thr;*/}
    static WebServer* get_instance()
    {
        if(instance==nullptr)
            instance=new WebServer();
        return instance;
    }
private:
    WebServer() {done=false; thr=/*new */boost::thread(boost::bind(WebServer::server));}
    static WebServer* instance;
    static void server();
    static void data_handling(boost::asio::ip::tcp::socket* sock);
    static std::string extract_json(char* data);
    static unsigned short const port=15578;
    static int const max_length=4096;
    boost::thread thr;
};

WebServer* WebServer::instance = nullptr;

std::string WebServer::extract_json(char* data)
{
    std::string a=data;
    unsigned long pos_b_num=a.find("Content-Length: "); //16sym length
    unsigned long pos_e_num=a.find("\r\n",pos_b_num);
    pos_b_num+=16;
    std::string number=a.substr(pos_b_num,pos_e_num-pos_b_num);
    unsigned long num=atoi(number.c_str());
    if(num>3000)
        std::__throw_logic_error("too long msg");
    unsigned long pos=a.find('{');
    std::string json=a.substr(pos,num);
    return json;
}

void WebServer::data_handling(boost::asio::ip::tcp::socket* sock)
{
    try
    {
        char data[max_length];
        boost::system::error_code error;
        size_t length = sock->read_some(boost::asio::buffer(data), error);
        sock->send(boost::asio::buffer("HTTP/1.1 200 OK\r\nContent-length:0\r\n\r\n"));
        sock->close();
        delete sock;
        if (error == boost::asio::error::eof)
            ;//do something
        else if (error)
            throw boost::system::system_error(error);
        std::string json=extract_json(data);
        std::cout << json << std::endl;
        Message* mes=parse_json_from_telegram(json);
        std::string ch_id=mes->chat_id;
        Message_Handler handle;
        std::string message=handle(mes);
        send_message(message,ch_id);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
    //insert into the database
}

void WebServer::server()
{
    boost::asio::io_service io;
    boost::asio::ip::tcp::acceptor a(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port));
    while(!done)
    {
        auto *sock=new boost::asio::ip::tcp::socket(io);
        a.accept(*sock);
        if(!done)
        {
            boost::thread t(boost::bind(WebServer::data_handling, sock));
            t.detach();
        }
        else
        {
            sock->send(boost::asio::buffer("HTTP/1.1 200 OK\r\nContent-length:0\r\n\r\n"));
            //
            delete sock;
        }
    }
}

#endif