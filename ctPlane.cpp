#include "ctPlane.h"
#include <QFile>
#include <QImage>
#include <QVector>
#include <QColor>
#include <QDebug>
#include "ctShaderManager.h"
#include "ctScene.h"

ctPlane::ctPlane(ctShaderManager * t_shaderManager, ctScene * t_scene, QOpenGLContext * t_OpenGLContext, QVector3D t_AA, QVector3D t_BB, PlaneType t_type)
{
    SetDefault(t_shaderManager, t_scene, t_OpenGLContext);
    m_currentType = t_type;
    m_AA = t_AA;
    m_BB = t_BB;

    m_rows = 1;
    m_cols = 1;
    m_offset = QVector2D(1,1);
    m_frame = 0;
    //SetupPlaneCoords(t_AA, t_BB);
}

ctPlane::~ctPlane()
{
    //UNDONE: NEED COMPLETE DESTRUSTION
    qDebug()<<"ctPlane Destroyed";

    if(meshVBO)
        GetOpenGLContext()->functions()->glDeleteBuffers(1, &meshVBO);
    //glDeleteTextures(1, &textureIndex);
//    delete[] planePositions;
//    delete[] planeIndexes;
//    delete[] planeTextureCoords;
    if(planeIndexes) delete[] planeIndexes;
    if(m_texture) delete m_texture;
}

void ctPlane::GenerateCompleteBuffer(QVector3D t_AA, QVector3D t_BB, bool t_selfGeneredTextureCoord)
{
    float* planePositions = new float[12];

    planePositions[0] = t_AA.x();
    planePositions[1] = t_AA.y();
    planePositions[2] = t_AA.z();

    planePositions[3] = t_BB.x();
    planePositions[4] = t_AA.y();
    planePositions[5] = t_AA.z();

    planePositions[6] = t_BB.x();
    planePositions[7] = t_BB.y();
    planePositions[8] = t_BB.z();

    planePositions[9] = t_AA.x();
    planePositions[10] = t_BB.y();
    planePositions[11] = t_BB.z();

    //SetupIndexes
    if(planeIndexes) delete[] planeIndexes;
    planeIndexes = new unsigned short[6];

    planeIndexes[0] = 2;
    planeIndexes[1] = 1;
    planeIndexes[2] = 0;

    planeIndexes[3] = 3;
    planeIndexes[4] = 2;
    planeIndexes[5] = 0;


    //float* planeTextureCoords = new float[8];
    if(m_rows == 1 && m_cols == 1)
    {
        planeTextureCoords[0] = 1;
        planeTextureCoords[1] = 1;
        planeTextureCoords[2] = 0;
        planeTextureCoords[3] = 1;
        planeTextureCoords[4] = 0;
        planeTextureCoords[5] = 0;
        planeTextureCoords[6] = 1;
        planeTextureCoords[7] = 0;
    }
    //SetupTextureCoords();

    const int positionsOffset = 12;
    const int texCoordOffset = 8;
    float * completeBuffer = new float[positionsOffset + texCoordOffset]; // positions count + texture coord count
    for(int i = 0; i < positionsOffset; ++i)
    {
        completeBuffer[i] = planePositions[i];
    }
    for(int i = 0; i < texCoordOffset; ++i)
    {
        completeBuffer[positionsOffset + i] = planeTextureCoords[i];
    }


    if(meshVBO)
        GetOpenGLContext()->functions()->glDeleteBuffers(1, &meshVBO);
    GetOpenGLContext()->functions()->glBindBuffer(GL_ARRAY_BUFFER, 0);
    GetOpenGLContext()->functions()->glGenBuffers(1, &meshVBO);
    GetOpenGLContext()->functions()->glBindBuffer(GL_ARRAY_BUFFER, meshVBO);

    GetOpenGLContext()->functions()->glBufferData(GL_ARRAY_BUFFER, (positionsOffset + texCoordOffset) * sizeof(GLfloat),
        completeBuffer, GL_STATIC_DRAW);

    delete[] completeBuffer;

    delete[] planePositions;
    //delete[] planeIndexes;
    //delete[] planeTextureCoords;
}

void ctPlane::SetTexture(QString t_textureFileName, bool t_needResize)
{
    SetTexture(t_textureFileName.toStdString().c_str(), t_needResize);
}

void ctPlane::SetTexture(const char* t_textureFileName, bool t_needResize)
{
    if(!m_texture) m_texture = new ctTexture(t_textureFileName);

    //textureIndex = m_texture->GetTextureIndex();//CreateTexture(t_textureFileName);

    if (!m_texture->GetTextureIndex())
    {
        qDebug()<<"BadTextureIndex";
        return;
    }

    GetOpenGLContext()->functions()->glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureIndex());

    if(t_needResize)
    {
        m_AA = QVector3D((m_texture->GetWidth()/2)/m_cols, (m_texture->GetHeight()/2)/m_rows, m_AA.z());
        m_BB = QVector3D((-(m_texture->GetWidth()/2))/m_cols, (-(m_texture->GetHeight()/2))/m_rows, m_BB.z());
        ResizeMesh(m_AA, m_BB);
    }
}

void ctPlane::SetColor(QVector3D t_color)
{
    GetMaterial()->SetRGB(t_color);
}
void ctPlane::SetColor(QVector4D t_color)
{
    GetMaterial()->SetRGBA(t_color);
}

void ctPlane::ResizeMesh(QVector3D t_AA, QVector3D t_BB)
{
    m_AA = t_AA;
    m_BB = t_BB;
    GenerateCompleteBuffer(t_AA, t_BB);
}

void ctPlane::Draw(QMatrix4x4 t_projectionMatrix)
{
    DrawTextured(t_projectionMatrix);
}

void ctPlane::DrawTextured(QMatrix4x4 t_projectionMatrix)
{
    const int positionsOffset = 12 * sizeof(float);
    if(m_currentType == ctPlane::Textured)
    {
        GetOpenGLContext()->functions()->glActiveTexture(GL_TEXTURE0);
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureIndex());
    }

   GetOpenGLContext()->functions()->glBindBuffer(GL_ARRAY_BUFFER, meshVBO);

    if (posAtribLoc != -1)
    {
        GetOpenGLContext()->functions()->glVertexAttribPointer(posAtribLoc, 3, GL_FLOAT, GL_FALSE,
            (3 * sizeof(float)), (const GLvoid*)0);
        GetOpenGLContext()->functions()->glEnableVertexAttribArray(posAtribLoc);
    }
    else
    {qDebug()<<"isShit pos!!!";}
    if (colorAtribLoc != -1 && m_currentType == ctPlane::Textured)
    {
        GetOpenGLContext()->functions()->glVertexAttribPointer(colorAtribLoc, 2, GL_FLOAT, GL_FALSE,
                              (2 * sizeof(float)), (const GLvoid*)positionsOffset);
        GetOpenGLContext()->functions()->glEnableVertexAttribArray(colorAtribLoc);
    }
    else if(colorAtribLoc == -1 && m_currentType == ctPlane::Textured)
    {qDebug()<<"isShit color!!!";}

//    GetOpenGLContext()->functions()->glVertexAttribPointer(colorAtribLoc, 3, GL_FLOAT, GL_FALSE, 0, planeColor);

//        GetOpenGLContext()->functions()->glEnableVertexAttribArray(colorAtribLoc);

    m_currentShader->bind();

    m_currentShader->setUniformValue(matrixUniform, t_projectionMatrix);
    if(m_currentType == ctPlane::Textured)
        m_currentShader->setUniformValue(textureLocation, 0);
    if(m_currentType == ctPlane::Colored)
        m_currentShader->setUniformValue(colorAtribLoc, GetMaterial()->GetRGBA());
    m_currentShader->setUniformValue(transformMatrixUniform, m_transform->GetGlobalTransformMatrix().GetMatrix());
    m_currentShader->setUniformValue(materialUniform, GetMaterial()->GetRGBA());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, planeIndexes);

    //qDebug()<<"DrawTexturedPlane";

    GetOpenGLContext()->functions()->glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_currentShader->release();
}

void ctPlane::InitShader()
{
    if(m_currentType == ctPlane::Colored)
    {
        m_currentShader = m_shaderManager->GetShaderProgram("coloredShader");//
        //m_currentShader = m_shaderManager->SetUpShaderProgram("texturedModelVertexShaderSource", "texturedFragmentShaderSource", "texturedPlaneShader");// need replace this part to outside
        GettingAttributes(m_currentShader);
    }
    else if(m_currentType == ctPlane::Textured)
    {
        m_currentShader = m_shaderManager->SetUpShaderProgram("texturedModelVertexShaderSource", "texturedFragmentShaderSource", "texturedPlaneShader");// need replace this part to outside
        GettingAttributes(m_currentShader);
    }
}

void ctPlane::InitShader(const char *t_shaderProgrammName)
{
    m_currentShader = m_shaderManager->GetShaderProgram(t_shaderProgrammName);
    GettingAttributes(m_currentShader);
}
void ctPlane::InitShader(QString t_shaderProgrammName)
{
    m_currentShader = m_shaderManager->GetShaderProgram(t_shaderProgrammName);
    GettingAttributes(m_currentShader);
}
void ctPlane::InitShader(QOpenGLShaderProgram *t_initedShader)
{
    m_currentShader = t_initedShader;
    GettingAttributes(m_currentShader);
}

void ctPlane::GettingAttributes(QOpenGLShaderProgram * t_shaderProgram)
{
    if(m_currentType == Colored)
    {
        posAtribLoc = t_shaderProgram->attributeLocation("_POSITION");
        colorAtribLoc =t_shaderProgram->uniformLocation("_COLOR");
        matrixUniform = t_shaderProgram->uniformLocation("_VIEWPROJECTIONMATRIX");
        transformMatrixUniform = t_shaderProgram->uniformLocation("_TRANSFORMMATRIX");
        materialUniform = t_shaderProgram->uniformLocation("_MATERIAL");
    }
    else if(m_currentType == Textured)
    {
        if(!t_shaderProgram)
        {qDebug()<<"Shit Shader Program";}
        posAtribLoc = t_shaderProgram->attributeLocation("_POSITION");
        colorAtribLoc = t_shaderProgram->attributeLocation("_TEXCOORD");
        matrixUniform = t_shaderProgram->uniformLocation("_VIEWPROJECTIONMATRIX");
        transformMatrixUniform = t_shaderProgram->uniformLocation("_TRANSFORMMATRIX");
        textureLocation = t_shaderProgram->uniformLocation("_TEX1");
        materialUniform = t_shaderProgram->uniformLocation("_MATERIAL");
    }
}

void ctPlane::Init()
{
    InitShader();
    GenerateCompleteBuffer(m_AA, m_BB);
    m_isInitialized = true;
    //qDebug()<<"Init ctPlane";
    //StartUpInit
}

void ctPlane::Draw()
{
    Draw(m_projectionMatrix.GetMatrix());
    //SelfDrawing
}

void ctPlane::Update()
{
    ctObject::Update();
    //Update Some States
}

void ctPlane::SetDefault(ctShaderManager * t_shaderManager, ctScene * t_scene, QOpenGLContext * t_OpenGLContex)
{
    ctObject::SetDefault(t_shaderManager, t_scene, t_OpenGLContex);
    meshVBO = 0;
    //textureIndex = 0;
    m_texture = 0;
    planeIndexes = 0;
}

ctEntity* ctPlane::Clone()
{
    //----------FROM-ENTITY-------------
    ctPlane* tmp = new ctPlane(GetShaderManager(), GetScene(), GetOpenGLContext(), m_AA, m_BB, m_currentType);//ctPlane(GetShaderManager(), GetScene(), GetOpenGLContext());//(ctObject*)ctEntity::Clone();
    tmp->SetName(GetName() + QString("_Clone"));
    //----------FROM-OBJECT-------------
    ctMatrix4 tmpMat = GetProjectionMatrix();
    tmp->SetProjectionMatrix(tmpMat);
    tmp->GetTransform()->SetLocalMatrix(m_transform->GetLocalTransformMatrix());
    tmp->GetTransform()->SetParent(m_transform->GetParent());
    //----------FROM-PLANE--------------
    tmp->posAtribLoc = posAtribLoc;
    tmp->colorAtribLoc = colorAtribLoc;
    tmp->textureLocation = textureLocation;
    tmp->matrixUniform = matrixUniform;
    tmp->transformMatrixUniform = transformMatrixUniform;
    //tmp->textureIndex = textureIndex;
    tmp->meshVBO = meshVBO;


    return (ctEntity*)tmp;
}

void ctPlane::SetTexture(ctTexture *t_texture)
{
    if(m_texture)
    {delete m_texture;}

    m_texture = t_texture;
}

void ctPlane::SetTextureGrid(int t_rows, int t_cols)
{
    m_rows = t_rows;
    m_cols = t_cols;
    SetupTextureCoords();
}
void ctPlane::SetupTextureCoords()
{
    float tw = 1.0f / m_cols;//colsCount;
    float th = 1.0f / m_rows;//rowsCount;

    m_texCoords[0] = QVector2D(0, 1.0f - th); // 0 0
    m_texCoords[1] = QVector2D(0, 1.0f);      // 0 1
    m_texCoords[2] = QVector2D(tw, 1.0f);     // 1 1
    m_texCoords[3] = QVector2D(tw, 1.0f - th);// 1 0

    for(int i = 0; i < 4; ++i) m_texCoords[i]+=m_offset;

    planeTextureCoords[0] = m_texCoords[2].x();//1;
    planeTextureCoords[1] = m_texCoords[2].y();//1;
    planeTextureCoords[2] = m_texCoords[1].x();//0;
    planeTextureCoords[3] = m_texCoords[1].y();//1;
    planeTextureCoords[4] = m_texCoords[0].x();//0;
    planeTextureCoords[5] = m_texCoords[0].y();//0;
    planeTextureCoords[6] = m_texCoords[3].x();//1;
    planeTextureCoords[7] = m_texCoords[3].y();//0;

    GenerateCompleteBuffer(m_AA, m_BB, false);
}
void ctPlane::SetFrame(int t_frame)
{
    m_frame = t_frame;
    int nCols = m_cols;
    int nRows = m_rows;

    float u = nCols > 1 ? ((float)(m_frame % nCols)) / nCols : 0;
    float v = nRows > 1 && nCols > 0 ? ((float)(m_frame / nCols)) / nRows : 0;

    //material.SetTextureOffset("_MainTex", new Vector2(u, -v));
    SetTextureOffset(QVector2D(u,-v));
}
int ctPlane::GetFrame()
{
    return m_frame;
}
void ctPlane::SetTextureOffset(QVector2D t_offset)
{
    m_offset = t_offset;
    SetupTextureCoords();
}

ctTexture* ctPlane::GetTexture()
{
    return m_texture;
}
