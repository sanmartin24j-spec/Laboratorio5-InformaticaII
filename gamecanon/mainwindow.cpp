
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

#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Juego de Artilleria - Laboratorio 5");
    resize(900, 550);
    setMinimumSize(900, 550);

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


    connect(botonDisparar, &QPushButton::clicked,
            this, &MainWindow::disparar);

    connect(temporizador, &QTimer::timeout,
            this, &MainWindow::actualizarSimulacion);
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

    // TITULO
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

    // MARCADORES
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


    painter.setBrush(QColor(110, 175, 255));

    painter.drawRect(115, suelo - 35, 30, 35);
    painter.drawRect(155, suelo - 55, 30, 55);
    painter.drawRect(195, suelo - 35, 30, 35);

    painter.setBrush(QColor(255, 150, 150));

    int derecha = width() - 225;

    painter.drawRect(derecha, suelo - 35, 30, 35);
    painter.drawRect(derecha + 40, suelo - 55, 30, 55);
    painter.drawRect(derecha + 80, suelo - 35, 30, 35);

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

    QString textoTurno =
        "Turno del jugador " +
        QString::number(jugadorActual);

    if (proyectilActivo)
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
