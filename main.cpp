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

using namespace rapidjson;
using namespace std;

struct Client{
public:
    string chat_id;
    string *words;
};


bool suitable(string post, string words, int size){
    // итерируемся по post в поисках подстроки words[i]
    bool result = false;
    if (post.find(words) != string::npos) result = true;


    /* bool result = false;
     for (int i = 0; i < size; ++i) {
         if (post.find(words[i]) != string::npos) result = true;
     }*/
     return result;
}


void find_siutable(Post *post){
//цикл по всем записям в базе и вызов suitable. Если true - send_message
}


int main() {

    Message *message;
    string json = "{""\"update_id\":654794788,""\"message\":""{""\"message_id\":29,""\"from\":""{""\"id\":462194591,""\"is_bot\":false,""\"first_name\":\"Alex\",""\"username\":\"Sighr\",""\"language_code\":\"ru\"""},""\"chat\":""{""\"id\":462194591,""\"first_name\":\"Alex\",""\"username\":\"Sighr\",""\"type\":\"private\"""},""\"date\":1523365267,""\"text\":\"МЕНЕДЖЕР\"""}""}";
    message = parse_json_from_telegram(json);

    //1. тут идет добавление в базу данных от Леши

    cout << "id чата = " << message->chat_id<< endl << "Ключевое слово: " << message->text << endl;
    cout << endl;
    cout << endl;
    cout << endl;

    //2. Тут идет вытаскиваение записи из группы вконтакте.

    string json_post = get_post();

    //cout << json_post << endl;

    Post* post = parse_json_from_vk(json_post);

    cout << post->text << endl;

    //3. Прогон всех пользователей из базы по записи. При соответветствии - отсылка записи пользователю.

   /* string json_post_prev = get_post(); //  Тело приложения
    while (true) {
        string json_post = get_post();

        if (new_post != post_prev) {
            Post *post = parse_json_from_vk(json_post);
            find_siutable(post);
        } else cout << "No update" << endl;

        json_post_prev = json_post;

        sleep(1800);  //4. Ждем пол часа - достаем еще запись и сравниваем ее с предыдущей. если != то п 3.

    }*/


    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    if (suitable(post->text,message->text,3)) cout << "Подходящая запись!";
    else cout << "Не подходит";



    return 0;
}