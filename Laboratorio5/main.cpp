
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

#include "particula.h"
#include "obstaculo.h"

using namespace std;

int main()
{

    const double anchoCaja = 100.0;
    const double altoCaja = 80.0;


    const double dt = 0.1;
    const int pasos = 100;


    vector<particula> particulas;

    particulas.emplace_back(1, 10, 10, 4, 0, 2, 3);
    particulas.emplace_back(2, 20, 10, -4, 0, 3, 3);
    particulas.emplace_back(3, 50, 40, 2, -3, 2, 3);
    particulas.emplace_back(4, 70, 60, -3, -2, 4, 5);


    vector<Obstaculo> obstaculos;

    obstaculos.emplace_back(1, 20, 30, 10, 0.7);
    obstaculos.emplace_back(2, 60, 15, 10, 0.7);
    obstaculos.emplace_back(3, 35, 55, 10, 0.7);
    obstaculos.emplace_back(4, 80, 50, 10, 0.7);


    int siguienteId = 5;


    ofstream archivo("trayectorias.csv");
    ofstream archivoColisiones("colisiones.csv");

    if (!archivo.is_open() || !archivoColisiones.is_open())
    {
        cout << "Error al crear archivos CSV." << endl;
        return 1;
    }

    archivo << "tiempo,id,x,y,vx,vy\n";
    archivoColisiones << "tiempo,id,tipo_colision\n";

    archivo << fixed << setprecision(5);
    archivoColisiones << fixed << setprecision(5);

    cout << "Iniciando simulacion..." << endl;
    cout << "Particulas iniciales: "<< particulas.size() << endl;


    for (int paso = 0; paso <= pasos; paso++)
    {
        double tiempo = paso * dt;

        // GUARDAR ESTADO ACTUAL
        for (const auto &p : particulas)
        {
            archivo << tiempo << ","
                    << p.getId() << ","
                    << p.getX() << ","
                    << p.getY() << ","
                    << p.getVx() << ","
                    << p.getVy() << "\n";
        }

        if (paso == pasos)
        {
            break;
        }

        double tiempoSiguiente = (paso + 1) * dt;

        //mov y res
        for (auto &p : particulas)
        {
            p.mover(dt);


            vector<string> eventos =
                p.rebotarEnPared(anchoCaja, altoCaja);

            for (const string &evento : eventos)
            {
                archivoColisiones
                    << tiempoSiguiente << ","
                    << p.getId() << ","
                    << evento << "\n";
            }

            for (const auto &obstaculo : obstaculos)
            {
                string ladoImpacto;

                if (p.rebotarEnObstaculo(
                        obstaculo, ladoImpacto))
                {
                    archivoColisiones
                        << tiempoSiguiente << ","
                        << p.getId() << ","
                        << "obstaculo_"
                        << obstaculo.getId()
                        << "_" << ladoImpacto << "\n";
                }
            }
        }

        bool huboFusion;

        do
        {
            huboFusion = false;

            for (size_t i = 0;
                 i < particulas.size() && !huboFusion;
                 i++)
            {
                for (size_t j = i + 1;
                     j < particulas.size();
                     j++)
                {
                    if (particulas[i].tocaParticula(
                            particulas[j]))
                    {
                        int id1 = particulas[i].getId();
                        int id2 = particulas[j].getId();

                        particula nueva =
                            particulas[i].fusionarCon(
                                particulas[j],
                                siguienteId
                                );

                        archivoColisiones
                            << tiempoSiguiente << ","
                            << siguienteId << ","
                            << "fusion_" << id1
                            << "_" << id2 << "\n";

                        cout << "Tiempo: "
                             << tiempoSiguiente
                             << " | Fusion: "
                             << id1 << " + " << id2
                             << " -> " << siguienteId
                             << endl;


                        particulas.erase(
                            particulas.begin() + j
                            );

                        particulas.erase(
                            particulas.begin() + i
                            );


                        particulas.push_back(nueva);

                        siguienteId++;
                        huboFusion = true;
                        break;
                    }
                }
            }

        } while (huboFusion);
    }

    archivo.close();
    archivoColisiones.close();

    cout << "\nSimulacion finalizada." << endl;
    cout << "Particulas finales: "
         << particulas.size() << endl;

    cout << "Archivos generados:" << endl;
    cout << "- trayectorias.csv" << endl;
    cout << "- colisiones.csv" << endl;

    return 0;
}
