#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Node structure for trie
struct Node {
    struct Node* nextChar[128]; // array to store pointers to child nodes
    bool EOW; // flag to mark end of word
    char ch; // character stored in the node
    char desc[100]; // description of the word
};

// global variable
struct Node* root = NULL; // pointer to the root oftrie
int num = 0; // counter to print number

// function for make new node
struct Node* newNode(char ch) {
    // allocate momory for new node
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    if (temp) {
        temp->ch = ch;
        temp->EOW = false;
        memset(temp->nextChar, 0, sizeof(temp->nextChar)); // intialize node to NULL
        temp->desc[0] = '\0'; 
    }
    return temp;
}

// insert word to trie function
void trieInsert(const char* str, int idx, const char* desc) {
    // if root does not exist 
    if (!root) {
        root = newNode('S'); 
        return;
    }
    int len = strlen(str);
    struct Node* curr = root;
    // creating nodes
    while (idx < len) {
        if (!curr->nextChar[str[idx]]) {
            curr->nextChar[str[idx]] = newNode(str[idx]); // create new node if word doesnt exist
        }
        curr = curr->nextChar[str[idx++]]; // move to next node
    }
    curr->EOW = true; 
    strcpy(curr->desc, desc); 
}

// recursive function to search word
struct Node* searchWordRecursive(struct Node* root, const char* search, int idx) {
    // Tget through the end of string or until end or null
    while (root != NULL && search[idx] != '\0') {
        root = root->nextChar[search[idx]]; // move to next node
        idx++;
    }
    //return the node if exist
    return (root && root->EOW) ? root : NULL;
}

// function to print word by given prefix
void printTriesByPrefix(struct Node* root, char* temp, int size) {
    if (!root) {
        printf("Root is empty\n");
        return;
    }
    if (size > 0) {
        temp[size - 1] = root->ch; 
        if (root->EOW) { // check if it's the end of a word
            temp[size] = '\0'; // terminate the string
            printf("%d. %s\n", num++, temp); // print the word with its number
        }
    }

    // recursively call the function for all child nodes
    for (int i = 0; i < 128; i++) {
        if (root->nextChar[i]) {
            printTriesByPrefix(root->nextChar[i], temp, size + 1);
        }
    }
}

//// function to search for words with a given prefix
void searchWordByPrefix(struct Node* root, const char* search) {
    int len = strlen(search);
    char temp[len + 2]; // temporary string to store the prefix and words
    int idx = 0;

    // get through the trie based on the prefix
    while (len != idx) {
        if (!root) {
        	puts("");
            printf("No words found with the given prefix.\n");
            printf("Press enter to continue...");
            return;
        }
        if (!root->nextChar[search[idx]]) {
        	puts("");
            printf("No words found with the given prefix.\n");
            printf("Press enter to continue...");
            return;
        }
        root = root->nextChar[search[idx]];
        temp[idx] = search[idx];
        idx++;
    }

    // if the prefix itself is a valid word, print it
    if (root && root->EOW) {
        temp[idx] = '\0'; // terminate the string
        printf("%d. %s\n", num++, temp); // print the word with its number
    }

    // print all words with the given prefix
    for (int i = 0; i < 128; i++) {
        if (root && root->nextChar[i]) {
            printTriesByPrefix(root->nextChar[i], temp, idx);
        }
    }
}


// forward declaration of menu function
void menu();

// input slang function
void inputSlang() {
    char slang[100];
    // get input for the new slang word
    while (1) {
        int space = 0;
        printf("Input a new slang word [Must be more than 1 character and contain no spaces]: ");
        scanf("%99[^\n]", slang);
        getchar();
        int len = strlen(slang);
        if (len > 1) {
            for (int i = 0; i < len; i++) {
                if (slang[i] == ' ') {
                    space = 1;
                    break;
                }
            }
            if (space == 0) {
                break;
            }
        }
    }

    // description input
    char desc[100];
    while (1) {
        int spaceCount = 0;
        printf("Input a new slang word description [Must be more than 2 words]: ");
        scanf("%99[^\n]", desc);
        getchar();
        int len = strlen(desc);
        for (int i = 0; i < len; i++) {
            if (desc[i] == ' ') {
                spaceCount++;
            }
        }
        if (spaceCount > 1) {
            break;
        }
    }

    // to chect if the slang word is already exist or not
    struct Node* search = searchWordRecursive(root, slang, 0);
    if (search) {
        puts("");
        puts("Successfully updated a slang word.");
        puts("Press enter to continue...");
        getchar();
    } else {
        puts("");
        puts("Successfully released new slang word.");
        puts("Press enter to continue...");
        getchar();
    }
    trieInsert(slang, 0, desc); // insert the slang word into the trie
}

// function to search for a slang word
void searchSlang() {
    char slang[100];
    int space = 0;
    // input for searching slang word
    while (1) {
        printf("Input a slang word to be searched [Must be more than 1 character and contain no spaces]: ");
        scanf("%99[^\n]", slang);
        getchar();
        int len = strlen(slang);
        if (len > 1) {
            for (int i = 0; i < len; i++) {
                if (slang[i] == ' ') {
                    space = 1;
                    break;
                }
            }
            if (space == 0) {
                break;
            }
        }
    }

    // searching slang word from the trie
    struct Node* search = searchWordRecursive(root, slang, 0);
    if (search) {
        puts("");
        printf("Slang word  : %s\n", slang);
        printf("Description : %s\n", search->desc);
        puts("");
        printf("Press enter to continue..."); getchar();
    } else {
        puts("");
        printf("There is no word \"%s\" in the dictionary.\n", slang);
        printf("Press enter to continue..."); getchar();
    }
}

// function to search with given prefix
void searchByPrefix() {
    char prefix[100];
    // get input for the prefix to be searched
    while (1) {
        int space = 0;
        printf("Input a slang word prefix to be searched [Must be more than 1 character and contain no spaces]: ");
        scanf("%99[^\n]", prefix);
        getchar(); 
        int len = strlen(prefix);
        if (len > 1) {
            for (int i = 0; i < len; i++) {
                if (prefix[i] == ' ') {
                    space = 1;
                    break;
                }
            }
            if (space == 0) {
                break;
            }
        } else {
            printf("Prefix must be more than 1 character and contain no spaces.\n");
        }
    }

    num = 1;
    searchWordByPrefix(root, prefix); // search for words with the given prefix in the trie
    getchar(); 
}

//view all word in tree function
void viewAll() {
    bool wordsExist = false; // flag to track if any words exist in the trie
    for (int i = 0; i < 128; i++) {
        if (root->nextChar[i]) {
            wordsExist = true; // set the flag to true if at least one word is found
            break;
        }
    }
    
    if (!wordsExist) { // check if no words exist in the trie
        puts("");
        printf("There are no slang words yet in the dictionary.\n");
        printf("Press enter to continue...");
        getchar();
        return;
    }
    
    char temp[100];
    num = 1;
    printTriesByPrefix(root, temp, 0); // print all words in the trie
    puts("\n Press enter to continue...");
    getchar();
}



// print menu function
void printMenu() {
    system("cls");
    puts("1. Release a new slang word");
    puts("2. Search a slang word");
    puts("3. View all slang words starting with a certain prefix word");
    puts("4. View all slang words");
    puts("5. Exit");
    printf(">> ");
}

// main menu function
void menu() {
    char temp[100];
    while (1) {
        printMenu();
        int option;
        scanf("%d", &option);
        getchar();
        switch (option) {
            case 1:
                inputSlang();
                break;
            case 2:
                searchSlang();
                break;
            case 3:
                searchByPrefix();
                break;
            case 4:
                viewAll();
                break;
            case 5:
                puts("Thank you... Have a nice day :)");
                exit(0);
                break;
        }
    }
}


int main() {
    trieInsert("S", 0, ""); // initialize the trie with a dummy node
    menu(); 
    return 0;
}

