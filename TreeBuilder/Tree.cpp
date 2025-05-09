#include "Tree.h"

#include <stack>

template <typename T>
typename tree_builder::tree<T>::node_type* tree_builder::tree<T>::add_root(
    const T& data)
{
    roots_.emplace_back(std::make_unique<node_type>(data, e_node_kind::root));
    return roots_.back().get();
}

template <typename T>
typename tree_builder::tree<T>::node_type* tree_builder::tree<T>::
add_root(T&& data)
{
    roots_.emplace_back(std::make_unique<node_type>(std::move(data), e_node_kind::root));
    return roots_.back().get();
}

template <typename T>
std::vector<typename tree_builder::tree<T>::node_type*> tree_builder::tree<T>::
get_roots() const
{
    std::vector<node_type*> result;
    result.reserve(roots_.size());
    for (const auto& ptr : roots_) {
        result.push_back(ptr.get());
    }
    return result;
}

template <typename T>
void tree_builder::tree<T>::translate(const e_type_of_tree_translation type)
{
    for (auto& root : roots_) {
        translate_node(root.get(), type);
    }
}

template <typename T>
template <typename OutputType>
std::vector<OutputType*> tree_builder::tree<T>::path(
    e_type_of_path_on_tree type) const
{
    std::vector<OutputType*> result;
    for (auto* root : get_roots()) {
        traverse<OutputType>(root, type, result);
    }
    return result;
}

template <typename T>
template <typename OutputType>
std::map<std::string, std::vector<OutputType*>> tree_builder::tree<T>::
map() const
{
    std::map<std::string, std::vector<OutputType*>> result;
    for (auto* root : get_roots()) {
        std::vector<OutputType*> path;
        traverse<OutputType>(root, e_type_of_path_on_tree::pre_order, path);
        result["Root"] = path;
    }
    return result;
}

template <typename T>
void tree_builder::tree<T>::translate_node(node_type* root,
    e_type_of_tree_translation type)
{
    if (!root) return;

    std::stack<node_type*> stack;
    stack.push(root);

    while (!stack.empty()) {
        node_type* node = stack.top();
        stack.pop();

        switch (type) {
        case e_type_of_tree_translation::mirror:
            std::reverse(node->Children_.begin(), node->Children_.end());
            break;
        case e_type_of_tree_translation::sort_ascending:
            std::sort(node->Children_.begin(), node->Children_.end(),
                [](auto& a, auto& b) {
                    return a->Data() < b->Data();
                });
            break;
        case e_type_of_tree_translation::sort_descending:
            std::sort(node->Children_.begin(), node->Children_.end(),
                [](auto& a, auto& b) {
                    return a->Data() > b->Data();
                });
            break;
        }

        for (auto it = node->Children_.rbegin(); it != node->Children_.rend();
            ++it)
        {
            stack.push(it->get()); 
        }
    }
}
