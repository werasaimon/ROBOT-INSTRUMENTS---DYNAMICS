#include "SceneCore.h"
#include "OpenGL/OpenGLRender.h"
#include "OpenGL/geometry_opengl.h"
#include <QFile>


namespace
{
namespace
{

    bool CopyFileFromToResources( QFile &mFile, const char* fileName )
    {
        QFile FileRead(fileName);

        bool success = true;
        success &=  FileRead.open( QFile::ReadOnly );
        success &= mFile.open( QFile::WriteOnly | QFile::Truncate );
        success &= mFile.write(  FileRead.readAll() ) >= 0;
        FileRead.close();
        mFile.close();

        return success;
    }


    bool isShaderRender = true;
}

}

SceneCore::SceneCore()
{
   initializeOpenGLFunctions();
}

void SceneCore::initCamera()
{
    mWidth  = 600;
    mHeight = 400;

    float aspect = mWidth / mHeight;
    float zNear  = 1.0;
    float zFar   = 250;
    float fov    = 30.0;

    mCamera.SetAspect(aspect);
    mCamera.SetNear(zNear);
    mCamera.SetFar(zFar);
    mCamera.SetAngle(fov);

    mEye = (Vector3(0,0,mCameraZDistance = -20));
    mTarget = (Vector3(0,0,0));
    mUp = (Vector3(0,1,0));

    mCameraAngleYaw = 0;
    mCameraAnglePitch = 0;
}

void SceneCore::initShader()
{
    //------------------------------------------------------------------------------------//

    // Compile vertex shader
    if (!mProgramShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/object2.vsh"))
        qDebug() << "Error";

    // Compile fragment shader
    if (!mProgramShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/object2.fsh"))
        qDebug() << "Error";

    // Link shader pipeline
    if (!mProgramShader.link())
        qDebug() << "Error";

    //------------------------------------------------------------------------------------//
}

void SceneCore::initLights()
{
    mLight = ILight(ILight::LightType::Point);
    mLight.SetPosition(Vector4(5.f, 10.f, 25.f, 1.f));
    mLight.SetDirection(Vector4(-1.0f, -1.0f, -1.0f, 0.0f));

}

void SceneCore::DrawShader(IQCamera _camera, float _width, float _height, GLShaderProgram *_ProgramShader )
{

    glCullFace((mSceneDscriptor.m_IsInvertClipping) ? GL_FRONT : GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, _width , _height);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //----------------------------------------------------------------------------//

    _ProgramShader->bind();

    _ProgramShader->UniformValue("u_ProjectionMatrix"  , _camera.ProjectionMatrix());
    _ProgramShader->UniformValue("u_ViewMatrix"        , _camera.ViewMatrix());

    _ProgramShader->UniformValue("u_eyePosition" , _camera.GetEye());


    _ProgramShader->UniformValue("u_MainLightPower",mSceneDscriptor.m_MainLightPower=true);
    _ProgramShader->setUniformValue("u_CountLights", 1);


    //-------------------------------------------------------------//

    _ProgramShader->UniformValue(("u_LightProperty[" + std::to_string(0) + "].AmbienceColor").c_str(), mLight.AmbienceColor());
    _ProgramShader->UniformValue(("u_LightProperty[" + std::to_string(0) + "].DiffuseColor").c_str(), mLight.DiffuseColor());
    _ProgramShader->UniformValue(("u_LightProperty[" + std::to_string(0) + "].SpecularColor").c_str(), mLight.SpecularColor());
    _ProgramShader->UniformValue(("u_LightProperty[" + std::to_string(0) + "].ReflectionColor").c_str(), mLight.ReflectionColor());
    _ProgramShader->UniformValue(("u_LightProperty[" + std::to_string(0) + "].Position").c_str(), mLight.Position());
    _ProgramShader->UniformValue(("u_LightProperty[" + std::to_string(0) + "].Direction").c_str(), mLight.Direction());
    _ProgramShader->setUniformValue(("u_LightProperty[" + std::to_string(0) + "].Cutoff").c_str(), mLight.Cutoff());
    _ProgramShader->setUniformValue(("u_LightProperty[" + std::to_string(0) + "].Power").c_str(), mLight.Power());
    _ProgramShader->setUniformValue(("u_LightProperty[" + std::to_string(0) + "].Type").c_str(), int(mLight.Type()));


    //-------------------------------------------------------------//
    Matrix4 MultMatrix =  mSceneDscriptor.m_TRANSFORM_Geometry.GetTransformMatrix();//  mGeometriMesh->GetTransformMatrix();

    glPushMatrix();
    glMultMatrixf(MultMatrix);

    _ProgramShader->UniformValue("u_ModelMatrix"  , MultMatrix);
    _ProgramShader->UniformValue("u_NormalMatrix"  , MultMatrix.GetTranspose());

    IMaterial *material = mMaterial;
    _ProgramShader->UniformValue("u_MaterialProperty.DiffuseColor", material->DiffuseColor());
    _ProgramShader->UniformValue("u_MaterialProperty.AmbienceColor", material->AmbienceColor());
    _ProgramShader->UniformValue("u_MaterialProperty.SpecularColor", material->SpecularColor());
    _ProgramShader->UniformValue("u_MaterialProperty.Shines", material->Shines());
    _ProgramShader->UniformValue("u_IsUseDiffuseMap", material->isUseDiffuseMap());
    _ProgramShader->UniformValue("u_IsUseNormalMap",  material->isUseNormalMap());

    if( material->isUseDiffuseMap())
    {
        material->DiffuseMapTexture()->bind(0);
        _ProgramShader->setUniformValue("u_DiffuseMapTexture", 0);
    }


    if( material->isUseNormalMap())
    {
        material->NormalMapTexture()->bind(1);
        _ProgramShader->setUniformValue("u_NormalMapTexture", 1);
    }

    for (int i = 0; i < 4; ++i)
    {
        geometry_opengl *geometry_engine_render = new geometry_opengl(mGeometriMesh[i]);
        geometry_engine_render->drawCubeGeometry(&mProgramShader);
        delete geometry_engine_render;
    }


    glPopMatrix();
    //-------------------------------------------------------------//

    _ProgramShader->release();
    //--------------------------------------------------//

    glLoadIdentity();
    glFlush();

}

void SceneCore::DrawLine(IQCamera _camera, float _width, float _height)
{

    glCullFace((mSceneDscriptor.m_IsInvertClipping) ? GL_FRONT : GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, _width , _height);

    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(_camera.ProjectionMatrix());

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(_camera.ViewMatrix());

    Matrix4 MultMatrix =  mSceneDscriptor.m_TRANSFORM_Geometry.GetTransformMatrix();

    glPushMatrix();
    glMultMatrixf(MultMatrix);

    for (int i = 0; i < 4; ++i)
    {
        glColor3f(1,1,1);
        OpenGLRender::DrawComponentMeshLines(mGeometriMesh[i]);
    }

    glPopMatrix();
}

bool SceneCore::initialization()
{
    mWidth  = 600;
    mHeight = 400;

    NullAllKey();
    initShader();
    initCamera();
    initLights();

    //===========================//

    mMaterial = new IMaterial;
    mMaterial->setDiffuseMap(":/datafiles/cube.bmp");
    mMaterial->setShines(10.0f);
    mMaterial->setDiffuseColor(Vector3(0.1f, 0.1f, 0.1f));
    mMaterial->setAmbienceColor(Vector3(1.0f, 1.0f, 1.0f));
    mMaterial->setSpecularColor(Vector3(1.0f, 1.0f, 1.0f));


    MeshGenerator::EllipsoidDescriptor eclipse_dscp2(Vector3(0.5));
    mGeometriMesh[0] = new IMeshGenerate(eclipse_dscp2);
    mGeometriMesh[0]->CalculateNormals();
    mGeometriMesh[0]->CalculateTangets();
    mGeometriMesh[0]->EnableVertexColors(Vector3(0.1));

    MeshGenerator::CuboidDescriptor cuboid_1(Vector3(5,0.2,0.2));
    mGeometriMesh[1] = new IMeshGenerate(cuboid_1);
    mGeometriMesh[1]->CalculateNormals();
    mGeometriMesh[1]->CalculateTangets();
    mGeometriMesh[1]->EnableVertexColors(Vector3(0.1));

    MeshGenerator::CuboidDescriptor cuboid_2(Vector3(0.2,5,0.2));
    mGeometriMesh[2] = new IMeshGenerate(cuboid_2);
    mGeometriMesh[2]->CalculateNormals();
    mGeometriMesh[2]->CalculateTangets();
    mGeometriMesh[2]->EnableVertexColors(Vector3(0.1));

    MeshGenerator::CuboidDescriptor cuboid_3(Vector3(0.2,0.2,5));
    mGeometriMesh[3] = new IMeshGenerate(cuboid_3);
    mGeometriMesh[3]->CalculateNormals();
    mGeometriMesh[3]->CalculateTangets();
    mGeometriMesh[3]->EnableVertexColors(Vector3(0.1));


    ///-------------------------------///

    mSceneDscriptor.m_MainLightPower = 1.0;
    mSceneDscriptor.m_IsDrawLines = true;
    mSceneDscriptor.m_IsDrawShader = true;
    mSceneDscriptor.m_IsInvertClipping = false;
    mSceneDscriptor.m_type_projection = SceneDscriptor2::CAMERA_STATUS::M_PRESPECTIV;
}

void SceneCore::render(float FrameTime)
{
    mCameraAnglePitch = IClamp(mCameraAnglePitch,scalar(-M_PI * 0.495),scalar(M_PI * 0.495));
    Matrix4 MRot;
    Vector3 ZBuffLength = Vector3::Z * mCameraZDistance;
    MRot = Matrix4::CreateRotationAxis( Vector3::X , -mCameraAnglePitch);
    MRot = Matrix4::CreateRotationAxis( Vector3::Y , -mCameraAngleYaw) * MRot;
    mEye = (ZBuffLength * MRot);


    mCamera.SetOrthographic(mSceneDscriptor.m_type_projection == SceneDscriptor2::CAMERA_STATUS::M_ORTHO);
    mCamera.LookAt(mEye, mTarget , mUp);


    if(mSceneDscriptor.m_IsDrawLines)
    {
       DrawLine(mCamera,mWidth,mHeight);
    }

    if(mSceneDscriptor.m_IsDrawShader)
    {
       DrawShader(mCamera,mWidth,mHeight,&mProgramShader);
    }

}

void SceneCore::update()
{
   //mGeometriMesh->SetTransformMatrix(mSceneDscriptor.m_TRANSFORM_Geometry.GetTransformMatrix());
   mLight.SetPosition(Vector4(mSceneDscriptor.m_LightPosition.x , mLight.Position().y , mSceneDscriptor.m_LightPosition.z, mLight.Position().w));
}

void SceneCore::resize(float width, float height)
{
    mWidth  = width;
    mHeight = height;

    float aspect = mWidth / mHeight;
    float zNear  = 1.0;
    float zFar   = 250;
    float fov    = 30.0;

    mCamera.SetAspect(aspect);
    mCamera.SetNear(zNear);
    mCamera.SetFar(zFar);
    mCamera.SetAngle(fov);
}

void SceneCore::mouseMove(float x, float y, int button)
{
    if(button){}

    data_mouse.mouseX = x;
    data_mouse.mouseY = y;

    if( mMouseButton == Qt::MouseButton::MidButton )
    {
        float speedX = (data_mouse.mouseX - data_mouse.mouseOldX);
        float speedY = (data_mouse.mouseY - data_mouse.mouseOldY);

        float coff = 1.f / 1000.0;

        mCameraAngleYaw   -= speedX * coff;
        mCameraAnglePitch += speedY * coff;

        data_mouse.mouseOldX = data_mouse.mouseX;
        data_mouse.mouseOldY = data_mouse.mouseY;

    }
}

void SceneCore::mousePress(float x, float y, int button)
{
    mMouseButton = button;
    data_mouse.mouseX = data_mouse.mouseOldX = x;
    data_mouse.mouseY = data_mouse.mouseOldY = y;
}

void SceneCore::mouseReleasePress(float x, float y, int button)
{
    mMouseButton = button;
    data_mouse.mouseX = data_mouse.mouseOldX = x;
    data_mouse.mouseY = data_mouse.mouseOldY = y;
}

void SceneCore::mouseWheel(float delta)
{
    mCameraZDistance += (delta * 0.02f);
    mCameraZDistance  = IMath::IClamp(IMath::IAbs(mCameraZDistance),2.f,1024.f);
}

void SceneCore::keyboard(int key)
{

}

void SceneCore::destroy()
{

}
