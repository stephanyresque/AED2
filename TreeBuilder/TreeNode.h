#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "TreeBuilderData.h"

namespace tree_builder
{
    /**
     * A generic node class used to construct tree structures.
     * This template class represents a node in a tree, which stores a piece of
     * data, its kind (root, internal or leaf), and its relationship to parent
     * and children nodes.
     *
     * @tparam T The type of the data stored in the node.
     */
    template<typename T>
    class tree_node
    {
    public:
         using node_type = tree_node<T>;

        /**
         * Constructs a new tree node by copying the given data.
         *
         * @param data The data to store in the node.
         * @param kind The kind of the node (default is e_node_kind::internal).
         */
        explicit tree_node(const T& data, e_node_kind kind =
            e_node_kind::internal) : data_(data), parent_(nullptr), kind_(kind)
            {}

        /**
         * Constructs a new tree node by moving the given data.
         *
         * @param data The data to move into the node.
         * @param kind The kind of the node (default is e_node_kind::internal).
         */
        explicit tree_node(T&& data, e_node_kind kind = e_node_kind::internal)
            noexcept : data_(std::move(data)), parent_(nullptr), kind_(kind) {}

        // Deleted copy constructor and assignment operator
        tree_node(const tree_node&) = delete;
        tree_node& operator=(const tree_node&) = delete;

        // Defaulted move constructor and move assignment operator
        tree_node(tree_node&&) noexcept = default;
        tree_node& operator=(tree_node&&) noexcept = default;

        // Destructor
        ~tree_node() = default;

    public:    
        /**
         * Adds a new child node with a copy of the given data.
         * This method creates a new child node using the provided data and node
         * kind. The newly created child will have the current node set as its
         * parent.
         *
         * @param data The data to be stored in the new child node (copied).
         * @param kind The type of the node (e_node_kind).
         * @return A pointer to the newly created child node.
         */
        node_type* add_child(const T& data, e_node_kind kind =
            e_node_kind::internal);

        /**
         * Adds a new child node by moving the given data.
         * This overload creates a new child node using move semantics to avoid
         * unnecessary copies. The newly created child will have the current
         * node set as its parent.
         *
         * @param data The data to be moved into the new child node.
         * @param kind The type of the node (e_node_kind).
         * @return A pointer to the newly created child node.
         */
        node_type* add_child(T&& data, e_node_kind kind =
            e_node_kind::internal);

        /**
         * Removes a specific child node from this node.
         * This function searches for the specified child pointer in the current
         * node's children. If found, it detaches the child (resets its parent
         * pointer) and erases it from the list.
         *
         * @param child A pointer to the child node to be removed.
         */
        void remove_child(node_type* child);

        /**
         * Retrieves all child nodes of this node.
         * Returns a vector of raw pointers to each child node in the order they
         * were added.
         *
         * @return A vector of raw pointers to the current node's children.
         */
        std::vector<node_type*> get_children() const noexcept;

        /**
         * Gets a const reference to the data stored in the node.
         *
         * @return Const reference to the node's data.
         */
        const T& get_data() const noexcept { return data_; }

        /**
         * Gets a mutable reference to the data stored in the node.
         *
         * @return Reference to the node's data.
         */
        T& data() noexcept { return data_; }

        /**
         * Returns a pointer to the parent node, or nullptr if this node is the
         * root.
         *
         * @return A raw pointer to the parent node.
         */
        node_type* get_parent() const noexcept { return parent_; }

        /**
         * Checks if this node is a leaf (has no children).
         *
         * @return True if the node has no children, false otherwise.
         */
        bool is_leaf() const noexcept { return children_.empty(); }

        /**
         * Gets the kind of this node.
         *
         * @return The node kind (internal or leaf).
         */
        e_node_kind get_kind() const noexcept { return kind_; }

        /**
         * @brief Sets the kind of this node.
         *
         * @param kind The new node kind.
         */
        void set_kind(e_node_kind kind) noexcept { kind_ = kind; }

    private:
        /** The data stored in this node. */
        T data_;
     
        /** Pointer to the parent node (nullptr if root). */
        node_type* parent_;
     
        /** The kind of the node (internal or leaf). */
        e_node_kind kind_;
     
        /** List of owned child nodes. */
        std::vector<std::unique_ptr<node_type>> children_;
    };
};