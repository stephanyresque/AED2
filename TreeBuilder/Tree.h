#pragma once

#include <vector>
#include <memory>
#include <stack>
#include <tuple>

#include "TreeNode.h"

namespace tree_builder
{
    /**
     * A generic tree structure supporting multiple root nodes and common tree
     * operations.
     * This class supports operations such as adding root nodes, traversing the
     * tree using pre-order, in-order or post-order paths, and transforming the
     * structure of the tree via mirroring or sorting subtrees. It is designed
     * to handle trees with multiple roots (i.e., a forest structure).
     *
     * @tparam T The type of data stored in each node.
     */
    template<typename T>
    class tree
    {
    public:
        using node_type = tree_node<T>;

        // Constructor and destructor
        tree() = default;
        ~tree() = default;

    public:
        /**
         * Adds a new root node to the tree with a copy of the given data.
         * The new node is initialized as a root and added to the internal root
         * list.
         *
         * @param data The data to be copied into the new root node.
         * @return A pointer to the newly created root node.
         */
        node_type* add_root(const T& data)
        {
              roots_.emplace_back(std::make_unique<node_type>(data,
                  e_node_kind::root));
              return roots_.back().get();
        }

        /**
         * Adds a new root node to the tree using move semantics.
         * Useful for avoiding unnecessary data copies when adding a root node.
         *
         * @param data The data to be moved into the new root node.
         * @return A pointer to the newly created root node.
         */
        node_type* add_root(T&& data)
        {
            roots_.emplace_back(std::make_unique<node_type>(std::move(data),
                e_node_kind::root));
            return roots_.back().get();
        }

        /**
         * Retrieves all root nodes of the tree.
         *
         * @return A vector of raw pointers to the root nodes of the tree.
         */
        std::vector<node_type*> get_roots() const
        {
            std::vector<node_type*> result;
            result.reserve(roots_.size());
            for (const auto& ptr : roots_)
            {
                result.push_back(ptr.get());
            }
            return result;
        }

        /**
         * Applies a structural transformation to all root subtrees.
         * This function traverses each subtree from the root and applies the
         * given transformation type, such as mirroring or sorting.
         *
         * @param type The transformation operation to apply on each node. Must
         * be one of the values in e_type_of_tree_translation.
         */
        void translate(const e_type_of_tree_translation type)
        {
            for (auto& root : roots_)
            {
                translate_node(root.get(), type);
            }
        }

        /**
         * Traverses the tree using the specified traversal strategy.
         * Supports pre-order, in-order, and post-order traversals.
         *
         * @tparam OutputType The type to reinterpret each traversed node as.
         * @param type The type of traversal path to use.
         * @return A vector of raw pointers to the nodes in traversal order.
         */
        template<typename OutputType>
        std::vector<OutputType*> path(e_type_of_path_on_tree type) const
        {
            std::vector<OutputType*> result;
            for (auto* root : get_roots()) {
                traverse<OutputType>(root, type, result);
            }
            return result;
        }

        
        /**
         * Clears the entire tree, removing all root nodes.
         * All dynamically allocated nodes are deleted.
         */
        void clear() { roots_.clear(); }

    private:
        /**
        * Translates a subtree starting from a given root node.
        *
        * Supported transformations include:
        * - Mirroring the order of children.
        * - Sorting children in ascending or descending order.
        *
        * @param root Pointer to the root node of the subtree to transform.
        * @param type The type of transformation to apply.
        */
        void translate_node(node_type* root, e_type_of_tree_translation type)
        {
            if (!root) return;

            std::stack<node_type*> stack;
            stack.push(root);

            while (!stack.empty()) {
                node_type* node = stack.top();
                stack.pop();

                switch (type) {
                case e_type_of_tree_translation::mirror:
                    std::reverse(node->children_.begin(), node->children_.end());
                    break;
                case e_type_of_tree_translation::sort_ascending:
                    std::sort(node->children_.begin(), node->children_.end(),
                        [](auto& a, auto& b) {
                            return a->data() < b->data();
                        });
                    break;
                case e_type_of_tree_translation::sort_descending:
                    std::sort(node->children_.begin(), node->children_.end(),
                        [](auto& a, auto& b) {
                            return a->data() > b->data();
                        });
                    break;
                }

                for (auto it = node->children_.rbegin(); it !=
                    node->children_.rend(); ++it)
                {
                    stack.push(it->get());
                }
            }
        }

        /**
         * Performs a depth-first traversal of the tree from a given root.
         * This helper function populates the result vector according to the
         * specified traversal strategy.
         *
         * @tparam OutputType The type to reinterpret each node pointer as.
         * @param root The root node of the subtree to traverse.
         * @param type The traversal order (pre-order, in-order, post-order).
         * @param result The vector that will store the traversal result.
         */
        template<typename OutputType>
        void traverse(node_type* root, e_type_of_path_on_tree type,
            std::vector<OutputType*>& result) const
        {
            if (!root) return;

            using stack_item = std::tuple<node_type*, size_t, bool>;

            switch (type)
            {
                case e_type_of_path_on_tree::pre_order:
                {
                    std::stack<node_type*> stack;
                    stack.push(root);
                    while (!stack.empty())
                    {
                        node_type* node = stack.top();
                        stack.pop();
                        result.push_back(reinterpret_cast<OutputType*>(node));
                        auto& children = node->get_children();
                        for (auto it = children.rbegin(); it != children.rend();
                            ++it)
                            stack.push(*it);
                    }
                    break;
                }
                case e_type_of_path_on_tree::in_order:
                {
                    std::stack<stack_item> stack;
                    stack.emplace(root, 0, false);

                    while (!stack.empty()) {
                        auto& [node, index, visited] = stack.top();
                        auto& children = node->get_children();
                        const size_t mid = children.size() / 2;

                        if (node->is_leaf()) {
                            result.push_back(reinterpret_cast<OutputType*>(
                                node));
                            stack.pop();
                        }
                        else if (!visited) {
                            for (size_t i = children.size(); i-- > mid;)
                                stack.emplace(children[i], 0, false);

                            stack.top() = std::make_tuple(node, 0, true);

                            for (size_t i = mid; i-- > 0;)
                                stack.emplace(children[i], 0, false);
                        }
                        else {
                            result.push_back(reinterpret_cast<OutputType*>(
                                node));
                            stack.pop();
                        }
                    }
                    break;
                }
                case e_type_of_path_on_tree::post_order:
                {
                    std::stack<std::pair<node_type*, bool>> stack;
                    stack.emplace(root, false);

                    while (!stack.empty()) {
                        auto [node, visited] = stack.top();
                        stack.pop();

                        if (visited) {
                            result.push_back(reinterpret_cast<OutputType*>(
                                node));
                        }
                        else {
                            stack.emplace(node, true);
                            auto& children = node->get_children();
                            for (auto it = children.rbegin(); it !=
                                children.rend(); ++it)
                                stack.emplace(*it, false);
                        }
                    }
                    break;
                }
            }
        }

    private:
        /** List of root nodes in the tree. */
        std::vector<std::unique_ptr<node_type>> roots_;
    };    
};

