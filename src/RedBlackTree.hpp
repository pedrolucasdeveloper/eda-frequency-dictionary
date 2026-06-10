#ifndef RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_HPP

#include "Dictionary.hpp"

#include <vector>
#include <utility>
#include <string>
#include <cstddef>

template <typename Key, typename Value>
class RedBlackTree : public Dictionary<Key, Value> {
private:
    enum Color {
        RED,
        BLACK
    };

    struct Node {
        Key key;
        Value value;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node()
            : key(),
              value(),
              color(BLACK),
              left(NULL),
              right(NULL),
              parent(NULL) {}

        Node(const Key& key, const Value& value, Node* nil)
            : key(key),
              value(value),
              color(RED),
              left(nil),
              right(nil),
              parent(nil) {}
    };

    Node* root;
    Node* nil;
    std::size_t elementCount;
    mutable DictionaryMetrics metricsData;

    void rotateLeft(Node* x) {
        Node* y = x->right;

        x->right = y->left;

        if (y->left != nil) {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;

        metricsData.rotations++;
    }

    void rotateRight(Node* y) {
        Node* x = y->left;

        y->left = x->right;

        if (x->right != nil) {
            x->right->parent = y;
        }

        x->parent = y->parent;

        if (y->parent == nil) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }

        x->right = y;
        y->parent = x;

        metricsData.rotations++;
    }

    void fixInsert(Node* node) {
        while (node->parent->color == RED) {
            Node* parent = node->parent;
            Node* grandparent = parent->parent;

            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                if (uncle->color == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    node = grandparent;
                } else {
                    if (node == parent->right) {
                        node = parent;
                        rotateLeft(node);
                        parent = node->parent;
                        grandparent = parent->parent;
                    }

                    parent->color = BLACK;
                    grandparent->color = RED;
                    rotateRight(grandparent);
                }
            } else {
                Node* uncle = grandparent->left;

                if (uncle->color == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    node = grandparent;
                } else {
                    if (node == parent->left) {
                        node = parent;
                        rotateRight(node);
                        parent = node->parent;
                        grandparent = parent->parent;
                    }

                    parent->color = BLACK;
                    grandparent->color = RED;
                    rotateLeft(grandparent);
                }
            }
        }

        root->color = BLACK;
    }

    void transplant(Node* oldNode, Node* newNode) {
        if (oldNode->parent == nil) {
            root = newNode;
        } else if (oldNode == oldNode->parent->left) {
            oldNode->parent->left = newNode;
        } else {
            oldNode->parent->right = newNode;
        }

        newNode->parent = oldNode->parent;
    }

    Node* minimum(Node* node) const {
        while (node->left != nil) {
            node = node->left;
        }

        return node;
    }

    void fixDelete(Node* node) {
        while (node != root && node->color == BLACK) {
            if (node == node->parent->left) {
                Node* sibling = node->parent->right;

                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateLeft(node->parent);
                    sibling = node->parent->right;
                }

                if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                    sibling->color = RED;
                    node = node->parent;
                } else {
                    if (sibling->right->color == BLACK) {
                        sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = node->parent->right;
                    }

                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    sibling->right->color = BLACK;
                    rotateLeft(node->parent);
                    node = root;
                }
            } else {
                Node* sibling = node->parent->left;

                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateRight(node->parent);
                    sibling = node->parent->left;
                }

                if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                    sibling->color = RED;
                    node = node->parent;
                } else {
                    if (sibling->left->color == BLACK) {
                        sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = node->parent->left;
                    }

                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    sibling->left->color = BLACK;
                    rotateRight(node->parent);
                    node = root;
                }
            }
        }

        node->color = BLACK;
    }

    Node* findNode(const Key& key) const {
        Node* current = root;

        while (current != nil) {
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

        return nil;
    }

public:
    RedBlackTree()
        : root(NULL),
          nil(new Node()),
          elementCount(0),
          metricsData() {
        nil->left = nil;
        nil->right = nil;
        nil->parent = nil;
        root = nil;
    }

    ~RedBlackTree() {
        clear();
        delete nil;
    }

    bool insert(const Key& key, const Value& value) override {
        Node* parent = nil;
        Node* current = root;

        while (current != nil) {
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

        Node* newNode = new Node(key, value, nil);
        newNode->parent = parent;

        if (parent == nil) {
            root = newNode;
        } else if (key < parent->key) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        elementCount++;

        fixInsert(newNode);

        return true;
    }

    bool update(const Key& key, const Value& value) override {
        Node* node = findNode(key);

        if (node == nil) {
            return false;
        }

        node->value = value;
        return true;
    }

    Value* get(const Key& key) override {
        Node* node = findNode(key);

        if (node == nil) {
            return NULL;
        }

        return &node->value;
    }

    const Value* get(const Key& key) const override {
        Node* node = findNode(key);

        if (node == nil) {
            return NULL;
        }

        return &node->value;
    }

    bool contains(const Key& key) override {
        return findNode(key) != nil;
    }

    bool remove(const Key& key) override {
        Node* nodeToRemove = findNode(key);

        if (nodeToRemove == nil) {
            return false;
        }

        Node* movedNode = nodeToRemove;
        Color originalColor = movedNode->color;
        Node* replacement;

        if (nodeToRemove->left == nil) {
            replacement = nodeToRemove->right;
            transplant(nodeToRemove, nodeToRemove->right);
        } else if (nodeToRemove->right == nil) {
            replacement = nodeToRemove->left;
            transplant(nodeToRemove, nodeToRemove->left);
        } else {
            movedNode = minimum(nodeToRemove->right);
            originalColor = movedNode->color;
            replacement = movedNode->right;

            if (movedNode->parent == nodeToRemove) {
                replacement->parent = movedNode;
            } else {
                transplant(movedNode, movedNode->right);
                movedNode->right = nodeToRemove->right;
                movedNode->right->parent = movedNode;
            }

            transplant(nodeToRemove, movedNode);
            movedNode->left = nodeToRemove->left;
            movedNode->left->parent = movedNode;
            movedNode->color = nodeToRemove->color;
        }

        delete nodeToRemove;
        elementCount--;

        if (originalColor == BLACK) {
            fixDelete(replacement);
        }

        return true;
    }

    void clear() override {
        if (root == nil) {
            return;
        }

        std::vector<Node*> stack;
        stack.push_back(root);

        while (!stack.empty()) {
            Node* node = stack.back();
            stack.pop_back();

            if (node->left != nil) {
                stack.push_back(node->left);
            }

            if (node->right != nil) {
                stack.push_back(node->right);
            }

            delete node;
        }

        root = nil;
        nil->parent = nil;
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

        while (current != nil || !stack.empty()) {
            while (current != nil) {
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
        return "Red-Black Tree";
    }

    DictionaryMetrics metrics() const override {
        return metricsData;
    }

    void resetMetrics() override {
        metricsData = DictionaryMetrics();
    }
};

#endif