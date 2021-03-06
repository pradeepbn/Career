#include <thread>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

struct Word
{
  char * data;
  int count;

  Word ( char * data_ ) : data( ::strdup(data_) ) {}
  
  Word () : data((char *)malloc(32))
  {}
};

static std::vector<Word*> s_wordsArray;
static Word s_word;
static int s_totalFound;
static string searchString;


// Worker thread: consume words passed from the main thread and insert them
// in the 'word list' (s_wordsArray), while removing duplicates. Terminate when
// the word 'end' is encountered.
/*
 * This thread needs to be locked
 * Typical producer-consumer problem
 */
static void workerThread ()
{
  bool endEncountered = false;
  bool found = false;
  
  while (!endEncountered)
  {
    if (s_word.data[0]) // Do we have a new word?
    {
      Word * w = new Word(s_word); // Copy the word
      cout << w->data << endl;
      
      endEncountered = std::strcmp( s_word.data, "end" ) == 0;

      s_word.data[0] = 0; // Inform the producer that we consumed the word
      
      if (!endEncountered)
      {
        // Do not insert duplicate words
        for ( auto p : s_wordsArray )
        {
          if (!std::strcmp( p->data, w->data ))
          {
            ++p->count;
            found = true;
            break;
          }
        }

        if (!found)
            cout << "Appending to string" << endl;
          searchString = "";
          searchString.append(w->data);
        cout << searchString << endl;
         // s_wordsArray.push_back( w );
      }
    }
  }
};

// Read input words from STDIN and pass them to the worker thread for
// inclusion in the word list.
// Terminate when the word 'end' has been entered.
//
static void readInputWords ()
{
  bool endEncountered = false;
  
  std::thread * worker = new std::thread( workerThread );

  char * linebuf = new char[32];
  
  while (!endEncountered)
  {
    if (!std::gets( linebuf )) // EOF?
      return;
    
    endEncountered = std::strcmp( linebuf, "end" ) == 0;

    // Pass the word to the worker thread
    std::strcpy( s_word.data, linebuf );
	/*
	 * Do not loop here, make it event drive- either use monitors of semaphore
	 * This is a typical producer-consumer problem
	 */
    while (s_word.data[0]); // Wait for the worker thread to consume it
  }

  worker->join(); // Wait for the worker to terminate
}

// Repeatedly ask the user for a word and check whether it was present in the word list
// Terminate on EOF
//
/*
 * Can be better than O(n)
 */
static void lookupWords ()
{
  //bool found;
  char * linebuf = new char[32];
    
  for(;;)
  {
    std::printf( "\nEnter a word for lookup:" );
    if (std::scanf( "%s", linebuf ) == EOF)
      return;

    // Initialize the word to search for
    Word * w = new Word(linebuf);
    //std::strcpy( w->data, linebuf );

    // Search for the word
    /*unsigned i;
    for ( i = 0; i < s_wordsArray.size(); ++i )
    {
      if (std::strcmp( s_wordsArray[i]->data, w->data ) == 0)
      {
        found = true;
        break;
      }
    }*/
    size_t found = searchString.find(w->data, 0, 5);
    cout << searchString << endl;
    cout << found <<"," << string(w->data, 0 , 5) << endl;

    //i--;
    if (found != string::npos)
    {
        cout << "SUCCESS" << endl;
      //std::printf( "SUCCESS: '%s' was present %d times in the initial word list\n",
       //            s_wordsArray[i]->data, s_wordsArray[i]->count );
      ++s_totalFound;
    }
    else
      std::printf( "'%s' was NOT found in the initial word list\n", w->data );
  }
}

int main ()
{
  try
  {
    readInputWords();
    
    // Sort the words alphabetically
	/* 
	 * This can be avoided if the heap/ tree is used to store at strings
	 */
    std::sort( s_wordsArray.begin(), s_wordsArray.end() );

    // Print the word list
    std::printf( "\n=== Word list:\n" );
    for ( auto p : s_wordsArray )
      std::printf( "%s %d\n", p->data, p->count );

    lookupWords();

    printf( "\n=== Total words found: %d\n", s_totalFound );
  }
  catch (std::exception & e)
  {
    std::printf( "error %s\n", e.what() );
  }
  
  return 0;
}

