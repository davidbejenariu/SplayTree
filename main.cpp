#include <fstream>

using namespace std;

ifstream fin("abce.in");
ofstream fout("abce.out");

template<typename T>
struct Node {
    T m_key;
    Node *m_parent;
    Node *m_left, *m_right;

    Node() : m_key(), m_parent(nullptr), m_left(nullptr), m_right(nullptr) {}
    explicit Node(T key, Node *parent = nullptr, Node *left = nullptr, Node *right = nullptr) :
            m_key(key), m_parent(parent), m_left(left), m_right(right) {}

    explicit Node(Node *node) {
        m_key = node->m_key;
        m_parent = node->m_parent;
        m_left = node->m_left;
        m_right = node->m_right;
    }
};

template<typename T>
class SplayTree {
public:
    SplayTree() : m_root(nullptr), m_size(0) {}

    Node<T> *getRoot() {
        return m_root;
    }

    /// find

    bool find(const T &key) {
        Node<T> *found = findNode(key);

        if (found) {
            splay(found);
            return true;
        }

        return false;
    }

    /// insert

    void insert(const T key) {
        if (!findNode(key)) {
            auto *newNode = new Node<T>(key);
            Node<T> *current = m_root;
            Node<T> *parent = nullptr;

            ++m_size;

            while (current) {
                parent = current;

                if (key < current->m_key) {
                    current = current->m_left;
                } else {
                    current = current->m_right;
                }

                newNode->m_parent = parent;
            }

            if (!parent) {
                m_root = newNode;
            } else {
                if (newNode->m_key < parent->m_key) {
                    parent->m_left = newNode;
                } else {
                    parent->m_right = newNode;
                }

                splay(newNode);
            }
        }
    }

    /// erase

    void erase(T key) { // top-down deletion
        if (m_root) {
            Node<T> *found = findNode(key);

            if (found) {
                splay(found);

                if (!m_root->m_left) { // daca subarborele stang e null, fiul drept devine radacina
                    m_root = m_root->m_right;
                    delete found;
                } else { // facem join
                    Node<T> *rightSubtree = m_root->m_right;
                    m_root = m_root->m_left;

                    delete found;
                    m_root->m_parent = nullptr;

                    Node<T> *maxLeftSubtree = maximum(m_root);
                    splay(maxLeftSubtree);

                    if (rightSubtree) {
                        rightSubtree->m_parent = m_root;
                    }

                    m_root->m_right = rightSubtree;
                }

                --m_size;
            }
        }
    }

    /// successor and predecessor

    T successor(T key) {
        Node<T> *current = m_root;
        Node<T> *ant = nullptr;

        while (current) {
            ant = current;

            if (key < current->m_key) {
                current = current->m_left;
            } else if (key > current->m_key) {
                current = current->m_right;
            } else {
                return current->m_key;
            }
        }

        if (ant->m_key > key) {
            return ant->m_key;
        } else {
            return successor(ant)->m_key;
        }
    }

    T predecessor(T key) {
        Node<T> *current = m_root;
        Node<T> *ant = nullptr;

        while (current) {
            ant = current;

            if (key < current->m_key) {
                current = current->m_left;
            } else if (key > current->m_key) {
                current = current->m_right;
            } else {
                return current->m_key;
            }
        }

        if (ant->m_key < key) {
            return ant->m_key;
        } else {
            return predecessor(ant)->m_key;
        }
    }

    /// print elements in sorted order

    void inorder(Node<T> *node, int x, int y) {
        if (node->m_left) {
            inorder(node->m_left, x, y);
        }

        if (node->m_key >= x && node->m_key <= y) {
            fout << node->m_key << ' ';
        }

        if (node->m_right) {
            inorder(node->m_right, x, y);
        }
    }

    void printSorted(int x, int y) {
        inorder(m_root, x, y);
    }

private:
    Node<T> *m_root;
    int m_size;

    /// rotations

    void rotateRight(Node<T> *node) {
        Node<T> *child = node->m_left;

        node->m_left = child->m_right;

        if (node->m_left) {
            node->m_left->m_parent = node;
        }

        child->m_right = node;
        child->m_parent = node->m_parent;

        if (node->m_parent) {
            if (node->m_key < node->m_parent->m_key) {
                node->m_parent->m_left = child;
            } else {
                node->m_parent->m_right = child;
            }
        }

        node->m_parent = child;
    }

    void rotateLeft(Node<T> *node) {
        Node<T> *child = node->m_right;

        node->m_right = child->m_left;

        if (node->m_right) {
            node->m_right->m_parent = node;
        }

        child->m_left = node;
        child->m_parent = node->m_parent;

        if (node->m_parent) {
            if (node->m_key < node->m_parent->m_key) {
                node->m_parent->m_left = child;
            } else {
                node->m_parent->m_right = child;
            }
        }

        node->m_parent = child;
    }

    /// splay

    void splay(Node<T> *node) {
        while (node->m_parent) {
            if (node->m_parent == m_root) {
                if (node == node->m_parent->m_left) { // zig rotation
                    rotateRight(node->m_parent);
                } else { // zag rotation
                    rotateLeft(node->m_parent);
                }
            } else {
                Node<T> *p = node->m_parent;
                Node<T> *g = p->m_parent;

                if (node == node->m_parent->m_left && p == p->m_parent->m_left) { // zig-zig rotation
                    rotateRight(g);
                    rotateRight(p);
                } else if (node == node->m_parent->m_right && p == p->m_parent->m_right) { // zag-zag rotation
                    rotateLeft(g);
                    rotateLeft(p);
                } else if (node == node->m_parent->m_left && p == p->m_parent->m_right) { // zig-zag rotation
                    rotateRight(p);
                    rotateLeft(g);
                } else { // zag-zig rotation
                    rotateLeft(p);
                    rotateRight(g);
                }
            }
        }

        m_root = node;
    }

    /// find

    Node<T> *findNode(T key) {
        Node<T> *current = m_root;
        Node<T> *ant = nullptr;

        while (current) {
            ant = current;

            if (key < current->m_key) {
                current = current->m_left;
            } else if (key > current->m_key) {
                current = current->m_right;
            } else {
                return current;
            }
        }

        if (ant) {
            splay(ant);
        }

        return nullptr;
    }

    /// successor and predecessor

    Node<T> *minimum(Node<T> *node) {
        Node<T> *temp = node;

        if (temp->m_left) {
            do {
                temp = temp->m_left;
            }
            while (temp->m_left);
        }

        splay(temp);
        return temp;
    }

    Node<T> *maximum(Node<T> *node) {
        Node<T> *temp = node;

        if (temp->m_right) {
            do {
                temp = temp->m_right;
            }
            while (temp->m_right);
        }

        splay(temp);
        return temp;
    }

    Node<T> *successor(Node<T> *node) {
        if (node->m_right) {
            return minimum(node->m_right);
        }

        Node<T> *p = node->m_parent;

        if (p) {
            while (p->m_right == node) {
                node = p;
                p = p->m_parent;

                if (p == nullptr) {
                    break;
                }
            }
        }

        splay(p);
        return p;
    }

    Node<T> *predecessor(Node<T> *node) {
        if (node->m_left) {
            return maximum(node->m_left);
        }

        Node<T> *p = node->m_parent;

        if (p) {
            while (p->m_left == node) {
                node = p;
                p= p->m_parent;

                if (p == nullptr) {
                    break;
                }
            }
        }

        splay(p);
        return p;
    }
};

int main() {
    int nrCom;
    int com, x, y;
    SplayTree<int> tree;

    fin >> nrCom;

    for (int i = 0; i < nrCom; ++i) {
        fin >> com >> x;

        if (com == 1) {
            tree.insert(x);
        } else if (com == 2) {
            tree.erase(x);
        } else if (com == 3) {
            fout << tree.find(x) << '\n';
        } else if (com == 4) {
            fout << tree.predecessor(x) << '\n';
        } else if (com == 5) {
            fout << tree.successor(x) << '\n';
        } else {
            fin >> y;
            tree.printSorted(x, y);
            fout << '\n';
        }
    }
}
