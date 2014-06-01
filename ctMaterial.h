#ifndef CTMATERIAL_H
#define CTMATERIAL_H

#include <QVector4D>
#include <QVector3D>

class ctMaterial
{
private:
    QVector4D m_rgba;
public:
    ctMaterial();
    QVector4D GetRGBA();
    void SetRGBA(QVector4D t_rgba);
    QVector3D GetRGB();
    void SetRGB(QVector3D t_rgb);
    float GetAlpha();
    void SetAlpha(float t_alpha);
};

#endif // CTMATERIAL_H
