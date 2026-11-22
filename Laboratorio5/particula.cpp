
#include "particula.h"

particula::particula(int id, double x, double y,double vx, double vy,double masa, double radio):
    id(id), x(x), y(y), vx(vx), vy(vy),
    masa(masa), radio(radio) {}

void particula::mover(double dt)
{
    x = x + vx * dt;
    y = y + vy * dt;
}

void particula::rebotarEnPared(double anchoCaja, double altoCaja)
{
    if (x + radio >= anchoCaja)
    {
        x = anchoCaja - radio;
        vx = -vx;
    }
    else if (x - radio <= 0)
    {
        x = radio;
        vx = -vx;
    }

    if (y + radio >= altoCaja)
    {
        y = altoCaja - radio;
        vy = -vy;
    }
    else if (y - radio <= 0)
    {
        y = radio;
        vy = -vy;
    }
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
