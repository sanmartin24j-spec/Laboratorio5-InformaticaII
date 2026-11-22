#ifndef PARTICULA_H
#define PARTICULA_H

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

void mover(double dt);
void rebotarEnPared(double anchoCaja, double altoCaja);
int getId() const;
double getX() const;
double getY() const;
double getVx() const;
double getVy() const;

};

#endif // PARTICULA_H
