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
        int height_;
        std::unique_ptr<Node> left_, right_;
    public:
        Node(const Key& key) : key_(key), value_(key), height_(1), left_(nullptr), right_(nullptr) {}
        Node(const Key& key, const Value& value) : key_(key), value_(value), height_(1), left_(nullptr), right_(nullptr) {}
        // All these getter methods are O(1)
        Node* left() {
            return left_.get();
        }
        Node* right() {
            return right_.get();
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
        int calculate_height(std::unique_ptr<Node>& node)
        {
            if (node == nullptr) {
                return 0;
            }
            else{
                int lDepth = calculate_height(node->left_);
                int rDepth = calculate_height(node->right_);
                return (lDepth > rDepth ? (lDepth + 1) : (rDepth + 1));
            }
        }
        friend class AVLTree;
    };

private:
    std::unique_ptr<Node> root_;
    int size_;
    void update_height(const Key& key) {
        std::unique_ptr<Node>* cur;
        for (cur = &root_;
            cur->get() != nullptr;
            cur = key < (*cur)->key_ ? &(*cur)->left_ : &(*cur)->right_) {
            balance(*cur);
            if (key == (*cur)->key_) {
                return;
            }
        }
    }
public:
    AVLTree() : root_(nullptr),size_(0) {}
    Value& operatorbadvalue(const Key& key) {
        // Try to find the node with the value we want:
        std::unique_ptr<Node>* cur = &root_;
        cur = insert(key, *cur);
        return (*cur)->value_;
    }
    Value& operator[](const Key& key) {
        // Try to find the node with the value we want:
        std::unique_ptr<Node>* cur;
        for (cur = &root_;
            cur->get() != nullptr;
            cur = key < (*cur)->key_ ? &(*cur)->left_ : &(*cur)->right_) {
            balance(*cur);
            if (key == (*cur)->key_) {
                return (*cur)->value_;
            }
        }
        // If we didn't find it, insert a new node with that key:
        // (This is the same behaviour as an std::map.)
        *cur = std::make_unique<Node>(key);
        ++size_;
        balance(*cur);
        return (*cur)->value_;
    }
    Node* get_node(Key key) {
        return insert(key, root_)->get();
    }
    const int height(const std::unique_ptr<Node>& node)  const {
        if (node == nullptr) { return -1; }
        return node->height_;
    }
    const int height(Node* node) const {
        if (node == nullptr) { return -1; }
        return node->height_;
    }
    int size() {
        return size_;
    }
    std::unique_ptr<Node>& rotate_right(std::unique_ptr<Node>& node)
    {
        auto node_right = std::move(node->right_);
        node->right_ = std::move(node_right->left_);
        node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
        node_right->height_ = std::max(height(node_right->right_), node->height_) + 1;
        node_right->left_ = std::move(node);
        node = std::move(node_right);
        return node_right->left_;
    }
    std::unique_ptr<Node>& rotate_left(std::unique_ptr<Node>& node)
    {
        auto node_left = std::move(node->left_);
        node->left_ = std::move(node_left->right_);
        node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
        node_left->height_ = std::max(height(node_left->left_), node->height_) + 1;
        node_left->right_ = std::move(node);
        node = std::move(node_left);
        return node_left->right_;
    }
    std::unique_ptr<Node>& balance(std::unique_ptr<Node>& node)
    {
        static const int MAX_BALANCE = 1;
        if (node == nullptr) { return node; }
        if (height(node->left_) - height(node->right_) > MAX_BALANCE) {
            if (height(node->left_->left_) >= height(node->left_->right_)) {
                rotate_left(node);
            }
            else {
                rotate_right(node->left_);
                rotate_left(node);
            }
        }
        else if (height(node->right_) - height(node->left_) > MAX_BALANCE) {
            if (height(node->right_->right_) >= height(node->right_->left_)) {
                rotate_right(node);
            }
            else {
                rotate_left(node->right_);
                rotate_right(node);
            }
        }
        node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
        return node;
    }
    std::unique_ptr<Node>* insert(const Key& key, std::unique_ptr<Node>& node) {
        if (node == nullptr) {
            ++size_;
            node = std::make_unique<Node>(key);
        }
        else if (key < node->key_) {
            insert(key, node->left_);
        }
        else if (node->key_ < key) {
            insert(key, node->right_);
        }
        std::unique_ptr<Node>* cur = &node;
        balance(node);
        return cur;
    }
    Node* root() {
        return root_.get();
    }
    void print(Node * node) {
        if (node == nullptr) { return; }
        std::cout << node->key() << ":" << node->value() << ", ";
        print(node->left());
        print(node->right());
        return;
    }
    void balance_to_key(Key key) {
        insert(key, root_);
    }
    void recurse_balance(std::unique_ptr<Node>& node) {
        if (node == nullptr) { return; }
        balance(node);
        recurse_balance(node->left_);
        recurse_balance(node->right_);
        return;
    }
    void full_balance() {
        return recurse_balance(root_);
    }
};