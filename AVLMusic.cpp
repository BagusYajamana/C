#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Music {
    char title[31];
    char singer[31];
    char type[5]; // "pop", "jazz", or "rock"
} Music;

typedef struct Node {
    Music data;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(Node *node) {
    return (node == NULL) ? 0 : node->height;
}

Node* createNode(Music data) {
    Node *node = (Node*) malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

Node* rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Node *node) {
    return (node == NULL) ? 0 : height(node->left) - height(node->right);
}

Node* insertNode(Node *node, Music data) {
    if (node == NULL) {
        return createNode(data);
    }

    if (strcmp(data.title, node->data.title) < 0) {
        node->left = insertNode(node->left, data);
    } else if (strcmp(data.title, node->data.title) > 0) {
        node->right = insertNode(node->right, data);
    } else {
        return node; 
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && strcmp(data.title, node->left->data.title) < 0) {
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(data.title, node->right->data.title) > 0) {
        return leftRotate(node);
    }

    if (balance > 1 && strcmp(data.title, node->left->data.title) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(data.title, node->right->data.title) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node* minValueNode(Node *node) {
    Node *current = node;

    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}

Node* deleteNode(Node *root, char *title) {
    if (root == NULL) {
        return root;
    }

    if (strcmp(title, root->data.title) < 0) {
        root->left = deleteNode(root->left, title);
    } else if (strcmp(title, root->data.title) > 0) {
        root->right = deleteNode(root->right, title);
    } else {
        if ((root->left == NULL) || (root->right == NULL)) {
            Node *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }

            free(temp);
        } else {
            Node *temp = minValueNode(root->right);

            root->data = temp->data;

            root->right = deleteNode(root->right, temp->data.title);
        }
    }

    if (root == NULL) {
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0) {
        return rightRotate(root);
    }

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0) {
        return leftRotate(root);
    }

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void printBorder() {
    printf("+------------------------------+------------------------------+------+\n");
}

void printHeader() {
    printBorder();
    printf("| %-28s | %-28s | %-4s |\n", "Title", "Singer", "Type");
    printBorder();
}

void preorder(Node *root) {
    if (root != NULL) {
        printf("| %-28s | %-28s | %-4s |\n", root->data.title, root->data.singer, root->data.type);
        preorder(root->left);
        preorder(root->right);
    }
}

int isValidType(char *type) {
    return strcmp(type, "pop") == 0 || strcmp(type, "jazz") == 0 || strcmp(type, "rock") == 0;
}

int main() {
    Node *root = NULL;
    int choice;
    char title[31], singer[31], type[5];

    while (1) {
        printf("Menu:\n");
        printf("1. Add Data\n");
        printf("2. Delete Data\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        if (choice == 1) {
            printf("Enter title (3-30 characters): ");
            fgets(title, 31, stdin);
            title[strcspn(title, "\n")] = 0;

            printf("Enter singer (3-30 characters): ");
            fgets(singer, 31, stdin);
            singer[strcspn(singer, "\n")] = 0;

            printf("Enter type (pop, jazz, rock): ");
            fgets(type, 5, stdin);
            type[strcspn(type, "\n")] = 0;

            if (strlen(title) < 3 || strlen(title) > 30 || strlen(singer) < 3 || strlen(singer) > 30 || !isValidType(type)) {
                printf("Invalid input. Please try again.\n");
            } else {
                Music music;
                strcpy(music.title, title);
                strcpy(music.singer, singer);
                strcpy(music.type, type);

                root = insertNode(root, music);
                printf("Music added successfully.\n");
            }
        } else if (choice == 2) {
            printf("Enter title to delete: ");
            fgets(title, 31, stdin);
            title[strcspn(title, "\n")] = 0;

            root = deleteNode(root, title);
            printf("Music deleted successfully.\n");
        } else if (choice == 3) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }

        printf("Current Music List (Preorder):\n");
        printHeader();
        preorder(root);
        printBorder();
        printf("\n");
    }

    return 0;
}

