#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <cstring>
#include <mutex>

//maximum number of characters that a word can have
#define WORD_MAX_SIZE 32

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
  //set the mutex for the duration of a scoped block
  std::lock_guard<std::mutex> lock(wordLock);

  bool endEncountered = false;
  bool found = false;

  //remove the duplicates
  while (!endEncountered)
  {
    //check if there is a new word
    if (s_word.data[0])
    {
      // Create a new Word object with the input data
      // ??? do I need a delete w later?
      Word *w = new Word(s_word.data);
      ++w->count;
      //printf("workerThread local w: %s, %i \n", w->data, w->count);

      // Check if the word "end" is encountered
      endEncountered = std::strcmp(s_word.data, "end") == 0;
      // Inform the producer that we consumed the word
      s_word.data[0] = 0;

      if (!endEncountered)
      {
        // Do not insert duplicate words
        for (auto p : s_wordsArray)
        {
          if (!std::strcmp(p->data, w->data))
          {
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
}

// Read input words from STDIN and pass them to the worker thread for
// inclusion in the word list.
// Terminate when the word 'end' has been entered.
//
void readInputWords()
{
  bool endEncountered = false;

  //TODO is it better to use object instead of pointer here?
  std::thread *worker = new std::thread(workerThread);

  // buffer to store input word fro, user
  char *linebuf = new char[WORD_MAX_SIZE];

  while (!endEncountered)
  {
    std::cout << "Please enter a word: " << std::endl;

    // inputSuccess is used to check the validity of the word user entered
    int inputSuccess = std::scanf("%s", linebuf);
    // inputSize is used to store the length of the word that user entered
    unsigned int inputSize = (unsigned)strlen(linebuf);
    // Manage input word length, when it is too long, asks the user to enter a shorter word
    while (inputSize > (WORD_MAX_SIZE - 1))
    {
      std::cout << "You are allowed to enter up to " << WORD_MAX_SIZE - 1 << "(included) charcters. You entered: "
                << inputSize << " characters. Please enter a shorter word: " << std::endl;
      inputSuccess = std::scanf("%s", linebuf);
      inputSize = (unsigned)strlen(linebuf);

      // if end of file is detected return
      if (inputSuccess == EOF) //eof
        return;
    }

    // check if the word "end" has been encountered
    endEncountered = std::strcmp(linebuf, "end") == 0;

    //copy linebuf data into s_word.data
    //TODO check if my_strdup returns NULL manage
    s_word.data = my_strdup(linebuf);
  }

  // Wait for the worker to terminate
  worker->join();

  //do I need a destructor for workerThread ???
}

// Repeatedly ask the user for a word and check whether it was present in the word list
// Terminate on EOF
//

// TODO: add param by reference "s-total found" to remove global variable
void lookupWords()
{
  bool found = false;
  char *linebuf = new char[WORD_MAX_SIZE];

  //TODO: not sure about the ending conditions
  //TODO: do I need to introduce filestream to be able to use EOF ?
  //std::scanf("%s", linebuf) != EOF
  while (1)
  {
    std::cout << "Enter a word for lookup: " << std::endl;

    // TODO: print out linebuf, TODO try /0
    // ctrl+D to test EOF
    if (std::scanf("%s", linebuf) == EOF)
      return;

    //TODO: replace w with local object instead of new
    // Initialize the word to search for
    Word *w = new Word(linebuf);

    // Search for the word
    for (unsigned int i = 0; i < s_wordsArray.size(); ++i)
    {
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
  //TODO understand try{} catch{}
  try
  {
    // Read input words from user entry via STDIN
    readInputWords();

    // Sort the words alphabetically
    //TODO: strcmp to remplace compare words
    std::sort(s_wordsArray.begin(), s_wordsArray.end(), compareWords);

    // Print the word list
    std::printf("\n=== Word list:\n");
    for (auto p : s_wordsArray)
      std::printf("%s %d\n", p->data, p->count);

    lookupWords();

    printf("\n=== Total words found: %d\n", s_totalFound);

    //TODO: do I need to add throw to select the characters I can enter
    //TODO: add throw to check the length of the string entered
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