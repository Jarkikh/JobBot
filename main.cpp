#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
//#include "src/api.h"
#include <iostream>
#include <cstring>
#include <curl/curl.h>
#include <unistd.h>
#include <algorithm>
#include "JsonParse.h"
#include "DB.h"
#include "WebServer.h"
#include <vector>

using namespace rapidjson;
using namespace std;

bool suitable(string &post, vector<string*> &words){
    // итерируемся по post в поисках подстроки words[i]
    int coincidence_count = 0;
    int num = 0;
    unsigned long size =  words.size();
    double coefficient = 0.5;
    // bool result = false;

    while (num < size){
        if (post.find(*(words[num])) != string::npos) coincidence_count++;
        num++;
    }

    return coincidence_count >= size*coefficient;
}


void find_suitable(Post *post){
//цикл по всем записям в базе и вызов suitable. Если true - send_message
    string **subscr = get_all_subscribers();
    int size = get_size();
    vector<string*> all_words;
    all_words.reserve(10);
    string post_lower=boost::algorithm::to_lower_copy(post->text);
    for (int i = 0; i < size; ++i)
    {
        string chat_id = subscr[i][0];
        subscr[i][1]=decode_url(subscr[i][1]);
        for (std::string::iterator it = subscr[i][1].begin(); it < subscr[i][1].end(); it++)
        {
            string *one_word = new string();
            while (*it != ',')
            {
                one_word->push_back(*it);
                it++;
            }
            all_words.push_back(one_word);

        }
        if (suitable(post_lower, all_words))
        {
            post->text=encode_url(post->text);
            send_message(post->text, chat_id);
            return;
        }
    }
    for(int i=0;i<size;++i)
        delete[] subscr[i];
    delete[] subscr;
}

void stop_func()
{
    while(!done)
        cin >> done;
}


int main() {

    //3. Прогон всех пользователей из базы по записи. При соответветствии - отсылка записи пользователю.
    done=false;
    WebServer* ser=WebServer::get_instance();
    boost::thread stopper(boost::bind(stop_func));
    int count = 10; //количество возможных групп

    string offset = "1";

    string id_1 = "165952932";
    string id_2 = "34116496";

    vector<string> groups;
    groups.reserve(count);
    groups.push_back(id_1);
    groups.push_back(id_2);

    vector<Post*> previous_post;
    previous_post.resize(count);

    for (int i = 0; i < previous_post.size(); ++i) {
        previous_post[i] = new Post();
    }



    // string json_post_prev = get_post(id); //  Тело приложения
    //auto post_prev = new Post();

    while (!done) {
        for (int i = 0; i < groups.size(); ++i) {
            string json_post_new = get_post(groups[i],offset);

            Post *new_post = parse_json_from_vk(json_post_new);
            //cout << previous_post[i]->text << endl;
            new_post->text=decode_url(new_post->text);

            if (new_post->text != previous_post[i]->text) {
                find_suitable(new_post);
            } else cout << "No update" << endl;

            previous_post[i]->text = new_post->text;

            delete(new_post);
        }

        sleep(1800);  //4. Ждем пол часа - достаем еще запись и сравниваем ее с предыдущей. если != то п 3.

    }
    //while(!done);
    ser->stop();
    return 0;
}