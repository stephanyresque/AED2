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

        node_type* AddRoot(const T& data) {
            Roots_.emplace_back(std::make_unique<node_type>(data, NodeKind::Root));
            return Roots_.back().get();
        }

        node_type* AddRoot(T&& data) {
            Roots_.emplace_back(std::make_unique<node_type>(std::move(data), NodeKind::Root));
            return Roots_.back().get();
        }

        std::vector<node_type*> GetRoots() const {
            std::vector<node_type*> result;
            result.reserve(Roots_.size());
            for (const auto& ptr : Roots_) {
                result.push_back(ptr.get());
            }
            return result;
        }

        void Clear() {
            Roots_.clear();
        }

        void Translate(ETypeOfTreeTranslation type) {
            for (auto& root : Roots_) {
                TranslateNode(root.get(), type);
            }
        }

        template<typename OutputType>
        std::vector<OutputType*> Path(ETypeOfPathOnTree type) const {
            std::vector<OutputType*> result;
            for (auto* root : GetRoots()) {
                Traverse<OutputType>(root, type, result);
            }
            return result;
        }

        template<typename OutputType>
        std::map<std::string, std::vector<OutputType*>> Map() const {
            std::map<std::string, std::vector<OutputType*>> result;
            for (auto* root : GetRoots()) {
                std::vector<OutputType*> path;
                Traverse<OutputType>(root, ETypeOfPathOnTree::PreOrder, path);
                result["Root"] = path;
            }
            return result;
        }

    private:
        std::vector<std::unique_ptr<node_type>> Roots_;

        void TranslateNode(node_type* node, ETypeOfTreeTranslation type) {
            if (!node) return;

            switch (type) {
            case ETypeOfTreeTranslation::Mirror:
            case ETypeOfTreeTranslation::Reverse:
                std::reverse(node->Children_.begin(), node->Children_.end());
                break;
            case ETypeOfTreeTranslation::SortAscending:
                std::sort(node->Children_.begin(), node->Children_.end(), [](auto& a, auto& b) {
                    return a->Data() < b->Data();
                });
                break;
            case ETypeOfTreeTranslation::SortDescending:
                std::sort(node->Children_.begin(), node->Children_.end(), [](auto& a, auto& b) {
                    return a->Data() > b->Data();
                });
                break;
            }

            for (auto& child : node->Children_) {
                TranslateNode(child.get(), type);
            }
        }

        template<typename OutputType>
        void Traverse(node_type* node, ETypeOfPathOnTree type, std::vector<OutputType*>& result) const {
            if (!node) return;
            switch (type) {
            case ETypeOfPathOnTree::PreOrder:
                result.push_back(reinterpret_cast<OutputType*>(node));
                for (auto* child : node->GetChildren())
                    Traverse<OutputType>(child, type, result);
                break;
            case ETypeOfPathOnTree::InOrder:
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
            case ETypeOfPathOnTree::PostOrder:
                for (auto* child : node->GetChildren())
                    Traverse<OutputType>(child, type, result);
                result.push_back(reinterpret_cast<OutputType*>(node));
                break;
            }
        }
    };    
};

