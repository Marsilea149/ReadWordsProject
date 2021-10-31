# Change Log
All notable changes to this project will be documented in this file.

## Saturday 2021-10-30
### Added
- Added Valgrind, program is running fine, but there is memory leak
- Added ReadMe
### Changed
- convert Word into a class, make count private, definitelyLost 118 bytes, indirectLost 21 bytes. Reason: improve data protection, make code more readable and easier to understand (see **Issue 8**)
- make data as private mamber for Word class, improved Word class comments (see **Issue 8**))
- Convert s_totalFound as param of function lookupWords() instead of global variable. Reason: global variable not necessary here.
- reorganize files Reason: ease the readability and make the code easier to be maintained
- cleaned main.cpp
### Fixed
- changed s_wordArray to contain Word objects instead of pointers, defLost 33 byte, 0 indLost Reason: There is still memory leak, I try to replace pointer objects to avoid memory leak. (see **Issue 7**)
- moved size into private in Word class, only defLost 13 bytes ((see **Issue 8**))
- fixed memory leak with setData improved (see **Issue 1**)
 
## Sunday 2021-10-24

### Added
- added check to managed user input length (see **Issue 5**)
- added debugging cout to make sure threads are running as expected
- added eofEncountered to manage the case where we have user input without 'end' word before the end of file (see **Issue 6**)
### Changed
- changed 32 as WORD_MAX_SIZE using define
### Fixed
- replaced char * with smart pointer in readInput() and lookupWords() Reason: linebuf get deleted automatically once it is not used anymore (see **Issue 7**)

## Saturday 2021-10-23
 
### Added
- added mutex to prevent others from accessing workerThread (see **Issue2**)
- added helper function to sort alphabetically the s_WorldArray (see **Issue3**)
- added delete for new variables to fix memory leak issue inside of lookupWords
- added comment with TODO to improve the code

### Changed
- cleaning the main cpp, removed useless cout and comments
 
### Fixed
- fixed memory issue when copying string using 'std::strcpy' in readInputWords(), not tested on Valgrind (see **Issue 1**)
- fixed word search segmentation fault, cleaned the for loop logic for search word (see **Issue 4**)


### Issue 1: Memory Leak
```std::strcpy``` is used without managing the memory of the parameters. Specially, the destination memory is not properly allocated with the correct size before calling this method. This causes memory leak. 
*Solution:* use ```free()``` to release destination memory, then use ```malloc``` to allocate the correct size of memory, finally call ```std::strcpy``` and set the terminator character. This is implemented in ```Word::SetData```.

### Issue 2: Thread Safety
The workerThread and the readInputWords are working simutaneously in two seperate threads. But they are using the same data ```s_word```. There is a risk of thread safety. 
*Solution:* I added a mutex for workerThread to ensure that shared data access is protected within the scope of workerThread.

### Issue 3: Unimplemented Code
In ```main```, the ```std::sort``` is not working, it needs a compare function to be able to sort the two input words alphabetically. 
*Solution:* I added a function ```bool compareWords(Word const &first, Word const &second)``` to compare two string alphabetically.
*FurtherProposition:* Maybe I can use ```strcmp``` in the implementation to optimize the code. 

### Issue 4: Wrong Logic
In ```lookupWords```, the ```Search for the word``` part has problem of logic. 
*Solution:* I changed its logic. 

### Issue 5: Input Management
In ```readInputWords```, the user input word length is not managed. User may enter more than 31 characters, thus exceeding allocated memory.  
*Solution:* I added a check to ask user enter another word in this case.

### Issue 6: Input Management
In ```readInputWords```, if the user inputs EOF, the program crashes brutally, generating ```core dumped```
*Solution:* We need to end the program properly, I added an invalid argument exception to manage this case

### Issue 7: Memory Leak
In both ```lookupWords``` and ```readInputWords```, ```new``` operator is used to dynamically allocate memory. But the pointer object has not been deleted once it is not used anymore. It causes memory leak. 
*Solution:* When it is possible, I replaced them by smart pointer, so that the memory can be managed automatically.

### Issue 8: Code Quality
The Word struct make ```data``` and ```count``` public, thus anyone can access it. This is also hard to read and to maintain.
*Solution:* I converted the struct into a class and made the data as private members. With getters and setters, we can prevent from unwanted access, simplify the maintenance and reduce errors.


### Issue 9: Memory Issue
In the constructor of ```Word```, ```strdup``` is used. ```strdup``` calls ```malloc```, so the memory cleanup should be done with ```free()```. But, it is missing in the code. 
*Solution:* properly allocates memory and once the object is not used anymore, free the memory using ```free()``` in the destructor of the class.


## Further changes I would like to do
- Unit test compareWords()
- Unit test Word class 
- Convert s_word as param of functions instead of global variable
- Convert s_wordArray as a param of functions instead of global variable
- Integ test readInputWords()
- Integ test workerThread()
- Integ test lookupWords()
- Make endEncountered a private member of Word class
- Convert s-wordsArray into a std::map, with 'found' as key
- Improve compareWords implementation with "strcmp" (make all upper case)
- Test the code on windows and MacOS environment