//
// Created by root on 16.04.18.
//

#ifndef JOBBOT_JSONPARSE_H
#define JOBBOT_JSONPARSE_H
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "VK.h"
#include "Telegram.h"

using namespace std;
using namespace rapidjson;


Message* parse_json_from_telegram(string json){
    Message *message = new Message();

    return message;
}


Post* parse_json_from_vk(string json) {
    Post *post = new Post();
   
    return post;
}

#endif