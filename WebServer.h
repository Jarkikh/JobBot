#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "Telegram.h"
#include "JsonParse.h"
#include "DB.h"

bool done;


class WebServer
{
public:
    WebServer() {done=false; thr=/*new */boost::thread(boost::bind(WebServer::server));}
    void stop() {done=true; this->~WebServer(); }
    ~WebServer() { thr.join(); /*delete thr;*/}
private:
    static void server();
    static void data_handling(boost::asio::ip::tcp::socket* sock);
    static std::string parse_text_and_db(Message*);
    static std::string extract_json(char* data);
    static unsigned short const port=15578;
    static int const max_length=4096;
    boost::thread thr;
};

std::string WebServer::parse_text_and_db(Message* mes)
{
    std::string message=mes->first_name+string("%2c ");
    if(mes->text.substr(0,3)=="add")
    {
        unsigned long begin=mes->text.find(':',0);
        ++begin;
        unsigned long end = begin;
        unsigned long prend = end;
        while((end=mes->text.find(',',begin),prend)<=begin)
        {
            message+='\'';
            std::string tag;
            tag=mes->text.substr(begin,end-begin);
            prend=end;
            begin=end+1;
            add_tag(mes->chat_id,tag);
            message+=(tag+std::string("' added %0A"));
        }
        return message;
    }
    else if(mes->text.substr(0,6)=="remove")
    {
        unsigned long begin=mes->text.find(':',0);
        ++begin;
        unsigned long end = begin;
        unsigned long prend = end;
        while((end=mes->text.find(',',begin),prend)<=begin)
        {
            message+='\'';
            std::string tag;
            tag=mes->text.substr(begin,end-begin);
            prend=end;
            begin=end+1;
            delete_tag(mes->chat_id,tag);
            message+=(tag+std::string("' removed %0A"));
        }
        return message;
    }
    else if(mes->text.substr(0,6)=="unsubs")
    {
        delete_all_tags(mes->chat_id);
        return message+std::string("you successfully unsubscribed!");
    }
    else if(mes->text.substr(0,4)=="list")
    {
        return message+std::string("your tags are%3a ")+get_tags_by_subscriber(mes->chat_id);
    }
    else if(mes->text.substr(0,4)=="help")
    {
        return std::string("Usage:%0Aadd:%20tag1,%20tag2...%20-%20to%20add%20you%20in%20subscriber's%20list%20on%20these%20tags%0Aremove:%20tag1,%20tag2...%20-%20to%20remove%20you%20from%20subscriber's%20list%20on%20these%20tags%0Aunsubscribe%20-%20to%20remove%20you%20from%20subscriber's%20list%0Alist%20-%20to%20list%20your%20tags");
    }
    else
        return message+std::string("we cannot recognize your message%0ATry \"help\"");
}

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
        std::string name=mes->first_name;
        std::string message=parse_text_and_db(mes);
        send_message(message,ch_id);
        delete mes;
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