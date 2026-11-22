#include <string>
#include <vector>
#include "particula.h"

particula::particula(int id, double x, double y,double vx, double vy,double masa, double radio):
    id(id), x(x), y(y), vx(vx), vy(vy),
    masa(masa), radio(radio) {}

void particula::mover(double dt)
{
    x = x + vx * dt;
    y = y + vy * dt;
}

std::vector<std::string> particula::rebotarEnPared(
    double anchoCaja, double altoCaja)
{
    std::vector<std::string> colisiones;

    // Pared derecha
    if (x + radio >= anchoCaja && vx > 0)
    {
        x = anchoCaja - radio;
        vx = -vx;
        colisiones.push_back("pared_derecha");
    }

    // Pared izquierda
    else if (x - radio <= 0 && vx < 0)
    {
        x = radio;
        vx = -vx;
        colisiones.push_back("pared_izquierda");
    }

    // Pared inferior
    if (y + radio >= altoCaja && vy > 0)
    {
        y = altoCaja - radio;
        vy = -vy;
        colisiones.push_back("pared_inferior");
    }

    // Pared superior
    else if (y - radio <= 0 && vy < 0)
    {
        y = radio;
        vy = -vy;
        colisiones.push_back("pared_superior");
    }

    return colisiones;
}

int particula::getId() const
{
    return id;
}

double particula::getX() const
{
    return x;
}

double particula::getY() const
{
    return y;
}

double particula::getVx() const
{
    return vx;
}

double particula::getVy() const
{
    return vy;
}
