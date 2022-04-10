#include "IQCamera.h"


namespace IEngine
{



IQCamera::IQCamera()
    : mAngle(45.0),
      mAspect(1),
      mNear(1e-2),
      mFar(100),
      mAtDistance(1),
      mAngleOfViewSize(1.0),
      mOrthographic(false),
      mRotationConj(1,0,0,0),
      mTranslation(0,0,1)
{

}


IQCamera::~IQCamera()
{

}

//=====================================//


Quaternion IQCamera::Rotation() const
{
    return mRotationConj.GetConjugate();
}


Matrix4 IQCamera::ProjectionViewMatrix() const
{
    return ProjectionMatrix() * ViewMatrix();
}


void IQCamera::SetAngle(float angle)
{
    mAngle = angle;
}

void IQCamera::SetAspect(float aspect)
{
    mAspect = aspect;
}

void IQCamera::SetNear(float near)
{
    mNear = near;
}

void IQCamera::SetFar(float far)
{
    mFar = far;
}

void IQCamera::SetAtDistance(float atDist)
{
    mAtDistance = atDist;
}

scalar IQCamera::AngleOfViewSize() const
{
    return mAngleOfViewSize;
}

void IQCamera::SetOrthographic(bool orthographic)
{
    mOrthographic = orthographic;
}

//=====================================//


Matrix4 IQCamera::ProjectionMatrix() const
{

    Matrix4 P;
    using namespace std;
    const scalar far = mAtDistance + mFar;
    const scalar near = mNear;

    // http://stackoverflow.com/a/3738696/148668
    if(mOrthographic)
    {
        const scalar f = 0.5;
        const scalar left = -f*mAspect;
        const scalar right = f*mAspect;
        const scalar bottom = -f;
        const scalar top = f;
        const scalar tx = (right+left)/(right-left);
        const scalar ty = (top+bottom)/(top-bottom);
        const scalar tz = (far+near)/(far-near);
        const scalar z_fix = 0.5 /mAtDistance / tan(mAngle*0.5 * (M_PI/180.) );

        P = Matrix4
                (z_fix*2./(right-left), 0, 0, -tx,
                 0, z_fix*2./(top-bottom), 0, -ty,
                 0, 0, -z_fix*2./(far-near),  -tz,
                 0, 0, 0, 1);
        P = P.GetTranspose();

        P.SetToIdentity();
        P.GetData()[0] = z_fix*2./(right-left);
        P.GetData()[5] = z_fix*2./(top-bottom);
        P.GetData()[10] = -z_fix*2./(far-near);
        P.GetData()[12] = -tx;
        P.GetData()[13] = -ty;
        P.GetData()[14] = -tz;


        //P = Matrix4::CreateOrtho(near , far , near , far , near , far , z_fix);
    }
    else
    {
        const scalar yScale = IMath::ITan(M_PI*0.5 - 0.5*mAngle*M_PI/180.);
        // http://stackoverflow.com/a/14975139/148668
        const scalar xScale = yScale/mAspect;
        P = Matrix4(xScale, 0, 0, 0,
                    0, yScale, 0, 0,
                    0, 0, -(far+near)/(far-near), -1,
                    0, 0, -2.*near*far/(far-near), 0);

      //  std::cout << "werw";
        P = Matrix4::CreatePerspective( mAngle , mAspect , mNear , mFar );
    }

    return P;
}



void IQCamera::LookAt(const Vector3 &eye, const Vector3 &at, const Vector3 &up)
{

    mRotationConj = Quaternion::LookAtRH( eye , at , up ); //IQuaternion::LookAtLH(eye,at,up);
    mTranslation = /*mRotationConj.GetConjugate() **/ eye;

    mTransformMatrix.SetToIdentity();
    mTransformMatrix.Rotate(mRotationConj.GetConjugate());
    mTransformMatrix.Translate(-mTranslation);

}




Matrix4 IQCamera::ViewMatrix() const
{
    return mTransformMatrix;//(  Matrix4::CreateRotation(mRotationConj).GetInverse() * Matrix4::CreateTranslation(-mTranslation));
}



void IQCamera::DollyZoom(const scalar &fov)
{
    const scalar old_angle = mAngle;
    if(old_angle + fov < IGL_CAMERA_MIN_ANGLE)
    {
        mOrthographic = true;
    }
    else if(old_angle + fov > IGL_CAMERA_MIN_ANGLE)
    {
        mOrthographic = false;
    }

    if(!mOrthographic)
    {
        mAngle += fov;
        mAngle = IMath::IMin(scalar(89.),IMath::IMax(scalar(IGL_CAMERA_MIN_ANGLE),mAngle));
        // change in distance
        const scalar s = (2.*IMath::ITan(old_angle/2./180.*M_PI)) /
                         (2.*IMath::ITan(mAngle/2./180.*M_PI));

        mAngleOfViewSize = (4.*IMath::ITan(mAngle/2./180.*M_PI));
        const scalar old_at_dist = mAtDistance;
        mAtDistance = old_at_dist * s;
        Dolly(Vector3(0,0,1)*(mAtDistance - old_at_dist));

    }

}

void IQCamera::Orbit(const Quaternion &q , const Vector3& origin)
{

    LookAt(q*(Vector3::Z * mAtDistance) , origin , Vector3::Y );
}



Matrix4 IQCamera::GetAffine() const
{
    Matrix4 t = Matrix4::IDENTITY;
    t =     Matrix4::CreateRotation(mRotationConj).GetInverse();
    t = t * Matrix4::CreateTranslation(-mTranslation);
    return t;
}

Matrix4 IQCamera::GetInverse() const
{
    Matrix4 t = Matrix4::IDENTITY;
    t =     Matrix4::CreateTranslation(-mTranslation);
    t = t * Matrix4::CreateRotation(mRotationConj);
    return t;
}



Vector3 IQCamera::GetEye() const
{
    return -(Vector3(0,0,0) * GetInverse());
}

Vector3 IQCamera::GetAt() const
{
    return -(Vector3(0,0,-1)*mAtDistance) * GetInverse();
}

Vector3 IQCamera::GetUp() const
{
    return Matrix4::CreateRotation(mRotationConj.GetConjugate()) * Vector3(0,1,0);
}

Vector3 IQCamera::UnitPlane() const
{
    // Distance of center pixel to eye
    const scalar d = 1.0;
    const scalar a = mAspect;
    const scalar theta = mAngle*M_PI/180.;
    const scalar w =2.*IMath::ISqrt(-d*d/(a*a*IMath::IPow(tan(0.5*theta),2.)-1.))*a*IMath::ITan(0.5*theta);
    const scalar h = w/a;
    return Vector3(w*0.5,h*0.5,-d);
}

void IQCamera::Dolly(const Vector3 &dv)
{
    mTranslation += dv;
}


}
