#include <iostream>
#include <curl/curl.h>
#include <unistd.h>
#include <algorithm>
#include "cstring"
#include "VK.h"
#include "Telegram.h"
#include "JsonParse.h"
#include "WebServer.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "DB.h"

using namespace rapidjson;
using namespace std;

struct Client{
public:
    string chat_id;
    string *words;
};


bool suitable(string &post, string &words){
    // итерируемся по post в поисках подстроки words[i]
    bool result = false;
    if (post.find(words) != string::npos) result = true;

    return result;
}


void find_siutable(Post *post){
//цикл по всем записям в базе и вызов suitable. Если true - send_message
    Selection* selection = get_all_subscribers();
    for (int i = 0; i < selection->size; ++i) {
        for (int j = 0; j < selection->subs->sz; ++j) {
            if (suitable(post->text,selection->subs[i].tags[j])) {
                send_message(post->text,selection->subs[i].chat_id);
                break;
            }
        }
    }
/*
    string **subscr = get_all_subscribers();
    int size = get_size();
    for (int i = 0; i < size; ++i) {
        string chat_id = subscr[i][0];
        for(std::string::iterator it = subscr[i][1].begin(); it < subscr[i][1].end(); it++){
            string one_word = "";
            while (*it!=' '){
                one_word.push_back(*it);
                it++;
            }
            if (suitable(post->text,one_word)) {
                send_message(post->text,chat_id);
                return;
            }
        }
    }*/

}


int main() {
    WebServer ser;
    //3. Прогон всех пользователей из базы по записи. При соответветствии - отсылка записи пользователю.

    string json_post_prev = get_post(); //  Тело приложения
    Post *post_prev = parse_json_from_vk(json_post_prev);
    while (true) {
        string json_post_new = get_post();
        Post *new_post = parse_json_from_vk(json_post_new);

        //cout << post_prev->text << endl;

        if (new_post->text != post_prev->text) {
            find_siutable(new_post);
        }// else cout << "No update" << endl;

        post_prev = new_post;

        sleep(30);  //4. Ждем пол часа - достаем еще запись и сравниваем ее с предыдущей. если != то п 3.

    }

    return 0;
}
