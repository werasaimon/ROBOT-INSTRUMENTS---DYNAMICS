import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import remoteclient 1.0

Window {
    visible: true
    width: 700
    height: 350
    color: "#514c4c"
    title: qsTr("RemoteControl-WiFi")

    // Клиент WiFi-управления робокаром
    RemoteClient
    {
        id : remote_client
    }

    // IP-адрес робокара
    TextEdit
    {
        id: textIP
        x: 4
        y: 8
        width: 148
        height: 28
        text: qsTr("192.168.1.100")
        font.pixelSize: 20
    }

    // Кнопка конекта к IP-адресу робокара
    Button
    {
        id: button
        x: 158
        y: 0
        width: 100
        height: 36
        text: qsTr("connect")

        onClicked:
        {
            if(remote_client.onConnect(textIP.text))
            {
                text  = "stop"
                remote_client.speed_Turn(0);
                remote_client.slider_KD((slider1.value*slider1.value)/10000.0);
                remote_client.slider_KI((slider2.value)/1.0);
                remote_client.slider_KF((slider3.value)/1.0);
                remote_client.slider_KT(slider4.value);
                remote_client.slider_KP((slider.value*slider.value)/10000.0);
            }
            else
            {
                text  = "connect";
            }
        }
    }

    // Коффициент скорости поворота
    SpinBox
    {
        id: spinBox
        x: 300
        y: 2
        width: 123
        height: 34
        to: 10
        from: -10
        value: 1

        onValueChanged:
        {
            remote_client.coffTurn(value)
        }
    }

    // Кнока вкл\выкл светодиода
    Switch
    {
        id: elementLED
        x: 448
        y: 2
        text: "POS"
        leftPadding: 0
        font.bold: false

        onCheckedChanged:
        {
            remote_client.enabled_LED(checked)
        }
    }

    // Кнопка фиксации джойстика скорости
    Switch
    {
        id: elementFix
        x: 557
        y: 2
        text: qsTr("Fixation")

        onCheckedChanged:
        {
           sliderSpeed.value = 0;
           remote_client.speed_PWM(sliderSpeed.value)
        }
    }

    // Джойстик поворота робокара
    Dial
    {
        id: dial
        x: 0
        y: 83
        width: 221
        height: 220
        to: 170
        from: -170

        background:
        Rectangle
        {
            color: "#1791b9"
            radius: width / 2
            border.color: "#000000"
        }

        onMoved:
        {
            console.log(value)
            remote_client.speed_Turn(value)
        }

        onPressedChanged:
        {
            value=0;
            remote_client.speed_Turn(value)
        }

        Text
        {
            id: element
            x: 89
            y: 99
            text: qsTr("Turn")
            font.bold: true
            font.pixelSize: 20
        }
    }

    // Джойстик скорости робокара
    Slider
    {
        id: sliderSpeed
        x: 481
        y: 88
        width: 219
        height: 220
        to: 1024
        from: -1024
        orientation: Qt.Vertical
        value: 0.5

        background:
        Rectangle
        {
            x : 0
            y : 0
            height: width
            color: "#1791b9"
            radius: width / 2
            border.color: "#000000"
        }

        onMoved:
        {
            remote_client.speed_PWM(value)
        }

        onPressedChanged:
        {
            if(!elementFix.checked)
            {
                value = 0;
                remote_client.speed_PWM(value)
            }
        }

        Text
        {
            id: element1
            x: 18
            y: 99
            text: qsTr("Speed")
            font.bold: true
            font.pixelSize: 20
        }
    }



    Slider
    {
        id: slider
        x: 233
        y: 78
        width: 40
        height: 240
        scale: 1
        orientation: Qt.Vertical
        value: 45

        // stepSize: 2
        from: 1
        to: 500


        ToolTip
        {
            parent: slider.handle
            visible: slider.pressed
            text: Number(Math.pow(slider.valueAt(slider.position).toFixed(1),2.0)/10000.0).
            toLocaleString(Qt.locale("de_DE"),"f",4)
        }

        onValueChanged:
        {
            console.log((value*value)/10000.0)
                   console.log(value)
            remote_client.slider_KP((slider.value*slider.value)/10000.0);
        }
    }



    Slider {
        id: slider1
        x: 279
        y: 78
        width: 40
        height: 240
        orientation: Qt.Vertical
        value: 4.5

        // stepSize: 2
        from: 0
        to: 100


         ToolTip
         {
            parent: slider1.handle
            visible: slider1.pressed
            text: Number(Math.pow(slider1.valueAt(slider1.position).toFixed(1),2.0)/10000.0).
            toLocaleString(Qt.locale("de_DE"),"f",4)
         }

         onValueChanged:
         {
             console.log((value*value)/10000.0)
                    console.log(value)
             remote_client.slider_KD((slider1.value*slider1.value)/10000.0);
         }
    }

    Slider
    {
        id: slider2
        x: 330
        y: 78
        width: 40
        height: 240
        orientation: Qt.Vertical
        value: 5
        // stepSize: 2
        from: 0
        to: 10000

        ToolTip
        {
            parent: slider2.handle
            visible: slider2.pressed
            text: Number(Math.pow(slider2.valueAt(slider2.position).toFixed(1),1.0)/1.0).
            toLocaleString(Qt.locale("de_DE"),"f",2)
        }

        onValueChanged:
        {
            console.log((value*value)/1.0)
            console.log(value)
            remote_client.slider_KI((slider2.value)/1.0);
        }
    }


    Slider
    {
        id: slider3
        x: 376
        y: 78
        width: 40
        height: 240
        orientation: Qt.Vertical
        value: 1
        from: 0
        to: 90

        ToolTip
        {
            visible: slider3.pressed
            text: Number(Math.pow(slider3.valueAt(slider3.position).toFixed(1),1.0)/1.0).
                    toLocaleString(Qt.locale("de_DE"),"f",2)
            parent: slider3.handle
        }


        onValueChanged:
        {
            console.log((value)/1.0)
            console.log(value)
            remote_client.slider_KF((slider3.value)/1.0);
        }

    }

    Slider
    {
        id: slider4
        x: 422
        y: 78
        width: 40
        height: 240
        to: 250
        orientation: Qt.Vertical
        value: 200


        ToolTip
        {
            visible: slider4.pressed
            text: Number(slider4.valueAt(slider4.position).toFixed(1)).
                    toLocaleString(Qt.locale("de_DE"),"f",2)
            parent: slider4.handle
        }


        onValueChanged:
        {
            console.log(value)
            remote_client.slider_KT(slider4.value);
        }
    }
}
