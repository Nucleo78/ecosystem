#include <iostream>
#include <limits>
#include "../include/Universe.h"

int main() {
    int width, height, numSheep, numWolves, seed, numTurns;

    // Lecture des paramètres de l'utilisateur
    std::cout << "Quelle taille le monde devra faire ?\n";
    std::cout << "Largeur: ";
    std::cin >> width;
    std::cout << "Hauteur: ";
    std::cin >> height;

    std::cout << "Combien de moutons devra contenir l'univers ? ";
    std::cin >> numSheep;
    std::cout << "Combien de loups devra contenir l'univers ? ";
    std::cin >> numWolves;
    while ((numSheep + numWolves) > width * height) {
        std::cout << "Il y a trop de loup et de mouton par rapport a la taille de l'univers" << std::endl;
        std::cout << "Combien de moutons devra contenir l'univers ? ";
        std::cin >> numSheep;
        std::cout << "Combien de loups devra contenir l'univers ? ";
        std::cin >> numWolves;
    }

    std::cout << "Seed ? (mettre zero pour une seed aleatoire): ";
    std::cin >> seed;

    Universe universe(width, height);
    universe.initialize(numSheep, numWolves, seed);

    // Boucle de simulation
    int turn = 0;
    while (true) {
        std::cout << "\nTour actuel: " << turn << "\n";
        universe.printState();
        std::cout << "Nombre de moutons: " << universe.getNbSheep() << "    " << "Nombre de Loups: " << universe.getNbWolf() << std::endl;
        std::cout << "1. Passer au tour suivant\n";
        std::cout << "2. Continuer pendant x tours\n";
        std::cout << "3. Quitter\n";
        std::cout << "Choix: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            universe.simulateTurn();
            turn++;
        } else if (choice == 2) {
            std::cout << "Nombre de tours à simuler: ";
            std::cin >> numTurns;
            for (int i = 0; i < numTurns; ++i) {
                std::cout << "\nTour : " << turn << "\n";
                universe.simulateTurn();
                std::cout << "Nombre de moutons: " << universe.getNbSheep() << "    " << "Nombre de Loups: "
                    << universe.getNbWolf() << std::endl;
                if (!universe.hasAliveAnimals()) {
                    break;
                }
                turn++;
            }
        } else if (choice == 3) {
            break;
        } else {
            std::cout << "Choix invalide. Veuillez réessayer.\n";
        }

        if (!universe.hasAliveAnimals()) {
            universe.printState();
            std::cout << "All animals are dead. Simulation ends.\n";
            break;
        }
    }

    return 0;
}