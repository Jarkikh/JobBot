//
// Created by root on 16.04.18.
//

#include <iostream>
#include <cstring>
#include <curl/curl.h>
#include "VK-master/src/api.h"
#include "VK-master/src/attachment.h"
#include "VK-master/src/json.hpp"
#include "VK-master/src/messages.h"
#include "VK-master/src/messages.cpp"
#include "VK-master/src/attachment.cpp"
#include "VK-master/src/api.cpp"

using namespace std;
struct Post{
public:
    string text;

    Post(string text){
        this->text = text;
    }
    Post(){
        this->text = "null";
    }
};

string* to_lower(string* str){

}

static size_t write_data(char *ptr, size_t size, size_t nmemb, string* data) {
    if (data) {
        data->append(ptr, size*nmemb);
        return size*nmemb;
    }
    else return 0;  // будет ошибка
}


string get_post(const string &group_id, const string& offset){
/*

       VK::Client api;
       string login = "+79161807262";
       string pass = "LeshaGrisha";
       string access_token = "0ea566f70ea566f70ea566f7870ec73ee400ea50ea566f7546b512c23a26dc290ca23ef";
       if(api.auth(login, pass, access_token)){
           cout << "Auth ok" << endl;
           cout << "Access token: " << api.access_token() << endl;
           cout << api.call("wall.get", "owner_id=-"+group_id+"&offset="+offset+"&count=1");
       }
       else{
           cout << "Auth fail" << endl;
       }
*/

    CURL *curl_handle;
    curl_handle = curl_easy_init();

    std::string content;

    if (curl_handle)
    {
        //string access_token = "0ea566f70ea566f70ea566f7870ec73ee400ea50ea566f7546b512c23a26dc290ca23ef";
        string url="https://api.vk.com/method/wall.get?v=5.52&access_token=0ea566f70ea566f70ea566f7870ec73ee400ea50ea566f7546b512c23a26dc290ca23ef&owner_id=-"+group_id+"&offset="+offset+"&count=1";
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &content);
        //curl_easy_setopt(curl_handle, CURLOPT_VERBOSE,1);

        CURLcode res = curl_easy_perform(curl_handle);
        //std::cout << content << std::endl;

        curl_easy_cleanup(curl_handle);
    }
    return content;
}