#include <iostream>
using namespace std;

// ==================== SPLAY TREE ====================

struct SplayNode {
    int key;
    SplayNode* left;
    SplayNode* right;

    SplayNode(int k) {
        key = k;
        left = NULL;
        right = NULL;
    }
};

void rotateRight(SplayNode*& root) {
    SplayNode* p = root->left;
    root->left = p->right;
    p->right = root;
    root = p;
}

void rotateLeft(SplayNode*& root) {
    SplayNode* p = root->right;
    root->right = p->left;
    p->left = root;
    root = p;
}

void splay(SplayNode*& root, int key) {
    if (root == NULL || root->key == key)
        return;

    if (key < root->key) {
        if (root->left == NULL)
            return;

        if (key < root->left->key) {
            splay(root->left->left, key);
            rotateRight(root);
        }
        else if (key > root->left->key) {
            splay(root->left->right, key);
            if (root->left->right != NULL)
                rotateLeft(root->left);
        }

        if (root->left != NULL)
            rotateRight(root);
    }
    else {
        if (root->right == NULL)
            return;

        if (key > root->right->key) {
            splay(root->right->right, key);
            rotateLeft(root);
        }
        else if (key < root->right->key) {
            splay(root->right->left, key);
            if (root->right->left != NULL)
                rotateRight(root->right);
        }

        if (root->right != NULL)
            rotateLeft(root);
    }
}

void insertSplay(SplayNode*& root, int key) {
    if (root == NULL) {
        root = new SplayNode(key);
        return;
    }

    splay(root, key);

    if (root->key == key)
        return;

    SplayNode* node = new SplayNode(key);

    if (key < root->key) {
        node->right = root;
        node->left = root->left;
        root->left = NULL;
    }
    else {
        node->left = root;
        node->right = root->right;
        root->right = NULL;
    }

    root = node;
}

bool searchSplay(SplayNode*& root, int key) {
    if (root == NULL)
        return false;

    splay(root, key);
    return root->key == key;
}

void deleteSplay(SplayNode*& root, int key) {
    if (root == NULL)
        return;

    splay(root, key);

    if (root->key != key)
        return;

    SplayNode* oldRoot = root;

    if (root->left == NULL) {
        root = root->right;
    }
    else {
        SplayNode* rightPart = root->right;
        root = root->left;
        splay(root, key);
        root->right = rightPart;
    }

    delete oldRoot;
}

SplayNode* copySplay(SplayNode* root) {
    if (root == NULL)
        return NULL;

    SplayNode* node = new SplayNode(root->key);
    node->left = copySplay(root->left);
    node->right = copySplay(root->right);
    return node;
}

void freeSplay(SplayNode*& root) {
    if (root == NULL)
        return;

    freeSplay(root->left);
    freeSplay(root->right);
    delete root;
    root = NULL;
}

int heightSplay(SplayNode* root) {
    if (root == NULL)
        return 0;

    int leftHeight = heightSplay(root->left);
    int rightHeight = heightSplay(root->right);

    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int depthSplay(SplayNode* root, int key, int level = 0) {
    if (root == NULL)
        return -1;

    if (root->key == key)
        return level;

    if (key < root->key)
        return depthSplay(root->left, key, level + 1);

    return depthSplay(root->right, key, level + 1);
}

void printSplay(SplayNode* root) {
    if (root == NULL) {
        cout << "Tree is empty.\n";
        cout << "Height: 0\n";
        return;
    }

    SplayNode* queue[10000];
    SplayNode* parent[10000];
    int levels[10000];
    char sides[10000];

    int first = 0, last = 0;
    queue[last] = root;
    parent[last] = NULL;
    levels[last] = 0;
    sides[last] = '-';
    last++;

    cout << "No.\tLevel\tParent\tSide\tKey\n";

    while (first < last) {
        SplayNode* node = queue[first];
        SplayNode* par = parent[first];
        int level = levels[first];
        char side = sides[first];

        cout << first + 1 << "\t"
            << level << "\t";

        if (par == NULL)
            cout << "-\t";
        else
            cout << par->key << "\t";

        cout << side << "\t"
            << node->key << "\n";

        if (node->left != NULL && last < 10000) {
            queue[last] = node->left;
            parent[last] = node;
            levels[last] = level + 1;
            sides[last] = 'L';
            last++;
        }

        if (node->right != NULL && last < 10000) {
            queue[last] = node->right;
            parent[last] = node;
            levels[last] = level + 1;
            sides[last] = 'R';
            last++;
        }

        first++;
    }

    cout << "Height: " << heightSplay(root) << "\n";
}

// ==================== RED-BLACK TREE ====================

enum Color {
    RED,
    BLACK
};

struct RBNode {
    int key;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;

    RBNode(int k) {
        key = k;
        color = RED;
        left = NULL;
        right = NULL;
        parent = NULL;
    }
};

void rotateLeftRB(RBNode*& root, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rotateRightRB(RBNode*& root, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;

    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

void fixInsertRB(RBNode*& root, RBNode* node) {
    while (node != root &&
        node->parent != NULL &&
        node->parent->color == RED) {

        RBNode* parent = node->parent;
        RBNode* grand = parent->parent;

        if (parent == grand->left) {
            RBNode* uncle = grand->right;

            if (uncle != NULL && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grand->color = RED;
                node = grand;
            }
            else {
                if (node == parent->right) {
                    node = parent;
                    rotateLeftRB(root, node);
                    parent = node->parent;
                    grand = parent->parent;
                }

                parent->color = BLACK;
                grand->color = RED;
                rotateRightRB(root, grand);
            }
        }
        else {
            RBNode* uncle = grand->left;

            if (uncle != NULL && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grand->color = RED;
                node = grand;
            }
            else {
                if (node == parent->left) {
                    node = parent;
                    rotateRightRB(root, node);
                    parent = node->parent;
                    grand = parent->parent;
                }

                parent->color = BLACK;
                grand->color = RED;
                rotateLeftRB(root, grand);
            }
        }
    }

    root->color = BLACK;
}

void insertRB(RBNode*& root, int key) {
    RBNode* parent = NULL;
    RBNode* current = root;

    while (current != NULL) {
        parent = current;

        if (key == current->key)
            return;

        if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    RBNode* node = new RBNode(key);
    node->parent = parent;

    if (parent == NULL)
        root = node;
    else if (key < parent->key)
        parent->left = node;
    else
        parent->right = node;

    fixInsertRB(root, node);
}

RBNode* searchRB(RBNode* root, int key) {
    while (root != NULL) {
        if (root->key == key)
            return root;

        if (key < root->key)
            root = root->left;
        else
            root = root->right;
    }

    return NULL;
}

RBNode* minimumRB(RBNode* node) {
    while (node != NULL && node->left != NULL)
        node = node->left;

    return node;
}

void transplantRB(RBNode*& root, RBNode* u, RBNode* v) {
    if (u->parent == NULL)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != NULL)
        v->parent = u->parent;
}

Color colorOf(RBNode* node) {
    return node == NULL ? BLACK : node->color;
}

void fixDeleteRB(RBNode*& root, RBNode* x, RBNode* parent) {
    while (x != root && colorOf(x) == BLACK) {
        if (parent == NULL)
            break;

        if (x == parent->left) {
            RBNode* sibling = parent->right;

            if (colorOf(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotateLeftRB(root, parent);
                sibling = parent->right;
            }

            if (sibling == NULL) {
                x = parent;
                parent = x->parent;
            }
            else if (colorOf(sibling->left) == BLACK &&
                colorOf(sibling->right) == BLACK) {
                sibling->color = RED;
                x = parent;
                parent = x->parent;
            }
            else {
                if (colorOf(sibling->right) == BLACK) {
                    if (sibling->left != NULL)
                        sibling->left->color = BLACK;

                    sibling->color = RED;
                    rotateRightRB(root, sibling);
                    sibling = parent->right;
                }

                sibling->color = parent->color;
                parent->color = BLACK;

                if (sibling->right != NULL)
                    sibling->right->color = BLACK;

                rotateLeftRB(root, parent);
                x = root;
                parent = NULL;
            }
        }
        else {
            RBNode* sibling = parent->left;

            if (colorOf(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotateRightRB(root, parent);
                sibling = parent->left;
            }

            if (sibling == NULL) {
                x = parent;
                parent = x->parent;
            }
            else if (colorOf(sibling->right) == BLACK &&
                colorOf(sibling->left) == BLACK) {
                sibling->color = RED;
                x = parent;
                parent = x->parent;
            }
            else {
                if (colorOf(sibling->left) == BLACK) {
                    if (sibling->right != NULL)
                        sibling->right->color = BLACK;

                    sibling->color = RED;
                    rotateLeftRB(root, sibling);
                    sibling = parent->left;
                }

                sibling->color = parent->color;
                parent->color = BLACK;

                if (sibling->left != NULL)
                    sibling->left->color = BLACK;

                rotateRightRB(root, parent);
                x = root;
                parent = NULL;
            }
        }
    }

    if (x != NULL)
        x->color = BLACK;
}

void deleteRB(RBNode*& root, int key) {
    RBNode* z = searchRB(root, key);

    if (z == NULL)
        return;

    RBNode* y = z;
    Color originalColor = y->color;
    RBNode* x = NULL;
    RBNode* xParent = NULL;

    if (z->left == NULL) {
        x = z->right;
        xParent = z->parent;
        transplantRB(root, z, z->right);
    }
    else if (z->right == NULL) {
        x = z->left;
        xParent = z->parent;
        transplantRB(root, z, z->left);
    }
    else {
        y = minimumRB(z->right);
        originalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            xParent = y;
            if (x != NULL)
                x->parent = y;
        }
        else {
            xParent = y->parent;
            transplantRB(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplantRB(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (originalColor == BLACK)
        fixDeleteRB(root, x, x != NULL ? x->parent : xParent);
}

void freeRB(RBNode*& root) {
    if (root == NULL)
        return;

    freeRB(root->left);
    freeRB(root->right);
    delete root;
    root = NULL;
}

int heightRB(RBNode* root) {
    if (root == NULL)
        return 0;

    int leftHeight = heightRB(root->left);
    int rightHeight = heightRB(root->right);

    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int depthRB(RBNode* root, int key, int level = 0) {
    if (root == NULL)
        return -1;

    if (root->key == key)
        return level;

    if (key < root->key)
        return depthRB(root->left, key, level + 1);

    return depthRB(root->right, key, level + 1);
}

void printRB(RBNode* root) {
    if (root == NULL) {
        cout << "Tree is empty.\n";
        cout << "Height: 0\n";
        return;
    }

    RBNode* queue[10000];
    RBNode* parent[10000];
    int levels[10000];
    char sides[10000];

    int first = 0, last = 0;
    queue[last] = root;
    parent[last] = NULL;
    levels[last] = 0;
    sides[last] = '-';
    last++;

    cout << "No.\tLevel\tParent\tSide\tKey\tColor\n";

    while (first < last) {
        RBNode* node = queue[first];
        RBNode* par = parent[first];
        int level = levels[first];
        char side = sides[first];

        cout << first + 1 << "\t"
            << level << "\t";

        if (par == NULL)
            cout << "-\t";
        else
            cout << par->key << "\t";

        cout << side << "\t"
            << node->key << "\t"
            << (node->color == RED ? "RED" : "BLACK")
            << "\n";

        if (node->left != NULL && last < 10000) {
            queue[last] = node->left;
            parent[last] = node;
            levels[last] = level + 1;
            sides[last] = 'L';
            last++;
        }

        if (node->right != NULL && last < 10000) {
            queue[last] = node->right;
            parent[last] = node;
            levels[last] = level + 1;
            sides[last] = 'R';
            last++;
        }

        first++;
    }

    cout << "Height: " << heightRB(root) << "\n";
}

// ==================== 2-3-4 TREE ====================

struct Node234 {
    int count;
    int keys[3];
    Node234* child[4];

    Node234() {
        count = 0;

        for (int i = 0; i < 4; i++)
            child[i] = NULL;
    }
};

Node234* create234(int key) {
    Node234* node = new Node234;
    node->keys[0] = key;
    node->count = 1;
    return node;
}

void add234Key(Node234*& root, int key) {
    if (root == NULL) {
        root = create234(key);
        return;
    }

    int i = root->count - 1;

    while (i >= 0 && key < root->keys[i]) {
        root->keys[i + 1] = root->keys[i];
        i--;
    }

    root->keys[i + 1] = key;
    root->count++;
}

Node234* convert234(RBNode* root) {
    if (root == NULL)
        return NULL;

    // A black node represents a 2-3-4 node.
    // Its red children contribute additional keys.

    if (root->color == RED)
        return NULL;

    Node234* node = new Node234;

    RBNode* leftRed = root->left;
    RBNode* rightRed = root->right;

    bool hasLeftRed =
        leftRed != NULL && leftRed->color == RED;

    bool hasRightRed =
        rightRed != NULL && rightRed->color == RED;

    if (hasLeftRed && hasRightRed) {
        node->count = 3;
        node->keys[0] = leftRed->key;
        node->keys[1] = root->key;
        node->keys[2] = rightRed->key;

        node->child[0] = convert234(leftRed->left);
        node->child[1] = convert234(leftRed->right);
        node->child[2] = convert234(rightRed->left);
        node->child[3] = convert234(rightRed->right);
    }
    else if (hasLeftRed) {
        node->count = 2;
        node->keys[0] = leftRed->key;
        node->keys[1] = root->key;

        node->child[0] = convert234(leftRed->left);
        node->child[1] = convert234(leftRed->right);
        node->child[2] = convert234(root->right);
    }
    else if (hasRightRed) {
        node->count = 2;
        node->keys[0] = root->key;
        node->keys[1] = rightRed->key;

        node->child[0] = convert234(root->left);
        node->child[1] = convert234(rightRed->left);
        node->child[2] = convert234(rightRed->right);
    }
    else {
        node->count = 1;
        node->keys[0] = root->key;

        node->child[0] = convert234(root->left);
        node->child[1] = convert234(root->right);
    }

    return node;
}

int height234(Node234* root) {
    if (root == NULL)
        return 0;

    int maxHeight = 0;

    for (int i = 0; i <= root->count; i++) {
        int h = height234(root->child[i]);

        if (h > maxHeight)
            maxHeight = h;
    }

    return maxHeight + 1;
}

void print234(Node234* root, int level = 0, int number = 1) {
    if (root == NULL)
        return;

    cout << number << "\t" << level << "\t";

    for (int i = 0; i < root->count; i++)
        cout << root->keys[i] << " ";

    cout << "\n";

    for (int i = 0; i <= root->count; i++)
        print234(root->child[i], level + 1, number * 10 + i);
}

void free234(Node234*& root) {
    if (root == NULL)
        return;

    for (int i = 0; i < 4; i++)
        free234(root->child[i]);

    delete root;
    root = NULL;
}

// ==================== TREE OPERATIONS ====================

void differenceSplay(SplayNode* root1, SplayNode* root2,
    SplayNode*& result) {
    if (root1 == NULL)
        return;

    if (!searchSplay(root2, root1->key))
        insertSplay(result, root1->key);

    differenceSplay(root1->left, root2, result);
    differenceSplay(root1->right, root2, result);
}

void differenceRB(RBNode* root1, RBNode* root2,
    RBNode*& result) {
    if (root1 == NULL)
        return;

    if (searchRB(root2, root1->key) == NULL)
        insertRB(result, root1->key);

    differenceRB(root1->left, root2, result);
    differenceRB(root1->right, root2, result);
}

void printAllSplay(SplayNode* t1, SplayNode* t2) {
    cout << "\nSPLAY TREE T1:\n";
    printSplay(t1);

    cout << "\nSPLAY TREE T2:\n";
    printSplay(t2);
}

void printAllRB(RBNode* t1, RBNode* t2) {
    cout << "\nRED-BLACK TREE T1:\n";
    printRB(t1);

    cout << "\nRED-BLACK TREE T2:\n";
    printRB(t2);
}

// ==================== MAIN ====================

int main() {
    SplayNode* s1 = NULL;
    SplayNode* s2 = NULL;

    RBNode* r1 = NULL;
    RBNode* r2 = NULL;

    int n, m, key, K;

    cout << "Number of keys for T1: ";
    cin >> n;

    cout << "Enter keys for T1:\n";

    for (int i = 0; i < n; i++) {
        cin >> key;
        insertSplay(s1, key);
        insertRB(r1, key);
    }

    cout << "Number of keys for T2: ";
    cin >> m;

    cout << "Enter keys for T2:\n";

    for (int i = 0; i < m; i++) {
        cin >> key;
        insertSplay(s2, key);
        insertRB(r2, key);
    }

    cout << "Enter K: ";
    cin >> K;

    // A: Search K in T1.
    cout << "\n========== TASK A ==========\n";

    cout << "Splay tree contains K: "
        << (searchSplay(s1, K) ? "YES" : "NO") << "\n";

    cout << "Red-black tree contains K: "
        << (searchRB(r1, K) != NULL ? "YES" : "NO") << "\n";

    printAllSplay(s1, s2);
    printAllRB(r1, r2);

    // B: T1 = keys of T1 that are not in T2.
    cout << "\n========== TASK B ==========\n";

    SplayNode* newSplay = NULL;
    RBNode* newRB = NULL;

    differenceSplay(s1, s2, newSplay);
    differenceRB(r1, r2, newRB);

    freeSplay(s1);
    freeRB(r1);

    s1 = newSplay;
    r1 = newRB;

    cout << "Difference T1 - T2 was constructed.\n";

    printAllSplay(s1, s2);
    printAllRB(r1, r2);

    // V: Find the depth of K and show a copy.
    cout << "\n========== TASK V ==========\n";

    SplayNode* splayCopy = copySplay(s1);

    int ds = depthSplay(s1, K);
    int dr = depthRB(r1, K);

    if (ds == -1)
        cout << "K is absent from the splay tree.\n";
    else
        cout << "Depth of K in the splay tree: " << ds << "\n";

    if (dr == -1)
        cout << "K is absent from the red-black tree.\n";
    else
        cout << "Depth of K in the red-black tree: " << dr << "\n";

    cout << "\nCopy of the splay tree:\n";
    printSplay(splayCopy);

    printAllSplay(s1, s2);
    printAllRB(r1, r2);

    // G: Convert the red-black tree into a 2-3-4 tree.
    cout << "\n========== TASK G ==========\n";

    Node234* tree234 = convert234(r1);

    cout << "Red-black tree after task V:\n";
    printRB(r1);

    cout << "\nConverted 2-3-4 tree:\n";

    if (tree234 == NULL) {
        cout << "Tree is empty.\n";
        cout << "Height: 0\n";
    }
    else {
        cout << "No.\tLevel\tKeys\n";
        print234(tree234);
        cout << "Height: " << height234(tree234) << "\n";
    }

    // Free all allocated memory.
    freeSplay(s1);
    freeSplay(s2);
    freeSplay(splayCopy);

    freeRB(r1);
    freeRB(r2);

    free234(tree234);

    return 0;
}