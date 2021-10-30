#ifndef WORD_HPP
#define WORD_HPP

//maximum number of characters that a word can have
#define WORD_MAX_SIZE 32

/**This class represents a word, with its 'count', which is defined as the number of times the word has been entered
 * The maximum size of word is defined by WORD_MAX_SIZE. When the user input exceeds the WORD_MAX_SIZE, an error is sent. 
 */
class Word
{

public:
  /**Constructor with a character array
   * @param[in] data_: pointer pointing to the beginning of the input character array
   */
  Word(char *data_);

  /**Constructor without input
   * By defaul, data is initialized to empty
   * and count is initialized to 0
   */
  Word();

  /**Destructor
   */
  ~Word();

  /**Increment the number of times this word appears by one
   */
  void incrementCount();

  /**Return count as an unsigned int
   */ 
  unsigned int getCount();

  /**Method used to free the memory of 's_word.data' first, then copy the data,
   * finally add the end terminator so that there is no memory leak
   * @param[in] inputPtr: pointer pointing to the first character of the input array
   * @param[in] inputSize: the size of the input array
   */
  void setData(char const *inputPtr, unsigned int const inputSize);

  /**Get the word data
   * @return: a char pointer pointing to the beginning of word data
   * (FutureWork: Here I will chose to use smart pointer so that the memory get managed correctly when data is not used anymore)
   */
  char *getData() const;

private:
  // pointer pointing to the start of the word array
  char *data;

  //number of times the word entered by user
  unsigned int count;

  // size of data
  unsigned int size;
};

#endif