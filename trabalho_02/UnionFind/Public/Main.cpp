#include "Union.h"
#include <iostream>
#include <sstream>
#include <string>

#ifdef _WIN32
#include <windows.h>
void clearScreen() { system("cls"); }
#else
void clearScreen() { std::cout << "\033[2J\033[1;1H"; }
#endif

void printMenu(const std::string& lastAction) {
    std::cout << "=========== SISTEMA DE CONJUNTOS DISJUNTOS ===========\n";
    std::cout << "Comandos disponíveis:\n\n";
    std::cout << "1. Make-Set <valor>\n";
    std::cout << "2. Show-Set <valor ou id>\n";
    std::cout << "3. Destroy-Set <valor ou id>\n";
    std::cout << "4. Union <valor1> <valor2>\n";
    std::cout << "5. Find-Set <valor>\n";
    std::cout << "6. Size-Set <valor ou id>\n";
    std::cout << "7. Exit\n\n";
    std::cout << "Última operação: " << lastAction << "\n";
    std::cout << "=======================================================\n";
}

int main() {
    UnionManager manager;
    std::string input;

    while (true) {
        clearScreen();
        printMenu(manager.GetLastAction());
        std::cout << "> ";
        std::getline(std::cin, input);
        std::stringstream ss(input);

        std::string cmd, arg1, arg2;
        ss >> cmd >> arg1 >> arg2;

        if (cmd == "Exit" || cmd == "Quit") break;
        else if (cmd == "Make-Set") manager.MakeSet(arg1);
        else if (cmd == "Show-Set") {
            if (arg1.find('.') != std::string::npos)
                manager.ShowSet(arg1);
            else
                manager.ShowSet(std::stoi(arg1));
        }
        else if (cmd == "Destroy-Set") {
            if (arg1.find('.') != std::string::npos)
                manager.DestroySet(arg1);
            else
                manager.DestroySet(std::stoi(arg1));
        }
        else if (cmd == "Union") manager.Union(arg1, arg2);
        else if (cmd == "Find-Set") manager.FindSet(arg1);
        else if (cmd == "Size-Set") {
            if (arg1.find('.') != std::string::npos)
                manager.SizeSet(arg1);
            else
                manager.SizeSetById(std::stoi(arg1));
        }
        else manager.MakeSet("Comando inválido");
    }

    return 0;
}
