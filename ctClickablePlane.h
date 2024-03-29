#ifndef CTCLICKABLEPLANE_H
#define CTCLICKABLEPLANE_H

#include "ctPlane.h"
#include "ctRect.h"

class ctClickablePlane : public ctPlane
{
private:
    ctRect m_rect;
    QVector4D m_rectColor;
    bool m_drawRect;
    QOpenGLShaderProgram * m_lineShader;

    GLuint posAtribLoc;
    GLuint colorUniformLoc;
    GLuint matrixUniform;
    GLuint transformMatrixUniform;
    GLuint lineMaterialUniform;
    GLuint meshVBOlines;

    GLshort* rectDotIndexes;

    void GettingLineAttributes();
    void GenerateVBOforRect();
    void DrawRectLines();
    virtual void ResizeMesh(QVector3D t_AA, QVector3D t_BB);
protected:
    virtual void SetDefault(ctShaderManager *, ctScene *, QOpenGLContext *);
public:
    //ctClickablePlane();
    explicit ctClickablePlane(ctShaderManager *);
    ctClickablePlane(ctShaderManager *, ctScene *);
    ctClickablePlane(ctShaderManager *, ctScene *, QOpenGLContext *);
    ctClickablePlane(ctShaderManager *, ctScene *, QOpenGLContext *, QVector3D t_AA, QVector3D t_BB, PlaneType t_type);

    virtual ~ctClickablePlane();

    void SetRect(const ctRect& t_rect);
    ctRect GetRect() const;
    ctRect GetTransformedRect();

    bool IsIntersect(QVector3D);
    //--------------------------
    virtual void Update();
    virtual void Draw();
    virtual void Init();
    virtual ctEntity* Clone();
    //--------------------------
};

#endif // CTCLICKABLEPLANE_H
