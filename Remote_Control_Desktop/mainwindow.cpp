#include "mainwindow.h"
#include "ui_mainwindow.h"




static int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    const int dividend = out_max - out_min;
    const int divisor = in_max - in_min;
    const int delta = x - in_min;

    return (delta * dividend + (divisor / 2)) / divisor + out_min;
}


/**/
struct Quaternionn
{
  float w, x, y, z;
};

struct DataDescriptor
{
  int num;
  Quaternion Quat;
  float EulerX;
  float EulerY;
  float EulerZ;

  //--------------//

  float AccBiasX;
  float AccBiasY;
  float AccBiasZ;

  float GyroBiasX;
  float GyroBiasY;
  float GyroBiasZ;

  float MagBiasX;
  float MagBiasY;
  float MagBiasZ;

  float MagScaleX;
  float MagScaleY;
  float MagScaleZ;

  //--------------//

} _dataDescriptor;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isPlot = false;

    // create a QUDP socket
    m_socket      = new QUdpSocket(this);
    timer         = new QTimer(this);

    connect(ui->pushButton_OnOFF , SIGNAL(clicked()) , this , SLOT(onStartTime()) );
    connect(timer , SIGNAL(timeout()) , this , SLOT(Updatee()) );

    connect(ui->horizontalSliderX, SIGNAL(valueChanged(int)), this, SLOT(SliderSpeedX(int)));
    connect(ui->horizontalSliderY, SIGNAL(valueChanged(int)), this, SLOT(SliderSpeedY(int)));

    connect(ui->dial, SIGNAL(valueChanged(int)), this, SLOT(DialCofficient(int)));

    CofficientTurn = 1;

    coff = 1;

    m_Port = 8888;

    // Инициализация SDL для использования джойстика
    SDL_Init(SDL_INIT_JOYSTICK);
    // Включаем
    SDL_JoystickEventState(SDL_ENABLE);
    // Открываем ;)
    joy = SDL_JoystickOpen(0);


    // timer->start(50);


    this->setStyleSheet(" QMainWindow { background-color: gray } ");



//    for (int i = 0; i < 100; ++i)
//    {
//       _X.append(i);
//       _Y.append(cos(i) * 180.0/M_PI);
//    }


    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    //ui->customPlot->graph(0)->setData(_X, _Y);

    QPen pen1;
    pen1.setWidth(3);
    pen1.setColor(QColor(255,0,0));
    ui->customPlot->graph(0)->setPen(pen1);

    QPen pen2;
    pen2.setWidth(3);
    pen2.setColor(QColor(0,255,0));
    ui->customPlot->graph(1)->setPen(pen2);

    QPen pen3;
    pen3.setWidth(3);
    pen3.setColor(QColor(0,0,255));
    ui->customPlot->graph(2)->setPen(pen3);

    Numer = 0;

    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, MAX);
    ui->customPlot->yAxis->setRange(-180, 180);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::onStartTime()
{
    // Запускаем таймер
    if(!timer->isActive())
    {
        timer->start(5);

        ui->pushButton_OnOFF->setText("STOP");
    }
    else
    {
        data_trransmission.speed_PWM_X=0;
        data_trransmission.speed_PWM_Y=0;
        Updatee();

        timer->stop();

        ui->pushButton_OnOFF->setText("START");
    }

}

void MainWindow::Updatee()
{

    data_trransmission.is_null_pos = false;

    if(ui->checkGamepad->isChecked())
    {
      // Обробатываем событие джойстика
      SDL_PollEvent(&event);

     // Получаем значения, соответствующие смещению  аналгово джойстика
      yAxis = SDL_JoystickGetAxis(joy, 1);
      xAxis = SDL_JoystickGetAxis(joy, 2);

      qDebug() << "xAxis " << xAxis << "  yAxis " << yAxis;

      ui->horizontalSliderX->setValue(xAxis);
      ui->horizontalSliderY->setValue(yAxis);

      if(SDL_JoystickGetButton(joy,1))
      {
          qDebug() << "LED";
          data_trransmission.is_null_pos = true;
      }
    }

     //---------------------------------------------------------------//

     int mSpeed = yAxis / 1.f;//32;
     int mTurn  = xAxis / 1.f;//32;
     //float mCofficientTurn = 1.f;///CofficientTurn;



//     if(IMath::IAbs(mSpeed) < 128) mSpeed = 0;
//     if(IMath::IAbs(mSpeed) < 128) mSpeed = 0;

      //qDebug() << "turn " << mTurn / coff;

    // mSpeed = map(mSpeed,-32767,32767,-1024,1024);

     data_trransmission.speed_PWM_X = int(-mSpeed);
     data_trransmission.speed_PWM_Y = int(-mSpeed);
     data_trransmission.turn = -mTurn * coff;


     //---------------------------------------------------------------//

     // отыслаем данные на ROBO_CAR
     QString ipAddresStr = ui->lineEdit->text();
     m_socket->writeDatagram( (char*)&data_trransmission , sizeof(DataPacketRemote) , QHostAddress( ipAddresStr ) , m_Port);


     char buff[255]="no packet .... \n";
     int nsize = m_socket->readDatagram( (char*)&_dataDescriptor , sizeof(DataDescriptor) );
     buff[nsize]=0;

     //ui->plainTextEdit->setPlainText(buff);
     qDebug() << nsize << buff;


     Quaternion rot( _dataDescriptor.Quat.w , Vector3(_dataDescriptor.Quat.x, _dataDescriptor.Quat.z, _dataDescriptor.Quat.y) );
     Quaternion q = _dataDescriptor.Quat;
     Vector3 euler = q.GetEulerAngles();

     Matrix3 m;
     m.SetToIdentity();
     m.RotateAxis((euler.x),Vector3::X);
     m.RotateAxis((euler.y),Vector3::Y);
     m.RotateAxis((euler.z),Vector3::Z);

     m.SetToIdentity();
     m.RotateAxis(IMath::IDegreesToRadians(_dataDescriptor.EulerX),Vector3::X);
     m.RotateAxis(IMath::IDegreesToRadians(_dataDescriptor.EulerY),Vector3::Z);
     m.RotateAxis(IMath::IDegreesToRadians(_dataDescriptor.EulerZ),Vector3::Y);
     static_cast<SceneCore*>(ui->widget_gl->scene())->mSceneDscriptor.m_TRANSFORM_Geometry.mBasis=rot.GetRotMatrix();



     if(isPlot == true)
     {
         if(Numer > MAX - 10)
         {
             _X_EulerX.clear();
             _Y_EulerX.clear();

             _X_EulerY.clear();
             _Y_EulerY.clear();

             _X_EulerZ.clear();
             _Y_EulerZ.clear();

             Numer=0;
             //ui->customPlot->graph(0)->data()->clear();
         }

          Numer+=0.1;
         _X_EulerX.append(Numer);
         _Y_EulerX.append(euler.x * 180.0/M_PI);
         ui->customPlot->graph(0)->setData(_X_EulerX, _Y_EulerX);

         _X_EulerY.append(Numer);
         _Y_EulerY.append(euler.y * 180.0/M_PI);
         ui->customPlot->graph(1)->setData(_X_EulerY, _Y_EulerY);

         _X_EulerZ.append(Numer);
         _Y_EulerZ.append(euler.z * 180.0/M_PI);
         ui->customPlot->graph(2)->setData(_X_EulerZ, _Y_EulerZ);
        // ui->customPlot->graph(0)->rescaleAxes();
         ui->customPlot->replot();
     }
     else
     {
         _X_EulerX.clear();
         _Y_EulerX.clear();

         _X_EulerY.clear();
         _Y_EulerY.clear();

         _X_EulerZ.clear();
         _Y_EulerZ.clear();
     }

}


void MainWindow::SliderSpeedX(int value)
{
    ///qDebug() << value;
    xAxis = value;
}


void MainWindow::SliderSpeedY(int value)
{
    ///qDebug() << value;
    yAxis = value;
}

void MainWindow::DialCofficient(int value)
{
    qDebug() << value;
    coff = value;
}


void MainWindow::on_horizontalSlider_KP_sliderMoved(int position)
{

    qDebug() << "KP: " << (position * position) / 10000.f;
    data_trransmission.kp = (position * position) / 10000.f;
}


void MainWindow::on_horizontalSlider_KD_sliderMoved(int position)
{
    qDebug() << "KD: " << (position * position) / 10000.f;
    data_trransmission.kd = (position * position) / 10000.f;
}


void MainWindow::on_horizontalSlider_KI_sliderMoved(int position)
{
    qDebug() << "KI: " << (position * position) / 10000.f;
    data_trransmission.ki = (position * position) / 10000.f;
}


void MainWindow::on_horizontalSlider_KF_sliderMoved(int position)
{
    qDebug() << "KF: " << (position * position) / 1000.f;
    data_trransmission.kf = (position * position) / 10000.f;
}


void MainWindow::on_pushButton_clicked()
{
    _X_EulerX.clear();
    _Y_EulerX.clear();

    _X_EulerY.clear();
    _Y_EulerY.clear();

    _X_EulerZ.clear();
    _Y_EulerZ.clear();

    Numer=500;
   // ui->customPlot->graph(0)->data()->clear();
}


void MainWindow::on_horizontalSlider_KT_sliderMoved(int position)
{
    qDebug() << "KT: " << (position);
    data_trransmission.kt = position;
}


void MainWindow::on_checkBox_Plot_toggled(bool checked)
{
    qDebug() << "Cheker: " << checked;
    isPlot = checked;
}

