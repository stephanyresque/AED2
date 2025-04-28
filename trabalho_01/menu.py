def exibir_menu_principal():
    print("+=================================================+")
    print("|             Jogo de Damas -- Versão 0.1          |")
    print("+=================================================+")
    print("|                                                 |")
    print("| Menu:                                           |")
    print("| [1] Iniciar um novo jogo                        |")
    print("| [2] Carregar um jogo [interrompido/finalizado]   |")
    print("| [3] Salvar o jogo atual                         |")
    print("| [4] Sair                                        |")
    print("|                                                 |")
    print("+=================================================+")
    print("| Mensagem:                                       |")
    print("|                                                 |")
    print("+=================================================+")

def iniciar_novo_jogo():
    # Simulando o tabuleiro: 8 linhas x 8 colunas
    # "P" para peça do jogador
    # "C" para peça do computador
    # "." para casa vazia

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
            print(f"[{casa}]", end="")
        print()
    print("+=================================================+\n")

def salvar_jogo(tabuleiro, nome_arquivo="jogo_salvo.txt"):
    try:
        with open(nome_arquivo, "w") as arquivo:
            for linha in tabuleiro:
                arquivo.write(" ".join(linha) + "\n")
        print(f"\nJogo salvo com sucesso no arquivo '{nome_arquivo}'.\n")
    except Exception as e:
        print(f"\nErro ao salvar o jogo: {e}\n")

def carregar_jogo(nome_arquivo="jogo_salvo.txt"):
    try:
        tabuleiro = []
        with open(nome_arquivo, "r") as arquivo:
            linhas = arquivo.readlines()
            for linha in linhas:
                linha = linha.strip().split()
                tabuleiro.append(linha)

        print("\nJogo carregado com sucesso!\n")
        exibir_tabuleiro(tabuleiro)

        return tabuleiro

    except FileNotFoundError:
        print(f"\nArquivo '{nome_arquivo}' não encontrado. Salve um jogo antes de tentar carregar.\n")
    except Exception as e:
        print(f"\nErro ao carregar o jogo: {e}\n")


#=======================================================================#

def main():
    while True:
        exibir_menu_principal()
        escolha = input("Digite a opção desejada: ")

        if escolha == "1":
            print("Iniciando um novo jogo...")
            tabuleiro = iniciar_novo_jogo()

        elif escolha == "2":
            print("Carregando jogo...")
            try:
                tabuleiro = carregar_jogo()
            except Exception:
                print("\nNão foi possível carregar o jogo.\n")

        elif escolha == "3":
            print("Salvando jogo atual...")
            try:
                salvar_jogo(tabuleiro)
            except NameError:
                print("\nNenhum jogo iniciado ainda. Inicie um novo jogo primeiro!\n")

        elif escolha == "4":
            print("Saindo do jogo. Até a próxima!")
            break

        else:
            print("Opção inválida. Por favor, escolha uma opção válida.")

if __name__ == "__main__":
    main()
