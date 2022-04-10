#-------------------------------------------------
#
# Project created by QtCreator 2016-07-02T02:00:03
#
#-------------------------------------------------

QT += core gui network sensors



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++11
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++11

TARGET = RemoteManagerForAndroid
TEMPLATE = app


LIBS += -lSDL2

SOURCES += main.cpp\
        Engine/IGeometry/ILight.cpp \
        Engine/IGeometry/IQCamera.cpp \
        Engine/IGeometry/Mesh/IMesh.cpp \
        Engine/IGeometry/Mesh/IMeshGeneratorCone.cpp \
        Engine/IGeometry/Mesh/IMeshGeneratorCuboid.cpp \
        Engine/IGeometry/Mesh/IMeshGeneratorCylinder.cpp \
        Engine/IGeometry/Mesh/IMeshGeneratorDetails.cpp \
        Engine/IGeometry/Mesh/IMeshGeneratorEllipsoid.cpp \
        GLWidget.cpp \
        Scene/IMaterial.cpp \
        Scene/OpenGL/OpenGLRender.cpp \
        Scene/OpenGL/geometry_opengl.cpp \
        Scene/SceneCore.cpp \
        Scene/SceneMain.cpp \
        Scene/Shader/Shader.cpp \
        mainwindow.cpp \
        qcustomplot.cpp

HEADERS  += mainwindow.h \
    Engine/IGeometry/ILight.h \
    Engine/IGeometry/IQCamera.h \
    Engine/IGeometry/Mesh/IIndexUtil.h \
    Engine/IGeometry/Mesh/IMesh.h \
    Engine/IGeometry/Mesh/IMeshGenerator.h \
    Engine/IGeometry/Mesh/IMeshGeneratorDetails.h \
    Engine/IGeometry/Mesh/ITypes.h \
    Engine/IGeometry/Mesh/memory/dvector.h \
    Engine/IGeometry/Mesh/memory/iterator_util.h \
    Engine/IGeometry/Mesh/memory/refcount_vector.h \
    Engine/IGeometry/Mesh/memory/small_list_set.h \
    Engine/IMath/IAffineTransform.h \
    Engine/IMath/IFunc.h \
    Engine/IMath/IMatrix2x2.h \
    Engine/IMath/IMatrix3x3.h \
    Engine/IMath/IMatrix4x4.h \
    Engine/IMath/IQuaternion.h \
    Engine/IMath/IReal.h \
    Engine/IMath/ISpherical.h \
    Engine/IMath/ITransform.h \
    Engine/IMath/IVector.h \
    Engine/IMath/IVector2D.h \
    Engine/IMath/IVector3D.h \
    Engine/IMath/IVector4D.h \
    Engine/engine.hpp \
    Engine/imaths.hpp \
    GLWidget.h \
    Scene/IMaterial.h \
    Scene/OpenGL/OpenGLRender.h \
    Scene/OpenGL/geometry_opengl.h \
    Scene/SceneCore.h \
    Scene/SceneMain.h \
    Scene/Shader/Shader.h \
    qcustomplot.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    files.qrc \
    shaders.qrc

