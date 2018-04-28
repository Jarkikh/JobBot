//
// Created by root on 16.04.18.
//

#include <iostream>

using namespace std;

struct Message{
    string chat_id;
    string text;

    Message(){

    }
    ~Message(){

    }
};

void send_message(string message, string chat_id){
    // используем curl
}