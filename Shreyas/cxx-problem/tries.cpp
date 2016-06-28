#include <iostream>

using namespace std;
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

int main() {
    string s = "Hello";
    put(s, 10);
    put("Hi", 2);
    
    put("Hello", 120);
    cout << get("Hi") << endl;
    while (1) {
        string input;
        cin >> input;
        int freq = get(input);
        put (input, ++freq);
        cout << "Frequency of word" << freq << endl;
    }
    return 0;
}
