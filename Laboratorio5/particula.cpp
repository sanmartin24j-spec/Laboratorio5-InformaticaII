
#include "particula.h"

particula::particula(int id, double x, double y,double vx, double vy,double masa, double radio):
    id(id), x(x), y(y), vx(vx), vy(vy),
    masa(masa), radio(radio) {}

void particula::mover(double dt)
{
    x = x + vx * dt;
    y = y + vy * dt;
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
