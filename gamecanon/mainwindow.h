
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVector>
#include <QRectF>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QDoubleSpinBox;
class QPushButton;
class QTimer;
class QPaintEvent;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;


    struct Bloque
    {
        QRectF area;
        int propietario;
        double resistencia;
    };

    QVector<Bloque> bloques;

    const double masaProyectil = 2.0;
    const double factorDanio = 0.04;
    const double restitucionBloque = 0.6;

    bool procesarImpactoBloques();


    QDoubleSpinBox *controlAngulo;
    QDoubleSpinBox *controlVelocidad;
    QPushButton *botonDisparar;
    QTimer *temporizador;


    int jugadorActual = 1;
    int vidaJugador1 = 100;
    int vidaJugador2 = 100;

    bool juegoTerminado = false;
    int ganador = 0;

    bool procesarImpactoJugador();


    bool proyectilActivo = false;
    double proyectilX = 0;
    double proyectilY = 0;
    double velocidadX = 0;
    double velocidadY = 0;
    double tiempoVuelo = 0;
    int rebotesSuelo = 0;


    const double gravedad = 240.0;
    const double dt = 0.016;
    const double radioProyectil = 7.0;

    void disparar();
    void actualizarSimulacion();
    void terminarDisparo();

};

#endif

