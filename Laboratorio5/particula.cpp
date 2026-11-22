#include <string>
#include <vector>
#include "particula.h"
#include "obstaculo.h"
#include <algorithm>
#include <cmath>
#include <iostream>

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


bool particula::tocaObstaculo(const Obstaculo &obstaculo) const
{
    double cercanoX = std::clamp(
        x,
        obstaculo.getX(),
        obstaculo.getX() + obstaculo.getLado()
        );

    double cercanoY = std::clamp(
        y,
        obstaculo.getY(),
        obstaculo.getY() + obstaculo.getLado()
        );

    double diferenciaX = x - cercanoX;
    double diferenciaY = y - cercanoY;

    double distanciaCuadrada =
        diferenciaX * diferenciaX +
        diferenciaY * diferenciaY;

    return distanciaCuadrada <= radio * radio;
}


bool particula::rebotarEnObstaculo(
    const Obstaculo &obstaculo,
    std::string &ladoImpacto)
{
    double ox = obstaculo.getX();
    double oy = obstaculo.getY();
    double lado = obstaculo.getLado();
    double e = obstaculo.getRestitucion();

    double cercanoX = std::clamp(x, ox, ox + lado);
    double cercanoY = std::clamp(y, oy, oy + lado);

    double dx = x - cercanoX;
    double dy = y - cercanoY;

    double distancia2 = dx * dx + dy * dy;

    if (distancia2 > radio * radio)
    {
        return false;
    }

    double nx = 0;
    double ny = 0;
    double penetracion = 0;

    if (distancia2 > 0)
    {
        double distancia = std::sqrt(distancia2);

        nx = dx / distancia;
        ny = dy / distancia;

        penetracion = radio - distancia;
    }
    else
    {

        double izquierda = x - ox;
        double derecha = ox + lado - x;
        double arriba = y - oy;
        double abajo = oy + lado - y;

        double minimo = std::min(
            std::min(izquierda, derecha),
            std::min(arriba, abajo)
            );

        if (minimo == izquierda)
            nx = -1;
        else if (minimo == derecha)
            nx = 1;
        else if (minimo == arriba)
            ny = -1;
        else
            ny = 1;

        penetracion = radio + minimo;
    }


    x += nx * penetracion;
    y += ny * penetracion;


    double velocidadNormal = vx * nx + vy * ny;

    if (id == 3 && obstaculo.getId() == 2 &&
        velocidadNormal < 0)
    {
        std::cout << "Colision particula 3 - obstaculo 2\n"
                  << "Normal: (" << nx << ", " << ny << ")\n"
                  << "Velocidad antes: (" << vx << ", " << vy << ")\n"
                  << "Velocidad normal: " << velocidadNormal
                  << "\n";
    }


    if (velocidadNormal >= 0)
    {
        return false;
    }


    vx -= (1 + e) * velocidadNormal * nx;
    vy -= (1 + e) * velocidadNormal * ny;

    if (nx < -0.5 && std::abs(ny) < 0.5)
        ladoImpacto = "izquierdo";
    else if (nx > 0.5 && std::abs(ny) < 0.5)
        ladoImpacto = "derecho";
    else if (ny < -0.5 && std::abs(nx) < 0.5)
        ladoImpacto = "superior";
    else if (ny > 0.5 && std::abs(nx) < 0.5)
        ladoImpacto = "inferior";
    else
        ladoImpacto = "esquina";

    return true;
}
