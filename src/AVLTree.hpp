#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "Dictionary.hpp"

#include <vector>
#include <utility>
#include <string>
#include <cstddef>
#include <algorithm>

template <typename Key, typename Value>
class AVLTree : public Dictionary<Key, Value> {
private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        Node* parent;
        int height;

        Node(const Key& key, const Value& value)
            : key(key),
              value(value),
              left(NULL),
              right(NULL),
              parent(NULL),
              height(1) {}
    };

    Node* root;
    std::size_t elementCount;
    mutable DictionaryMetrics metricsData;

    int height(Node* node) const {
        return node == NULL ? 0 : node->height;
    }

    void updateHeight(Node* node) {
        if (node != NULL) {
            int leftHeight = height(node->left);
            int rightHeight = height(node->right);
            node->height = std::max(leftHeight, rightHeight) + 1;
        }
    }

    int balanceFactor(Node* node) const {
        if (node == NULL) {
            return 0;
        }

        return height(node->left) - height(node->right);
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* transfer = y->left;

        y->left = x;
        x->right = transfer;

        if (transfer != NULL) {
            transfer->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == NULL) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        x->parent = y;

        updateHeight(x);
        updateHeight(y);

        metricsData.rotations++;

        return y;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* transfer = x->right;

        x->right = y;
        y->left = transfer;

        if (transfer != NULL) {
            transfer->parent = y;
        }

        x->parent = y->parent;

        if (y->parent == NULL) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }

        y->parent = x;

        updateHeight(y);
        updateHeight(x);

        metricsData.rotations++;

        return x;
    }

    void rebalance(Node* node) {
        while (node != NULL) {
            updateHeight(node);

            int balance = balanceFactor(node);

            if (balance > 1) {
                if (balanceFactor(node->left) < 0) {
                    rotateLeft(node->left);
                }

                Node* newRoot = rotateRight(node);
                node = newRoot->parent;
            } else if (balance < -1) {
                if (balanceFactor(node->right) > 0) {
                    rotateRight(node->right);
                }

                Node* newRoot = rotateLeft(node);
                node = newRoot->parent;
            } else {
                node = node->parent;
            }
        }
    }

    Node* minimum(Node* node) const {
        while (node != NULL && node->left != NULL) {
            node = node->left;
        }

        return node;
    }

    Node* findNode(const Key& key) const {
        Node* current = root;

        while (current != NULL) {
            metricsData.keyComparisons++;

            if (key == current->key) {
                return current;
            }

            metricsData.keyComparisons++;

            if (key < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        return NULL;
    }

public:
    AVLTree()
        : root(NULL),
          elementCount(0),
          metricsData() {}

    ~AVLTree() {
        clear();
    }

    bool insert(const Key& key, const Value& value) override {
        if (root == NULL) {
            root = new Node(key, value);
            elementCount++;
            return true;
        }

        Node* parent = NULL;
        Node* current = root;

        while (current != NULL) {
            parent = current;

            metricsData.keyComparisons++;

            if (key == current->key) {
                return false;
            }

            metricsData.keyComparisons++;

            if (key < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        Node* newNode = new Node(key, value);
        newNode->parent = parent;

        if (key < parent->key) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        elementCount++;

        rebalance(parent);

        return true;
    }

    bool update(const Key& key, const Value& value) override {
        Node* node = findNode(key);

        if (node == NULL) {
            return false;
        }

        node->value = value;
        return true;
    }

    Value* get(const Key& key) override {
        Node* node = findNode(key);

        if (node == NULL) {
            return NULL;
        }

        return &node->value;
    }

    const Value* get(const Key& key) const override {
        Node* node = findNode(key);

        if (node == NULL) {
            return NULL;
        }

        return &node->value;
    }

    bool contains(const Key& key) override {
        return findNode(key) != NULL;
    }

    bool remove(const Key& key) override {
        Node* node = findNode(key);

        if (node == NULL) {
            return false;
        }

        if (node->left != NULL && node->right != NULL) {
            Node* successor = minimum(node->right);

            node->key = successor->key;
            node->value = successor->value;
            node = successor;
        }

        Node* child = node->left != NULL ? node->left : node->right;
        Node* parent = node->parent;

        if (child != NULL) {
            child->parent = parent;
        }

        if (parent == NULL) {
            root = child;
        } else if (node == parent->left) {
            parent->left = child;
        } else {
            parent->right = child;
        }

        delete node;
        elementCount--;

        rebalance(parent);

        return true;
    }

    void clear() override {
        if (root == NULL) {
            return;
        }

        std::vector<Node*> stack;
        stack.push_back(root);

        while (!stack.empty()) {
            Node* node = stack.back();
            stack.pop_back();

            if (node->left != NULL) {
                stack.push_back(node->left);
            }

            if (node->right != NULL) {
                stack.push_back(node->right);
            }

            delete node;
        }

        root = NULL;
        elementCount = 0;
    }

    std::size_t size() const override {
        return elementCount;
    }

    std::vector<std::pair<Key, Value> > items() const override {
        std::vector<std::pair<Key, Value> > result;
        result.reserve(elementCount);

        std::vector<Node*> stack;
        Node* current = root;

        while (current != NULL || !stack.empty()) {
            while (current != NULL) {
                stack.push_back(current);
                current = current->left;
            }

            current = stack.back();
            stack.pop_back();

            result.push_back(std::make_pair(current->key, current->value));

            current = current->right;
        }

        return result;
    }

    std::string name() const override {
        return "AVL Tree";
    }

    DictionaryMetrics metrics() const override {
        return metricsData;
    }

    void resetMetrics() override {
        metricsData = DictionaryMetrics();
    }
};

#endif