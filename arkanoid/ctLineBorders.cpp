#include "ctLineBorders.h"

ctLineBorders::ctLineBorders(ctShaderManager * t_shaders) : ctObject(t_shaders),planeIndexes(0)
{SetDefault(t_shaders, 0, 0);}
ctLineBorders::ctLineBorders(ctShaderManager * t_shaders, ctScene * t_scene) : ctObject(t_shaders, t_scene),planeIndexes(0)
{SetDefault(t_shaders, t_scene, 0);}
ctLineBorders::ctLineBorders(ctShaderManager * t_shaders, ctScene * t_scene, QOpenGLContext * t_OGLContext) : ctObject(t_shaders, t_scene, t_OGLContext),planeIndexes(0)
{SetDefault(t_shaders, t_scene, t_OGLContext);}
ctLineBorders::~ctLineBorders()
{}
//TODO: need refine!!!
void ctLineBorders::GenerateCompleteBuffer()
{
    int posBuffersSize = 3 * m_dots.count();
    float* planePositions = new float[posBuffersSize];

    int tmp = 0;
    for(int i = 0; i < m_dots.count(); ++i)
    {
        planePositions[i + (2*i)] = m_dots[i].x();
        planePositions[i + 1 + (2*i)] = m_dots[i].y();
        planePositions[i + 2 + (2*i)] = m_dots[i].z();
        //tmp+=2;
    }
//    planePositions[0] = t_AA.x();
//    planePositions[1] = t_AA.y();
//    planePositions[2] = t_AA.z();

//    planePositions[3] = t_BB.x();
//    planePositions[4] = t_AA.y();
//    planePositions[5] = t_AA.z();

//    planePositions[6] = t_BB.x();
//    planePositions[7] = t_BB.y();
//    planePositions[8] = t_BB.z();

//    planePositions[9] = t_AA.x();
//    planePositions[10] = t_BB.y();
//    planePositions[11] = t_BB.z();

    //SetupIndexes
    if(planeIndexes) delete[] planeIndexes;
    planeIndexes = new unsigned short[m_dots.count()];

    for(int i = 0; i < m_dots.count(); ++i)
    {
        planeIndexes[i] = i;
    }
//    planeIndexes[0] = 2;
//    planeIndexes[1] = 1;
//    planeIndexes[2] = 0;

//    planeIndexes[3] = 3;
//    planeIndexes[4] = 2;
//    planeIndexes[5] = 0;


    //float* planeTextureCoords = new float[8];
    //SetupTextureCoords();

    //const int positionsOffset = 12;
    //const int texCoordOffset = 8;
    //float * completeBuffer = new float[positionsOffset + texCoordOffset]; // positions count + texture coord count
//    for(int i = 0; i < positionsOffset; ++i)
//    {
//        completeBuffer[i] = planePositions[i];
//    }
//    for(int i = 0; i < texCoordOffset; ++i)
//    {
//        completeBuffer[positionsOffset + i] = planeTextureCoords[i];
//    }


    if(meshVBO)
        GetOpenGLContext()->functions()->glDeleteBuffers(1, &meshVBO);
    GetOpenGLContext()->functions()->glBindBuffer(GL_ARRAY_BUFFER, 0);
    GetOpenGLContext()->functions()->glGenBuffers(1, &meshVBO);
    GetOpenGLContext()->functions()->glBindBuffer(GL_ARRAY_BUFFER, meshVBO);

    GetOpenGLContext()->functions()->glBufferData(GL_ARRAY_BUFFER, (posBuffersSize) * sizeof(GLfloat),
        planePositions, GL_STATIC_DRAW);

    //delete[] completeBuffer;

    delete[] planePositions;
}

void ctLineBorders::GettingAttributes(QOpenGLShaderProgram * t_shaderProgram)
{
    posAtribLoc   = t_shaderProgram->attributeLocation("_POSITION");
    matrixUniform = t_shaderProgram->uniformLocation("_VIEWPROJECTIONMATRIX");
    transformMatrixUniform = t_shaderProgram->uniformLocation("_TRANSFORMMATRIX");
    materialUniform = t_shaderProgram->uniformLocation("_MATERIAL");
}

void ctLineBorders::Init()
{
    m_currentShader = GetShaderManager()->GetShaderProgram("lineShader");
    GettingAttributes(m_currentShader);

    qDebug()<<intersection(QVector3D(2,1,0),QVector3D(9,4,0),QVector3D(2,6,0),QVector3D(4,4,0)).pointOfIntersection;
}

//TODO: need refine!!!
void ctLineBorders::Update()
{}
//TODO: need refine!!!
void ctLineBorders::Draw()
{
    //if(!rectDotIndexes) return;
    //glDisable(GL_DEPTH_TEST);
    GetOpenGLContext()->functions()->glBindBuffer(GL_ARRAY_BUFFER, meshVBO);

    if (posAtribLoc != -1)
    {
        GetOpenGLContext()->functions()->glVertexAttribPointer(posAtribLoc, 3, GL_FLOAT, GL_FALSE,
            (3 * sizeof(GLfloat)), (const GLvoid*)0);
        GetOpenGLContext()->functions()->glEnableVertexAttribArray(posAtribLoc);
    }
    else
    {qDebug()<<"isShit pos!!!";}

    m_currentShader->bind();

    m_currentShader->setUniformValue(matrixUniform, GetProjectionMatrix().GetMatrix());
    m_currentShader->setUniformValue(materialUniform, GetMaterial()->GetRGBA());
    m_currentShader->setUniformValue(transformMatrixUniform, GetTransform()->GetGlobalTransformMatrix().GetMatrix());

    glDrawElements(GL_LINE_LOOP, m_dots.count(), GL_UNSIGNED_SHORT, planeIndexes);

    GetOpenGLContext()->functions()->glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_currentShader->release();
}

void ctLineBorders::AddDot(QVector3D t_dot)
{
    m_dots.append(t_dot);
}

QVector<QVector3D>* ctLineBorders::GetDots()
{
    return &m_dots;
}
