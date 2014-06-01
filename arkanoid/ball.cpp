#include "ball.h"

Ball::Ball(ctShaderManager * t_shaders) : ctPlane(t_shaders), m_radius(0)
{}
Ball::Ball(ctShaderManager * t_shaders, ctScene * t_scene) : ctPlane(t_shaders, t_scene), m_radius(0)
{}
Ball::Ball(ctShaderManager *t_shaders, ctScene *t_scene, QOpenGLContext *t_OGLContext) : ctPlane(t_shaders, t_scene, t_OGLContext, QVector3D(512,384,-20), QVector3D(-512, -384, -20), ctPlane::Textured), m_radius(0)
{
    m_moveVector = QVector3D(0,0,0);
    m_speed = 0;
}

Ball::~Ball()
{}

void Ball::Update()
{
    ctPlane::Update();
    GetTransform()->MoveBy(m_moveVector * m_speed);
}

void Ball::Init()
{
    SetTexture(":/texture/circles/yellow4.png", true);
    m_radius = GetTexture()->GetWidth()/2;
    ctPlane::Init();
}

QVector3D Ball::GetMovingVector()
{
    return m_moveVector;
}
void Ball::SetMovingVector(QVector3D t_moveVector)
{
    m_moveVector = t_moveVector.normalized();
}

void Ball::SetMovingVector(QVector2D t_moveVector)
{
    m_moveVector = QVector3D(t_moveVector.x(), t_moveVector.y(), 0);
}

float Ball::GetSpeed()
{
    return m_speed;
}
void Ball::SetSpeed(float t_speed)
{
    m_speed = t_speed;
}

float Ball::GetRadius()
{
    return m_radius;
}
