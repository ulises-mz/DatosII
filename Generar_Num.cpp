#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>

int main() {
    // Crear un vector con números del 1 al 10000
    std::vector<int> numeros;
    for (int i = 1; i <= 36*256; ++i) {
        numeros.push_back(i);
    }

    // Mezclar los números en orden aleatorio
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(numeros.begin(), numeros.end(), generator);

    // Crear y escribir en el archivo
    std::ofstream archivo("numeros_36KB.txt");
    if (archivo.is_open()) {
        for (size_t i = 0; i < numeros.size(); ++i) {
            archivo << numeros[i];
            if (i != numeros.size() - 1) {
                archivo << ",";
            }
        }
        archivo.close();
        std::cout << "Archivo generado exitosamente.\n";
    } else {
        std::cerr << "No se pudo abrir el archivo.\n";
    }

    return 0;
}
