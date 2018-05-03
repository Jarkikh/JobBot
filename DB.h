#ifndef DB_H
#define DB_H

#include <iostream>
#include <string>

using namespace std;

struct Tags
{
    string chat_id;
    int sz;
    string* tags;
    Tags(){sz=0;}
    ~Tags(){}
};

struct Selection
{
    int size;
    Tags* subs;
    Selection(){size=0;subs=nullptr;}
    ~Selection(){}
};

Selection DB;

void add_tag(const string &chat_id,const string &tag)
{
    int i=0,j=0;
    for(i=0;i<DB.size;++i)
    {
        if(DB.subs[i].chat_id==chat_id)
        {
            for(j=0;j<DB.subs[i].sz;++j)
            {
                if(DB.subs[i].tags[j]==tag)
                {
                    return;
                }
            }
            break;
        }
    }
    if(i==DB.size)
    {
        Tags* new_subs = new Tags[++(DB.size)];
        for(int k=0;k<DB.size-1;++k)
        {
            new_subs[k]=DB.subs[k];
        }
        new_subs[DB.size-1].chat_id=chat_id;
        new_subs[DB.size-1].sz=1;
        new_subs[DB.size-1].tags=new string[new_subs[DB.size-1].sz];
        new_subs[DB.size-1].tags[0]=tag;
        if(DB.subs!=nullptr)
            delete[] DB.subs;
        DB.subs=new_subs;
        return;
    }
    if(j==DB.subs[i].sz)
    {
        ++(DB.subs[i].sz);
        string *new_tags = new string[DB.subs[i].sz];
        for (int k = 0; k < DB.subs[i].sz-1; ++k)
        {
            new_tags[k] = DB.subs[i].tags[k];
        }
        new_tags[DB.subs[i].sz-1]=tag;
        delete[] DB.subs[i].tags;
        DB.subs[i].tags = new_tags;

        return;
    }
}

void delete_tag(const string &chat_id,const string &tag)
{

}

void delete_all_tags(const string &chat_id)
{

}

string get_tags_by_subscriber(const string &chat_id)
{
    return string("nil");
}

Selection* get_all_subscribers()
{
    return &DB;
}

#endif