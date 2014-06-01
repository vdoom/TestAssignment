#ifndef CTLINEBORDERS_H
#define CTLINEBORDERS_H

#include "ctObject.h"
#include <QtGui/QOpenGLFunctions>
#include "ctShaderManager.h"
#include "ctTransform.h"
#include <QVector3D>
#include <QVector>

struct intersectInfo
{
    QVector3D Vec1Begin;
    QVector3D Vec1End;
    QVector3D Vec2Begin;
    QVector3D Vec2End;
    bool isIntesect;
    QVector3D pointOfIntersection;
};

class ctLineBorders : public ctObject
{
private:
    QOpenGLShaderProgram * m_currentShader;

    GLuint posAtribLoc;
    GLuint colorAtribLoc;
    GLuint textureLocation;
    GLuint matrixUniform;
    GLuint transformMatrixUniform;
    GLuint materialUniform;
    GLuint meshVBO;
    unsigned short* planeIndexes;

    QVector<QVector3D> m_dots;

    void GettingAttributes(QOpenGLShaderProgram * t_shaderProgram);

public:
    explicit ctLineBorders(ctShaderManager *);
    ctLineBorders(ctShaderManager *, ctScene *);
    ctLineBorders(ctShaderManager *, ctScene *, QOpenGLContext *);
    virtual ~ctLineBorders();

    void SetColor(QVector3D t_color);
    void SetColor(QVector4D t_color);

    //----Overload-functions----
    virtual void Update();
    virtual void Draw();
    virtual void Init();
    virtual ctEntity* Clone(){return 0;}
    //--------------------------

    void GenerateCompleteBuffer();
    void AddDot(QVector3D t_dot);
    QVector<QVector3D> * GetDots();

    intersectInfo intersection(QVector3D t_vec1, QVector3D t_vec2,
                            QVector3D t_vec3, QVector3D t_vec4)
    {
        intersectInfo info;
        info.Vec1Begin = t_vec1;
        info.Vec1End = t_vec2;
        info.Vec2Begin = t_vec3;
        info.Vec2End = t_vec4;
        info.isIntesect = true;
        int d = (t_vec1.x()-t_vec2.x())*(t_vec3.y()-t_vec4.y()) - (t_vec1.y()-t_vec2.y())*(t_vec3.x()-t_vec4.x());
        if (d == 0)
        {
            info.isIntesect = false;
            return info;
        }

        int xi = ((t_vec3.x()-t_vec4.x())*(t_vec1.x()*t_vec2.y()-t_vec1.y()*t_vec2.x())-(t_vec1.x()-t_vec2.x())*(t_vec3.x()*t_vec4.y()-t_vec3.y()*t_vec4.x()))/d;
        int yi = ((t_vec3.y()-t_vec4.y())*(t_vec1.x()*t_vec2.y()-t_vec1.y()*t_vec2.x())-(t_vec1.y()-t_vec2.y())*(t_vec3.x()*t_vec4.y()-t_vec3.y()*t_vec4.x()))/d;

        info.pointOfIntersection = QVector3D(xi, yi, 0);
        return info;
     }

};

#endif // CTLINEBORDERS_H
