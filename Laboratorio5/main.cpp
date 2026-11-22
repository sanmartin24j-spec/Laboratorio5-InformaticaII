#include <iostream>
#include <fstream>
#include <vector>
#include "particula.h"
#include "obstaculo.h"

using namespace std;

int main()
{
 vector<particula> particulas;

particulas.emplace_back(1, 15, 35, 12, 0, 2, 3);
particulas.emplace_back(2, 30, 20, -4, 2, 3, 4);
particulas.emplace_back(3, 50, 40, 2, -3, 2, 3);
particulas.emplace_back(4, 70, 60, -3, -2, 4, 5);

vector<Obstaculo> obstaculos;

obstaculos.emplace_back(1, 20, 30, 10, 0.7);
obstaculos.emplace_back(2, 60, 15, 10, 0.7);
obstaculos.emplace_back(3, 35, 55, 10, 0.7);
obstaculos.emplace_back(4, 80, 50, 10, 0.7);

cout << "\nObstaculos creados:\n";

for (const auto &obstaculo : obstaculos)
{
    cout << "Obstaculo: " << obstaculo.getId()
    << " | X: " << obstaculo.getX()
    << " | Y: " << obstaculo.getY()
    << " | Lado: " << obstaculo.getLado()
    << " | Restitucion: "
    << obstaculo.getRestitucion()
    << endl;
}


const double dt = 0.1;
const int pasos = 100;

const double anchoCaja = 100.0;
const double altoCaja = 80.0;

ofstream archivo("trayectorias.csv");
ofstream archivoColisiones("colisiones.csv");

if (!archivo.is_open())
{
    cout << "Error al crear el archivo." << endl;
    return 1;
}

if (!archivoColisiones.is_open())
{
    cout << "Error al crear colisiones.csv" << endl;
    return 1;
}

archivoColisiones << "tiempo,id,tipo_colision\n";

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
        for (auto &particula : particulas)
        {
            particula.mover(dt);

            std::vector<std::string> eventos =
                particula.rebotarEnPared(anchoCaja, altoCaja);

            for (const string &evento : eventos)
            {
                archivoColisiones
                    << (paso + 1) * dt << ","
                    << particula.getId() << ","
                    << evento << "\n";
            }
            for (const auto &obstaculo : obstaculos)
            {
                std::string ladoImpacto;

                if (particula.rebotarEnObstaculo(
                        obstaculo, ladoImpacto))
                {
                    archivoColisiones
                        << (paso + 1) * dt << ","
                        << particula.getId() << ","
                        << "obstaculo_" << obstaculo.getId()
                        << "_" << ladoImpacto << "\n";
                }
            }
        }
    }
}
    archivo.close();
    archivoColisiones.close();

    cout << "Simulacion finalizada." << endl;
    cout << "Archivo trayectorias.csv generado." << endl;

    return 0;

}