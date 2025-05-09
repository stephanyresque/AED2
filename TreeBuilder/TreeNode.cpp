#include "TreeNode.h"

template <typename T>
typename tree_builder::tree_node<T>::node_type* tree_builder::tree_node<T>::
add_child(const T& data, e_node_kind kind)
{
    children_.emplace_back(std::make_unique<node_type>(data, kind));
    auto* child = children_.back().get();
    child->Parent_ = this;
    return child;
}

template <typename T>
typename tree_builder::tree_node<T>::node_type* tree_builder::tree_node<T>::
add_child(T&& data, e_node_kind kind)
{
    children_.emplace_back(std::make_unique<node_type>(std::move(data), kind));
    auto* child = children_.back().get();
    child->Parent_ = this;
    return child;
}

template <typename T>
void tree_builder::tree_node<T>::remove_child(node_type* child)
{
    auto it = std::find_if(children_.begin(), children_.end(),
        [&](const std::unique_ptr<node_type>& ptr)
            { return ptr.get() == child; }
        );
    if (it != children_.end()) {
        (*it)->Parent_ = nullptr;
        children_.erase(it);
    }
}

template <typename T>
std::vector<typename tree_builder::tree_node<T>::node_type*> tree_builder::
tree_node<T>::get_children() const noexcept
{
    std::vector<node_type*> result;
    result.reserve(children_.size());
    for (const auto& ptr : children_) {
        result.push_back(ptr.get());
    }
    return result;
}
