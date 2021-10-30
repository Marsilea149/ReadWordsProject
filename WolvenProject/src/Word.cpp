#include "../include/Word.hpp"
#include <string.h>
#include <cstring>
#include <iostream>

//TODO add a state machine for word : initializing, wordUpdating, wordConsuming

Word::Word(char *c_string) : count(0)
{
    //TODO use my_strdup

    //sz_ = strlen(c_string) + 1;
    //data = std::make_unique<char[]>(sz_);
    //strcpy(data.get(), c_string);
    //strcpy(data, c_string);

    sz_ = strlen(c_string) + 1;
    char *output = (char *)malloc((sz_ + 1) * sizeof(char));
    if (output == NULL)
        data = nullptr;
    else
        data = (char *)memcpy(output, c_string, sz_);
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
char *Word::getData() const
{
    //TODO nullptr management
    if(data != nullptr)
    {
        std::cout << "===========word is null" << std::endl;
        return data;
    }
    else
        return nullptr;
}

bool Word::isNewDataAvailable() const
{
    if (data == nullptr)
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
    if (data == nullptr)
        return false;
    else
        return (std::strcmp(data, "end") == 0);
}

bool Word::isConsumed() const
{
    if (data == nullptr)
        return false;
    else
        return (data[0] == 0);
}

void Word::setData(std::unique_ptr<char[]> &inputPtr)
{
    std::cout << "===setData===begin===" << std::endl;
    sz_ = strlen(inputPtr.get()) + 1;
    data = (char *)malloc((sz_ + 1) * sizeof(char));
    if (inputPtr == nullptr)
        data = nullptr;
    else
    {
        (char *)memcpy(data, inputPtr.get(), sz_);
        ++count;
    }

    print();
    std::cout << "===setData===end===" << std::endl;
}

void Word::setConsumed()
{
    data = nullptr;
    sz_ = 0;
    count = 0;
}

void Word::print() const
{
    std::cout << "**data:  " << data << std::endl;
    std::cout << "**sz:    " << sz_ << std::endl;
    std::cout << "**count: " << count << std::endl;
}

// char *Word::my_strdup(char *input)
// {
//     // We need strlen(src) + 1, since we have to account for '\0'
//     int len = strlen(input) + 1;
//     char *output = (char *)malloc((len + 1) * sizeof(char));
//     if (output == NULL)
//         return NULL;
//     output = (char *)memcpy(output, input, len);
//     return output;
// }