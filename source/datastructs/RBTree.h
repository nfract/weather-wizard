#ifndef SRKWW_RBTREE_H
#define SRKWW_RBTREE_H

#include <iostream>
#include <vector>
using namespace std;


enum Color{RED, BLACK};


// Store station code and data
struct PrecipitationNormal
{
    string stationCode;
    float normalAverageForYear = 0;
    float* normalAverageByMonth = {};
};


// Store data for each location/station
struct Node
{
public:
    Color color;
    Node* parent;
    Node* left;
    Node* right;
    PrecipitationNormal thisYear;

    Node(string place, float level, float* arr) {
        color = RED;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        thisYear.stationCode = place;
        thisYear.normalAverageForYear = level;
        thisYear.normalAverageByMonth = arr;
    }
};


class RBTree
{
private:
    Node* treeRoot;

    bool leftNeeded, rightNeeded, leftRightNeeded, rightLeftNeeded;

    // Balance nodes
    Node* Balance(Node* root, string place, float level, float* arr)
    {
        bool redRedConflict=false;

        if (root == nullptr)
            return new Node(place,level,arr);
        else if (root->thisYear.normalAverageForYear > level)
        {
            root->left = Balance(root->left,place,level,arr);
            root->left->parent = root;
            if(root != treeRoot)
            {
                if(root->color==RED && root->left->color==RED)
                    redRedConflict = true;
            }
        }
        else
        {
            root->right = Balance(root->right, place, level, arr);
            root->right->parent = root;
            if(root != treeRoot)
            {
                if(root->color==RED && root->right->color==RED)
                    redRedConflict = true;
            }
        }

        if (leftNeeded)
        {
            root = RotateLeft(root);
            root->color = BLACK;
            root->left->color = RED;
            leftNeeded = false;
        }
        else if (rightNeeded)
        {
            root = RotateRight(root);
            root->color = BLACK;
            root->right->color = RED;
            rightNeeded  = false;
        }
        else if (rightLeftNeeded)
        {
            root->right = RotateRight(root->right);
            root->right->parent = root;
            root = RotateLeft(root);
            root->color = BLACK;
            root->left->color = RED;
            rightLeftNeeded = false;
        }
        else if (leftRightNeeded)
        {
            root->left = RotateLeft(root->left);
            root->left->parent = root;
            root = RotateRight(root);
            root->color = BLACK;
            root->right->color = RED;
            leftRightNeeded = false;
        }

        if (redRedConflict && root == root->parent->right)
        {
            if (root->parent->left == nullptr || root->parent->left->color == BLACK)
            {
                if (root->left != nullptr && root->left->color == RED)
                    rightLeftNeeded = true;
                else if (root->right != nullptr && root->right->color == RED)
                    leftNeeded = true;
            }
            else
            {
                root->parent->left->color = BLACK;
                root->color = BLACK;
                if (root->parent != treeRoot)
                    root->parent->color = RED;
            }
        }
        else if (redRedConflict)
        {
            if (root->parent->right == nullptr || root->parent->right->color == BLACK)
            {
                if (root->left != nullptr && root->left->color == RED)
                    rightNeeded = true;
                else if (root->right != nullptr && root->right->color == RED)
                    leftRightNeeded = true;
            }
            else
            {
                root->parent->right->color = BLACK;
                root->color = BLACK;
                if (root->parent != treeRoot)
                    root->parent->color = RED;
            }
        }

        return root;
    }

    // Perform left rotation
    Node* RotateLeft(Node* root)
    {
        Node* rightNode = root->right;
        Node* leftNode = root->right->left;
        rightNode->left = root;
        root->right = leftNode;
        root->parent = rightNode;

        if (leftNode!=nullptr)
            leftNode->parent = root;

        return rightNode;
    }

    // Perform right rotation
    Node* RotateRight(Node* root)
    {
        Node* leftNode = root->left;
        Node* rightNode = root->left->right;
        leftNode->right = root;
        root->left = rightNode;
        root->parent = leftNode;

        if (rightNode!=nullptr)
            rightNode->parent = root;

        return leftNode;
    }

    // Insert all nodes into passed in vector
    void GetAllNodes(Node* root, vector<Node*>& allNodes)
    {
        if (root != nullptr)
        {
            GetAllNodes(root->left, allNodes);
            allNodes.push_back(root);
            GetAllNodes(root->right, allNodes);
        }
    }


public:
    RBTree()
    {
        treeRoot = nullptr;
        leftNeeded,rightNeeded,leftRightNeeded,rightLeftNeeded = false;
    }

    // Insert nodes into RBTree
    void Insert(string place, float level, float* arr)
    {
        if (treeRoot == nullptr)
        {
            treeRoot = new Node(place, level, arr);
            treeRoot->color = BLACK;
            return;
        }

        treeRoot = Balance(treeRoot, place, level, arr);
    }

    // Returns vector of k nodes with highest normalAverage
    vector<Node*> ReturnMax(int k)
    {
        vector<Node*> allNodes;
        GetAllNodes(treeRoot, allNodes);
        vector<Node*> returnVector;

        for (int i=0; i<k; i++)
            returnVector.push_back(allNodes.at(allNodes.size()-1-i));

        return returnVector;
    }

    // Returns vector of k nodes with lowest normalAverage
    vector<Node*> ReturnMin(int k)
    {
        vector<Node*> allNodes;
        GetAllNodes(treeRoot, allNodes);
        vector<Node*> returnVector;

        for (int i=0; i<k; i++)
            returnVector.push_back(allNodes.at(i));

        return returnVector;
    }
};

#endif //SRKWW_RBTREE_H
