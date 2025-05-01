import os
from datetime import datetime
from os.path import basename

SAVE_DIR = "jogos_salvos"
os.makedirs(SAVE_DIR, exist_ok=True)

current_game_filename = None

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
        # Toma só o nome do arquivo, sem diretório
        arquivo_base = basename(nome_arquivo)

        with open(nome_arquivo, "r") as arquivo:
            for linha in arquivo:
                tabuleiro.append(linha.strip().split())

        # Guarda apenas o basename na sessão
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
            print(f"Simulando movimentação do jogador {turno}...")  
            turno = "C" if turno == "H" else "H"  

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
            visualizar_arvore_decisao()          
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

def visualizar_arvore_decisao():
    print("\nVisualização da árvore de decisão ainda não está integrada.")
    print("Essa funcionalidade será implementada e conectada ao módulo C++.\n")

#=======================================================================#

def main():
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

if __name__ == "__main__":
    main()
