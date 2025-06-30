# UnionFind

## Descrição

Este programa implementa um sistema de **conjuntos disjuntos** (Union-Find) em C++. Ele fornece um menu interativo via terminal que permite criar conjuntos, unir conjuntos, encontrar representantes e consultar tamanhos de conjuntos.

## Estrutura de arquivos

```
UnionFind/
├── Public/
│   ├── Main.cpp    # Função main, menu de texto e parsing de comandos
│   └── Union.h     # Declarações de Element, DisjointSet e UnionManager
├── Private/
│   └── Union.cpp   # Implementação dos métodos de UnionManager
├── .idea/          # Configurações do projeto (IDE IntelliJ/Rider)
└── UnionFind.sln   # Solução do Visual Studio (opcional)
```

## Pré-requisitos

* **Windows** com **Visual Studio** instalado (ou Build Tools for Visual C++)
* **Linux/macOS** com **g++** instalado

## Como compilar

### Windows (Developer Command Prompt for VS)

1. Abra o **x64 Native Tools Command Prompt for VS**.
2. Navegue até a pasta do projeto:

   ```batch
   cd C:\caminho\para\trabalho_02\UnionFind
   ```
3. Compile com o compilador do Visual C++:

   ```batch
   cl /EHsc /std:c++17 /I Public Public\Main.cpp Private\Union.cpp /Fe:unionfind.exe
   ```
4. Execute o programa:

   ```batch
   unionfind.exe
   ```

### Linux/macOS (g++)

1. Abra um terminal e navegue até a pasta do projeto:

   ```bash
   cd /caminho/para/trabalho_02/UnionFind
   ```
2. Compile com g++:

   ```bash
   g++ -std=c++17 -Wall -IPublic Public/Main.cpp Private/Union.cpp -o unionfind
   ```
3. Execute:

   ```bash
   ./unionfind
   ```

## Uso

No menu interativo, digite um dos comandos disponíveis:

* `Make-Set <valor>`      : Cria um novo conjunto contendo `<valor>`.
* `Show-Set <valor|id>`   : Exibe elementos do conjunto que contém `<valor>` ou com o ID informado.
* `Destroy-Set <valor|id>`: Remove completamente o conjunto correspondente.
* `Union <valor1> <valor2>`: Une os conjuntos de `<valor1>` e `<valor2>`.
* `Find-Set <valor>`      : Mostra o representante do conjunto de `<valor>`.
* `Size-Set <valor|id>`   : Mostra o tamanho do conjunto.
* `Exit`                  : Fecha o programa.

**Exemplo de sessão**:

```
> Make-Set A
> Make-Set B
> Union A B
> Show-Set A      # Lista A B
> Size-Set A      # Mostra tamanho 2
> Exit
```

---

