#include <iostream>
#include <vector>
#include <mutex>

#include "../include/WordDic.hpp"
#include "../include/Word.hpp"

//TODO try to put as param remove from global
//global variables
// vector used to store the list of words entered by user
// variable global only can be accessed here
static std::vector<Word *> s_wordsArray;
// current word
Word s_word;

//TODO remove it from global
// total number of words found during the research of the user
int s_totalFound;

// variable used protect shared data from being simultaneously accessed by multiple threads
std::mutex wordLock;


int main() {
    std::cout << "Step1 - Read Input Words Please enter a word:" << std::endl;

}


// // Worker thread: consume words passed from the main thread and insert them
// // in the 'word list' (s_wordsArray), while removing duplicates. Terminate when
// // the word 'end' is encountered.
// void workerThread()
// {
//   //set the mutex for the duration of a scoped block
//   std::lock_guard<std::mutex> lock(wordLock);

//   //make endEncountered global
//   bool endEncountered = false;
//   bool found = false;

//   //remove the duplicates
//   while (!endEncountered)
//   {
//     //check if there is a new word
//     if (s_word.data[0])
//     {
//       // Create a new Word object with the input data
//       // ??? do I need a delete w later?
//       Word *w = new Word(s_word.data);
//       ++w->count;
//       //printf("workerThread local w: %s, %i \n", w->data, w->count);

//       // Check if the word "end" is encountered
//       endEncountered = std::strcmp(s_word.data, "end") == 0;
//       // Inform the producer that we consumed the word
//       s_word.data[0] = 0;

//       if (!endEncountered)
//       {
//         // Do not insert duplicate words
//         for (auto p : s_wordsArray)
//         {
//           if (!std::strcmp(p->data, w->data))
//           {
//             ++p->count;
//             found = true;
//             break;
//           }
//         }

//         if (!found)
//           s_wordsArray.push_back(w);
//       }
//       //printf("workerThread Output: %s\n", s_word.data);
//     }
//   }
// }