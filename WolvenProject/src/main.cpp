#include <iostream>
#include <vector>

#include "../include/WordProcess.hpp"
#include "../include/Word.hpp"

// current word
Word s_word;

//TODO remove it from global
// total number of words found during the research of the user
int s_totalFound;

int main()
{
    try
    {
        readInputWords();

        // // Sort the words alphabetically
        // std::sort(s_wordsArray.begin(), s_wordsArray.end());

        // Print the word list
        std::printf("\n=== Word list:\n");
        for (auto p : s_wordsArray)
            std::printf("%s\n", p->getData());

        // lookupWords();

        printf("\n=== Total words found: %d\n", s_totalFound);
    }
    catch (std::exception &e)
    {
        std::printf("error %s\n", e.what());
    }

    return 0;
}