#include "obstaculo.h"


#include "obstaculo.h"

Obstaculo::Obstaculo(int id, double x, double y,
double lado, double restitucion) : id(id), x(x), y(y),
lado(lado), restitucion(restitucion)
{
}

int Obstaculo::getId() const
{
    return id;
}

double Obstaculo::getX() const
{
    return x;
}

double Obstaculo::getY() const
{
    return y;
}

double Obstaculo::getLado() const
{
    return lado;
}

double Obstaculo::getRestitucion() const
{
    return restitucion;
}

