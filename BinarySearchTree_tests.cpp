// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <sstream>

using namespace std;

void insert_helper(BinarySearchTree<int> &tree) {

    tree.insert(5);

    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.height() == 1);

    ASSERT_TRUE(tree.find(5) != tree.end());

    tree.insert(7);
    tree.insert(2);
    tree.insert(10);
    tree.insert(9);
    tree.insert(1);
    tree.insert(3);
    tree.insert(4);
}

TEST(bst_simple_test) {
    BinarySearchTree<int> tree;
    BinarySearchTree<int>::Iterator null;
    ASSERT_TRUE(tree.size() == 0);
    ASSERT_TRUE(tree.height() == 0);
    ASSERT_TRUE(tree.empty());
    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(tree.max_element() == null);
    ASSERT_TRUE(tree.min_element() == null);
    ASSERT_TRUE(tree.min_greater_than(5) == null);
    ASSERT_TRUE(tree.find(100) == null);

    tree.insert(5);

    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.height() == 1);
    ASSERT_FALSE(tree.empty());

    ASSERT_TRUE(tree.find(5) != tree.end());
    ASSERT_TRUE(tree.find(100) == null);

    tree.insert(7);
    tree.insert(3);
    ASSERT_FALSE(tree.empty());

    ASSERT_TRUE(tree.size() == 3);
    ASSERT_TRUE(tree.height() == 2);

    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(*tree.max_element() == 7);
    ASSERT_TRUE(*tree.min_element() == 3);
    ASSERT_TRUE(*tree.min_greater_than(5) == 7);
    ostringstream oss_tree;
    oss_tree << tree;
    ASSERT_TRUE(oss_tree.str() == "[ 3 5 7 ]")

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");
}

TEST(bst_insert_test) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());

    tree.insert(5);

    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.height() == 1);

    ASSERT_TRUE(tree.find(5) != tree.end());

    tree.insert(7);
    tree.insert(2);
    tree.insert(10);
    tree.insert(9);
    tree.insert(1);
    tree.insert(3);
    tree.insert(4);
    tree.insert(6);
    tree.insert(8);

    ASSERT_TRUE(tree.size() == 10);
    ASSERT_TRUE(tree.height() == 5);

    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(*tree.max_element() == 10);
    ASSERT_TRUE(*tree.min_element() == 1);
    ASSERT_TRUE(*tree.min_greater_than(1) == 2);
    ASSERT_TRUE(*tree.min_greater_than(8) == 9);
    ASSERT_TRUE(*tree.min_greater_than(7) == 8);
    BinarySearchTree<int>::Iterator null;
    ASSERT_TRUE(tree.min_greater_than(10) == null);
    ostringstream oss_tree;
    oss_tree << tree;
    ASSERT_TRUE(oss_tree.str() == "[ 1 2 3 4 5 6 7 8 9 10 ]")

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "5 2 1 3 4 7 6 10 9 8 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "1 2 3 4 5 6 7 8 9 10 ");
}

TEST(bst_find_test) { //
    BinarySearchTree<int> tree;

    insert_helper(tree);

    ASSERT_TRUE(tree.size() == 8);
    ASSERT_TRUE(tree.height() == 4);

    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(*tree.max_element() == 10);
    ASSERT_TRUE(*tree.min_element() == 1);
    ASSERT_TRUE(*tree.min_greater_than(1) == 2);
    ASSERT_TRUE(*tree.min_greater_than(8) == 9);
    ASSERT_TRUE(*tree.min_greater_than(7) == 9);
    BinarySearchTree<int>::Iterator null;
    ASSERT_TRUE(tree.min_greater_than(10) == null);
    ostringstream oss_tree;
    oss_tree << tree;
    ASSERT_TRUE(oss_tree.str() == "[ 1 2 3 4 5 7 9 10 ]")

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "5 2 1 3 4 7 10 9 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_TRUE(oss_inorder.str() == "1 2 3 4 5 7 9 10 ");

    ASSERT_TRUE(*tree.find(7) == 7);
    ASSERT_TRUE(*tree.find(2) == 2);
    ASSERT_TRUE(*tree.find(10) == 10);
    ASSERT_TRUE(*tree.find(9) == 9);
    ASSERT_TRUE(*tree.find(1) == 1);
    ASSERT_TRUE(*tree.find(3) == 3);
    ASSERT_TRUE(*tree.find(4) == 4);
    ASSERT_TRUE(tree.find(100) == null);
}

TEST(bst_copy_test) { //
    BinarySearchTree<int> tree;

    insert_helper(tree);


    BinarySearchTree<int> tree_cpy;

    tree_cpy = tree;

    ASSERT_TRUE(tree_cpy.size() == 8);
    ASSERT_TRUE(tree_cpy.height() == 4);

    ASSERT_TRUE(tree_cpy.check_sorting_invariant());
    ASSERT_TRUE(*tree_cpy.max_element() == 10);
    ASSERT_TRUE(*tree_cpy.min_element() == 1);
    ASSERT_TRUE(*tree_cpy.min_greater_than(1) == 2);
    ASSERT_TRUE(*tree_cpy.min_greater_than(8) == 9);
    ASSERT_TRUE(*tree_cpy.min_greater_than(7) == 9);
    BinarySearchTree<int>::Iterator null;
    ASSERT_TRUE(tree_cpy.min_greater_than(10) == null);
    ostringstream oss_tree;
    oss_tree << tree_cpy;
    ASSERT_TRUE(oss_tree.str() == "[ 1 2 3 4 5 7 9 10 ]")

    ostringstream oss_preorder;
    tree_cpy.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "5 2 1 3 4 7 10 9 ");

    ostringstream oss_inorder;
    tree_cpy.traverse_inorder(oss_inorder);
    ASSERT_TRUE(oss_inorder.str() == "1 2 3 4 5 7 9 10 ");

    ASSERT_TRUE(*tree_cpy.find(7) == 7);
    ASSERT_TRUE(*tree_cpy.find(2) == 2);
    ASSERT_TRUE(*tree_cpy.find(10) == 10);
    ASSERT_TRUE(*tree_cpy.find(9) == 9);
    ASSERT_TRUE(*tree_cpy.find(1) == 1);
    ASSERT_TRUE(*tree_cpy.find(3) == 3);
    ASSERT_TRUE(*tree_cpy.find(4) == 4);
    ASSERT_TRUE(tree_cpy.find(100) == null);
}

TEST(test_sort_invariant) { //
    BinarySearchTree<int> tree;
    
    insert_helper(tree);


    *tree.begin() = 2;
    
    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "5 2 2 3 4 7 10 9 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_TRUE(oss_inorder.str() == "2 2 3 4 5 7 9 10 ");


    ASSERT_FALSE(tree.check_sorting_invariant());

    BinarySearchTree<int> tree2;
    tree2.insert(5);
    tree2.insert(7);
    tree2.insert(2);
    tree2.insert(10);
    tree2.insert(9);
    tree2.insert(1);
    tree2.insert(3);
    tree2.insert(4);

    *++tree2.begin() = 20;

    ostringstream oss_preorder_2;
    tree2.traverse_preorder(oss_preorder_2);
    ASSERT_EQUAL(oss_preorder_2.str(), "5 20 1 3 4 7 10 9 ");

    ostringstream oss_inorder_2;
    tree2.traverse_inorder(oss_inorder_2);
    ASSERT_TRUE(oss_inorder_2.str() == "1 20 3 4 5 7 9 10 ");
}

TEST_MAIN()
