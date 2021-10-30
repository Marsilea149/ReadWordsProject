#ifndef WORDPROCESS_HPP
#define WORDPROCESS_HPP
#include <vector>

//TODO use Forward declaration to remove this include
#include "../include/Word.hpp"



//TODO try to put as param remove from global
//TODO convert  s_wordsArray into std::map, so that it owns the count with it
//global variables
// vector used to store the list of words entered by user
// variable global only can be accessed here
static std::vector<Word *> s_wordsArray;

// //TODO add forward declaration to remove includes from hpp
// #include <memory>
// #include <mutex>

// // variable used protect shared data from being simultaneously accessed by multiple threads
// std::mutex wordLock;

/**Previously named "workerThread"
 * Check if the word is already entered, if yes, only update the count of word. 
 * If the word has not been encountered, add the word to "word list" (s_wordsArray), and update the count of word.
 * Terminates when the word 'end' is encountered.
 */
void processWord(Word *word);


/**Read input from STDIN, build the word object, pass word object to processWord()
 * Terminates when the word "end" is encountered
 */ 
void readInputWords();

void lookupWords();

#endif