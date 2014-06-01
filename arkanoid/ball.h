#ifndef BALL_H
#define BALL_H
#include "ctPlane.h"

class Ball : public ctPlane
{
private:
    QVector3D m_moveVector;
    float m_speed;
    float m_radius;
public:
    explicit Ball(ctShaderManager *);
    Ball(ctShaderManager *, ctScene *);
    Ball(ctShaderManager *, ctScene *, QOpenGLContext *);
    virtual ~Ball();

    virtual void Update();
    virtual void Init();
    virtual ctEntity* Clone(){return 0;}

    QVector3D GetMovingVector();
    void SetMovingVector(QVector3D);
    void SetMovingVector(QVector2D);

    float GetSpeed();
    void SetSpeed(float);

    float GetRadius();
};

#endif // BALL_H
