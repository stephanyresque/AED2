#pragma once
#include <cstdint>

namespace tree_builder
{
    /**
     * Enum representing the kind of node within a tree.
     * This enum is used to distinguish the role of each node within the tree
     * structure.
     *
     * - root: The root node of the tree. This is the topmost node and has no
     * parent.
     * - internal: An internal node within the tree. These nodes have a parent
     * and may have one or more children.
     * - leaf: A leaf node. These nodes do not have any children.
     */
    enum class e_node_kind : uint8_t {
        root,
        internal,
        leaf
    };

    /**
     * Enum representing different types of tree translation operations.
     * This enum is used to indicate how the structure or order of a tree should
     * be transformed.
     *
     * - mirror: Creates a mirror image of the tree. Each node's children are
     * reversed, effectively reflecting the tree.
     * - reverse: Reverses the order of nodes at each level without changing
     * structure. Similar to mirror but typically applied on a per-level basis.
     * - sort_ascending: Sorts the nodes in ascending order based on a
     * comparator.
     * - sort_descending: Sorts the nodes in descending order based on a
     * comparator.
     */
    enum class e_type_of_tree_translation : uint8_t {
        mirror,
        reverse,
        sort_ascending,
        sort_descending
    };

    /**
     * Enum representing different types of tree traversal paths.
     * This enum defines how nodes are visited when traversing a tree.
     *
     * - pre_order: Visits the current node before its child nodes (Root ->
     * Left -> Right).
     * - in_order: Visits the left child, then the current node, then the right
     * child (Left -> Root -> Right).
     * - post_order: Visits all child nodes before the current node (Left ->
     * Right -> Root).
     */
    enum class e_type_of_path_on_tree : uint8_t {
        pre_order,
        in_order,
        post_order
    };
};    