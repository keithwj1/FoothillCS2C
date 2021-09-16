#pragma once

#include <memory>
#include <vector>
#include <algorithm>

template <typename Key, typename Value = Key>
class AVLTree {
public:
    class Node {
    private:
        Key key_;
        Value value_;
        int balance_factor_,height_;
        Node *left_, *right_;
        Node(const Key& key) : key_(key), value_(key), balance_factor_(1), height_(1), left_(nullptr), right_(nullptr) {}
        Node(const Key& key, const Value& value) : key_(key), value_(value), balance_factor_(1),height_(1), left_(nullptr), right_(nullptr) {}

    public:
        ~Node() {
            delete left_;
            delete right_;
        }
        // All these getter methods are O(1)
        Node* left() {
            return left_;
        }
        Node* right() {
            return right_;
        }
        const Key& key() const {
            return key_;
        }
        const Value& value() const {
            return value_;
        }
        const int height() const {
            return height_;
        }
        const int balance_factor() const {
            return right_->height() - left_->height();
        }
        friend class AVLTree<Key, Value>;
    };

private:
    Node *root_;
    int size_;
    Node* rotate_right(Node*& node){
        Node* node_right = node->right_;
        node->right_ = node_right->left_;
        node_right->left_ = node;
        node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
        node_right->height_ = std::max(height(node_right->right_), node->height_) + 1;
        node = node_right;
        return node_right->left_;
    }
    Node* rotate_left(Node * &node){
        Node * node_left = node->left_;
        node->left_ = node_left->right_;
        node_left->right_ = node;
        node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
        node_left->height_ = std::max(height(node_left->left_), node->height_) + 1;
        node = node_left;
        return node_left->right_;
     }
    Node* rotate_left_twice(Node* &node){
        rotate_right(node->left_);
        return rotate_left(node);
    }
    Node* rotate_right_twice(Node*& node){
        rotate_left(node->right_);
        return rotate_right(node);
    }
public:
    AVLTree() : root_(nullptr), size_(0) {}
    ~AVLTree() {
        delete root_;
    }
    // TODO: Add code to update the balance factor and do rebalancing...
    // This overloaded operator method is O(log(size()))
    Value& operator[](const Key& key) {
        // Try to find the node with the value we want:
        auto cur = insert(key, root_);
        return cur->value_;
    }
    const int height(Node* node) const{
        if (node == nullptr) { return -1; }
        return node->height_;
    }
    int get_balance(Node* node){
        if (node == nullptr) { return 0; }
        return height(node->left_) - height(node->right_);
    }
    void print(Node* node) {
        if (node == nullptr) { return; }
        std::cout << node->key() << ":" << node->value() << ", ";
        print(node->left());
        print(node->right());
        return;
    }
    Node* balance(Node * &node){
        const int MAX_BALANCE = 1;
        if (node == nullptr) { return node; }

        if (height(node->left_) - height(node->right_) > MAX_BALANCE) {
            if (height(node->left_->left_) >= height(node->left_->right_)) {
                rotate_left(node);
            }
            else {
                rotate_left_twice(node);
            }
        }
        else {
            if (height(node->right_) - height(node->left_) > MAX_BALANCE) {
                if (height(node->right_->right_) >= height(node->right_->left_)) {
                    rotate_right(node);
                }
                else {
                    rotate_right_twice(node);
                }
            }
        }
        node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
        return node;
    }
    int size() {
        return size_;
    }
    Node* get_node(Key key) {
        return insert(key, root_);
    }
    Node* root() {
        return root_;
    }
    Node* insert(const Key& key, Node*& node){
        if (node == nullptr) {
            ++size_;
            node = new Node(key);
        }
        else if (key < node->key_) {
            insert(key, node->left_);
        }
        else if (node->key_ < key) {
            insert(key, node->right_);
        }
        return balance(node);
    }
};