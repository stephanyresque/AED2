# ==== IMPORTAÇÕES ===
import os
import string
import random
import ctypes
from os.path import basename
from datetime import datetime
from ctypes import c_int, POINTER

SAVE_DIR = "jogos_salvos"
os.makedirs(SAVE_DIR, exist_ok=True)

current_game_filename = None

_dll_path = os.path.join(os.path.dirname(__file__), "TreeBuilder.dll")
tree_dll  = ctypes.CDLL(_dll_path)

class TreeNode(ctypes.Structure): pass
class Tree(ctypes.Structure):     pass

class ETypeOfTreeTranslation:
    Mirror        = 0
    SortAscending = 1
    SortDescending= 2

tree_dll.start_tree.argtypes     = [c_int, c_int]
tree_dll.start_tree.restype      = POINTER(Tree)

tree_dll.get_first_node.argtypes = [POINTER(Tree)]
tree_dll.get_first_node.restype  = POINTER(TreeNode)

tree_dll.get_node_value.argtypes = [POINTER(TreeNode)]
tree_dll.get_node_value.restype  = c_int

tree_dll.get_children_count.argtypes = [POINTER(TreeNode)]
tree_dll.get_children_count.restype  = c_int

tree_dll.get_child_at.argtypes   = [POINTER(TreeNode), c_int]
tree_dll.get_child_at.restype    = POINTER(TreeNode)

tree_dll.delete_int_tree.argtypes = [POINTER(Tree)]
tree_dll.delete_int_tree.restype  = None

def _imprime_subarvore(node_ptr, code2board, nivel=0):
    val = tree_dll.get_node_value(node_ptr)
    indent = "   " * nivel

    print(f"{indent}└─ {val}")

    board = code2board.get(val)
    if board is not None:
        for row in board:

            print(f"{indent}    " + " ".join(row))

    cnt = tree_dll.get_children_count(node_ptr)
    for i in range(cnt):
        filho = tree_dll.get_child_at(node_ptr, i)
        _imprime_subarvore(filho, code2board, nivel + 1)

def coord_to_idx(coord: str):
    coord = coord.strip().upper()
    col = int(coord[1:]) - 1
    row = string.ascii_uppercase.index(coord[0])
    return row, col


def idx_to_coord(row: int, col: int):
    return f"{string.ascii_uppercase[row]}{col+1}"

def validar_e_mover(tabuleiro, origem, destino, turno):
    i0, j0 = coord_to_idx(origem)
    i1, j1 = coord_to_idx(destino)
    p = tabuleiro[i0][j0]
    esperado = "P" if turno=="H" else "C"

    if p != esperado:
        print("A casa de origem não contém sua peça.")
        return False

    if tabuleiro[i1][j1] != ".":
        print("A casa de destino não está vazia.")
        return False

    if abs(i1-i0)==1 and abs(j1-j0)==1:
        # executa a jogada
        tabuleiro[i1][j1] = p
        tabuleiro[i0][j0] = "."
        return True

    if abs(i1-i0)==2 and abs(j1-j0)==2:
        mi, mj = (i0+i1)//2, (j0+j1)//2
        mid = tabuleiro[mi][mj]
        inimigo = "C" if turno=="H" else "P"
        if mid == inimigo:

            tabuleiro[mi][mj] = "."
            tabuleiro[i1][j1] = p
            tabuleiro[i0][j0] = "."
            return True
        else:
            print("Você precisa pular sobre uma peça inimiga.")
            return False

    print("Movimento inválido. Só são permitidos deslocamentos diagonais de 1 ou salto de 2.")
    return False

def limpar_tela():
    os.system('cls' if os.name == 'nt' else 'clear')

def exibir_menu_principal():
    print("+=================================================+")
    print("|             Jogo de Damas -- Versão 0.1          |")
    print("+=================================================+")
    print("|                                                 |")
    print("| Menu:                                           |")
    print("| [1] Iniciar um novo jogo                        |")
    print("| [2] Carregar um jogo [interrompido/finalizado]   |")
    print("| [3] Sair                                        |")
    print("|                                                 |")
    print("+=================================================+")

def iniciar_novo_jogo():
    global current_game_filename
    current_game_filename = f"jogo_{datetime.now().strftime('%Y%m%d_%H%M%S')}.txt"

    tabuleiro = [["." for _ in range(8)] for _ in range(8)]

    for linha in range(3):  
        for coluna in range(8):
            if (linha + coluna) % 2 == 1:
                tabuleiro[linha][coluna] = "C"  

    for linha in range(5, 8): 
        for coluna in range(8):
            if (linha + coluna) % 2 == 1:
                tabuleiro[linha][coluna] = "P"  

    print("\nNovo jogo iniciado!\n")
    exibir_tabuleiro(tabuleiro)

    return tabuleiro

def exibir_tabuleiro(tabuleiro):
    print("+=================================================+")
    print("|                 Tabuleiro Atual                 |")
    print("+=================================================+")
    letras = "ABCDEFGH"
    print("   ", end="")
    for i in range(8):
        print(f" {i+1} ", end="")
    print()

    for idx, linha in enumerate(tabuleiro):
        print(f"{letras[idx]} ", end="")
        for casa in linha:
            if casa == "D":
                print("[D]", end="")  # Dama do jogador
            elif casa == "K":
                print("[K]", end="")  # Dama do computador
            else:
                print(f"[{casa}]", end="")
        print()
    print("+=================================================+\n")

def salvar_jogo(tabuleiro, nome_arquivo=None):
    global current_game_filename

    if nome_arquivo is None:
        if current_game_filename is None:
            current_game_filename = f"jogo_{datetime.now().strftime('%Y%m%d_%H%M%S')}.txt"
        nome_arquivo = current_game_filename
    else:
        current_game_filename = nome_arquivo

    try:
        caminho = os.path.join(SAVE_DIR, nome_arquivo)
        with open(caminho, "w") as arquivo:
            for linha in tabuleiro:
                arquivo.write(" ".join(linha) + "\n")
        print(f"\nJogo salvo com sucesso em '{caminho}'.\n")
    except Exception as e:
        print(f"\nErro ao salvar o jogo: {e}\n")

def carregar_jogo(nome_arquivo):
    global current_game_filename
    tabuleiro = []
    try:

        arquivo_base = basename(nome_arquivo)

        with open(nome_arquivo, "r") as arquivo:
            for linha in arquivo:
                tabuleiro.append(linha.strip().split())

        current_game_filename = arquivo_base

        print(f"\nJogo '{arquivo_base}' carregado com sucesso!\n")
        exibir_tabuleiro(tabuleiro)
        return tabuleiro

    except FileNotFoundError:
        print(f"\nArquivo '{basename(nome_arquivo)}' não encontrado.\n")
        return None
    except Exception as e:
        print(f"\nErro ao carregar o jogo: {e}\n")
        return None


def menu_partida(tabuleiro):
    turno = "H"  

    while True:
        limpar_tela()
        print(f"\nÉ a vez do jogador: {turno}")
        exibir_tabuleiro(tabuleiro)
        print("[M] Mover peça")
        print("[J] Salvar jogo")
        print("[I] Interromper jogo (salvar e voltar ao menu principal)")
        print("[P] Ver peças capturadas")
        print("[D] Ver damas")
        print("[A] Visualizar árvore de decisão")
        print("[S] Sair do jogo completamente")

        opcao = input("Escolha uma opção: ").strip().upper()

        if opcao == "M":
            if turno == "H":

                while True:
                    mov = input("Digite seu movimento (ex: A3 B4): ").split()
                    if len(mov)!=2:
                        print("Formato inválido, use Origem Destino.")
                        continue
                    origem, destino = mov
                    if validar_e_mover(tabuleiro, origem, destino, turno):
                        break
                turno = "C"  
            else:

                moves = gerar_moves_simples(tabuleiro, turno)
                if not moves:
                    print("Computador não tem jogadas válidas. Você vence!")
                    exit()
                escolhido = random.choice(moves)

                tabuleiro[:] = escolhido  
                print("Computador jogou.")
                turno = "H"
            input("Pressione Enter para continuar...")  

        elif opcao == "J":
            salvar_jogo(tabuleiro)

        elif opcao == "I":
            limpar_tela()
            salvar_jogo(tabuleiro)
            print("Jogo interrompido. Voltando ao menu principal...\n")
            break

        elif opcao == "P":
            limpar_tela()
            mostrar_pecas_capturadas(tabuleiro)
            input("Pressione Enter para voltar ao menu...")

        elif opcao == "D":
            limpar_tela()
            mostrar_damas(tabuleiro)
            input("Pressione Enter para voltar ao menu...")

        elif opcao == "A":
            limpar_tela()                       
            visualizar_arvore_decisao(tabuleiro)            
            input("Pressione Enter para voltar ao menu...")

        elif opcao == "S":
            print("Encerrando completamente o jogo. Até mais!")
            exit()

        else:
            print("Opção inválida. Tente novamente.")
            input("Pressione Enter para continuar...")

def mostrar_pecas_capturadas(tabuleiro):
    total_inicial = 12
    pecas_p = 0
    pecas_c = 0

    for linha in tabuleiro:
        for peca in linha:
            if peca == "P":
                pecas_p += 1
            elif peca == "C":
                pecas_c += 1

    capturadas_pelo_computador = total_inicial - pecas_p
    capturadas_pelo_jogador = total_inicial - pecas_c

    print("\nPeças capturadas:")
    print(f"Jogador perdeu: {capturadas_pelo_computador} peça(s)")
    print(f"Computador perdeu: {capturadas_pelo_jogador} peça(s)\n")

def mostrar_damas(tabuleiro):
    letras = "ABCDEFGH"
    damas_jogador = []
    damas_computador = []

    for i in range(8):
        for j in range(8):
            if tabuleiro[i][j] == "D":
                damas_jogador.append(f"{letras[i]}{j+1}")
            elif tabuleiro[i][j] == "K":
                damas_computador.append(f"{letras[i]}{j+1}")

    print("\nPeças do jogador que são damas (D):", damas_jogador if damas_jogador else "Nenhuma")
    print("Peças do computador que são damas (K):", damas_computador if damas_computador else "Nenhuma\n")

def codigo_para_tabuleiro(tabuleiro):

    flat = tuple(cell for row in tabuleiro for cell in row)
    raw = hash(flat)                
    return raw & 0x7FFFFFFF       


def gerar_moves_simples(tabuleiro, turno):

    direcoes = [(-1, -1), (-1, +1)] if turno == "H" else [(+1, -1), (+1, +1)]
    filhos = []
    for i in range(8):
        for j in range(8):
            p = tabuleiro[i][j]
            if (turno == "H" and p == "P") or (turno == "C" and p == "C"):
                for di, dj in direcoes:
                    ni, nj = i + di, j + dj
                    if 0 <= ni < 8 and 0 <= nj < 8 and tabuleiro[ni][nj] == ".":
                        # copia o tabuleiro e movimenta
                        novo = [row[:] for row in tabuleiro]
                        novo[ni][nj], novo[i][j] = p, "."
                        filhos.append(novo)
    return filhos


def visualizar_arvore_decisao(tabuleiro):

    turno = "H"

    codigo_raiz = codigo_para_tabuleiro(tabuleiro)

    code2board = {}
    code2board[codigo_raiz] = tabuleiro

    arv = tree_dll.start_tree(codigo_raiz, ETypeOfTreeTranslation.Mirror)
    raiz = tree_dll.get_first_node(arv)

    filhos_tab = gerar_moves_simples(tabuleiro, turno)
    for filho_tab in filhos_tab:
        codigo_filho = codigo_para_tabuleiro(filho_tab)
        code2board[codigo_filho] = filho_tab
        tree_dll.add_child(raiz, codigo_filho)

    print("\nÁrvore de decisão:\n")
    _imprime_subarvore(raiz, code2board)

    tree_dll.delete_int_tree(arv)

def menu():
    while True:
        limpar_tela()
        exibir_menu_principal()
        escolha = input("Digite a opção desejada: ")

        if escolha == "1":
            limpar_tela()
            print("Iniciando um novo jogo...")
            tabuleiro = iniciar_novo_jogo()
            menu_partida(tabuleiro)

        elif escolha == "2":
            limpar_tela()
            # lista os jogos salvos
            arquivos = os.listdir(SAVE_DIR)
            print("Jogos salvos:")
            for f in arquivos:
                print("  -", f)
            print()
            nome = input("Digite o nome do arquivo a carregar: ").strip()
            caminho = os.path.join(SAVE_DIR, nome)
            print(f"\nCarregando '{caminho}'...")
            tabuleiro = carregar_jogo(caminho)
            if tabuleiro is not None:
                menu_partida(tabuleiro)
            else:
                input("Pressione Enter para voltar ao menu principal...")

        elif escolha == "3":
            print("Saindo do jogo. Até a próxima!")
            break

        else:
            print("Opção inválida. Por favor, escolha uma opção válida.")
            input("Pressione Enter para continuar...")