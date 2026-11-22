#ifndef OBSTACULO_H
#define OBSTACULO_H

class Obstaculo
{
private:
    int id;
    double x;
    double y;
    double lado;
    double restitucion;

public:
    Obstaculo(int id, double x, double y,
              double lado, double restitucion);

    int getId() const;
    double getX() const;
    double getY() const;
    double getLado() const;
    double getRestitucion() const;
};

#endif