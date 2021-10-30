#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <cstring>
#include <mutex>

#include "../include/Word.hpp"

// Note: static keyword: variable global only can be accessed here

//TODO try to put as param remove from global
// vector used to store the list of words entered by user
static std::vector<Word> s_wordsArray;

// current word
Word s_word;

// variable used protect shared data from being simultaneously accessed by multiple threads
std::mutex wordLock;

// true when end of file is encountered, false else
bool eofEncountered;

/**Worker thread: consume words passed from the main thread and insert them
 * in the 'word list' (s_wordsArray), while removing duplicates. Terminate when
 * the word 'end' is encountered.
 */
void workerThread()
{
  //set the mutex for the duration of a scoped block
  std::lock_guard<std::mutex> lock(wordLock);

  bool endEncountered = false;
  //remove the duplicates
  while (!endEncountered)
  {
    if (eofEncountered)
      break;

    //check if there is a new word
    if (s_word.getData()[0])
    {
      // Create a new Word object with the input data
      Word w(s_word.getData());
      w.incrementCount();

      // Check if the word "end" is encountered
      endEncountered = std::strcmp(s_word.getData(), "end") == 0;

      // Inform the producer that we consumed the word
      char *empty = (char *)"";
      s_word.setData(empty, 1);

      if (!endEncountered)
      {
        bool found = false;
        // Do not insert duplicate words
        for (auto p : s_wordsArray)
        {
          if (!std::strcmp(p.getData(), w.getData()))
          {
            p.incrementCount();
            found = true;
            break;
          }
        }
        if (!found)
          s_wordsArray.push_back(w);
      }
    }
  }
}

/**Read input words from STDIN and pass them to the worker thread for inclusion in the word list.
 * Terminate when the word 'end' has been entered. If there is no 'end' before EOF, an exception is thrown.
 * When user entered an word too long, print out a message to ask enter another word.
 */
void readInputWords()
{
  bool endEncountered = false;
  eofEncountered = false;
  // create a worker thread to process the word that the user entered
  // Note: I prefer to use the object here, because pointer with 'new' may cause memory issue
  std::thread worker = std::thread(workerThread);

  // buffer to store input word from user
  // Note: replaced char * with smart pointer so that linebuf get deleted automatically once it is not used anymore
  std::unique_ptr<char[]> linebuf(new char[WORD_MAX_SIZE]);

  while (!endEncountered)
  {
    std::cout << "Please enter a word: " << std::endl;
    // inputSuccess is used to check the validity of the word user entered
    int inputSuccess = std::scanf("%s", linebuf.get());

    // if end of file is detected throw an exception
    if (inputSuccess == EOF)
    {
      // Note: eofEncountered is introduced to informe worker thread to stop working as user input error happens
      eofEncountered = true;
      // Note: for thread safety, we have to make sure that worker thread has completely finished working, so that the thread object becomes non-joinable and can be destroyed safely.
      worker.join();
      throw(std::invalid_argument("EOF encountered without having 'end' word in user input. Quitting the program."));
    }

    // inputSize is used to store the length of the word that user entered
    unsigned int inputSize = (unsigned)strlen(linebuf.get());
    // Manage input word length, when it is too long, asks the user to enter a shorter word
    while (inputSize > (WORD_MAX_SIZE - 1))
    {
      std::cout << "You are allowed to enter up to " << WORD_MAX_SIZE - 1 << "(included) charcters. You entered: "
                << inputSize << " characters. Please enter a shorter word: " << std::endl;
      inputSuccess = std::scanf("%s", linebuf.get());
      inputSize = (unsigned)strlen(linebuf.get());

      // if end of file is detected return
      if (inputSuccess == EOF)
        return;
    }
    // check if the word "end" has been encountered
    endEncountered = std::strcmp(linebuf.get(), "end") == 0;

    //copy linebuf data into s_word.data
    s_word.setData(linebuf.get(), inputSize);

    while (s_word.getData()[0])
      ;
  }
  // Wait for the worker to terminate
  worker.join();
}

/**Repeatedly ask the user for a word and check whether it was present in the word list
 * erminate on EOF
 * @param[out] totalFound: total number of words found during lookupWords
 */
// TODO: add param by reference "s-total found" to remove global variable
void lookupWords(unsigned int &totalFound)
{
  // Note: smart pointer is chosen to better manage memory
  std::unique_ptr<char[]> linebuf(new char[WORD_MAX_SIZE]);

  while (1)
  {
    std::cout << "Enter a word for lookup: " << std::endl;

    // Note: please enter ctrl+D to test EOF in linux
    if (std::scanf("%s", linebuf.get()) == EOF)
      return;

    // Note: I chose to change point with 'new' into object to avoid memory issue
    // Initialize the word to search for
    Word w = Word(linebuf.get());

    bool found = false;
    // Search for the word
    for (unsigned int i = 0; i < s_wordsArray.size(); ++i)
    {
      if (std::strcmp(s_wordsArray[i].getData(), w.getData()) == 0)
      {
        found = true;
        ++totalFound;
        std::printf("SUCCESS: '%s' was present %d times in the initial word list\n",
                    s_wordsArray[i].getData(), s_wordsArray[i].getCount());
        break;
      }
    }

    if (!found)
    {
      std::printf("'%s' was NOT found in the initial word list\n", w.getData());
    }
  }
}

/**This method compares two Words alphabetically
 * @param[in] first: the first word to compare
 * @param[in] second: the second word to compare 
 * @return true if [first, second] is in alphabetical order, false else
 */
bool compareWords(Word const &first, Word const &second)
{
  std::string firstStr = first.getData();
  std::string secondStr = second.getData();
  //TODO try strcmp, put all upper case => indicate that in readme
  //minimum size between the two strings
  unsigned int minStringSize = std::min(firstStr.size(), secondStr.size());
  for (unsigned int i = 0; i < minStringSize; ++i)
  {
    if (firstStr[i] < secondStr[i])
      return true;
    else if (firstStr[i] > secondStr[i])
      return false;
  }
  return false;
}

int main()
{
  try
  {
    // Read input words from user entry via STDIN
    readInputWords();

    // Sort the words alphabetically
    std::sort(s_wordsArray.begin(), s_wordsArray.end(), compareWords);

    // Print the word list
    std::printf("\n=== Word list:\n");
    for (auto p : s_wordsArray)
      std::printf("%s %d\n", p.getData(), p.getCount());

    // lookup words and update 'totalFoundWords'
    unsigned int totalFoundWords = 0;
    lookupWords(totalFoundWords);
    printf("\n=== Total words found: %d\n", totalFoundWords);
  }
  catch (std::exception &e)
  {
    std::printf("error %s\n", e.what());
  }

  return 0;
}