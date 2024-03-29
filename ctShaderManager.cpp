#include "ctShaderManager.h"
#include <QDebug>

ctShaderManager::ctShaderManager()
{
    qDebug()<<"ERROR Shaders withput context";
}

ctShaderManager::ctShaderManager(QOpenGLContext *t_OpenGLContext)
{
    qDebug()<<"Sheders with context";
    m_OpenGLContext = t_OpenGLContext;
}

ctShaderManager::~ctShaderManager()
{
    QList<QString> tmpKeys = m_vertexShaders.keys();

    for(int i = 0; i < m_vertexShaders.size(); ++i)
    {
        delete m_vertexShaders[tmpKeys[i]];
    }
    m_vertexShaders.clear();

    tmpKeys = m_fragmentShaders.keys();

    for(int i = 0; i < m_fragmentShaders.size(); ++i)
    {
        delete m_fragmentShaders[tmpKeys[i]];
    }
    m_fragmentShaders.clear();

    //TODO: PERHEPS NEED FIX;
}

int ctShaderManager::AddFragmentShader(const char * t_shaderTxt, const char *t_name)
{
    QString shaderTxt(t_shaderTxt);
    QString name(t_name);
    return AddFragmentShader(shaderTxt, name);
}

int ctShaderManager::AddFragmentShader(QString t_shaderTxt, QString t_name)
{
    QOpenGLShader* tmpShader = new QOpenGLShader(QOpenGLShader::Fragment);//, 0);
    tmpShader->compileSourceCode(t_shaderTxt);
    m_fragmentShaders.insert(t_name, tmpShader);
    return 0;
}

int ctShaderManager::AddVertexShader(const char *t_shaderTxt, const char *t_name)
{
    QString shaderTxt(t_shaderTxt);
    QString name(t_name);
    return AddVertexShader(shaderTxt, name);
}

int ctShaderManager::AddVertexShader(QString t_shaderTxt, QString t_name)
{
    QOpenGLShader* tmpShader = new QOpenGLShader(QOpenGLShader::Vertex);//, 0);
    tmpShader->compileSourceCode(t_shaderTxt);
    m_vertexShaders.insert(t_name, tmpShader);
    return 0;
}

QOpenGLShaderProgram* ctShaderManager::SetUpShaderProgram(const char *t_vertexShaderName, const char *t_fragmentShaderName, const char *t_name)
{
    QString vertexShaderName(t_vertexShaderName);
    QString fragmentShaderName(t_fragmentShaderName);
    QString shaderProgtramName(t_name);
    return SetUpShaderProgram(vertexShaderName, fragmentShaderName, shaderProgtramName);
}

QOpenGLShaderProgram* ctShaderManager::SetUpShaderProgram(QString t_vertexShaderName, QString t_fragmentShaderName, QString t_name)
{
    QOpenGLShaderProgram * tmpProgram = new QOpenGLShaderProgram();
    tmpProgram->addShader(m_vertexShaders[t_vertexShaderName]);
    tmpProgram->addShader(m_fragmentShaders[t_fragmentShaderName]);
    tmpProgram->link();
    m_shaderPrograms.insert(t_name, tmpProgram);
    return tmpProgram;
}

QOpenGLShaderProgram* ctShaderManager::GetShaderProgram(QString t_name)
{
    if(m_shaderPrograms.contains(t_name))
    {
        //qDebug()<<"Shader: "<<t_name<<"Finded!";
        return m_shaderPrograms[t_name];
    }
    else
    {
        qDebug()<<"Cant Finde Shader!!!";
        return 0;
    }
}

QOpenGLShaderProgram* ctShaderManager::GetShaderProgram(const char *t_name)
{
    QString name(t_name);
    return GetShaderProgram(name);
}
