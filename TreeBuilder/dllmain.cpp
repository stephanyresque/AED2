#include <windows.h>

#include "Tree.h"
#include "TreeNode.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
	
    return TRUE;
}

using namespace tree_builder;

// DLL Export Interface
extern "C" {
    __declspec(dllexport) tree<int>* start_tree(const int root_value,
        int translation_type)
    {
        auto* new_tree = new tree<int>();
        new_tree->add_root(root_value);
        new_tree->translate(static_cast<e_type_of_tree_translation>(
            translation_type));
        return new_tree;
    }

    __declspec(dllexport) void translate_tree(tree<int>* tree,
        int translation_type)
    {
        if (tree)
        {
            tree->translate(static_cast<e_type_of_tree_translation>(
                translation_type));
        }
    }

    __declspec(dllexport) tree_node<int>* get_first_node(const tree<int>* tree)
    {
        if (!tree) return nullptr;
        const auto roots = tree->get_roots();
        return roots.empty() ? nullptr : roots[0];
    }

    __declspec(dllexport) int get_node_value(const tree_node<int>* node)
    {
        return node ? node->get_data() : 0;
    }

    __declspec(dllexport) tree_node<int>* add_child(tree_node<int>* parent,
        const int value)
    {
        return parent->add_child(value);
    }

    __declspec(dllexport) int get_children_count(const tree_node<int>* node)
    {
        if (!node) return 0;
        return static_cast<int>(node->get_children().size());
    }

    __declspec(dllexport) tree_node<int>* get_child_at(tree_node<int>* node,
        int index)
    {
        if (!node) return nullptr;
        const auto& children = node->get_children();
        if (index < 0 || index >= static_cast<int>(children.size()))
            return nullptr;
        return children[index];
    }

    __declspec(dllexport) void delete_int_tree(tree<int>* tree) { delete tree; }
}