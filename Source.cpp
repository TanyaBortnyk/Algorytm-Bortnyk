#include <iostream>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

// ==================== SPLAY TREE ====================
struct SplayNode {
    int key;
    SplayNode* left;
    SplayNode* right;
    explicit SplayNode(int k) : key(k), left(nullptr), right(nullptr) {}
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
    if (!root || root->key == key) return;
    if (key < root->key) {
        if (!root->left) return;
        if (key < root->left->key) {
            splay(root->left->left, key);
            rotateRight(root);
        }
        else if (key > root->left->key) {
            splay(root->left->right, key);
            if (root->left->right) rotateLeft(root->left);
        }
        if (root->left) rotateRight(root);
    }
    else {
        if (!root->right) return;
        if (key > root->right->key) {
            splay(root->right->right, key);
            rotateLeft(root);
        }
        else if (key < root->right->key) {
            splay(root->right->left, key);
            if (root->right->left) rotateRight(root->right);
        }
        if (root->right) rotateLeft(root);
    }
}

void insertSplay(SplayNode*& root, int key) {
    if (!root) { root = new SplayNode(key); return; }
    splay(root, key);
    if (root->key == key) return;
    SplayNode* node = new SplayNode(key);
    if (key < root->key) {
        node->right = root;
        node->left = root->left;
        root->left = nullptr;
    }
    else {
        node->left = root;
        node->right = root->right;
        root->right = nullptr;
    }
    root = node;
}

bool searchSplay(SplayNode*& root, int key) {
    if (!root) return false;
    splay(root, key);
    return root->key == key;
}

SplayNode* copySplay(SplayNode* root) {
    if (!root) return nullptr;
    SplayNode* node = new SplayNode(root->key);
    node->left = copySplay(root->left);
    node->right = copySplay(root->right);
    return node;
}

void freeSplay(SplayNode*& root) {
    if (!root) return;
    freeSplay(root->left);
    freeSplay(root->right);
    delete root;
    root = nullptr;
}

int heightSplay(SplayNode* root) {
    if (!root) return 0;
    return 1 + max(heightSplay(root->left), heightSplay(root->right));
}

int depthSplay(SplayNode* root, int key, int level = 0) {
    if (!root) return -1;
    if (root->key == key) return level;
    return key < root->key ? depthSplay(root->left, key, level + 1) : depthSplay(root->right, key, level + 1);
}

void printSplay(SplayNode* root) {
    if (!root) { cout << "Tree is empty.\nHeight: 0\n"; return; }
    struct Item { SplayNode* node; SplayNode* parent; int level; char side; };
    queue<Item> q;
    q.push({ root, nullptr, 0, '-' });
    cout << "No.\tLevel\tParent\tSide\tKey\n";
    int count = 1;
    while (!q.empty()) {
        Item item = q.front(); q.pop();
        cout << count++ << "\t" << item.level << "\t"
            << (!item.parent ? "-" : to_string(item.parent->key)) << "\t"
            << item.side << "\t" << item.node->key << "\n";
        if (item.node->left) q.push({ item.node->left, item.node, item.level + 1, 'L' });
        if (item.node->right) q.push({ item.node->right, item.node, item.level + 1, 'R' });
    }
    cout << "Height: " << heightSplay(root) << "\n";
}

// ==================== RED-BLACK TREE ====================
enum Color { RED, BLACK };
struct RBNode {
    int key;
    Color color;
    RBNode* left, * right, * parent;
    explicit RBNode(int k) : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

void rotateLeftRB(RBNode*& root, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rotateRightRB(RBNode*& root, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent) root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

void fixInsertRB(RBNode*& root, RBNode* node) {
    while (node != root && node->parent && node->parent->color == RED) {
        RBNode* parent = node->parent;
        RBNode* grand = parent->parent;
        if (parent == grand->left) {
            RBNode* uncle = grand->right;
            if (uncle && uncle->color == RED) {
                parent->color = uncle->color = BLACK;
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
            if (uncle && uncle->color == RED) {
                parent->color = uncle->color = BLACK;
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
    RBNode* parent = nullptr, * current = root;
    while (current) {
        parent = current;
        if (key == current->key) return;
        current = (key < current->key) ? current->left : current->right;
    }
    RBNode* node = new RBNode(key);
    node->parent = parent;
    if (!parent) root = node;
    else if (key < parent->key) parent->left = node;
    else parent->right = node;
    fixInsertRB(root, node);
}

RBNode* searchRB(RBNode* root, int key) {
    while (root) {
        if (root->key == key) return root;
        root = (key < root->key) ? root->left : root->right;
    }
    return nullptr;
}

int heightRB(RBNode* root) {
    if (!root) return 0;
    return 1 + max(heightRB(root->left), heightRB(root->right));
}

int depthRB(RBNode* root, int key, int level = 0) {
    if (!root) return -1;
    if (root->key == key) return level;
    return key < root->key ? depthRB(root->left, key, level + 1) : depthRB(root->right, key, level + 1);
}

void printRB(RBNode* root) {
    if (!root) { cout << "Tree is empty.\nHeight: 0\n"; return; }
    struct Item { RBNode* node; RBNode* parent; int level; char side; };
    queue<Item> q;
    q.push({ root, nullptr, 0, '-' });
    cout << "No.\tLevel\tParent\tSide\tKey\tColor\n";
    int count = 1;
    while (!q.empty()) {
        Item item = q.front(); q.pop();
        cout << count++ << "\t" << item.level << "\t"
            << (!item.parent ? "-" : to_string(item.parent->key)) << "\t"
            << item.side << "\t" << item.node->key << "\t"
            << (item.node->color == RED ? "RED" : "BLACK") << "\n";
        if (item.node->left) q.push({ item.node->left, item.node, item.level + 1, 'L' });
        if (item.node->right) q.push({ item.node->right, item.node, item.level + 1, 'R' });
    }
    cout << "Height: " << heightRB(root) << "\n";
}

void freeRB(RBNode*& root) {
    if (!root) return;
    freeRB(root->left);
    freeRB(root->right);
    delete root;
    root = nullptr;
}

// ==================== 2-3-4 TREE ====================
struct Node234 {
    int count;
    int keys[3];
    Node234* child[4];
    Node234() : count(0) { for (int i = 0; i < 4; i++) child[i] = nullptr; }
};

Node234* convert234(RBNode* root) {
    if (!root || root->color == RED) return nullptr;
    Node234* node = new Node234();
    RBNode* leftRed = root->left, * rightRed = root->right;
    bool hasLeft = (leftRed && leftRed->color == RED);
    bool hasRight = (rightRed && rightRed->color == RED);

    if (hasLeft && hasRight) {
        node->count = 3;
        node->keys[0] = leftRed->key; node->keys[1] = root->key; node->keys[2] = rightRed->key;
        node->child[0] = convert234(leftRed->left);
        node->child[1] = convert234(leftRed->right);
        node->child[2] = convert234(rightRed->left);
        node->child[3] = convert234(rightRed->right);
    }
    else if (hasLeft) {
        node->count = 2;
        node->keys[0] = leftRed->key; node->keys[1] = root->key;
        node->child[0] = convert234(leftRed->left);
        node->child[1] = convert234(leftRed->right);
        node->child[2] = convert234(root->right);
    }
    else if (hasRight) {
        node->count = 2;
        node->keys[0] = root->key; node->keys[1] = rightRed->key;
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
    if (!root) return 0;
    int h = 0;
    for (int i = 0; i <= root->count; i++) h = max(h, height234(root->child[i]));
    return h + 1;
}

void print234(Node234* root, int level = 0, int number = 1) {
    if (!root) return;
    cout << number << "\t" << level << "\t";
    for (int i = 0; i < root->count; i++) cout << root->keys[i] << " ";
    cout << "\n";
    for (int i = 0; i <= root->count; i++) print234(root->child[i], level + 1, number * 10 + i);
}

void free234(Node234*& root) {
    if (!root) return;
    for (int i = 0; i < 4; i++) free234(root->child[i]);
    delete root;
    root = nullptr;
}

// ==================== OPERATIONS ====================
void differenceSplay(SplayNode* r1, SplayNode* r2, SplayNode*& res) {
    if (!r1) return;
    if (!searchSplay(r2, r1->key)) insertSplay(res, r1->key);
    differenceSplay(r1->left, r2, res);
    differenceSplay(r1->right, r2, res);
}

void differenceRB(RBNode* r1, RBNode* r2, RBNode*& res) {
    if (!r1) return;
    if (!searchRB(r2, r1->key)) insertRB(res, r1->key);
    differenceRB(r1->left, r2, res);
    differenceRB(r1->right, r2, res);
}

// ==================== MAIN ====================
int main() {
    SplayNode* s1 = nullptr, * s2 = nullptr;
    RBNode* r1 = nullptr, * r2 = nullptr;
    int n, m, key, K;

    cout << "Number of keys for T1: "; cin >> n;
    cout << "Enter keys for T1:\n";
    for (int i = 0; i < n; i++) { cin >> key; insertSplay(s1, key); insertRB(r1, key); }

    cout << "Number of keys for T2: "; cin >> m;
    cout << "Enter keys for T2:\n";
    for (int i = 0; i < m; i++) { cin >> key; insertSplay(s2, key); insertRB(r2, key); }

    cout << "Enter K: "; cin >> K;

    // TASK A
    cout << "\n========== TASK A ==========\n";
    cout << "Splay tree contains K: " << (searchSplay(s1, K) ? "YES" : "NO") << "\n";
    cout << "Red-black tree contains K: " << (searchRB(r1, K) ? "YES" : "NO") << "\n";
    cout << "\nSplay T1:\n"; printSplay(s1);
    cout << "\nSplay T2:\n"; printSplay(s2);
    cout << "\nRB T1:\n"; printRB(r1);
    cout << "\nRB T2:\n"; printRB(r2);

    // TASK B
    cout << "\n========== TASK B ==========\n";
    SplayNode* newSplay = nullptr; RBNode* newRB = nullptr;
    differenceSplay(s1, s2, newSplay);
    differenceRB(r1, r2, newRB);
    freeSplay(s1); freeRB(r1);
    s1 = newSplay; r1 = newRB;
    cout << "\nSplay T1 (Diff):\n"; printSplay(s1);
    cout << "\nSplay T2:\n"; printSplay(s2);
    cout << "\nRB T1 (Diff):\n"; printRB(r1);
    cout << "\nRB T2:\n"; printRB(r2);

    // TASK V
    cout << "\n========== TASK V ==========\n";
    SplayNode* splayCopy = copySplay(s1);
    int ds = depthSplay(s1, K), dr = depthRB(r1, K);
    cout << "Depth of K in Splay: " << (ds == -1 ? "absent" : to_string(ds)) << "\n";
    cout << "Depth of K in RB: " << (dr == -1 ? "absent" : to_string(dr)) << "\n";
    cout << "\nCopy of Splay T1:\n"; printSplay(splayCopy);
    cout << "\nSplay T1:\n"; printSplay(s1);
    cout << "\nSplay T2:\n"; printSplay(s2);
    cout << "\nRB T1:\n"; printRB(r1);
    cout << "\nRB T2:\n"; printRB(r2);

    // TASK G
    cout << "\n========== TASK G ==========\n";
    cout << "RB T2 (before conversion):\n"; printRB(r2);
    Node234* tree234 = convert234(r2);
    cout << "\nRB T2 (after conversion):\n"; printRB(r2);
    cout << "\nConverted 2-3-4 tree from RB T2:\n";
    if (!tree234) cout << "Tree is empty.\nHeight: 0\n";
    else { cout << "No.\tLevel\tKeys\n"; print234(tree234); cout << "Height: " << height234(tree234) << "\n"; }

    // Cleanup
    freeSplay(s1); freeSplay(s2); freeSplay(splayCopy);
    freeRB(r1); freeRB(r2); free234(tree234);
    return 0;
}