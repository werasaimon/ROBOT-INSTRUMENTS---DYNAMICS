#ifndef OPENGLRENDER_H
#define OPENGLRENDER_H

#include "Engine/engine.hpp"


using namespace IEngine;

class OpenGLRender
{
    public:


    OpenGLRender() = default;
    OpenGLRender(const OpenGLRender&) = default;



    static void DrawComponentMeshLines(const IMesh *ComponentMesh );

    static void DrawComponentMeshPoints(const IMesh *ComponentMesh );

    static void DrawComponentMeshLine(const IMesh *ComponentMesh , const Matrix4& transform = Matrix4::IDENTITY);

    static void DrawComponentMeshFill(const IMesh *ComponentMesh , const Matrix4& transform = Matrix4::IDENTITY);

    static void DrawLine(const Vector3 &a , const Vector3 &b);


};

#endif // OPENGLRENDER_H
