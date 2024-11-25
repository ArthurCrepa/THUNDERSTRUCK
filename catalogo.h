//Catalogo de monstruos a alamcenar en un arbol binario
//Arturo Gómez Rodríguez - A01029606
//Desarrollado a apartir del 21 de octubre de 2024
#pragma once
#include <fstream> 
#include <iostream>
#include <string>

//Tras varias versiones logré implementar el template
template <typename T>
class MonstruosNodoArbol{
public:
    T info;
    int valor; //Valor asignado al monstruo para poder ser estructurado
    MonstruosNodoArbol <T>* left;
    MonstruosNodoArbol <T>* right;

    MonstruosNodoArbol(T dato, int val):info(dato), valor(val), left(nullptr), right(nullptr){}
};

template <typename T>
class Catalogo{
public:
    Catalogo();
    ~Catalogo();
    bool insertaIterativo(T dato, int val);
    bool loadFromCSV(const std::string filename);
    void liberarArbol();


private:
    MonstruosNodoArbol<T>* root;
    void liberarArbol(MonstruosNodoArbol<T>* nodo);
};

template <typename T>
Catalogo<T>::Catalogo():root(nullptr){}

template <typename T>
Catalogo<T>::~Catalogo(){
    liberarArbol(root); //Llamamos destructor
}
// En una version previa, se asignaria aleatoriamente nuevamente los valores al catalogo, lo cual fue brutal subiendo a una complejidad BIG O de N al cuadrado.
//Por razones obvias, fue eleminado ya que practicamente era inutil
template <typename T>
bool Catalogo<T>::insertaIterativo(T dato, int val){
    MonstruosNodoArbol<T>** current = &root;
    while (*current) {
        if (val < (*current)->valor)
            current = &(*current)->left;
        else if (val > (*current)->valor)
            current = &(*current)->right;
        else
            return false; //Ingrese valores de retorno
    }
    *current = new MonstruosNodoArbol<T>(dato, val);
    return *current != nullptr;
}

template <typename T>
void Catalogo<T>::liberarArbol(){
    liberarArbol(root);
    root = nullptr;
    //Asignamos root a nullpointer como medida de seguridad para que el codigo no falle
}

template <typename T>
void Catalogo<T>::liberarArbol(MonstruosNodoArbol<T>* nodo){
    //Caso peculiar: si se pone el siguiente cout esto se realizara hasta haber liberado el arbol completo, lo cual fue una confirmacion visual accidental del metodo
    //De hecho fue gracioso ya que este problema fue mencionado hace varias clases
    //cout<<"liberando Arbol";
    if(nodo){
        liberarArbol(nodo->left);
        liberarArbol(nodo->right);
        delete nodo;
    }
}
// Las siguientes condiciones fueron implementadas para poder identificar un error en proceso del .txt
template <typename T>
bool Catalogo<T>::loadFromCSV(const std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "No se pudo abrir el archivo " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Ignorar líneas vacías
        if (line.empty()) {
            std::cout << "Línea vacía encontrada, ignorada." << std::endl;
            continue;
        }

        // Buscar delimitador
        size_t delimPos = line.find(',');
        if (delimPos == std::string::npos) {
            std::cout << "Línea inválida (sin delimitador ','): " << line << std::endl;
            continue;
        }

        // Dividir la línea en dato y valor
        std::string dato = line.substr(0, delimPos);
        std::string valorStr = line.substr(delimPos + 1);

        // Validar que el valor es un número
        try {
            int valor = std::stoi(valorStr);
            insertaIterativo(dato, valor); // Insertar en el árbol
        } catch (const std::invalid_argument& e) {
            std::cout << "Valor no numérico en línea: " << line << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "Valor fuera de rango en línea: " << line << std::endl;
        }
    }

    file.close();
    
    return true;
}
