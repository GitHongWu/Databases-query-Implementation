/*avl.h*/

//
// AVL tree
//

#pragma once

#include <iostream>
#include <algorithm> // std::max
#include <stack>
#include <vector>
#include <cassert>

using namespace std;

template <typename TKey, typename TValue>
class avltree
{
private:
    struct NODE
    {
        TKey Key;
        TValue Value;
        int Height;
        NODE *Left;
        NODE *Right;
    };

    NODE *Root; // pointer to root node of tree (nullptr if empty)
    int Size;   // # of nodes in the tree (0 if empty)

    //
    // _inorder does the actual inorder traversal and output
    // to console.  Each key is output to the console followed
    // by " ", including the last key.
    //
    void _inorder(NODE *cur)
    {
        if (cur == nullptr)
            return;
        else
        {
            _inorder(cur->Left);
            cout << "(" << cur->Key << "," << cur->Value << "," << cur->Height << ")"
                 << " ";
            _inorder(cur->Right);
        }
    }

    void _inorder_keys(NODE *cur, vector<TKey> &V)
    {
        if (cur == nullptr)
            return;
        else
        {
            _inorder_keys(cur->Left, V);
            V.push_back(cur->Key);
            _inorder_keys(cur->Right, V);
        }
    }

    void _inorder_values(NODE *cur, std::vector<TValue> &V)
    {
        if (cur == nullptr)
            return;
        else
        {
            _inorder_values(cur->Left, V);
            V.push_back(cur->Value);
            _inorder_values(cur->Right, V);
        }
    }

    void _inorder_heights(NODE *cur, std::vector<int> &V)
    {
        if (cur == nullptr)
            return;
        else
        {
            _inorder_heights(cur->Left, V);
            V.push_back(cur->Height);
            _inorder_heights(cur->Right, V);
        }
    }

    //
    // _copytree:
    //
    // Given a tree, inserts the (key, value) pairs into *this* tree.
    //
    void _copytree(NODE *cur)
    {
        if (cur == nullptr)
            return;
        else
        {
            //
            // to copy and build an exact copy, we need to "visit" cur
            // first and insert that (key, value) pair first.  That way
            // all the nodes to the left will properly go to the left,
            // all the node to the right will properly go to the right.
            //
            insert(cur->Key, cur->Value);

            _copytree(cur->Left);
            _copytree(cur->Right);
        }
    }

    void _RotateToFix(NODE *Parent, NODE *N)
    {
        assert(N != nullptr);

        int hL = (N->Left == nullptr) ? -1 : N->Left->Height;
        int hR = (N->Right == nullptr) ? -1 : N->Right->Height;

        assert(abs(hL - hR) > 1);

        int hLL = -1, hLR = -1, hRR = -1, hRL = -1;
        //left-left case or left-right case
        if (N->Left != nullptr){
            hLL = (N->Left->Left == nullptr) ? -1 : N->Left->Left->Height;
            hLR = (N->Left->Right == nullptr) ? -1 : N->Left->Right->Height;
        }
        //right-right case or right-left case
        if(N->Right != nullptr){
            hRR = (N->Right->Right == nullptr) ? -1 : N->Right->Right->Height;
            hRL = (N->Right->Left == nullptr) ? -1 : N->Right->Left->Height;
            
        }

        if(hL > hR)    //rotate right
        {
            if(hLR > hLL)
                _LeftRotate(N, N->Left);
            _RightRotate(Parent, N);
        }
        else    //(hR > hL)    //rotate left
        {
            if(hRL > hRR)
                _RightRotate(N, N->Right);
            _LeftRotate(Parent, N);
        }
    }

    //
    // _RightRotate
    // _LeftRotate
    //
    // Rotates the tree around the node N, where Parent is N's parent.  Note that
    // Parent could be null, which means N is the root of the entire tree.  If
    // Parent denotes a node, note that N could be to the left of Parent, or to
    // the right.  You'll need to take all this into account when linking in the
    // new root after the rotation.  Don't forget to update the heights as well.
    //
    void _RightRotate(NODE *Parent, NODE *N)
    {

        assert(N != nullptr); // pre-conditions: N and L must exist
        assert(N->Left != nullptr);

        //
        // TODO
        //
        NODE *L = N->Left;
        NODE *B = L->Right;

        N->Left = B;
        L->Right = N;

        if (Parent == nullptr)
            Root = L;
        else if (L->Key < Parent->Key)
            Parent->Left = L;
        else
            Parent->Right = L;

        if (N->Left == nullptr && N->Right == nullptr)
            N->Height = 0;
        else if (N->Left == nullptr)
            N->Height = 1 + N->Right->Height;
        else if (N->Right == nullptr)
            N->Height = 1 + N->Left->Height;
        else
            N->Height = 1 + max(N->Left->Height, N->Right->Height);

        if (L->Left == nullptr)
            L->Height = 1 + L->Right->Height;
        else
            L->Height = 1 + max(L->Left->Height, L->Right->Height);
    }

    void _LeftRotate(NODE *Parent, NODE *N)
    {
        assert(N != nullptr); // pre-conditions: N and R must exist
        assert(N->Right != nullptr);

        //
        // TODO
        //
        
        NODE *R = N->Right;
        NODE *B = R->Left;

        N->Right = B;
        R->Left = N;

        if (Parent == nullptr)
            Root = R;
        else if (R->Key < Parent->Key)
            Parent->Left = R;
        else
            Parent->Right = R;

        if (N->Left == nullptr && N->Right == nullptr)
            N->Height = 0;
        else if (N->Left == nullptr)
            N->Height = 1 + N->Right->Height;
        else if (N->Right == nullptr)
            N->Height = 1 + N->Left->Height;
        else
            N->Height = 1 + max(N->Left->Height, N->Right->Height);

        if (R->Right == nullptr)
            R->Height = 1 + R->Left->Height;
        else
            R->Height = 1 + max(R->Left->Height, R->Right->Height);
    }

    bool _isBST(NODE *node)
    {
        if (node == nullptr)
            return true;

        /* false if left is > than node */
        if (node->Left != nullptr && node->Left->Key > node->Key)
            return false;

        /* false if right is < than node */
        if (node->Right != nullptr && node->Right->Key < node->Key)
            return false;

        /* false if, recursively, the left or right is not a BST */
        if (!_isBST(node->Left) || !_isBST(node->Right))
            return false;

        /* passing all that, it's a BST */
        return true;
    }

    bool _isBalanced(NODE *node){
        if(node == nullptr)
            return true;
        else{
            int hL, hR;
            if(node->Left == nullptr)
                hL = -1;
            else
                hL = node->Left->Height;
            
            if(node->Right == nullptr)
                hR = -1;
            else
                hR = node->Right->Height;

            if(abs(hL-hR) <= 1)
                return true;

            _isBalanced(node->Left);
            _isBalanced(node->Right);
        }
        return false;
    }

    bool _heightsCorrect(NODE* node){
        if(node == nullptr)
            return true;
        else{
            if(!_heightsCorrect(node->Left))
                return false;
            if(!_heightsCorrect(node->Right))
                return false;
            
            //children are correct
            int hL, hR;
            if(node->Left == nullptr)
                hL = -1;
            else
                hL = node->Left->Height;
            
            if(node->Right == nullptr)
                hR = -1;
            else
                hR = node->Right->Height;

            int newH = 1 + max(hL, hR);
            if(node->Height == newH)
                return true;
            else
                return false;
        }
    }

public:
    //
    // default constructor:
    //
    // Creates an empty tree.
    //
    avltree()
    {
        Root = nullptr;
        Size = 0;
    }

    //
    // copy constructor:
    //
    avltree(avltree &other)
    {
        Root = nullptr;
        Size = 0;

        _copytree(other.Root);
    }

    //destructor helper
    void DestroyRecursive(NODE *node)
    {
        if (node != nullptr)
        {
            DestroyRecursive(node->Left);
            DestroyRecursive(node->Right);
            delete node;
        }
    }
    //destructor
    ~avltree()
    {
        DestroyRecursive(Root);
    }

    //
    // size:
    //
    // Returns the # of nodes in the tree, 0 if empty.
    //
    int size()
    {
        return Size;
    }

    //
    // height
    //
    // Returns the height of tree; the height of an empty tree is
    // defined as -1.
    //
    int height()
    {
        if (Root == nullptr)
            return -1;
        else
            return Root->Height;
    }

    int height(NODE* node){
        if (node == nullptr)
            return -1;
        else
            return node->Height;
    }

    //
    // search:
    //
    // Searches the tree for the given key, returning a pointer to the
    // value if found and nullptr if not.  We return a pointer so the caller
    // may update the underlying value if needed.
    //
    TValue *search(TKey key)
    {
        NODE *cur = Root;
        while (cur != nullptr)
        {
            if (key == cur->Key) // already in tree
            {
                return &cur->Value;
            }

            if (key < cur->Key) // search left:
            {
                cur = cur->Left;
            }
            else
            {
                cur = cur->Right;
            }
        } //while

        // if get here, not found
        return nullptr;
    }

    //
    // insert
    //
    // Inserts the given (key, value) pair into the tree; if the key has
    // already been inserted then the function returns without changing
    // the tree.
    //
    void insert(TKey key, TValue value)
    {
        NODE *prev = nullptr;
        NODE *cur = Root;

        //
        // stack the nodes we visit so we can walk back up
        // the search path later, adjusting heights:
        //
        stack<NODE *> nodes;

        //
        // 1. Search to see if tree already contains key:
        //
        while (cur != nullptr)
        {
            if (key == cur->Key) // already in tree
                return;

            nodes.push(cur); // stack so we can return later:

            if (key < cur->Key) // search left:
            {
                prev = cur;
                cur = cur->Left;
            }
            else
            {
                prev = cur;
                cur = cur->Right;
            }
        } //while

        //
        // 2. if we get here, key is not in tree, so allocate
        // a new node to insert:
        //
        NODE *newNode;
        newNode = new NODE();
        newNode->Key = key;
        newNode->Value = value;
        newNode->Height = 0; // leaf node -> sub-tree of height 0:
        newNode->Left = nullptr;
        newNode->Right = nullptr;

        //
        // 3. link in the new node:
        //
        // NOTE: cur is null, and prev denotes node where
        // we fell out of the tree.  if prev is null, then
        // the tree is empty and the Root pointer needs
        // to be updated.
        //
        if (prev == nullptr)
            Root = newNode;
        else if (key < prev->Key)
            prev->Left = newNode;
        else
            prev->Right = newNode;

        //
        // 4. update size:
        //
        Size++;

        //
        // TODO!
        //
        //
        // 5. walk back up tree using stack and update heights:
        //
        // while (!nodes.empty())
        // {

        //     cur = nodes.top();
        //     nodes.pop();

        //     int heightLeft, heightRight;
        //     if (cur->Left == nullptr)
        //         heightLeft = -1;
        //     else
        //         heightLeft = cur->Left->Height;
        //     if (cur->Right == nullptr)
        //         heightRight = -1;
        //     else
        //         heightRight = cur->Right->Height;

        //     int newH = 1 + max(heightLeft, heightRight);

        //     cur->Height = newH;
        // }//my Code

        while (!nodes.empty())
        {
            cur = nodes.top();
            nodes.pop();

            //debug
            //cout << "stack pop: " << cur->Key << endl;

            int hL = (cur->Left == nullptr) ? -1 : cur->Left->Height;
            int hR = (cur->Right == nullptr) ? -1 : cur->Right->Height;
            int hCur = 1 + std::max(hL, hR);

            cur->Height = hCur; //update height

            if(_isBalanced(cur)){
                //debug
                //cout << "node key: " << cur->Key << " is avl, left height: " << height(cur->Left) << ", right height: " << height(cur->Right) << endl;
            }
            else{
                //debug
                //cout << "node key: " << cur->Key << " not avl, left height: " << height(cur->Left) << ", right height: " << height(cur->Right) << endl;
                if(nodes.empty()){
                    //parent is null
                    //debug
                    //cout << "empty stack" << endl;
                    _RotateToFix(nullptr, cur);
                }
                else{
                    //debug
                    //cout << "Parent: " << nodes.top()->Key << endl;
                    _RotateToFix(nodes.top(), cur);
                }
            }
        } //while

        //
        // done!
        //
    }

    //helper function for distance
    int _distanceRoot(NODE *r, TKey k)
    {
        if (r->Key == k)
            return 0;
        else if (r->Key > k)
            return 1 + _distanceRoot(r->Left, k);
        return 1 + _distanceRoot(r->Right, k);
    }

    int _distance(NODE *r, TKey k1, TKey k2)
    {
        if (r == nullptr)
            return 0;

        //both keys in left
        if (r->Key > k1 && r->Key > k2)
        {
            return _distance(r->Left, k1, k2);
        }
        //both keys in right
        if (r->Key < k1 && r->Key < k2)
        {
            return _distance(r->Right, k1, k2);
        }
        //one on each side of root
        //Eventurally code runs here

        //r->Key >= k1 && r->Key <= k2
        return _distanceRoot(r, k1) + _distanceRoot(r, k2);
    }

    //path distance between nodes
    int distance(TKey k1, TKey k2)
    {
        //valid k1 and k2
        if (search(k1) == nullptr || search(k2) == nullptr)
        {
            return -1;
        }
        //make k1 smaller than k2
        if (k1 > k2)
            swap(k1, k2);
        return _distance(Root, k1, k2);
    }

    //
    // inorder:
    //
    // Performs an inorder traversal of the tree, outputting
    // the keys to the console.
    //
    void inorder()
    {
        cout << "Inorder: ";

        _inorder(Root);

        cout << endl;
    }

    std::vector<TKey> inorder_keys()
    {
        std::vector<TKey> V;

        _inorder_keys(Root, V);

        return V;
    }

    std::vector<TValue> inorder_values()
    {
        std::vector<TValue> V;

        _inorder_values(Root, V);

        return V;
    }

    std::vector<int> inorder_heights()
    {
        std::vector<int> V;

        _inorder_heights(Root, V);

        return V;
    }

    void clear()
    {
        //
        // Re-initialize root pointer and size to "empty":
        //
        DestroyRecursive(Root);
        Root = nullptr;
        Size = 0;
    }

    bool isAVL(NODE* node)
    {
        //debug
        //cout << "node key: " << node->Key;

        // //if bst
        // if (_isBST(node))
        //     cout << ", isBst, ";
        // else
        //     cout << "not bst, ";
        
        // //if balanced
        // if(_isBalanced(node))
        //     cout << "isBanlanced, ";
        // else
        //     cout << "not balanced, ";

        // //if heightsCorrect
        // if(_heightsCorrect(node))
        //     cout << "heightCorrect" << endl;
        // else
        //     cout << "not heightCorrect" << endl;
        
        if (_isBST(node) && _isBalanced(node) && _heightsCorrect(node)){
            return true;
        }
        return false;
    }
};
