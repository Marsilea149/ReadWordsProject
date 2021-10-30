// static void readInputWords ()
// {
//   bool endEncountered = false;
  
//   // missing lock to protect data from being used by other thread
//   /* 
//   For example:
//   // To put outside of the method
//   // Protects shared data from being accessed at the same time
//   std::mutex acctLock;

//   // To put inside of the method
//   // The exception safe way to protect access
//   // to code within its scope. The lock is released 
//   // after execution leaves this scope
//   std::lock_guard<std::mutex> lock(acctLock);
//   */


//   std::thread * worker = new std::thread( workerThread );

//   char * linebuf = new char[32];
  
//   while (!endEncountered)
//   {
//     if(std::scanf("%s", linebuf) == EOF) //eof
//       return;

// 	//----seems redundant ----- not sure
//     endEncountered = std::strcmp( linebuf, "end" ) == 0;

//     //BWU: Segmentation fault, because the destination memory is not properly allocated
//     // Pass the word to the worker thread
//     std::strcpy( s_word.data, linebuf );
//     //??? seems useless
//     while (s_word.data[0]); // Wait for the worker thread to consume it
//   }

//   worker->join(); // Wait for the worker to terminate
// }