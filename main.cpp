#include <iostream>
#include <thread>
#include <vector>
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
int s_totalFound;

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

      //printf("workerThread 1: %s\n", s_word.data);
      // Create a new Word object with the input data
      Word *w = new Word(s_word.data);
      ++w->count;
      //printf("workerThread local w: %s, %i \n", w->data, w->count);

      endEncountered = std::strcmp(s_word.data, "end") == 0;
      //std::cout << "==endEncountered==" << endEncountered << std::endl;
      s_word.data[0] = 0; // Inform the producer that we consumed the word

      if (!endEncountered)
      {
        // Do not insert duplicate words
        for (auto p : s_wordsArray)
        {
          if (!std::strcmp(p->data, w->data))
          {
            //std::cout << "both string same: " << p->data << std::endl;
            ++p->count;
            found = true;
            break;
          }
        }

        if (!found)
          s_wordsArray.push_back(w);
      }

      //printf("workerThread Output: %s\n", s_word.data);
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
  std::cout << "***********start readInputWords***********" << std::endl;
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
  std::cout << "***********end readInputWords***********" << std::endl;
}

// Repeatedly ask the user for a word and check whether it was present in the word list
// Terminate on EOF
//
void lookupWords()
{
  std::cout << "***********start lookupWords***********" << std::endl;
  bool found = false;
  char *linebuf = new char[32];

  //TODO: not sure about the ending conditions
  //TODO: do I need to introduce filestream to be able to use EOF ?
  //std::scanf("%s", linebuf) != EOF
  while (1)
  {
    std::cout << "Enter a word for lookup: " << std::endl;

    if (std::scanf("%s", linebuf) == EOF)
      return;

    //TODO: replace w with local object instead of new
    // Initialize the word to search for
    Word *w = new Word(linebuf);

    // Search for the word
    for (unsigned int i = 0; i < s_wordsArray.size(); ++i)
    {
      //std::cout << "---s_wordsArray[i]->data---" << s_wordsArray[i]->data << std::endl;
      //std::cout << "---w->data---" << w->data << std::endl;
      if (std::strcmp(s_wordsArray[i]->data, w->data) == 0)
      {
        std::printf("SUCCESS: '%s' was present %d times in the initial word list\n",
                    s_wordsArray[i]->data, s_wordsArray[i]->count);
        found = true;
        ++s_totalFound;
        break;
      }
    }
    if (!found)
    {
      std::printf("'%s' was NOT found in the initial word list\n", w->data);
    }

    delete w;
  }
  delete linebuf;

  std::cout << "***********end lookupWords***********" << std::endl;
}

/**This method compares two Words alphabetically
 * @param[in] first: pointer pointing to the beginning of first Word 
 * @param[in] second: pointer pointing to the beginning of second Word 
 * @return true if [first, second] is in alphabetical order, false else
 */
bool compareWords(Word *first, Word *second)
{
  std::string firstStr = first->data;
  std::string secondStr = second->data;
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
  //TODO understand try{} catch{}
  try
  {
    /*******readInputWords()*****/
    readInputWords();

    // Sort the words alphabetically
    std::sort(s_wordsArray.begin(), s_wordsArray.end(), compareWords);

    // Print the word list
    std::printf("\n=== Word list:\n");
    for (auto p : s_wordsArray)
      std::printf("%s %d\n", p->data, p->count);

    lookupWords();

    printf("\n=== Total words found: %d\n", s_totalFound);
  }
  catch (std::exception &e)
  {
    std::printf("error %s\n", e.what());
  }
  // Free the memory address returned using malloc()
  free(s_word.data);

  //TODO: not sure if necessary
  s_wordsArray.clear();
  s_wordsArray.shrink_to_fit();
  std::cout << "The vector size is " << s_wordsArray.size() << ", and its "
            << "capacity is " << s_wordsArray.capacity() << std::endl;
  return 0;
}