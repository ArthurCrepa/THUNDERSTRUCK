#include <iostream>
#include "catalogo.h"
#include "dungeon.h"

using namespace std;

int main() {
    unsigned int inicio, fin;
    Catalogo    miCatalogo;
    Dungeon     miDungeon;

    if(!miCatalogo.loadFromCSV("monsters.csv")) {
        cout << "No se pudo crear el catalogo" << endl;
        return 0;
    }

    cout << "Creando Dungeon..." << endl;
    if(!miDungeon.createDungeon("dungeon.txt")) {
        cout << "No se pudo crear el Dungeon" << endl;
        return 0;
    }

    for(unsigned int c = 0; c < miDungeon.getSize(); c ++) {
        Monster *pMonster = nullptr, copiaMonstruo;

        pMonster = miCatalogo.getRandomMonster();
        if(!pMonster) {
            cout << "No se pudo obtener un monstruo del catalogo" << endl;
            return 0;
        }

        copiaMonstruo = *pMonster;
        if(!miDungeon.createRoom(c, copiaMonstruo)) {
            cout << "No se pudo insertar cuarto con monstruo al calabozo" << endl;
            return 0;
        }
    }

    miDungeon.imprimeCuartos();

    cout << "Elije una ruta:\n";
    do {
        cout << "Dame el cuarto inicial: ";
        cin >> inicio;
        cout << "Dame el cuarto final: ";
        cin >> fin;
    } while (fin > miDungeon.getSize() || inicio == fin);

    if(!miDungeon.trazaRuta(inicio, fin)) {
        cout << "No existe ruta de " << inicio << " a " << fin << "\n";
        return 0;
    }

    do {
        Cuarto *actual = nullptr;

        actual = miDungeon.cuartoActualRuta();
        if(actual) {
            cout << "Entrando a " << *actual << "\n";
        }
    } while (miDungeon.avanzaRuta());

    return 0;
}
