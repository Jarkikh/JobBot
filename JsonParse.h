//
// Created by root on 16.04.18.
//
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "VK.h"
#include "Telegram.h"

using namespace std;
using namespace rapidjson;

std::string GetElementValue(Value& val)
{
    if (val.GetType() == Type::kNumberType)
        return std::to_string(val.GetInt());
    else if (val.GetType() == Type::kStringType)
        return val.GetString();
    return "Unknown";
}


Message* parse_json_from_telegram(string json){
    Message *message = new Message();
    Document document;
    document.Parse(json.c_str());

    for (Value::MemberIterator m = document["message"].MemberBegin(); m != document["message"].MemberEnd(); ++m) {
        string current = m->name.GetString();
        string text = "text";
        string chat = "chat";
        if (current == text) {
            message->text = GetElementValue(m->value);
        }
        else if (current == chat){
            for (Value::MemberIterator m1 = m->value.MemberBegin(); m1 != m->value.MemberEnd(); ++m1) {
                string tmp1 = m1->name.GetString();
                string id = "id";
                if (tmp1 == id) message->chat_id = GetElementValue(m1->value);
            }
        }
    }
    return message;
}


Post* parse_json_from_vk(string json) {
    Post *post = new Post();
    Document document;
    document.Parse(json.c_str());

    const Value& a = document["response"];

    const Value& b = a["items"];

    for (SizeType i = 0; i < b.Size(); i++) {
        const Value& c = b[i];

        post->text = c["text"].GetString();
    }

    return post;
}

