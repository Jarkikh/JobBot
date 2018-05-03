#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "src/api.h"
#include <iostream>
#include <cstring>
#include <curl/curl.h>
#include <unistd.h>
#include <algorithm>
#include "JsonParse.h"
#include "DB.h"
#include "WebServer.h"

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
        for (int j = 0; j < selection->subs->size; ++j) {
            if (suitable(post->text,selection->subs[i].tags[j])) {
                send_message(post->text,selection->subs[i].chat_id);
                break;
            }
        }
    }
}


int main() {

    //3. Прогон всех пользователей из базы по записи. При соответветствии - отсылка записи пользователю.
    WebServer ser;

    string json_post_prev = get_post(); //  Тело приложения
    Post *post_prev = parse_json_from_vk(json_post_prev);
    while (true) {
        string json_post_new = get_post();
        Post *new_post = parse_json_from_vk(json_post_new);

        cout << post_prev->text << endl;

        if (new_post->text != post_prev->text) {
            find_siutable(new_post);
        } else cout << "No update" << endl;

        post_prev = new_post;

        sleep(1800);  //4. Ждем пол часа - достаем еще запись и сравниваем ее с предыдущей. если != то п 3.

    }

    return 0;
}