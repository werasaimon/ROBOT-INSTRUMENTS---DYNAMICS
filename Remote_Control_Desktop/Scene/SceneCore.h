#ifndef SCENECORE_H
#define SCENECORE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "Scene/SceneMain.h"

#include "SceneMain.h"
#include "Engine/engine.hpp"


#include "Shader/Shader.h"
#include "IMaterial.h"


struct SceneDscriptor2
{
    enum struct CAMERA_STATUS { M_ORTHO , M_PRESPECTIV };
    CAMERA_STATUS m_type_projection;

    float m_MainLightPower;
    bool  m_IsDrawLines;
    bool  m_IsDrawShader;
    bool  m_IsInvertClipping;


    Transform m_TRANSFORM_Geometry;
    Vector3 m_LightPosition;

};


class SceneCore : public SceneMain , protected QOpenGLFunctions
{

private :



    //=======================//

    struct Mouse
    {
        float mouseOldX;
        float mouseOldY;

        float mouseX;
        float mouseY;

    }  data_mouse;

    float mWidth;
    float mHeight;


    //-----------------------------//

    /// Shaders shines
    GLShaderProgram mProgramShader;

    //-----------------------------//
    float mCameraZDistance;
    float mCameraAngleYaw;
    float mCameraAnglePitch;

    //-----------------------------//

    ILight mLight;

    //-----------------------------//

    IMaterial *mMaterial;
    IMesh *mGeometriMesh[4];


    //-----------------------------//

    IQCamera mCamera;

    Vector3 mEye;
    Vector3 mTarget;
    Vector3 mUp;

     //-----------------------------//

public:



    SceneDscriptor2 mSceneDscriptor;

//    void UpdateGeometry(IMesh *_mesh)
//    {
//        assert(_mesh);
//        if(mGeometriMesh) delete mGeometriMesh;
//        mGeometriMesh[0] = _mesh;
//    }


public:

    SceneCore();


    void initCamera();
    void initShader();
    void initLights();

    void DrawShader(IQCamera _camera, float _width, float _height, GLShaderProgram *_ProgramShader);
    void DrawLine(IQCamera _camera, float _width, float _height );

    bool initialization();
    void render(float FrameTime);
    void update();
    void resize( float width , float height );

    void mouseMove( float x , float y  , int button);
    void mousePress( float x , float y , int button );
    void mouseReleasePress( float x , float y , int button );
    void mouseWheel( float delta );

    void keyboard(int key );
    void destroy();

};

#endif // SCENECORE_H
