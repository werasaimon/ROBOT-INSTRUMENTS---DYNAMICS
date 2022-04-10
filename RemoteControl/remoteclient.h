#ifndef REMOTECLIENT_H
#define REMOTECLIENT_H

#include <QObject>
#include <QString>
#include <QtDebug>

#include <QUdpSocket>
#include <QTimer>
#include <QtMath>



class RemoteClient : public QObject
{

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


    } packet_remote;


    Q_OBJECT

    // Декларация textInput в QML
    Q_PROPERTY( QString textInput READ textInput  WRITE setTextInput  NOTIFY textInputChanged)


  public:
    explicit RemoteClient(QObject *parent = nullptr);
             ~RemoteClient();

    // выввод данных
    QString textInput();
    void  setTextInput(const QString &textInput);

    // перемещения робокара
    Q_INVOKABLE void  speed_PWM( int speed );

    // поворот робокара
    Q_INVOKABLE void  speed_Turn( int speed_turn );

    //включение\выключение светодиода на пине 13\D1
    Q_INVOKABLE void  enabled_LED( bool enabled );

    Q_INVOKABLE void  slider_KP( float value );
    Q_INVOKABLE void  slider_KD( float value );
    Q_INVOKABLE void  slider_KI( float value );
    Q_INVOKABLE void  slider_KF( float value );
    Q_INVOKABLE void  slider_KT( float value );


    // кофициент скорости поворота
    Q_INVOKABLE void  coffTurn( int _coff_trun );

    // кнопка конекта к IP-адреса робокара
    Q_INVOKABLE bool  onConnect(QString _ip_address );



  private slots:

    // функция таймера
    void Update();

  signals:

    void textInputChanged();




  private:

  // Ввыввод даных от робокара
  QString m_textInput;

  // Сщетчик строк
  int m_NSizeLines;

  // таймер обмена данными
  QTimer  *m_Timer;

  // UDP-сокет для обмена данными с робокаром
  QUdpSocket *m_Socket;

  // IP-адрес робокара
  QHostAddress m_IPAddress;

  // Порт обмена данными
  quint16 m_Port;

  // Кофициент скорости поворота
  double m_CoffTurn;

  // Скорость перемещения робокара
  double m_Speed;

  // Скорость поворота робокара
  double m_Turn;


};

#endif // REMOTECLIENT_H
