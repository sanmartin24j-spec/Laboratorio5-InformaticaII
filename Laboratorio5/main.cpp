#include <iostream>
#include <fstream>
#include <vector>
#include "particula.h"

using namespace std;

int main()
{
 vector<particula> particulas;

particulas.emplace_back(1, 10, 10, 5, 3, 2, 3);
particulas.emplace_back(2, 30, 20, -4, 2, 3, 4);
particulas.emplace_back(3, 50, 40, 2, -3, 2, 3);
particulas.emplace_back(4, 70, 60, -3, -2, 4, 5);

const double dt = 0.1;
const int pasos = 100;

ofstream archivo("trayectorias.csv");

if (!archivo.is_open())
{
    cout << "Error al crear el archivo." << endl;
    return 1;
}

archivo << "tiempo,id,x,y,vx,vy\n";

for (int paso = 0; paso <= pasos; paso++)
{
    double tiempo = paso * dt;

    for (const auto &particula : particulas)//propied
    {
    archivo
    << tiempo << ","
    << particula.getId() << ","
    << particula.getX() << ","
    << particula.getY() << ","
    << particula.getVx() << ","
    << particula.getVy() << "\n";
    }

    if (paso < pasos)
    {
    for (auto &particula : particulas) //reco move
        {
         particula.mover(dt);
        }
    }
}

    archivo.close();

    cout << "Simulacion finalizada." << endl;
    cout << "Archivo trayectorias.csv generado." << endl;

    return 0;
}