#include "../include/Word.hpp"
#include <string.h>
#include <cstring>

Word::Word(char *c_string) : count(0)
{
    //TODO use my_strdup

    sz_ = strlen(c_string) + 1;
    //data = std::make_unique<char[]>(sz_);
    //strcpy(data.get(), c_string);
    strcpy(data, c_string);
}

Word::Word() : data{nullptr},
               sz_{0},
               count(0)
{
}

Word::~Word()
{
}

// std::unique_ptr<char[]> Word::getData() const
// {
//     std::unique_ptr<char[]> =
//     return data;
// }



bool Word::isNewDataAvailabel() const
{
    if (data != nullptr)
        return false;
    else
        return true;
}

void Word::incrementCount()
{
    ++count;
}

bool Word::isEnd() const
{
    return (std::strcmp(data, "end") == 0);
}

bool Word::isConsumed() const
{
    return (data[0]==0);
}