#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "Engine/engine.hpp"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void Update();

    void on_pushButton_Timer_clicked();

    void on_checkBox_Range_toggled(bool checked);

    void on_checkBox_Gimbal_toggled(bool checked);

private:

    // Порт обмена данными
    quint16 m_Port;
    int MAX = 100;//
    double Numer;
    QVector<double> _X_EulerX,_Y_EulerX;
    QVector<double> _X_EulerY,_Y_EulerY;
    QVector<double> _X_EulerZ,_Y_EulerZ;

    Ui::MainWindow *ui;
    QTimer *timer;
};

#endif // MAINWINDOW_H
