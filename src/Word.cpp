#include "../include/Word.hpp"
#include <cstring>
#include <stdlib.h>

Word::Word(char *data_) : count(0)
{
    size = strlen(data_) + 1;
    char *output = (char *)malloc((size + 1) * sizeof(char));
    if (output == NULL)
        data = nullptr;
    else
        data = (char *)memcpy(output, data_, size);
}

Word::Word() : data((char *)""),
               //1 for "/0"
               size(1),
               count(0)
{
}

Word::~Word()
{
    if (data)
        free(data);
}

void Word::incrementCount()
{
    ++count;
}

unsigned int Word::getCount()
{
    return count;
}

void Word::setData(char const *inputPtr, unsigned int const inputSize)
{
    free(data);
    data = (char *)malloc(inputSize + 1);
    strcpy(data, inputPtr);
    data[inputSize] = '\0';
}

char *Word::getData() const
{
    return data;
}
