#ifndef PARTICULA_H
#define PARTICULA_H

#include <string>
#include <vector>

class Obstaculo;

class particula
{
private:

int id;
double x;
double y;
double vx;
double vy;
double masa;
double radio;

public:

particula(int id, double x, double y,double vx, double vy,double masa, double radio);
particula fusionarCon(const particula &otra, int nuevoId) const;

void mover(double dt);
std::vector<std::string> rebotarEnPared(double anchoCaja, double altoCaja);
int getId() const;
double getX() const;
double getY() const;
double getVx() const;
double getVy() const;
bool tocaObstaculo(const Obstaculo &obstaculo) const;
bool tocaParticula(const particula &otra) const;
bool rebotarEnObstaculo(const Obstaculo &obstaculo,std::string &ladoImpacto);

};

#endif // PARTICULA_H
