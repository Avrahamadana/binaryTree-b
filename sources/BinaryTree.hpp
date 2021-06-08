#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;
namespace ariel
{

    template <typename T>
    class BinaryTree
    {
        struct Node
        {
            T data;
            Node *left;
            Node *right;
            Node *parent;
            Node(T d) : data(d), left(nullptr), right(nullptr), parent(nullptr){};
            Node *clone()
            {
                if (Node *tmp = new Node(data))
                {
                    if (left)
                    {
                        tmp->left = left->clone();
                        if (tmp->left != nullptr)
                        {
                            tmp->left->parent = tmp;
                        }
                    }
                    if (right)
                    {
                        tmp->right = right->clone();
                        if (tmp->right != nullptr)
                        {
                            tmp->right->parent = tmp;
                        }
                    }
                    return tmp;
                }
                return 0;
            };
        };

    private:
        Node *root;
        Node *findNode(const T &data, Node *node)
        {
            if (node != nullptr)
            {
                if (node->data == data)
                {
                    return node;
                }
                Node *nextNode = findNode(data, node->left);
                if (nextNode == nullptr)
                {
                    nextNode = findNode(data, node->right);
                }
                return nextNode;
            }
            return nullptr;
        }

    public:
        BinaryTree() : root(nullptr){};
        BinaryTree(const BinaryTree &other)
        {
            if (other.root != nullptr)
            {
                root = new Node(other.root->data);
                copyTree(root, other.root);
            }
        }
        void copyTree(Node *first, Node *other)
        {
            if (other->left != nullptr)
            {
                first->left = new Node(other->left->data);
                copyTree(first->left, other->left);
            }
            if (other->right != nullptr)
            {
                first->right = new Node(other->right->data);
                copyTree(first->right, other->right);
            }
        }
        ~BinaryTree()
        {
            deleteTree(root);
        }
        void deleteTree(Node *node)
        {
            if (node != nullptr)
            {
                deleteTree(node->left);
                deleteTree(node->right);
                delete node;
            }
        }

        BinaryTree(BinaryTree &&other) noexcept
        {
            root = other.root;
            other.root = nullptr;
        }

        BinaryTree &operator=(BinaryTree &&other) noexcept
        {
            root = other->root;
            other.root = nullptr;
            return *this;
        }
        BinaryTree &operator=(const BinaryTree other)
        {
            if (this != &other)
            {
                delete (this->root);
                root = new Node(other.root->data);
                copyTree(root, other.root);
            }
            return *this;
        }
        BinaryTree &add_root(T node)
        {
            if (root == nullptr)
            {
                root = new Node(node);
            }
            else
            {
                root->data = node;
            }
            return *this;
        }

        BinaryTree &add_left(T curr, T child)
        {
            Node *tempNode = findNode(curr, root);
            if (tempNode == nullptr)
            {
                throw("Dont have node!");
            }
            if (tempNode->left != nullptr)
            {
                tempNode->left->data = child;
            }
            else
            {
                Node *newNode = new Node(child);
                tempNode->left = newNode;
                newNode->parent = tempNode;
            }
            return *this;
        }

        BinaryTree &add_right(T curr, T child)
        {
            Node *tempNode = findNode(curr, root);
            if (tempNode == nullptr)
            {
                throw("Dont have node!");
            }
            if (tempNode->right != nullptr)
            {
                tempNode->right->data = child;
            }
            else
            {
                Node *newNode = new Node(child);
                tempNode->right = newNode;
                newNode->parent = tempNode;
            }
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &out, const BinaryTree<T> &binaryTree)
        {
            for (auto it = binaryTree.begin(); it != binaryTree.end(); ++it)
            {
                out << it << " ";
            }
            return out;
        }

        class InOrderIterator
        {
        private:
            Node *node;
            std::vector<Node *> stack;

        public:
            void insert(Node *n)
            {
                if (n == nullptr)
                {
                    return;
                }
                insert(n->right);
                stack.push_back(n);
                insert(n->left);
            }
            InOrderIterator(Node *n)
            {
                stack.push_back(nullptr);
                insert(n);
                node = stack.back();
                stack.pop_back();
            };
            bool operator==(const InOrderIterator &other) const
            {
                if (node == nullptr)
                {
                    return other.node == nullptr;
                }
                if (other.node == nullptr)
                {
                    return node == nullptr;
                }
                return node->data == other.node->data;
            }
            bool operator!=(const InOrderIterator &other) const
            {
                return !(*this == other);
            }
            InOrderIterator &operator++()
            {
                if (stack.size() > 0)
                {
                    node = stack.back();
                    stack.pop_back();
                }
                return *this;
            }
            T &operator*()
            {
                return node->data;
            }
            T *operator->() const
            { //check
                return &(node->data);
            }
            InOrderIterator operator++(int)
            {
                InOrderIterator t = *this;
                ++*this;
                return t;
            }
        };

        InOrderIterator begin_inorder()
        {
            return InOrderIterator{root};
        }
        InOrderIterator end_inorder()
        {
            return InOrderIterator{nullptr};
        }
        InOrderIterator begin()
        {
            return InOrderIterator{root};
        }
        InOrderIterator end()
        {
            return InOrderIterator{nullptr};
        }

        class PostOrderIterator
        {
        private:
            Node *node;
            std::vector<Node *> stack;

        public:
            void insert(Node *n)
            {
                if (n == nullptr)
                {
                    return;
                }
                stack.push_back(n);
                insert(n->right);
                insert(n->left);
            }
            PostOrderIterator(Node *n)
            {
                stack.push_back(nullptr);
                insert(n);
                node = stack.back();
                stack.pop_back();
            }
            bool operator==(const PostOrderIterator &other) const
            {
                if (node == nullptr)
                {
                    return other.node == nullptr;
                }
                if (other.node == nullptr)
                {
                    return node == nullptr;
                }
                return node->data == other.node->data;
            }
            bool operator!=(const PostOrderIterator &other) const
            {
                return !(*this == other);
            }
            PostOrderIterator &operator++()
            {
                if (stack.size() > 0)
                {
                    node = stack.back();
                    stack.pop_back();
                }
                return *this;
            }
            T *operator->() const
            {
                return &(node->data);
            }
            T &operator*()
            {
                return node->data;
            }
            PostOrderIterator operator++(int)
            {
                PostOrderIterator t = *this;
                ++*this;
                return t;
            }
        };

        PostOrderIterator begin_postorder()
        {
            return PostOrderIterator{root};
        }
        PostOrderIterator end_postorder()
        {
            return PostOrderIterator(nullptr);
        }

        class PreOrderIterator
        {
        private:
            Node *node;
            std::vector<Node *> stack;

        public:
            void insert(Node *n)
            {
                if (n == nullptr)
                {
                    return;
                }
                insert(n->right);
                insert(n->left);
                stack.push_back(n);
            }
            PreOrderIterator(Node *n)
            {
                stack.push_back(nullptr);
                insert(n);
                node = stack.back();
                stack.pop_back();
            }
            bool operator==(const PreOrderIterator &other) const
            {
                if (node == nullptr)
                {
                    return other.node == nullptr;
                }
                if (other.node == nullptr)
                {
                    return node == nullptr;
                }
                return node->data == other.node->data;
            }
            bool operator!=(const PreOrderIterator &other) const
            {
                return !(*this == other);
            }
            PreOrderIterator &operator++()
            {
                if (stack.size() > 0)
                {
                    node = stack.back();
                    stack.pop_back();
                }
                return *this;
            }
            T &operator*()
            {
                return node->data;
            }
            PreOrderIterator operator++(int)
            {
                PreOrderIterator t = *this;
                ++*this;
                return t;
            }
        };
        PreOrderIterator begin_preorder()
        {
            return PreOrderIterator{root};
        }
        PreOrderIterator end_preorder()
        {
            return PreOrderIterator(nullptr);
        }
    };
};
