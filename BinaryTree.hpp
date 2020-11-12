#ifndef _BINARY_TREE_
#define _BINARY_TREE_

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <queue>

namespace myDataStructure {
    template<typename T>
    class BinaryTree {
        T val;
        BinaryTree<T>* left;
        BinaryTree<T>* right;

    public:
        BinaryTree() = default;
        BinaryTree(const T& _val) : val(_val), left(nullptr), right(nullptr) {}
        BinaryTree(const T& _val, BinaryTree<T>* _left, BinaryTree<T>* _right) : val(_val), left(_left), right(_right) {}

        int countOfLeaves(void);
        int maxWidth(void);
        void swapLeftAndRight(void);
        std::vector<T> levelOrder(void);

        void alterLeft(BinaryTree<T>* _left);
        void alterRight(BinaryTree<T>* _right);
        BinaryTree<T>* getLeft(void);
        BinaryTree<T>* getRight(void);
        T getVal(void);

        void outToFile(void);

        template<typename Q>
        friend int doCountOfLeaves(BinaryTree<Q>* root);

        template<typename Q>
        friend int doMaxWidth(BinaryTree<Q>* root);

        template<typename Q>
        friend std::vector<Q> doLevelOrder(BinaryTree<Q>* root);

        template<typename Q>
        friend BinaryTree<Q>* doSwap(BinaryTree<Q>* root);

        template<typename Q>
        friend void doOutToFile(std::ofstream& out, BinaryTree<Q>* root, int cnt);
    };

    template<typename T>
    void doOutToFile(std::ofstream& out, BinaryTree<T>* root, int space) {
        if (nullptr == root) { return; }

        doOutToFile(out, root->getRight(), space + 6);
        out << std::endl;
        for (int i = 6; i < space; i++)  { out << " "; } 
        out << root->getVal() << std::endl;
        doOutToFile(out, root->getLeft(), space + 6);
    }

    template<typename T>
    BinaryTree<T>* doSwap(BinaryTree<T>* root) {
        if (nullptr == root) { return nullptr; }
        doSwap(root->left);
        doSwap(root->right);
        std::swap(root->left, root->right);
        return root;
    }

    template<typename T>
    int doCountOfLeaves(BinaryTree<T>* root) {
        if (nullptr == root) { return 0; }
        if (nullptr == root->left && nullptr == root->right) { return 1; }
        return doCountOfLeaves(root->left) + doCountOfLeaves(root->right);
    }

    template<typename T>
    int doMaxWidth(BinaryTree<T>* root) {
        int ans = 0;

        std::queue<BinaryTree<T>*> q;
        if (nullptr != root) { q.push(root); }

        while (!q.empty()) {
            int s = q.size();
            ans = (ans < s ? s : ans);
            for (int i = 0; i < s; i++) {
                auto front = q.front();
                q.pop();
                if (nullptr != front->left) { q.push(front->left); }
                if (nullptr != front->right) { q.push(front->right); }
            }
        }

        return ans;
    }

    template<typename T>
    std::vector<T> doLevelOrder(BinaryTree<T>* root) {
        std::vector<T> ans;
        std::queue<BinaryTree<T>*> q;
        if (nullptr != root) { q.push(root); }

        while (!q.empty()) {
            int s = q.size();
            for (int i = 0; i < s; i++) {
                auto front = q.front();
                ans.push_back(front->val);
                q.pop();
                if (nullptr != front->left) { q.push(front->left); }
                if (nullptr != front->right) { q.push(front->right); }
            }
        }

        return ans;
    }

    template<typename T>
    BinaryTree<T>* doDeserialize(std::vector<std::string>& items, int begin) {
        if (begin > items.size() || items[begin - 1] == "#") {
            return nullptr;
        }
        std::string front = items[begin - 1];
        BinaryTree<T>* root;
        try {
            root = new BinaryTree<T>(stoi(front));
        } catch (std::exception e) {
            root = new BinaryTree<T>(front[0]);
        }
        root->alterLeft(doDeserialize<T>(items, 2 * begin));
        root->alterRight(doDeserialize<T>(items, 2 * begin + 1));
        return root;
    }
        
    template<typename T>
    BinaryTree<T>* deserialize(const std::string& str) {
        std::istringstream iss(str + ",");
        std::string buf;
        std::vector<std::string> items;

        while (getline(iss, buf, ',')) {
            items.push_back(buf);
        }
        return doDeserialize<T>(items, 1);
    }

    template<typename T>
    BinaryTree<T>* deserializeFromMid(const std::string& str) {
        std::istringstream iss(str + ",");
        std::string buf;
        std::stack<BinaryTree<T>*> ops;
        std::stack<BinaryTree<T>*> nums;

        while (getline(iss, buf, ',')) {
            if (buf[0] == '*' || buf[0] == '/') {
                ops.push(new BinaryTree<T>(buf[0]));
            } else if (buf[0] == '+' || buf[0] == '-') {
                while (!ops.empty() && (ops.top()->getVal() == '*' || ops.top()->getVal() == '/')) {
                    BinaryTree<T>* front = ops.top();
                    ops.pop();
                    front->alterLeft(nums.top()); nums.pop();
                    front->alterRight(nums.top()); nums.pop();
                    nums.push(front);
                }
                ops.push(new BinaryTree<T>(buf[0]));
            } else if (buf[0] == '(') {
                ops.push(new BinaryTree<T>(buf[0]));
            } else if (buf[0] == ')') {
                while (ops.top()->getVal() != '(') {
                    BinaryTree<T>* front = ops.top();
                    ops.pop();
                    front->alterLeft(nums.top()); nums.pop();
                    front->alterRight(nums.top()); nums.pop();
                    nums.push(front);
                }
                ops.pop(); //Pop the last (.
            } else {
                nums.push(new BinaryTree<T>(buf[0]));
            }
        }
        while (!ops.empty()) {
            BinaryTree<T>* front = ops.top();
            ops.pop();
            front->alterLeft(nums.top()); nums.pop();
            front->alterRight(nums.top()); nums.pop();
            nums.push(front);
        }
        return nums.top();
    }

}

template<typename T>
void myDataStructure::BinaryTree<T>::outToFile(void) {
    std::ofstream out("res.txt", std::ios::out);
    
    doOutToFile(out, this, 6);
}

template<typename T>
int myDataStructure::BinaryTree<T>::countOfLeaves(void) {
    return doCountOfLeaves(this);
}

template<typename T>
int myDataStructure::BinaryTree<T>::maxWidth(void) {
    return doMaxWidth(this);
}

template<typename T>
std::vector<T> myDataStructure::BinaryTree<T>::levelOrder(void) {
    return doLevelOrder(this);
}

template<typename T>
void myDataStructure::BinaryTree<T>::swapLeftAndRight(void) {
    doSwap(this);
}

template<typename T>
void myDataStructure::BinaryTree<T>::alterLeft(BinaryTree<T>* _left) {
    left = _left;
}

template<typename T>
void myDataStructure::BinaryTree<T>::alterRight(BinaryTree<T>* _right) {
    right = _right;
}

template<typename T>
typename myDataStructure::BinaryTree<T>*
myDataStructure::BinaryTree<T>::getLeft(void) {
    return left;
}

template<typename T>
typename myDataStructure::BinaryTree<T>*
myDataStructure::BinaryTree<T>::getRight(void) {
    return right;
}

template<typename T>
T myDataStructure::BinaryTree<T>::getVal(void) {
    return val;
}

#endif