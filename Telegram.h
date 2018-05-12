//
// Created by root on 16.04.18.
//

#ifndef JOBBOT_TELEGRAM_H
#define JOBBOT_TELEGRAM_H


#include <iostream>


struct Message
{
    std::string chat_id;
    std::string text;
    std::string first_name;

    Message(){

    }
    ~Message(){

    }
};



void send_message(string message, string chat_id){
    std::string result=std::string("https://api.telegram.org/bot524193239:AAEn0Ovfo7jX3QXIYH19kIHlfNHEx1EBr2Y/sendMessage?chat_id=")+chat_id+std::string("&text=")+message;
    std::cout << result;
    CURL * curl_handle = curl_easy_init();
    if(curl_handle)
    {
        curl_easy_setopt(curl_handle, CURLOPT_URL, result.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_PROXY,"https://147.135.210.114:54566");
        //curl_easy_setopt(curl_handle, CURLOPT_VERBOSE,1);
        curl_easy_perform(curl_handle);
        curl_easy_cleanup(curl_handle);
    }
}

#endif //JOBBOT_TELEGRAM_H