#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <cstring>
#include <mutex>

//maximum number of characters that a word can have
#define WORD_MAX_SIZE 32

//CHANGELOG: convert Word into a class so that the data and count can be encapsulated. Avoid them from accessing by others uninetentionally
class Word
{

public:
  Word(char *data_) : // ::strdup() is C not C++. Note that we need to manually free data afterwards.
                      data(::strdup(data_)),
                      //We need to add 1 for "/0" to indicate the end of the char array
                      size((unsigned)strlen(data_) + 1),
                      count(0)
  {
  }

  Word() : data((char *)""),
            //1 for "/0"
           size(1),
           count(0)
  {
  }

  //CHANGELOG: add destructor so that the object can be properly destroyed: it allows to release memory before the class instance is destroyed. This must be done to avoid memory leak.

  //TODO: why I don't need to delete data???
  ~Word()
  {
    /*delete[] data;*/
    //CHANGELOOG: The standard library function strdup calls malloc(). The memory cleanup should be done using free().
    if (data)
      free(data);
  }

  void incrementCount()
  {
    ++count;
  }

  unsigned int getCount()
  {
    return count;
  }

  char *data;


private:
  unsigned int count;

  //CHANGELOG: add size so that we can handle data easily
  // size of data
  unsigned int size;
};

//TODO try to put as param remove from global
//global variables
// vector used to store the list of words entered by user
// variable global only can be accessed here
static std::vector<Word> s_wordsArray;
// current word
Word s_word;

//TODO remove it from global
// total number of words found during the research of the user
int s_totalFound;

// variable used protect shared data from being simultaneously accessed by multiple threads
std::mutex wordLock;

bool eofEncountered;

/**This method allows to allocate properly destination memory first,
 * then make a copy of string.
 * @param[in] input: pointer poiting to the first char of the input string, to be copied
 * @return pointer poiting to the first char of the copy of the input string
 */
char *my_strdup(char *input)
{
  //TODO try to use std::unique_ptr here
  // std::unique_ptr<unsigned char[]> output(new unsigned char[len]);
  //     std::copy_n(input.c_str(), input.length() + 1, output.get());

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
  std::cout << "=====================================workerThread begin========================" << std::endl;
  //set the mutex for the duration of a scoped block
  std::lock_guard<std::mutex> lock(wordLock);

  bool endEncountered = false;
  bool found = false;

  //remove the duplicates
  while (!endEncountered)
  {
    //TODO: Manage EOF without "end" having been entered
    if (eofEncountered)
      break;

    //check if there is a new word
    if (s_word.data[0])
    {
      std::cout << "===2===" << std::endl;
      // Create a new Word object with the input data
      // ??? do I need a delete w later?
      Word w(s_word.data);
      w.incrementCount();
      //printf("workerThread local w: %s, %i \n", w->data, w->count);

      // Check if the word "end" is encountered
      endEncountered = std::strcmp(s_word.data, "end") == 0;
      // Inform the producer that we consumed the word
      s_word.data[0] = 0;

      if (!endEncountered)
      {
        std::cout << "===3===" << std::endl;
        // Do not insert duplicate words
        for (auto p : s_wordsArray)
        {
          std::cout << "===4===" << std::endl;
          if (!std::strcmp(p.data, w.data))
          {
            p.incrementCount();
            found = true;
            break;
          }
        }
        std::cout << "===5===" << std::endl;
        if (!found)
          s_wordsArray.push_back(w);
      }
    }
  }
  std::cout << "=====================================workerThread end========================" << std::endl;
}

// Read input words from STDIN and pass them to the worker thread for
// inclusion in the word list.
// Terminate when the word 'end' has been entered.
//
void readInputWords()
{
  std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~readInputWords begin~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
  bool endEncountered = false;
  eofEncountered = false;
  //TODO is it better to use object instead of pointer here?
  std::thread worker = std::thread(workerThread);

  // buffer to store input word fro, user
  //char *linebuf = new char[WORD_MAX_SIZE];
  // CHANGELOG: replaced char * with smart pointer so that linebuf get deleted automatically once it is not used anymore
  std::unique_ptr<char[]> linebuf(new char[WORD_MAX_SIZE]);

  std::cout << "~~~1~~~" << std::endl;
  while (!endEncountered)
  {
    std::cout << "Please enter a word: " << std::endl;
    std::cout << "~~~2~~~" << std::endl;
    // inputSuccess is used to check the validity of the word user entered
    int inputSuccess = std::scanf("%s", linebuf.get());

    // if end of file is detected return / throw an exception TODO update comment
    if (inputSuccess == EOF)
    {
      //CHANGELOG: eofEncountered is introduced to informe worker thread to stop working as user input error happens
      eofEncountered = true;
      //CHANGELOG: for thread safety, we have to make sure that worker thread has completely finished working, so that the thread object becomes non-joinable and can be destroyed safely.
      worker.join();
      //TODO is it better to use return + a std::cout or use throw?
      throw(std::invalid_argument("EOF encountered without having 'end' word in user input. Quitting the program."));
      // return;
    }

    // inputSize is used to store the length of the word that user entered
    unsigned int inputSize = (unsigned)strlen(linebuf.get());
    // Manage input word length, when it is too long, asks the user to enter a shorter word
    while (inputSize > (WORD_MAX_SIZE - 1))
    {
      std::cout << "~~~3~~~" << std::endl;
      std::cout << "You are allowed to enter up to " << WORD_MAX_SIZE - 1 << "(included) charcters. You entered: "
                << inputSize << " characters. Please enter a shorter word: " << std::endl;
      inputSuccess = std::scanf("%s", linebuf.get());
      inputSize = (unsigned)strlen(linebuf.get());

      // if end of file is detected return
      if (inputSuccess == EOF) //eof
        return;
    }
    std::cout << "~~~4~~~" << std::endl;
    // check if the word "end" has been encountered
    endEncountered = std::strcmp(linebuf.get(), "end") == 0;

    //copy linebuf data into s_word.data
    //TODO check if my_strdup returns NULL manage
    s_word.data = my_strdup(linebuf.get());
  }
  std::cout << "~~~5~~~" << std::endl;
  // Wait for the worker to terminate
  worker.join();
  std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~readInputWords end~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
  //do I need a destructor for workerThread ???
}

// Repeatedly ask the user for a word and check whether it was present in the word list
// Terminate on EOF
//

// TODO: add param by reference "s-total found" to remove global variable
void lookupWords()
{
  bool found = false;
  std::unique_ptr<char[]> linebuf(new char[WORD_MAX_SIZE]);

  //TODO: not sure about the ending conditions
  //TODO: do I need to introduce filestream to be able to use EOF ?
  //std::scanf("%s", linebuf) != EOF
  while (1)
  {
    std::cout << "Enter a word for lookup: " << std::endl;

    // TODO: print out linebuf, TODO try /0
    // ctrl+D to test EOF
    if (std::scanf("%s", linebuf.get()) == EOF)
      return;

    //TODO: replace w with local object instead of new
    // Initialize the word to search for
    Word w = Word(linebuf.get());

    // Search for the word
    for (unsigned int i = 0; i < s_wordsArray.size(); ++i)
    {
      if (std::strcmp(s_wordsArray[i].data, w.data) == 0)
      {
        std::printf("SUCCESS: '%s' was present %d times in the initial word list\n",
                    s_wordsArray[i].data, s_wordsArray[i].getCount());
        found = true;
        ++s_totalFound;
        break;
      }
    }
    if (!found)
    {
      std::printf("'%s' was NOT found in the initial word list\n", w.data);
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
  std::string firstStr = first.data;
  std::string secondStr = second.data;
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
      std::printf("%s %d\n", p.data, p.getCount());

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
  // free(s_word.data);

  //TODO: not sure if necessary
  s_wordsArray.clear();
  s_wordsArray.shrink_to_fit();
  std::cout << "The vector size is " << s_wordsArray.size() << ", and its "
            << "capacity is " << s_wordsArray.capacity() << std::endl;
  return 0;
}