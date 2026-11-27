
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QColor>
#include <QFont>
#include <QRect>
#include <QString>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QTimer>
#include <algorithm>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Juego de Artilleria - Laboratorio 5");
    resize(900, 550);
    setMinimumSize(900, 550);

    setFixedSize(900, 550);

    int suelo = height() - 90;

    //IP1
    bloques.append({QRectF(115, suelo - 35, 30, 35), 1, 100});
    bloques.append({QRectF(155, suelo - 55, 30, 55), 1, 100});
    bloques.append({QRectF(195, suelo - 35, 30, 35), 1, 100});

    //IP2
    int derecha = width() - 225;

    bloques.append({QRectF(derecha, suelo - 35, 30, 35), 2, 100});
    bloques.append({QRectF(derecha + 40, suelo - 55, 30, 55), 2, 100});
    bloques.append({QRectF(derecha + 80, suelo - 35, 30, 35), 2, 100});


    QLabel *textoAngulo = new QLabel("Angulo:", this);
    textoAngulo->setGeometry(25, 105, 65, 30);
    textoAngulo->setStyleSheet("color: white;");

    controlAngulo = new QDoubleSpinBox(this);
    controlAngulo->setGeometry(90, 105, 100, 30);
    controlAngulo->setRange(10, 80);
    controlAngulo->setValue(50);
    controlAngulo->setSuffix(" grados");


    QLabel *textoVelocidad = new QLabel("Velocidad:", this);
    textoVelocidad->setGeometry(215, 105, 75, 30);
    textoVelocidad->setStyleSheet("color: white;");

    controlVelocidad = new QDoubleSpinBox(this);
    controlVelocidad->setGeometry(290, 105, 110, 30);
    controlVelocidad->setRange(80, 500);
    controlVelocidad->setValue(320);
    controlVelocidad->setSuffix(" u/s");


    botonDisparar = new QPushButton("DISPARAR", this);
    botonDisparar->setGeometry(430, 105, 130, 30);


    temporizador = new QTimer(this);
    temporizador->setInterval(16);


    connect(botonDisparar, &QPushButton::clicked,this, &MainWindow::disparar);

    connect(temporizador, &QTimer::timeout,this, &MainWindow::actualizarSimulacion);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(18, 35, 55));

    QFont fuente = painter.font();
    fuente.setPointSize(16);
    fuente.setBold(true);
    painter.setFont(fuente);

    painter.setPen(Qt::white);

    painter.drawText(
        QRect(0, 10, width(), 40),
        Qt::AlignCenter,
        "JUEGO DE ARTILLERIA"
        );

    // Gol
    fuente.setPointSize(11);
    painter.setFont(fuente);

    painter.setPen(QColor(100, 180, 255));
    painter.drawText(
        40, 75,
        "Jugador 1 - Vida: " +
            QString::number(vidaJugador1)
        );

    painter.setPen(QColor(255, 130, 130));
    painter.drawText(
        width() - 230, 75,
        "Jugador 2 - Vida: " +
            QString::number(vidaJugador2)
        );


    int suelo = height() - 90;

    painter.setPen(QPen(QColor(160, 180, 190), 3));
    painter.drawLine(20, suelo, width() - 20, suelo);


    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(50, 130, 240));
    painter.drawRect(50, suelo - 45, 45, 45);


    painter.setBrush(QColor(240, 90, 90));
    painter.drawRect(width() - 95, suelo - 45, 45, 45);

    for (const Bloque &bloque : bloques)
    {
        if (bloque.resistencia <= 0)
        {
            continue;
        }

        painter.setPen(Qt::NoPen);

        if (bloque.propietario == 1)
        {
            painter.setBrush(QColor(110, 175, 255));
        }
        else
        {
            painter.setBrush(QColor(255, 150, 150));
        }

        painter.drawRect(bloque.area);

        // ress
        painter.setPen(Qt::white);

        painter.drawText(
            QRectF(
                bloque.area.x() - 5,
                bloque.area.y() - 20,
                bloque.area.width() + 10,
                18
                ),
            Qt::AlignCenter,
            QString::number(bloque.resistencia, 'f', 0)
            );
    }




    if (proyectilActivo)
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 215, 70));

        painter.drawEllipse(
            QPointF(proyectilX, proyectilY),
            radioProyectil,
            radioProyectil
            );
    }

    //actual

    painter.setPen(QColor(255, 220, 120));

    QString textoTurno;

    if (juegoTerminado)
    {
    textoTurno = "GANADOR: JUGADOR "+ QString::number(ganador);
    }
    else
    {
    textoTurno = "Turno del jugador "+ QString::number(jugadorActual);
    }

    if (proyectilActivo || juegoTerminado)
    {
        textoTurno += " - Proyectil en movimiento";
    }

    painter.drawText(
        QRect(0, height() - 60, width(), 30),
        Qt::AlignCenter,
        textoTurno
        );
}


void MainWindow::disparar()
{
    if (proyectilActivo)
    {
        return;
    }

    const double pi = 3.14159265358979323846;

    double angulo = controlAngulo->value();
    double velocidad = controlVelocidad->value();

    double radianes = angulo * pi / 180.0;

    double direccion = 1.0;

    int suelo = height() - 90;

    if (jugadorActual == 1)
    {
        proyectilX = 100;
        direccion = 1.0;
    }
    else
    {
        proyectilX = width() - 100;
        direccion = -1.0;
    }

    proyectilY = suelo - 60;

    velocidadX =
        direccion * velocidad * std::cos(radianes);

    velocidadY =
        -velocidad * std::sin(radianes);

    tiempoVuelo = 0;
    rebotesSuelo = 0;
    proyectilActivo = true;

    botonDisparar->setEnabled(false);

    temporizador->start();

    update();
}

void MainWindow::actualizarSimulacion()
{
    if (!proyectilActivo)
    {
        return;
    }


    velocidadY += gravedad * dt;

    proyectilX += velocidadX * dt;
    proyectilY += velocidadY * dt;

    tiempoVuelo += dt;

    procesarImpactoBloques();

    if (procesarImpactoJugador())
    {
        return;
    }

    int suelo = height() - 90;

    if (proyectilX - radioProyectil <= 20 &&velocidadX < 0)
    {
        proyectilX = 20 + radioProyectil;
        velocidadX = -velocidadX;
    }

    if (proyectilX + radioProyectil >= width() - 20 &&velocidadX > 0)
    {
        proyectilX = width() - 20 - radioProyectil;
        velocidadX = -velocidadX;
    }

    if (proyectilY - radioProyectil <= 155 &&velocidadY < 0)
    {
        proyectilY = 155 + radioProyectil;
        velocidadY = -velocidadY;
    }

    if (proyectilY + radioProyectil >= suelo &&velocidadY > 0)
    {
        proyectilY = suelo - radioProyectil;
        velocidadY = -velocidadY;

        rebotesSuelo++;

        if (rebotesSuelo >= 2)
        {
            terminarDisparo();
            return;
        }
    }

    if (tiempoVuelo >= 10.0)
    {
        terminarDisparo();
        return;
    }

    update();
}


void MainWindow::terminarDisparo()
{
    temporizador->stop();

    proyectilActivo = false;

    if (jugadorActual == 1)
    {
        jugadorActual = 2;
    }
    else
    {
        jugadorActual = 1;
    }

    botonDisparar->setEnabled(true);

    update();
}


bool MainWindow::procesarImpactoBloques()
{
    for (Bloque &bloque : bloques)
    {
        if (bloque.propietario == jugadorActual ||
            bloque.resistencia <= 0)
        {
            continue;
        }

        // Punto del rectangulo mas cercano al proyectil.
        double cercanoX = std::max(
            bloque.area.left(),
            std::min(proyectilX, bloque.area.right())
            );

        double cercanoY = std::max(
            bloque.area.top(),
            std::min(proyectilY, bloque.area.bottom())
            );

        double dx = proyectilX - cercanoX;
        double dy = proyectilY - cercanoY;
        double distancia2 = dx * dx + dy * dy;

        if (distancia2 > radioProyectil * radioProyectil)
        {
            continue;
        }

        // Calcular la normal del impacto.
        double nx = 0;
        double ny = 0;
        double penetracion = 0;

        if (distancia2 > 0.000001)
        {
            double distancia = std::sqrt(distancia2);

            nx = dx / distancia;
            ny = dy / distancia;

            penetracion = radioProyectil - distancia;
        }
        else
        {
            double izquierda = proyectilX - bloque.area.left();
            double derecha = bloque.area.right() - proyectilX;
            double arriba = proyectilY - bloque.area.top();
            double abajo = bloque.area.bottom() - proyectilY;

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

            penetracion = radioProyectil + minimo;
        }

        double velocidadNormal =
            velocidadX * nx + velocidadY * ny;

        // Sacar el proyectil del bloque.
        proyectilX += nx * penetracion;
        proyectilY += ny * penetracion;

        if (velocidadNormal >= 0)
        {
            continue;
        }


        double rapidez = std::sqrt(
            velocidadX * velocidadX +
            velocidadY * velocidadY
            );

        double danio =
            factorDanio * masaProyectil * rapidez;

        bloque.resistencia = std::max(
            0.0, bloque.resistencia - danio
            );

        // Rebote inelastico.
        velocidadX -=
        (1 + restitucionBloque) * velocidadNormal * nx;

        velocidadY -=
        (1 + restitucionBloque) * velocidadNormal * ny;

        return true;
    }

    return false;
}


bool MainWindow::procesarImpactoJugador()
{
    int rival = (jugadorActual == 1) ? 2 : 1;


    for (const Bloque &bloque : bloques)
    {
        if (bloque.propietario == rival &&
            bloque.resistencia > 0)
        {
            return false;
        }
    }

    int suelo = height() - 90;

    QRectF areaRival;

    if (rival == 1)
        areaRival = QRectF(50, suelo - 45, 45, 45);
    else
        areaRival = QRectF(width() - 95, suelo - 45, 45, 45);


    double cercanoX = std::max(
        areaRival.left(),
        std::min(proyectilX, areaRival.right())
        );

    double cercanoY = std::max(
        areaRival.top(),
        std::min(proyectilY, areaRival.bottom())
        );

    double dx = proyectilX - cercanoX;
    double dy = proyectilY - cercanoY;

    if (dx * dx + dy * dy >
        radioProyectil * radioProyectil)
    {
        return false;
    }


    double rapidez = std::sqrt(
        velocidadX * velocidadX +
        velocidadY * velocidadY
        );

    int danio = std::max(
        1,
        static_cast<int>(
            std::round(factorDanio * masaProyectil * rapidez)
            )
        );

    int &vidaRival =
        (rival == 1) ? vidaJugador1 : vidaJugador2;

    vidaRival = std::max(0, vidaRival - danio);


    temporizador->stop();
    proyectilActivo = false;

    if (vidaRival == 0)
    {
        juegoTerminado = true;
        ganador = jugadorActual;
        botonDisparar->setEnabled(false);
    }
    else
    {
        terminarDisparo();
    }

    update();
    return true;
}

