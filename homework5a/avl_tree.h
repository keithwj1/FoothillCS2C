#pragma once

#include <memory>
#include <vector>

template <typename Key, typename Value=Key>
class AVLTree {
public:
  class Node {
  private:
    Key key_;
    Value value_;
    int balance_factor_;
    std::unique_ptr<Node> left_, right_;
    Node(const Key& key) : key_(key), height_(0) {}
    Node(const Key& key, const Value& value) : key_(key), value_(value), height_(0) {}

  public:
    // All these getter methods are O(1)
    Node *left() {
      return left_.get();
    }
    Node *right() {
      return right_.get();
    }
    const Key& key() const {
      return key_;
    }
    const Value& value() const {
      return value_;
    }
    const int balance_factor() const {
      return balance_factor_;
    }

    friend class AVLTree<Key, Value>;
  };

private:
  std::unique_ptr<Node> root_;
  int size_;
  int balance_factor;
public:
  AVLTree() : size_(0) {}

  // TODO: Add code to update the balance factor and do rebalancing...
  // This overloaded operator method is O(log(size()))
  Value& operator[](const Key& key) {
    // Try to find the node with the value we want:
    std::unique_ptr<Node> *cur;
    for (cur = &root_;
         cur->get() != nullptr;
         cur = key < (*cur)->key_ ? &(*cur)->left_ : &(*cur)->right_) {
      if (key == (*cur)->key_) {
        return (*cur)->value_;
      }
    }
    // If we didn't find it, insert a new node with that key:
    // (This is the same behaviour as an std::map.)
    cur->reset(new Node(key));
    ++size_;
    return (*cur)->value_;
  }
  int size() {
    return size_;
  }

  Node *root() {
    return root_.get();
  }
  void print() {
            std::vector<Node*> list;
            list.push_back(root());
            while (!list.empty()) {
                auto cur = list.back();
                if (cur != nullptr)
                {
                    std::cout << cur->value() << ", ";
                    list.pop_back();
                    list.push_back(cur->right());
                    list.push_back(cur->left());
                }
                else {
                    list.pop_back();
                }
            }
            std::cout << std::endl;
        }
};