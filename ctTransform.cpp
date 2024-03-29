#include "ctTransform.h"
#include "ctObject.h"
#include "QDebug"

ctTransform::ctTransform()
{
    SetDefault();
}

ctTransform::ctTransform(ctObject * t_entity)
{
    SetDefault();
    m_object = t_entity;
}

ctTransform::~ctTransform()
{}

void ctTransform::SetDefault()
{
    m_parent = 0;
    m_childrens = new QVector<ctTransform * >();
}

void ctTransform::Move(QVector3D t_pos)
{
    m_localTransform.TranslateTo(t_pos);
}

void ctTransform::MoveBy(QVector3D t_pos)
{
    m_localTransform.Translate(t_pos);
}

void ctTransform::Scale(QVector3D t_scl)
{
    m_localTransform.Scale(t_scl);
}

void ctTransform::RotateByX(float t_angle)
{
    m_localTransform.RotateX(t_angle);
}

void ctTransform::RotateByY(float t_angle)
{
    m_localTransform.RotateY(t_angle);
}

void ctTransform::RotateByZ(float t_angle)
{
    m_localTransform.RotateZ(t_angle);
}

ctMatrix4 ctTransform::GetLocalTransformMatrix() const
{
    return m_localTransform;
}

ctMatrix4 ctTransform::GetGlobalTransformMatrix()// const
{
    if(!m_parent)
    {
        m_globalTransform.operator=(m_localTransform);
        //m_globalTransform = m_localTransform;
        return m_globalTransform;
    }
    else
    {
//        ctMatrix4 tmpMat(m_parent->GetGlobalTransformMatrix());
//        //tmpMat
//        //QMatrix4x4 tmp1 = m_parent->GetGlobalTransformMatrix().GetMatrix();
//        //QMatrix4x4 tmp2 = m_localTransform.GetMatrix();
//        //tmp1 *= tmp2;
//        //tmpMat.SetMatrix(tmp1);
//        ctMatrix4 tmpMat1(m_localTransform);
//        //tmpMat1.Multiply(tmpMat);
//        m_globalTransform = tmpMat;//tmpMat1; //= tmpMat;
//        return m_globalTransform;
        ctMatrix4 tmpMat(m_parent->GetGlobalTransformMatrix());
        ctMatrix4 tmpMat1(m_localTransform);
        tmpMat.Multiply(tmpMat1);
        m_globalTransform = tmpMat;
        return tmpMat;
    }
}

ctMatrix4 operator * (ctMatrix4 & t_mat1, ctMatrix4 & t_mat2)
{
    ctMatrix4 tmp(t_mat1);
    tmp.Multiply(t_mat2);
    return tmp;
}

ctObject * ctTransform::GetGameObject() const
{
    return m_object;
}
//TODO: NEED TEST !!!
bool ctTransform::GetParentsVisibility() const
{
    if(!m_parent)
    {
        return m_object->IsVisible();
    }
    else
    {
        ctTransform * parent = m_parent;
        while(parent)
        {
            parent = parent->GetParent();
            if(!parent->GetGameObject()->IsVisible())
            {return false;}
            if(!parent->GetParent()) return parent->GetGameObject()->IsVisible();
        }
        parent->GetGameObject()->IsVisible();
    }
}

ctTransform * ctTransform::GetParent() const
{
    return m_parent;
}

bool ctTransform::IsVisible() const
{
    if(!m_object) return false;
    return m_object->IsVisible();
}

void ctTransform::SetVisible(bool t_visible)
{
    if(!m_object) return;
    if(t_visible) m_object->Show();
    else m_object->Hide();
}

void ctTransform::SetLocalMatrix(const ctMatrix4 & t_matrix)
{
    m_localTransform = t_matrix;
}

void ctTransform::SetLocalMatrix(const QMatrix4x4 & t_matrix)
{
    m_localTransform.SetMatrix(t_matrix);
}

void ctTransform::SetParent(const ctTransform * t_parent)
{
    m_parent = const_cast<ctTransform*>(t_parent);

    //ctMatrix4 tmp(m_parent->GetGlobalTransformMatrix().GetMatrix()); //= m_parent->GetGlobalTransformMatrix().Inverted();
    //ctMatrix4 tmp2(m_localTransform.GetMatrix());
    //tmp = tmp.Inverted();
    //tmp2.Multiply(tmp);
    //m_localTransform.Multiply(tmp2);
    //tmp2 = tmp2.Inverted();
    //tmp.Multiply(tmp2);
    //m_localTransform.Multiply(tmp);
}

void ctTransform::UpdateGlobalTransformMatrix()
{
    //TODO: NEED REFINE!!!
}

QVector3D ctTransform::GetLocalPos()
{
    QVector4D tmp = GetLocalTransformMatrix().GetMatrix().column(3);
    return QVector3D(tmp.x(), tmp.y(), tmp.z());
}

QVector3D ctTransform::GetGlobalPos()
{
    QVector4D tmp = GetGlobalTransformMatrix().GetMatrix().column(3);
    return QVector3D(tmp.x(), tmp.y(), tmp.z());
}
