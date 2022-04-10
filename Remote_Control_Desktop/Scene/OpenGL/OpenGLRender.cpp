#include "OpenGLRender.h"

#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/freeglut.h>




void OpenGLRender::DrawComponentMeshLines(const IMesh *ComponentMesh )
{
    //    glPushMatrix();
    //    glMultMatrixf(ComponentMesh->GetTransformMatrix());
    glLineWidth(3);
    for (Index4i t : ComponentMesh->Edges())
    {
        glBegin(GL_LINES);
        glColor3f(1,1,1);
        glVertex3fv(ComponentMesh->GetVertex(t[0]));
        glVertex3fv(ComponentMesh->GetVertex(t[1]));
        glEnd();
    }
    glLineWidth(1);
    //    glPopMatrix();
}

void OpenGLRender::DrawComponentMeshPoints(const IMesh *ComponentMesh )
{
//    glPushMatrix();
//    glMultMatrixf(ComponentMesh->GetTransformMatrix());
    glLineWidth(3);
    for (Vector3 t : ComponentMesh->Vertices())
    {
        glBegin(GL_POINTS);
        glColor3f(1,1,1);
        glVertex3fv(t);
        glEnd();
    }
    glLineWidth(1);
//    glPopMatrix();
}


void OpenGLRender::DrawComponentMeshLine(const IMesh *ComponentMesh, const Matrix4 &transform)
{
    glPushMatrix();
    glMultMatrixf(transform);
    glLineWidth(3);
    for (Index4i t : ComponentMesh->Edges())
    {
        glBegin(GL_LINES);
        glColor3f(1,1,1);
        glVertex3fv(ComponentMesh->GetVertex(t[0]));
        glVertex3fv(ComponentMesh->GetVertex(t[1]));
        glEnd();
    }
    glLineWidth(1);
    glPopMatrix();

}

void OpenGLRender::DrawComponentMeshFill(const IMesh *ComponentMesh, const Matrix4 &transform)
{
    glPushMatrix();
    glMultMatrixf(transform);
    glLineWidth(3);
    for (Index3i t : ComponentMesh->Triangles())
    {
        glBegin(GL_TRIANGLES);
        glVertex3fv(ComponentMesh->GetVertex(t[0]));
        glVertex3fv(ComponentMesh->GetVertex(t[1]));
        glVertex3fv(ComponentMesh->GetVertex(t[2]));
        glEnd();
    }
    glLineWidth(1);
    glPopMatrix();
}


void OpenGLRender::DrawLine(const Vector3 &a, const Vector3 &b)
{
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3f(1,1,1);
    glVertex3fv(a);
    glVertex3fv(b);
    glEnd();
    glPopMatrix();
}


