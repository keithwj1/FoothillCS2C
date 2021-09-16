#pragma once

#include <memory>
#include <vector>
namespace nonstd {

    template <typename T>
    class MultiSet {
    public:
        class Node {
        private:
            T value_;
            std::unique_ptr<Node> left_, right_;
            
        public:
            Node(const T& value) : value_(value) {}
            Node* left() const {
                return left_.get();
            }
            void set_left(const T& value) {
                left_ = std::make_unique<Node>(value);
            }
            Node* right() const {
                return right_.get();
            }
            void set_right(const T& value) {
                right_ = std::make_unique<Node>(value);
            }
            const T& value() const {
                return value_;
            }
            friend class MultiSet;
        };

    private:
        std::unique_ptr<Node> root_;
        int size_;
        void insert_node_(const T& value, Node* cur_node) {
            if (empty()) {
                root_ = std::make_unique<Node>(value);
                return;
            }
            if (value <= cur_node->value()) {
                if (cur_node->left() == nullptr) {
                    cur_node->set_left(value);
                    return;
                }
                return insert_node_(value, cur_node->left());
            }
            else {
                    if (cur_node->right() == nullptr) {
                    cur_node->set_right(value);
                    return;
                }
                return insert_node_(value, cur_node->right());
            }
        }
        int count_(const T& value, Node* cur_node, int& count) const {
            if (cur_node == nullptr) {
                return count;
            }
            else if (value == cur_node->value()) {
                ++count;
            }
            if (value <= cur_node->value()) {
                return count_(value, cur_node->left(), count);
            }
            else {
                return count_(value, cur_node->right(), count);
            }
        }
        void print_(std::vector<Node*>& list) {
            if (list.empty()) { return; }
            auto cur = list.back();
            if (cur == nullptr) {
                list.pop_back();
                return print_(list);
            }
            std::cout << cur->value() << ", ";
            list.pop_back();
            list.push_back(cur->right());
            list.push_back(cur->left());
            return print_(list);
        }
    public:
        MultiSet() : root_(nullptr), size_(0) {}

        int size() const {
            return size_;
        }

        Node* root() {
            return root_.get();
        }

        bool empty() const {
            if (root_ == nullptr) {
                return true;
            }
            return false;
        }
        
        void insert(const T& value) {
            insert_node_(value, this->root());
            ++size_;
        }
        
        int count(const T& value) const {
            int count = 0;
            return count_(value, root_.get(), count);
        }

        bool contains(const T& value) const {
            if (this->count(value) > 0) {
                return true;
            }
            return false;
        }

        //Prints all the values in the multiset
        void print() {
            std::vector<Node*> list;
            list.push_back(root());
            return print_(list);
            //Old Iterative version
            /*while (!list.empty()) {
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
            std::cout << std::endl;*/
        }
    };

} // namespace nonstd