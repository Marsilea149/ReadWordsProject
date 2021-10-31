<!-- Add banner here -->

# Read Words Project -- Wolven Planet Coding Assignement

This project has as input a file provided by Wolven Planet, named *buggy.cxx*. This file contains bugs, it is saved under 'input' folder. The objective is to fix all the bugs and make the program running correctly. In the ReadMe, I will first explain the content of the package, second how to compile and execute the code, third the principle issues that I noticed in *buggy.cxx* and the proposed solutions, finally the potential improvedment. 

The content of the program: 
The user is asked to enter words to build a 'Dictionary' of words. Each word can have upto 31 characters. The program will stop building the 'Dictionary' once it received the word "end" from user input. While the user is entering words, the program is processing the entered word so that no duplicate gets added into the 'Dictionary', and it updates at the same time the number of times the word has been entered. After that, the program displays all the words have been entered by the user in alphabetic order. Then the program asks the user to enter a word to lookup in the 'Dictionary'. After the user enters a word, the program will show message to notify if this word exists in the 'Disctionary' and it updates the number of times this word has been found. Once the user finished search words in the 'Dictionary', the program will display the total number of words that the user found in the 'Dictionary'. Note that during 'lookup' process, the "Total words found" is the number of times the user 'found' words. So if the same word is entered 3 times during 'lookup' process, the "Total words found" gets incremented by 3.  

# Table of contents

- [Read Words Project -- Wolven Planet Coding Assignement](#read-words-project----wolven-planet-coding-assignement)
- [Table of contents](#table-of-contents)
- [Installation](#installation)
- [Usage](#usage)
- [Development](#development)
    - [Testing](#testing)
- [Footer](#footer)

# Installation
[(Back to top)](#table-of-contents)

Prerequisit:
1. For this porject, Linux environment is used.
2. Valgrind installed for memory leak tracking
    To install it, run in your terminal:
    ``` sudo apt install valgrind  # Ubuntu, Debian, etc.
        sudo yum install valgrind  # RHEL, CentOS, Fedora, etc.
    ```
3. GTest installed
    To install it, follow the steps below: 
    ``` sudo apt-get install libgtest-dev
        sudo apt-get install cmake
        cd /usr/src/gtest
        sudo cmake CMakeLists.txt
        sudo make
        sudo cp *.a /usr/lib
    ```

To use this project, please follow the procedure below in order:
1. Download the zipped folder **Bingbing.WU** on your device and unzip it in your local folder *YourLocalFolder*.
2. Open the Makefile located at *YourLocalFolder* and change ```SRCDIR``` to ```*YourLocalFolder*/src```. 
  To obtain your local folder path, please of a terminal in your local folder, then type ```pwd``` in your terminal.
3. Compile the program: open a terminal in *YourLocalFolder*, type ```make```, an executable named **ReadWords** is generated in the same folder

# Usage
[(Back to top)](#table-of-contents)

Please follow the steps below to use the program after installation:
1. Run the executable that you generated, in your terminal of *YourLocalFolder*,  type ```./ReadWords```
2. In your terminal, the message ```Please enter a word``` is shown to ask you to enter a word. You can enter a word within **31** characters.
  Once you entered a word, please press *Enter* key, the message ```Please enter a word``` is shown again to ask you to enter another word.

    The program keeps asking you enter a word. When you are done with entering words, please enter the word "end" and press *Enter* to terminate readInputWord step.

    Note that you entercan words of length enter up to 31 characters, if you entered 32 or more characters, the word is not saved for lookup and the program displays a message notifying you the word you entered is to long and asks you to enter another word.

    Note that if you press *ctrl* + *d* during this step, as it expects the word "end" before end of file, the program will terminates with displaying the message ```EOF encountered without having 'end' word in user input. Quitting the program.```
3. Word list is shown in your terminal. It contains all the words you entered, without duplicate, in alphabetic order. And the message ```Enter a word for lookup:``` is shown in your terminal. 
4. Enter a word that you want to lookup, then press *Enter*. A message is displayed in your terminal to indcate if the word has been found among the words you entered previously. And the message ```Enter a word for lookup:``` shows again.
5. To stop the lookup process, type *ctrl* + *d* to make the program understand that it is the end of file. The program ends and displays the number of words got found during lookup process.

# Development
[(Back to top)](#table-of-contents)

To visualize the memory leak tracking: 
at step 1 of **Usage**, instead of typing ```./ReadWords```, please type in your terminal:
```
valgrind 
--leak-check=full          
--show-leak-kinds=all          
--track-origins=yes          
--verbose          
--log-file=valgrind-out.txt          
./ReadWords
```
You can then visualize the report named "valgrind-out.txt" in your *YourLocalFolder*. I successfully improved the code to not have any memory leak. There remains many other errors in the report though. 
Below is a section of the report I have:
```
==78110== HEAP SUMMARY:
==78110==     in use at exit: 0 bytes in 0 blocks
==78110==   total heap usage: 22 allocs, 37 frees, 75,266 bytes allocated
==78110== 
==78110== All heap blocks were freed -- no leaks are possible
==78110== 
==78110== ERROR SUMMARY: 65 errors from 27 contexts (suppressed: 0 from 0)
```

### Testing
[(Back to top)](#table-of-contents)
For the testing part, I have been tested with console while developping. I should have written unit tests before going to far in the code. I have already installed GTest on my VM. Finally, I did not have enough time to do the unit tests. I would love to have implemented unit tests and integration tests.

# Footer
[(Back to top)](#table-of-contents)