//Grafo para el nuevo modelo del dungeon
//Arturo Gómez Rodríguez - A01029606
//Desarrollado a apartir del 17 de noviembre de 2024
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "listaDobleDungeon.h" 
//No se incluyo using namespace std debido a problemas con el compilador en MacOS, por ende se ha utilizado std:: ...
//Inclui este archivo ya que tiene "CuartoDungeon"

//A base de "snippets" del codigo visto en clase
template <typename T>
class Graph{
private:
    class Vertex{
    public:
        T data;
        std::list<int> edges; // uso de #include <list>
    };
    Vertex* verts;
    unsigned int size;// Size no puede ser negativo

    //Implementamos metodo para borrar ya que este no puede ser modificado por el usuario
    //A su vez, debido al formato del template se sigue esta estructura antes de lllamar a los metodos publicos
    void liberarGraph(){
        std::cout<< "Liberando Dungeon... \n";
        if(verts){
            delete[] verts;
            verts = nullptr;
            size = 0;
            std::cout<< "Dungeon liberado!\n";
        }
        else{
            std::cout<<"No hay dungeon que liberar";
            //NOTA. con return 0; hay error, recordar que void no hay retornos.
        }
    }

public:
    Graph():verts(nullptr), size(0){}
    ~Graph(){//Se incluye destructor de Graph para poder liberar el grafo
        liberarGraph();//Metodo para liberar se mantiene en privado para no ser modificado por el usuario
    }
    //hacemos metodos y condiciones de retorno, metodos similares a las vistas en clase.
    bool createDungeon(unsigned int newSize){
        if(newSize== 0 || verts){
            return false;//No crear si su valor es 0 o si ya hay memoria
        }
        verts = new(std::nothrow)Vertex[newSize];
        if(!verts){
            return false;//Condicion por si no hay memoria
        }
        size= newSize; //Nuevo tamaño
        return true;
    }
    bool add(unsigned int de, unsigned int a){
        if(de>= size || a>= size){
            return false;//indices fuera de rango
        }
        verts[de].edges.push_back(a);
        return true;
    }
    bool setVerts(unsigned int vertex, const T& dato){// debe de ser Unsigned int para no recibir datos negativos
        if (vertex>= size){
            return false;
        }
        verts[vertex].data = dato;//Asigna los datos al vertice
        return true;
    }
    
    // Las siguientes condiciones fueron implementadas para poder identificar un error en proceso del .txt
    bool loadGraphFile(const std::string filename){ //tambien funciona bool loadGraphFile(const std::string& filename)
        std::ifstream file(filename);
        if (!file.is_open()){
            std::cout << "No se pudo abrir el archivo " << filename << std::endl; //Condicion para indentificar posible error al abrirlo
            return false;
        }
        string header;
        getline(file, header); // Leer y descartar el encabezado
        unsigned int variousVertex;
        if (!(file >> variousVertex)){
            std::cout<< "No se pudo leer el número de vértices\n"; //Condicion en caso de que el .txt no se lea adecuadamente
            return false;
        }

        if (!createGraph(variousVertex)){
            std::cout<< "No se pudo crear el grafo con "<< variousVertex<< " vértices\n"; //Condicion 
            return false;
        }

        std::string line;
        std::getline(file, line); //Saltar a la siguiente línea

        for (unsigned int i = 0; i < variousVertex; ++i){
            if (!std::getline(file, line)){
                std::cout << "No se pudo leer la línea para el vértice "<< i<< "\n"; //Condicion
                return false;
            }

            std::istringstream ss(line); //Logramos leer adecuadamente el .txt designado
            int nearby;
            while (ss >> nearby){
                if (!add(i, nearby)){
                    std::cout << "No se pudo hacer la conexión entre "<< i << " a "<< nearby<< "\n"; //condicion de conexiones entre vertices
                }
            }
        }

        std::cout << "Archivo cargado correctamente.\n";
        return true;
    }

    void print()const{ //Al ser constante no se modificará el valor, no es mutable
        for (unsigned int i = 0; i < size; ++i){
            std::cout << "["<< i<< "] "<<"Cuarto con "<< verts[i].data.name<< ": "; 
            for (int neighboringVerts : verts[i].edges) {
                std::cout<< neighboringVerts<< " ";
            }
            std::cout << std::endl; //Se imprimen por renglon
        }
    }

    void imprimirDungeonOrder(unsigned int start)const{
        if (start >= size){
            std::cout << "Índice inicial inválido\n";
            return;
        }

        std::list<int> queue; // Usamos un queue simple para recorrer el grafo
        bool* visited = new bool[size]; // Arreglo para marcar vértices visitados
        for (unsigned int i = 0; i < size; ++i){
            visited[i] = false; //Inicializa todos los valores a false
        }

        queue.push_back(start);
        visited[start] = true;

        while (!queue.empty()){
            int current = queue.front(); 
            queue.pop_front(); //liberamos queue segun el recorrido

            //Imprime el cuarto actual
            std::cout<< "Cuarto "<< current + 1<< ": "<< verts[current].data.name << std::endl;

            //Logra visitar las conexiones
            for (int neighbor:verts[current].edges) {
                if (!visited[neighbor]) {
                    queue.push_back(neighbor);
                    visited[neighbor] = true;
                }
            }
        }
    delete[] visited;//Liberamos memoria
    }
};

//Afortundamente debugear en template no fue tan complicado, errores designados entre pruebas marcaban en donde y porque se producian
