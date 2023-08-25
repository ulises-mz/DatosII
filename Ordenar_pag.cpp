#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>

const int PAGE_SIZE = 1024;  // Tamaño de página en bytes (1KB)
const int MEMORY_LIMIT = 12 * PAGE_SIZE;  // Límite de memoria en bytes (12KB)
const int NUMBERS_PER_PAGE = PAGE_SIZE / sizeof(int);  // Números por página

class PagedArray {
private:
    std::unordered_map<int, std::vector<int>> pages;  // Almacena las páginas cargadas en memoria
    std::queue<int> pageOrder;  // Almacena el orden en que se cargaron las páginas
    int currentPageCount = 0;

   

public:
    PagedArray(const char* filename) {
        // Lee el archivo decimal y crea las páginas en binario
        std::ifstream inputFile(filename);
        if (!inputFile) {
            std::cerr << "No se pudo abrir el archivo." << std::endl;
            return;
        }

        std::string line;
        int pageNumber = 0;
        std::vector<int> currentPage;

        while (std::getline(inputFile, line, ',')) {
            int num;
            try {
                num = std::stoi(line);
            } catch (const std::exception& e) {
                // Manejar errores de conversión si es necesario
                continue;
            }
            
            if (currentPage.size() >= NUMBERS_PER_PAGE) {
                pages[pageNumber] = currentPage;
                pageOrder.push(pageNumber);
                currentPage.clear();
                pageNumber++;
            }
            currentPage.push_back(num);
        }

        if (!currentPage.empty()) {
            pages[pageNumber] = currentPage;
            pageOrder.push(pageNumber);
        }

        inputFile.close();
        currentPageCount = pageNumber + 1;
    }
    int& operator[](int index) {
        int pageNumber = index / NUMBERS_PER_PAGE;
        int pageIndex = index % NUMBERS_PER_PAGE;

        if (pages.find(pageNumber) == pages.end()) {
            std::cerr << "Página no encontrada en memoria." << std::endl;
            return pages.begin()->second[0];  // Devuelve una referencia temporal en caso de error
        }

        pageOrder.push(pageNumber);  // Actualiza el orden de páginas cargadas
std::cout << "Accediendo a la página " << pageNumber << ", índice " << pageIndex << std::endl;

        return pages[pageNumber][pageIndex];
    }
};
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// ... Clase PagedArray y su implementación aquí ...

// Algoritmo de ordenamiento QuickSort
void quickSort(PagedArray& arr, int left, int right) {
    if (left < right) {
        int pivot = arr[right];
        int i = left - 1;
        for (int j = left; j <= right - 1; ++j) {
            if (arr[j] <= pivot) {
                ++i;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[right]);
        int pivotIndex = i + 1;

        quickSort(arr, left, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, right);
    }
}

// Algoritmo de ordenamiento Insertion Sort
void insertionSort(PagedArray& arr, int n) {
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// Algoritmo de ordenamiento Selection Sort
void selectionSort(PagedArray& arr, int n) {
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        std::swap(arr[i], arr[minIndex]);
    }
}

// Algoritmo de ordenamiento propuesto
void bubbleSort(PagedArray& arr, int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    // Analiza los argumentos de línea de comandos
    if (argc != 7) {
        std::cerr << argc << std::endl;
        std::cerr << "Uso: " << argv[0] << " -i <archivo> -a {QS|IS|SS|PS} -o <archivo_resultado>" << std::endl;
        return 1;
    }

    const char* inputFile = nullptr;
    const char* algorithm = nullptr;
    const char* outputFile = nullptr;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-i" && i + 1 < argc) {
            inputFile = argv[i + 1];
        } else if (std::string(argv[i]) == "-a" && i + 1 < argc) {
            algorithm = argv[i + 1];
        } else if (std::string(argv[i]) == "-o" && i + 1 < argc) {
            outputFile = argv[i + 1];
        }
    }

    if (!inputFile || !algorithm || !outputFile) {
        std::cerr << "Argumentos incompletos." << std::endl;
        return 1;
    }

    // Crea la instancia de PagedArray con el archivo binario
    PagedArray pagedArray(inputFile);

    // Realiza una copia del archivo binario para trabajar con él
    PagedArray pagedArrayCopy(inputFile);
int n = 0; // Initialize the variable to hold the number of elements

    // Open the input file to calculate the number of elements
    std::ifstream inputFileStream(inputFile);
    std::string line;
    while (std::getline(inputFileStream, line, ',')) {
        try {
            std::stoi(line); // Try to convert the line to an integer
            n++; // Increment the count if the conversion is successful
        } catch (const std::exception& e) {
            // Handle conversion errors if necessary
            continue;
        }
    }
    inputFileStream.close();

    // Ejecuta el algoritmo de ordenamiento seleccionado
    if (std::string(algorithm) == "QS") {
        quickSort(pagedArray, 0, n - 1);
    } else if (std::string(algorithm) == "IS") {
        insertionSort(pagedArray, n);
    } else if (std::string(algorithm) == "SS") {
        selectionSort(pagedArray, n);
    } else if (std::string(algorithm) == "PS") {
        bubbleSort(pagedArray, n);
    } else {
        std::cerr << "Algoritmo no válido." << std::endl;
        return 1;
    }

    // Escribe el resultado ordenado en el archivo resultado
    std::ofstream output(outputFile);
    for (int i = 0; i < n; ++i) {
        int num = pagedArray[i];
        output << num << std::endl;
    }
    output.close();

    std::cout << "Ordenamiento completado y guardado en " << outputFile << std::endl;

    return 0;
}

