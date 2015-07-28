#include <thread>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

/*************** Tries Implementation Start *****************/
#define KEY(tst) \
    tst->key
#define VAL(tst) \
    tst->val
#define MID(tst)\
    tst->mid
#define RIGHT(tst)\
    tst->right
#define LEFT(tst)\
    tst->left

int compare(char a, char b) {
    if (a < b) {return -1;}
    else if (a > b) {return 1;}
    else {return 0;}
}

struct TST {
    char key;
    int val;
    TST *left, *mid, *right;
    TST(char k, int v): key(k), val(v), left(NULL), mid(NULL), right(NULL) {}
    TST(char k): key(k), left(NULL), mid(NULL), right(NULL) {}
    TST(): val(0), left(NULL), mid(NULL), right(NULL) {}
};

TST *root = NULL;

void putWorker(TST *&root, const char *c, int size,int val) {
    if (size == 0) {
        return;
    }
    if (root == NULL) {
        if (size == 1) {
            root = new TST(*c, val);
        } else {
            root = new TST(*c, -1);
            putWorker(MID(root), c + 1, --size, val);
        }
        return;
    }
    int dir = compare(KEY(root), *c);
    if (dir > 0) {
        putWorker(root->left, c, size, val);
    } else if (dir < 0) {
        putWorker(root->right, c, size, val);
    } else {
        if (size != 1) {
            putWorker(root->mid, c + 1, --size, val);
        } else {
            VAL(root) = val;
            return;
        }
    }
}

void put(string s, int val) {
    putWorker(root, s.c_str(), s.size(), val);
}

int getWorker(TST *root, const char *c, int size) {

    if (root == NULL) {
        return -1;
    }
    if (size == 1 && root == NULL) {
        return -1;
    }
    if (size == 1 && compare(KEY(root), *c) == 0) {
        return VAL(root);
    }
    int dir = compare(KEY(root), *c);
    if (dir > 0) {
        return getWorker(LEFT(root), c, size);
    } else if (dir < 0) {
        return getWorker(RIGHT(root), c, size);
    } else {
        return getWorker(MID(root), c + 1, --size);
    }
}

int get(string s) {
    return getWorker(root, s.c_str(), s.size());
}
/*************** Tries Implementation end *****************/

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
      
      endEncountered = std::strcmp( s_word.data, "end" ) == 0;

      
      if (!endEncountered)
      {
        // Do not insert duplicate words
      /* for ( auto p : s_wordsArray )
        {
          if (!std::strcmp( p->data, w->data ))
          {
            ++p->count;
            found = true;
            break;
          }
        }

        if (!found)
          s_wordsArray.push_back( w ); */
          //cout << w->data << endl;
          string word = string(s_word.data);
          int freq = get(word);
          cout << "Adding new word:" << word << endl;
          put(word, ++freq);
      }
      s_word.data[0] = 0; // Inform the producer that we consumed the word
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
  bool found;
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
    /*
    unsigned i;
    for ( i = 0; i < s_wordsArray.size(); ++i )
    {
      if (std::strcmp( s_wordsArray[i]->data, w->data ) == 0)
      {
        found = true;
        break;
      }
    }

    i--;
    if (found)
    {
      std::printf( "SUCCESS: '%s' was present %d times in the initial word list\n",
                   s_wordsArray[i]->data, s_wordsArray[i]->count );
      ++s_totalFound;
    }
    else
      std::printf( "'%s' was NOT found in the initial word list\n", w->data );
   */
    string word = string(w->data);
    cout << "Search Word" << word << endl;
    int freq = get(word);
    if (freq == -1) {
      std::printf( "'%s' was NOT found in the initial word list\n", w->data );
    } else {
      std::printf( "SUCCESS: '%s' was present %d times in the initial word list\n",
                   w->data, freq);
   }
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

