#ifndef WORD_HPP
#define WORD_HPP

//TODO: use forward declaration then move it into cpp
#include <memory>

//maximum number of characters that a word can have
#define WORD_MAX_SIZE 32

/**This class represents a word, with the count of the word
 * The maximum size of word is 32. 
 */
class Word
{
public:
    /**Constructor with a chain of characters
     * @param[in] c_string: pointer pointing to the beginning of the input character chain
     */
    Word(char *c_string);

    /**Constructor without input
     * By defaul, data is initialized to empty
     * and count is initialized to 0
     */
    Word();

    /**Destructor
     */
    ~Word();

    /**Get the word data
     * @return: a smart pointer pointing to word data
     * (Here I chose to use smart pointer so that the memory get managed correctly when data is not used anymore)
     */
    //TODO: change return type into string maybe?
    // std::unique_ptr<char[]> getData() const;

    char *getData() const;

    // // Allocate a buffer to store contents of the file.
    // std::unique_ptr<char[]> buf(new char[size]);

    /**Verify if new data is available 
     * @return true if the word has new data, else return false
     * */
    bool isNewDataAvailable() const;

    /**Increment the number of times this word appears by one
     */
    void incrementCount();

    /**Check if word is equal to "end"
     * @return true if word equals to "end", false else
     */
    bool isEnd() const;

    /**Check if word has been consumed
     * @return true if the word has been consumed, false else
     */
    bool isConsumed() const;

    /**Set word as consumed, update the sz_ to 0 and the count to 0
     * 
     */ 
    void setConsumed();
    
    /**Set data with an input smart pointer, pointing to a char array
     * 
     */
    void setData(std::unique_ptr<char[]> &inputPtr);

    /**
     * 
     * 
     */
    void print() const;

    /**README TODO: by putting data and count into private section, we use encapsulation to protect 
         * these data from being accessed from another class
         */
private:
    //pointer pointing to word data
    //std::unique_ptr<char[]> data;
    char *data;

    size_t sz_;

    //number of times the word appears
    int count;

    // /**This method allows to allocate properly destination memory first,
    //  * then make a copy of string.
    //  * @param[in] input: pointer poiting to the first char of the input string, to be copied
    //  * @return pointer poiting to the first char of the copy of the input string
    //  */
    // char *my_strdup(char *input);
};

#endif