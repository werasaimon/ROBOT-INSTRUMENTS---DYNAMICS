#ifndef ORIENTATIONSENSOR_H
#define ORIENTATIONSENSOR_H


#include "Engine/engine.hpp"
#include "Engine/imaths.hpp"


using namespace IEngine;
using namespace IMath;


class OrientationSensor
{
    Quaternion m_Quat;
    Vector3 m_EulerAngle;


public:

    OrientationSensor()
    {

    }


    void setQuat(const Quaternion& _Quat)
    {
        m_Quat = _Quat;
    }

    void Update()
    {
        //m_EulerAngle = m_Quat.GetEulerAngles2();//
        m_EulerAngle = m_Quat.GetEulerAngleGimbalLock(Quaternion::RotSeq::yxz);
          //m_EulerAngle = m_Quat.GetEulerAngles();

//        if (m_EulerAngle.y >= M_PI)
//        {
//           m_EulerAngle.y -= M_PI * 2.f;
//        }
//        else if (m_EulerAngle.y < -M_PI)
//        {
//           m_EulerAngle.y += M_PI * 2.f;
//        }
    }


    void Update2()
    {
        //m_EulerAngle = m_Quat.GetEulerAngles2();//
        m_EulerAngle = m_Quat.GetEulerAngleGimbalLock(Quaternion::RotSeq::zyz);
    }


    const Vector3 &EulerAngle() const
    {
        return m_EulerAngle;
    }
};

#endif // ORIENTATIONSENSOR_H
