#include <iostream>
#include "cstring"
#include "VK.h"
#include "Telegram.h"
#include "JsonParse.h"

using namespace std;
int main(){

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

        return 0;
    }
}
