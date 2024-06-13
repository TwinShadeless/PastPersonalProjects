// AVLSet.hpp
//
// ICS 46 Spring 2020
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.h"
#include <iostream>


template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    ~AVLSet() noexcept override;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const noexcept;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    bool balancing;
    class Node {
    public:
        ElementType val;
        Node* parent;
        Node* left;
        Node* right;
        bool balancing;

        void balance();
        Node();
        Node(const ElementType& val, bool balancing);
        ~Node() noexcept;
        void add(const ElementType& element);
        bool contains(const ElementType& element) const;
        int height() const noexcept;
        void preorder(VisitFunction visit) const;
        void inorder(VisitFunction visit) const;
        void postorder(VisitFunction visit) const;
        void copy(Node& n);
    };
    Node* head;
    int tSize;
    int tHeight;

    // You'll no doubt want to add member variables and "helper" member
    // functions here.
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
    //Standard initializer
    this->head = nullptr;
    this->tSize = 0;
    this->tHeight = -1;
    this->balancing = shouldBalance;
}
template <typename ElementType>
AVLSet<ElementType>::Node::Node() {}

template <typename ElementType>
AVLSet<ElementType>::Node::Node(const ElementType& val, bool balancing) {
    //Node initializer
    this->val = val;
    this->parent = nullptr;
    this->left = nullptr;
    this->right = nullptr;
    this->balancing = balancing;
}

template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    //The destructor just deletes the head because the Node destructor is set up to delete its children first and prevent memory leakage
    delete this->head;
}

template <typename ElementType>
AVLSet<ElementType>::Node::~Node() noexcept
{
    //Check if children exist, then call the destructor on them before deleting self
    if (this->left != nullptr) {
        delete this->left;
        this->left = nullptr;
    }
    if (this->right != nullptr) {
        delete this->right;
        this->right = nullptr;
    }
    this->parent = nullptr;
}

//Creates a deep copy of an AVLSet. The node copy function is set up recursively so it just needs to be called on the head.
template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    if (s.head == nullptr) {
        this->head = nullptr;
        this->tSize = 0;
        this->tHeight = -1;
        this->balancing = s.balancing;
    }
    else {
        this->balancing = s.balancing;
        this->tSize = s.tSize;
        this->head = new Node;
        this->head->copy(*s.head);
    }
}

//Creates a deep copy of a node and all of its children
template <typename ElementType>
void AVLSet<ElementType>::Node::copy(Node& n) {
    this->val = n.val;
    if (n.left != nullptr) {
        this->left = new Node;
        this->left->parent = this;
        this->left->copy(*n.left);
    }
    if (n.right != nullptr) {
        this->right = new Node;
        this->right->parent = this;
        this->right->copy(*n.right);
    }
}

//Creates a copy of an expiring AVLSet. Just swaps the contents of both sets under the assumption that the input one is going to expire and will be destroyed.
template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept : balancing{ true }, head{ nullptr }, tSize{ 0 }
{
    Node* tempH = this->head;
    bool tempB = this->balancing;
    bool tempTS = this->tSize;
    int tempTH = this->tHeight;

    this->head = s.head;
    this->balancing = s.balancing;
    this->tSize = s.tSize;
    this->tHeight = s.tHeight;

    s.head = tempH;
    s.balancing = tempB;
    s.tSize = tempTS;
    s.tHeight = tempTH;

    tempH = nullptr;
}


//Overridden assignment operator. Deletes the old contents and then deep copies the contents of the input set.
template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    delete this->head;
    this->head = new Node;
    this->head->copy(*s.head);
    this->tSize = s.tSize;
    this->balancing = s.balancing;
    this->tHeight = s.tHeight;

    return *this;
}

//Overridden assignment operator for an expiring set. Swaps the contents like AVLSet(AVLSet&& s)
template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    Node* tempH = this->head;
    bool tempB = this->balancing;
    int tempTS = this->tSize;
    int tempTH = this->tHeight;

    this->head = s.head;
    this->balancing = s.balancing;
    this->tSize = s.tSize;
    this->tHeight = s.tHeight;

    s.head = tempH;
    s.balancing = tempB;
    s.tSize = tempTS;
    s.tHeight = tempTH;

    tempH = nullptr;
    return *this;
}

//Function implemented for the instructor to check if the tree is self balancing. True if it balances, false if it's just a binary tree.
template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

//Function adds new elements to the tree. If the current size is 0, it just generates a new node to be the head and returns. If not, it calls a recursive helper method
//from the Node class to find where the new element needs to go and re-balance the tree, then if the rebalancing rearranged the tree in such a way that the old root of
//the tree is no longer the root, the function iterates up to the top of the tree from the old root and sets it as the new root.
template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    if (this->head == nullptr) {
        this->head = new Node(element, this->balancing);
        this->tSize = 1;
        this->tHeight = 0;
        return;
    }
    this->head->add(element);
    this->tSize += 1;
    while (this->head->parent != nullptr) {
        this->head = this->head->parent;
    }
    this->tHeight = this->head->height();

}

//Recursive helper method for the Node class. Given an element to add, the function iterates down to where the Node should be placed using the comparison operator and
//calling add(). Once a spot for the new Node is found, it is placed and then the function iterates back up the tree, calling balance() on every node along the way.
template <typename ElementType>
void AVLSet<ElementType>::Node::add(const ElementType& element)
{
    if (element < this->val) {
        if (this->left == nullptr) {
            Node* newTree = new Node(element, this->balancing);
            newTree->parent = this;
            this->left = newTree;
        }
        else {
            this->left->add(element);
            if (this->balancing) {
                this->balance();
            }
        }
    }
    if (element > this->val) {
        if (this->right == nullptr) {
            Node* newTree = new Node(element, this->balancing);
            newTree->parent = this;
            this->right = newTree;
        }
        else {
            this->right->add(element);
            if (this->balancing) {
                this->balance();
            }
        }
    }
}

//A contains() method also exists for the Node class, so this function just calls that method on the root node.
template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    return this->head->contains(element);
}

//Function recursively iterates down to where the element should be using the comparison operator. This function takes O(logn) time.
template <typename ElementType>
bool AVLSet<ElementType>::Node::contains(const ElementType& element) const {
    if (this->val == element) {
        return true;
    }

    if (this->val > element) {
        if (this->left == nullptr) {
            return false;
        }
        else {
            return this->left->contains(element);
        }
    }

    if (this->val < element) {
        if (this->right == nullptr) {
            return false;
        }
        else {
            return this->right->contains(element);
        }
    }

    return false;
}

//Tree size is stored as a class attribute, so this function only takes O(1) time
template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return this->tSize;
}

//Same with this one
template <typename ElementType>
int AVLSet<ElementType>::height() const noexcept
{
    return this->tHeight;
}

//A private function used to update tHeight whenever add() or balance() are called. This function does have to iterate across the whole tree, so it takes O(n) time
template <typename ElementType>
int AVLSet<ElementType>::Node::height() const noexcept
{
    if (this->left == nullptr && this->right == nullptr) {
        return 0;
    }
    if (this->left == nullptr) {
        return 1 + this->right->height();
    }
    if (this->right == nullptr) {
        return 1 + this->left->height();
    }
    int heightLeft = this->left->height() + 1;
    int heightRight = this->right->height() + 1;
    if (heightLeft >= heightRight) {
        return heightLeft;
    }
    else {
        return heightRight;
    }
}

//The next 8 functions are all methods for traversing every node in the tree in different orders, all standard different traversals of a tree. The 
//instructor for this assignment decided to have us call a written in function on each method for his testing purposes, but these functions could
//easily be configured to simply print the traversed values to the console instead.
template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    this->head->preorder(visit);
}

template <typename ElementType>
void AVLSet<ElementType>::Node::preorder(VisitFunction visit) const
{
    visit(this->val);
    if (this->left != nullptr) {
        this->left->preorder(visit);
    }
    if (this->right != nullptr) {
        this->right->preorder(visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    this->head->inorder(visit);
}

template <typename ElementType>
void AVLSet<ElementType>::Node::inorder(VisitFunction visit) const
{
    if (this->left != nullptr) {
        this->left->inorder(visit);
    }
    visit(this->val);
    if (this->right != nullptr) {
        this->right->inorder(visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    this->head->postorder(visit);
}

template <typename ElementType>
void AVLSet<ElementType>::Node::postorder(VisitFunction visit) const
{
    if (this->left != nullptr) {
        this->left->postorder(visit);
    }
    if (this->right != nullptr) {
        this->right->postorder(visit);
    }
    visit(this->val);
}

//The meat and potatoes of the class. This function rebalances the tree so that lookup times are always O(logn). A node is unbalanced 
//if the difference between the height of its left and right subtrees is greater than 1. There are 4 cases where a node needs to be
//rebalanced, called LL, LR, RL, and RR. A visualization of each where X, Y, and Z are nodes being rebalanced and T1, T2, T3, and T4 are
//the already-balanced, sub-trees of those nodes with no more than a difference of 1 between their heights (they can be empty as well):
//
//       LL          LR           RL         RR
//       Z           Z            Z          Z
//      / \         / \          / \        / \
//     Y  T4       Y  T4        T1  Y      T1  Y
//    / \         / \              / \        / \
//   X  T3       T1  X            X  T4      T2  X
//  / \             / \          / \            / \
// T1 T2           T2 T3        T2 T3          T3 T4
//    
//LL and RR cases are the balanced by "rotating" the nodes like so:
// 
//        LL             RR
//        Y               Y
//     /     \         /     \
//    X       Z       Z       X
//   / \     / \     / \     / \
//  T1  T2  T3 T4   T1 T2   T3 T4
//
//LR and RL are solved by performing a rotation to turn them into LL/RR cases, then solving them the same way
// 
//    LR -> LL     RL -> RR
//       Z            Z
//      / \          / \
//     X  T4        T1  X
//    / \              / \
//   Y  T3            T2  Y
//  / \                  / \
// T1  T2               T3 T4
//
//Despite its length, this function consists mostly of pointer assignment to replicate the above rotations in code.
template <typename ElementType>
void AVLSet<ElementType>::Node::balance()
{
    int heightLeft = 0;
    int heightRight = 0;
    if (this->left != nullptr) {
        heightLeft = this->left->height() + 1;
    }
    if (this->right != nullptr) {
        heightRight = this->right->height() + 1;
    }

    //L cases
    if (heightLeft - heightRight > 1) {
        Node* tempZ = this;
        Node* tempY = this->left;
        Node* tempX;
        Node* tempT0;
        Node* tempT1;
        Node* tempT2;
        Node* tempT3;
        Node* tempT4;
        int sHeightLeft = 0;
        int sHeightRight = 0;

        if (this->left->left != nullptr) {
            sHeightLeft = this->left->left->height() + 1;
        }
        if (this->left->right != nullptr) {
            sHeightRight = this->left->right->height() + 1;
        }
        //Turns LR case into LL case
        if (sHeightRight > sHeightLeft) {
            tempX = this->left->right;
            tempT0 = this->parent;
            tempT1 = this->left->left;
            tempT2 = this->left->right->left;
            tempT3 = this->left->right->right;
            tempT4 = this->right;

            tempZ->left = tempX;
            tempY->parent = tempX;
            tempY->right = tempT2;
            tempX->parent = tempZ;
            tempX->left = tempY;

            if (tempT2 != nullptr) {
                tempT2->parent = tempY;
            }
        }
        //Balances LL case
        tempY = this->left;
        tempX = this->left->left;
        tempT0 = this->parent;
        tempT1 = this->left->left->left;
        tempT2 = this->left->left->right;
        tempT3 = this->left->right;
        tempT4 = this->right;

        tempZ->parent = tempY;
        tempZ->left = tempT3;
        tempY->parent = tempT0;
        tempY->right = tempZ;

        if (tempT0 != nullptr) {
            if (tempT0->left == tempZ) {
                tempT0->left = tempY;
            }
            else {
                tempT0->right = tempY;
            }
        }
        if (tempT3 != nullptr) {
            tempT3->parent = tempZ;
        }

        tempX = nullptr;
        tempY = nullptr;
        tempZ = nullptr;
        tempT0 = nullptr;
        tempT1 = nullptr;
        tempT2 = nullptr;
        tempT3 = nullptr;
        tempT4 = nullptr;
    }
    //R cases
    if (heightRight - heightLeft > 1) {
        Node* tempZ = this;
        Node* tempY = this->right;
        Node* tempX;
        Node* tempT0;
        Node* tempT1;
        Node* tempT2;
        Node* tempT3;
        Node* tempT4;
        int sHeightLeft = 0;
        int sHeightRight = 0;

        if (this->right->left != nullptr) {
            sHeightLeft = this->right->left->height() + 1;
        }
        if (this->right->right != nullptr) {
            sHeightRight = this->right->right->height() + 1;
        }
        //RL -> RR case
        if (sHeightLeft > sHeightRight) {
            tempX = this->right->left;
            tempT0 = this->parent;
            tempT1 = this->left;
            tempT2 = this->right->left->left;
            tempT3 = this->right->left->right;
            tempT4 = this->right->right;

            tempX->parent = tempZ;
            tempX->right = tempY;
            tempY->parent = tempX;
            tempY->left = tempT3;
            tempZ->right = tempX;

            if (tempT3 != nullptr) {
                tempT3->parent = tempY;
            }
        }

        //Balances RR case
        tempY = this->right;
        tempZ = this;
        tempT0 = this->parent;
        tempT1 = this->left;
        tempT2 = this->right->left;
        tempT3 = this->right->right->left;
        tempT4 = this->right->right->right;

        tempY->parent = tempT0;
        tempY->left = tempZ;
        tempZ->parent = tempY;
        tempZ->right = tempT2;

        if (tempT0 != nullptr) {
            if (tempT0->left == tempZ) {
                tempT0->left = tempY;
            }
            if (tempT0->right == tempZ) {
                tempT0->right = tempY;
            }
        }

        if (tempT2 != nullptr) {
            tempT2->parent = tempZ;
        }

        tempX = nullptr;
        tempY = nullptr;
        tempZ = nullptr;
        tempT0 = nullptr;
        tempT1 = nullptr;
        tempT2 = nullptr;
        tempT3 = nullptr;
        tempT4 = nullptr;
    }
}
#endif // AVLSET_HPP

