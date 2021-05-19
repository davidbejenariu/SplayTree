#include <iostream>
#include "SplayTree.h"

using namespace std;

/**
 * main function for testing class methods such as insertion, deletion, find, predecessor, succesor and
 * inorder traversal
 *
 * @return
 */

int main() {
    SplayTree<int> tree;

    tree.insert(3);
    tree.insert(5);
    tree.insert(1);
    tree.insert(3);

    cout << tree.getRoot()->m_key << ' ' << tree.getRoot()->m_left << ' ' << tree.getRoot()->m_right
        << ' ' << tree.getRoot()->m_parent << '\n';

    cout << tree.find(5) << '\n';
    cout << tree.getRoot()->m_key << ' ' << tree.getRoot()->m_left << ' ' << tree.getRoot()->m_right
         << ' ' << tree.getRoot()->m_parent << '\n';

    cout << tree.find(0) << '\n';
    cout << tree.getRoot()->m_key << ' ' << tree.getRoot()->m_left << ' ' << tree.getRoot()->m_right
         << ' ' << tree.getRoot()->m_parent << '\n';

    cout << tree.predecessor(3) << '\n';

    tree.erase(3);

    cout << tree.find(3) << '\n';

    tree.insert(4);
    tree.insert(10);
    tree.insert(2);
    tree.insert(6);
    tree.insert(9);
    tree.insert(201);
    tree.insert(2);
    tree.insert(22);
    tree.insert(222);

    tree.printSorted();
    cout << '\n';

    tree.erase(10);

    tree.printSorted();

    return 0;
}
