#include "ctMaterial.h"

ctMaterial::ctMaterial()
{
    m_rgba = QVector4D(1, 1, 1, 1);
}

QVector4D ctMaterial::GetRGBA()
{
    return m_rgba;
}

void ctMaterial::SetRGBA(QVector4D t_rgba)
{
    m_rgba = t_rgba;
}

QVector3D ctMaterial::GetRGB()
{
    return QVector3D(m_rgba.x(), m_rgba.y(), m_rgba.z());
}

void ctMaterial::SetRGB(QVector3D t_rgb)
{
    m_rgba.setX(t_rgb.x());
    m_rgba.setY(t_rgb.y());
    m_rgba.setZ(t_rgb.z());
}

float ctMaterial::GetAlpha()
{
    return m_rgba.w();
}

void ctMaterial::SetAlpha(float t_alpha)
{
    m_rgba.setW(t_alpha);
}
