#include <windows.h>

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

// DLL Export Interface
extern "C" {
    __declspec(dllexport) tree_builder::tree_node<int>* CreateIntNode(int value, int 
    kind);
    __declspec(dllexport) void DeleteIntNode(tree_builder::tree_node<int>* node);

    __declspec(dllexport) tree_builder::Tree<int>* CreateIntTree();
    __declspec(dllexport) void DeleteIntTree(tree_builder::Tree<int>* tree);
    __declspec(dllexport) tree_builder::tree_node<int>* AddRootToIntTree(tree_builder::Tree<int>* tree, int value);
}