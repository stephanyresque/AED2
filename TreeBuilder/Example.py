import ctypes
from ctypes import c_int, POINTER

tree_dll = ctypes.CDLL("./TreeBuilder.dll")

class ETypeOfTreeTranslation:
    Mirror = 0
    Reverse = 1
    SortAscending = 2
    SortDescending = 3

class TreeNode(ctypes.Structure):
    pass

class Tree(ctypes.Structure):
    pass

tree_dll.StartTree.argtypes = [c_int, c_int]
tree_dll.StartTree.restype = POINTER(Tree)

tree_dll.TranslateTree.argtypes = [POINTER(Tree), c_int]
tree_dll.TranslateTree.restype = None

tree_dll.GetFirstNode.argtypes = [POINTER(Tree)]
tree_dll.GetFirstNode.restype = POINTER(TreeNode)

tree_dll.GetNodeValue.argtypes = [POINTER(TreeNode)]
tree_dll.GetNodeValue.restype = c_int

tree_dll.DeleteIntTree.argtypes = [POINTER(Tree)]
tree_dll.DeleteIntTree.restype = None