
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

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

    // Controles
    QDoubleSpinBox *controlAngulo;
    QDoubleSpinBox *controlVelocidad;
    QPushButton *botonDisparar;
    QTimer *temporizador;

    // Estado del juego
    int jugadorActual = 1;
    int vidaJugador1 = 100;
    int vidaJugador2 = 100;

    // Estado del proyectil
    bool proyectilActivo = false;
    double proyectilX = 0;
    double proyectilY = 0;
    double velocidadX = 0;
    double velocidadY = 0;
    double tiempoVuelo = 0;
    int rebotesSuelo = 0;

    // Constantes fisicas
    const double gravedad = 240.0;
    const double dt = 0.016;
    const double radioProyectil = 7.0;

    void disparar();
    void actualizarSimulacion();
    void terminarDisparo();
};

#endif

