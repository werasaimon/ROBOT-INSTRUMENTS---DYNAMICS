#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// Подключаем SDL
#include <SDL2/SDL.h>
// main определена и в SDL и в QT, поэтому #undef
#undef main


#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>
#include <QVector2D>
#include <QVector>
#include <QMouseEvent>



namespace Ui
{
  class MainWindow;
}

// Структура данных для ROBO_CAR
// Структура кнотроля робокара
struct DataPacketRemote
{
    int  speed_PWM_X=0; // скорость вращения на ШИМ _А
    int  speed_PWM_Y=0; // скорость вращения на ШИМ _В
    int  turn = 0;
    bool is_null_pos;

    float kp;
    float kd;
    float ki;
    float kf;
    float kt;

    DataPacketRemote( int _x=0, int _y=0, bool _is_null_pos=false)
    :speed_PWM_X(_x),
     speed_PWM_Y(_y),
     is_null_pos(_is_null_pos)
    {

    }

};


QT_USE_NAMESPACE


class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();


    private slots:

        void onStartTime();
        void Updatee();


        void SliderSpeedX( int value );
        void SliderSpeedY( int value );
        void DialCofficient( int value );



        void on_horizontalSlider_KP_sliderMoved(int position);

        void on_horizontalSlider_KD_sliderMoved(int position);

        void on_horizontalSlider_KI_sliderMoved(int position);

        void on_horizontalSlider_KF_sliderMoved(int position);

        void on_pushButton_clicked();

        void on_horizontalSlider_KT_sliderMoved(int position);

        void on_checkBox_Plot_toggled(bool checked);

private:

        int coff;

        Ui::MainWindow *ui;

        DataPacketRemote data_trransmission;

        QUdpSocket *m_socket;

        QTimer *timer;

        // Переменная для нашего джойстика
        SDL_Joystick *joy;
        SDL_Event event;

        int xAxis;
        int yAxis;

        int CofficientTurn;

        // Порт обмена данными
        quint16 m_Port;
        int MAX = 100;//
        double Numer;
        QVector<double> _X_EulerX,_Y_EulerX;
        QVector<double> _X_EulerY,_Y_EulerY;
        QVector<double> _X_EulerZ,_Y_EulerZ;


        bool isPlot;


};

#endif // MAINWINDOW_H
