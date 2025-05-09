#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <map>

#include "TreeNode.h"

namespace tree_builder
{
    template<typename T>
    class tree
    {
    public:
        using node_type = tree_node<T>;

        tree() = default;
        ~tree() = default;

        node_type* add_root(const T& data);

        node_type* add_root(T&& data);

        std::vector<node_type*> get_roots() const;

        void translate(const e_type_of_tree_translation type);

        template<typename OutputType>
        std::vector<OutputType*> path(e_type_of_path_on_tree type) const;

        template<typename OutputType>
        std::map<std::string, std::vector<OutputType*>> map() const;

        void clear() { roots_.clear(); }

    private:
        std::vector<std::unique_ptr<node_type>> roots_;

        void translate_node(node_type* root, e_type_of_tree_translation type);

        template<typename OutputType>
        void traverse(node_type* node, e_type_of_path_on_tree type,
            std::vector<OutputType*>& result) const {
            if (!node) return;
            switch (type) {
            case e_type_of_path_on_tree::pre_order:
                result.push_back(reinterpret_cast<OutputType*>(node));
                for (auto* child : node->GetChildren())
                    Traverse<OutputType>(child, type, result);
                break;
            case e_type_of_path_on_tree::in_order:
                if (!node->IsLeaf()) {
                    auto children = node->GetChildren();
                    size_t mid = children.size() / 2;
                    for (size_t i = 0; i < mid; ++i)
                        Traverse<OutputType>(children[i], type, result);
                    result.push_back(reinterpret_cast<OutputType*>(node));
                    for (size_t i = mid; i < children.size(); ++i)
                        Traverse<OutputType>(children[i], type, result);
                } else {
                    result.push_back(reinterpret_cast<OutputType*>(node));
                }
                break;
            case e_type_of_path_on_tree::post_order:
                for (auto* child : node->GetChildren())
                    Traverse<OutputType>(child, type, result);
                result.push_back(reinterpret_cast<OutputType*>(node));
                break;
            }
        }
    };    
};

