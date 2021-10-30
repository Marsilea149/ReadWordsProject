
#include <thread>
#include <memory>
#include <cstring>

#include <iostream>

#include "../include/WordProcess.hpp"
// #include "../include/Word.hpp"

// void processWord(Word word)
// {
//     std::cout << "---processWord---" << std::endl;
// }

void processWord(Word *word)
{
    std::cout << "---processWord start---" << std::endl;

    bool endEncountered = false;
    bool found = false;

    //remove the duplicates
    while (!endEncountered)
    {
        //check if there is a new word
        if (word->isNewDataAvailable())
        {
            std::cout << "there is a new word" << std::endl;
            //word->print();

            // Check if the word "end" is encountered
            endEncountered = word->isEnd();
            std::cout << "---1---" << endEncountered << std::endl;

            std::cout << "---2---" << word->isConsumed() << std::endl;
            if (!endEncountered)
            {

                // Do not insert duplicate words
                for (auto p : s_wordsArray)
                {
                    std::cout << "---3---" << p->isNewDataAvailable() << std::endl;
                    if (true /*!std::strcmp(p->getData(), word->getData())*/)
                    {
                        std::cout << "---4---" << word->getData() << std::endl;
                        p->incrementCount();
                        found = true;
                        // Update that we consumed the word
                        word->setConsumed();
                        break;
                    }
                }

                if (!found)
                    s_wordsArray.push_back(word);
            }
        }
    }
    std::cout << "---processWord end---" << std::endl;
}

void readInputWords()
{
    std::cout << "$$$readInputWords begin$$$" << std::endl;
    bool endEncountered = false;

    Word word;
    std::cout << "$$$" << word.isConsumed() << "$$$" << std::endl;

    std::thread worker(&processWord, &word);

    std::cout << "$$$-------3--------$$$" << std::endl;
    //Temporary pointer used to store user input
    //README: smart pointer is used to better manage memory, when data is not used anymore
    //char *linebuf = new char[WORD_MAX_SIZE];
    std::unique_ptr<char[]> linebuf = std::make_unique<char[]>(WORD_MAX_SIZE);
    std::cout << "$$$-------4--------$$$" << std::endl;
    while (!endEncountered)
    {
        std::cout << "$$$-------5--------$$$" << std::endl;
        if (std::scanf("%s", linebuf.get()) == EOF) //eof
            return;
        std::cout << "$$$-------6--------$$$" << std::endl;
        //----seems redundant ----- not sure
        endEncountered = std::strcmp(linebuf.get(), "end") == 0;
        std::cout << "$$$-------7--------$$$" << std::endl;
        //     //BWU: Segmentation fault, because the destination memory is not properly allocated
        //     // Pass the word to the worker thread
        //     std::strcpy(s_word.data, linebuf);
        word.setData(linebuf);
        std::cout << "$$$-------8--------$$$" << std::endl;
        //??? seems useless
        // Wait for the worker thread to consume it
        while (word.isConsumed())
            ;
        std::cout << "$$$-------9--------$$$" << std::endl;
    }

    worker.join(); // Wait for the worker to terminate
    std::cout << "$$$-------10--------$$$" << std::endl;
}
