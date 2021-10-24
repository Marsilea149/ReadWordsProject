#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <vector>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <mutex>

struct Word
{
  char *data;
  int count;

  Word(char *data_) : // ::strdup() is C not C++. Note that we need to manually free data afterwards.
                      data(::strdup(data_)),
                      count(0)
  {
  }

  Word() : data((char *)""),
           count(0)
  {
  }
};

//global variables
std::vector<Word *> s_wordsArray;
Word s_word;

// variable used protect shared data from being simultaneously accessed by multiple threads
std::mutex wordLock;

/**This method allows to allocate properly destination memory first,
 * then make a copy of string.
 * @param[in] input: pointer poiting to the first char of the input string, to be copied
 * @return pointer poiting to the first char of the copy of the input string
 */
char *my_strdup(char *input)
{
  // We need strlen(src) + 1, since we have to account for '\0'
  int len = strlen(input) + 1;
  char *output = (char *)malloc((len + 1) * sizeof(char));
  if (output == NULL)
    return NULL;
  output = (char *)memcpy(output, input, len);
  return output;
}

// Worker thread: consume words passed from the main thread and insert them
// in the 'word list' (s_wordsArray), while removing duplicates. Terminate when
// the word 'end' is encountered.
void workerThread()
{
  std::cout << "***********start the worker thread***********" << std::endl;
  //std::lock_guard The class lock_guard is a mutex wrapper that provides a convenient RAII-style mechanism for owning a mutex for the duration of a scoped block.
  std::lock_guard<std::mutex> lock(wordLock);

  bool endEncountered = false;
  bool found = false;

  //remove the duplicates
  while (!endEncountered)
  {
    if (s_word.data[0]) // Do we have a new word?
    {

      printf("workerThread 1: %s\n", s_word.data);
      // Create a new Word object with the input data
      Word *w = new Word(s_word.data);
      ++w->count;
      printf("workerThread local w: %s, %i \n", w->data, w->count);

      endEncountered = std::strcmp(s_word.data, "end") == 0;
      std::cout << "==endEncountered==" << endEncountered << std::endl;
      s_word.data[0] = 0; // Inform the producer that we consumed the word

      if (!endEncountered)
      {
        // Do not insert duplicate words
        for (auto p : s_wordsArray)
        {
          if (!std::strcmp(p->data, w->data))
          {
            std::cout << "both string same: " << p->data << std::endl;
            ++p->count;
            found = true;
            break;
          }
        }

        if (!found)
          s_wordsArray.push_back(w);
      }

      printf("workerThread Output: %s\n", s_word.data);
    }
  }

  std::cout << "***********end the worker thread***********" << std::endl;
}

// Read input words from STDIN and pass them to the worker thread for
// inclusion in the word list.
// Terminate when the word 'end' has been entered.
//
void readInputWords()
{
  bool endEncountered = false;

  std::thread *worker = new std::thread(workerThread);

  char *linebuf = new char[32];

  while (!endEncountered)
  {
    std::cout << "-----readInput---" << std::endl;
    if (std::scanf("%s", linebuf) == EOF) //eof
      return;

    // for (unsigned int i = 0; i < 32; ++i)
    //   std::cout << i << ": " << *(linebuf + i) << std::endl;

    endEncountered = std::strcmp(linebuf, "end") == 0;

    // std::cout << "==endEncountered==" << endEncountered << std::endl;
    // Pass the word to the worker thread

    //copy linebuf data into s_word.data
    s_word.data = my_strdup(linebuf);
    // printf("Input: %s\n", linebuf);
  }

  // Wait for the worker to terminate
  worker->join();

  //do I need a destructor for workerThread ???
}

int main()
{

  /*******readInputWords()*****/
  readInputWords();

  // Print the word list
  std::printf("\n=== Word list:\n");
  for (auto p : s_wordsArray)
    std::printf("%s %d\n", p->data, p->count);

  // Free the memory address returned using malloc()
  free(s_word.data);

  return 0;
}