#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("IEngine - Werasaimon");

    timer         = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Update()));

    // create graph and assign data to it:
    ui->widget_Plot->addGraph();
    ui->widget_Plot->addGraph();
    ui->widget_Plot->addGraph();
    //ui->widget_Plot->graph(0)->setData(_X, _Y);

    QPen pen1;
    pen1.setWidth(3);
    pen1.setColor(QColor(255,0,0));
    ui->widget_Plot->graph(0)->setPen(pen1);

    QPen pen2;
    pen2.setWidth(3);
    pen2.setColor(QColor(0,255,0));
    ui->widget_Plot->graph(1)->setPen(pen2);

    QPen pen3;
    pen3.setWidth(3);
    pen3.setColor(QColor(0,0,255));
    ui->widget_Plot->graph(2)->setPen(pen3);

    Numer = 0;

    // give the axes some labels:
    ui->widget_Plot->xAxis->setLabel("Time");
    ui->widget_Plot->yAxis->setLabel("Rotate");
    // set axes ranges, so we see all data:
    ui->widget_Plot->xAxis->setRange(0, MAX);
    ui->widget_Plot->yAxis->setRange(-180, 180);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    ui->widget->keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    ui->widget->keyReleaseEvent(event);
}

void MainWindow::Update()
{
    /**/
    const Vector3 eulerAngle =
    static_cast<SceneEngineNuzzleGimbal*>(ui->widget->scene())->gimbalStabilization()->mAngleGimbalStabilization;

    if(Numer > MAX - 10)
    {
        _X_EulerX.clear();
        _Y_EulerX.clear();

        _X_EulerY.clear();
        _Y_EulerY.clear();

        _X_EulerZ.clear();
        _Y_EulerZ.clear();

        Numer=0;
    }

     Numer+=0.1;
    _X_EulerX.append(Numer);
    _Y_EulerX.append(eulerAngle.x * 180.0/M_PI);
    ui->widget_Plot->graph(0)->setData(_X_EulerX, _Y_EulerX);
     // ui->widget_Plot->graph(0)->rescaleAxes();

    _X_EulerY.append(Numer);
    _Y_EulerY.append(eulerAngle.y * 180.0/M_PI);
    ui->widget_Plot->graph(1)->setData(_X_EulerY, _Y_EulerY);
     // ui->widget_Plot->graph(1)->rescaleAxes();

    _X_EulerZ.append(Numer);
    _Y_EulerZ.append(eulerAngle.z * 180.0/M_PI);
    ui->widget_Plot->graph(2)->setData(_X_EulerZ, _Y_EulerZ);
     // ui->widget_Plot->graph(2)->rescaleAxes();

    ui->widget_Plot->replot();
    /**/

}

void MainWindow::on_pushButton_Timer_clicked()
{
    if(!timer->isActive())
    {
       timer->start(100);
    }
    else
    {
       timer->stop();
    }
}


void MainWindow::on_checkBox_Range_toggled(bool checked)
{
    static_cast<SceneEngineNuzzleGimbal*>(ui->widget->scene())->gimbalStabilization()->m_isRange = checked;
}


void MainWindow::on_checkBox_Gimbal_toggled(bool checked)
{
   static_cast<SceneEngineNuzzleGimbal*>(ui->widget->scene())->gimbalStabilization()->m_isStabilizationGimbal = checked;
}

