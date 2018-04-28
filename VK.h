//
// Created by root on 16.04.18.
//

#ifndef JOBBOT_VK_H
#define JOBBOT_VK_H

#include <iostream>
#include <cstring>
#include <curl/curl.h>

using namespace std;
struct Post{
public:
    string text;

    Post(){

    }
    ~Post(){

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


string get_post(){
    /*VK::Client api;
    if(api.auth(login, pass, access_token)){
        cout << "Auth ok" << endl;
        cout << "Access token: " << api.access_token() << endl << endl; //Нужно так, так как токен дается на 24 часа.
        cout << api.call("wall.post", "owner_id=12345&message=Test");
    }
    else{
        cout << "Auth fail" << endl;
    }*/

    CURL *curl_handle;
    curl_handle = curl_easy_init();

    std::string content;

    if (curl_handle)
    {
        //string access_token = "0ea566f70ea566f70ea566f7870ec73ee400ea50ea566f7546b512c23a26dc290ca23ef";
        curl_easy_setopt(curl_handle, CURLOPT_URL, "https://api.vk.com/method/wall.get?v=5.52&access_token=0ea566f70ea566f70ea566f7870ec73ee400ea50ea566f7546b512c23a26dc290ca23ef&owner_id=-34116496&offset=2&count=1");

        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &content);

        CURLcode res = curl_easy_perform(curl_handle);
        //std::cout << content << std::endl;

        curl_easy_cleanup(curl_handle);
    }
    return content;
}


#endif //JOBBOT_VK_H

