#include "testscene.h"
//#include "ctPlane.h"
#include "ctClickablePlane.h"
#include "ctShaderManager.h"
#include "ctWindow.h"
#include "ctTime.h"
#include "shaders.h"
#include "ctInput.h"
#include "ctInputEvent.h"
#include <QDesktopWidget>
#include "ctRand.h"
#include "ctButton.h"
#include "ctMover.h"
#include "arkanoid/ctLineBorders.h"
#include "arkanoid/ball.h"
#include "math.h"

#define Input GetWindow()->GetInput()->GetInputHelper()

testScene::testScene()
{
    SetIsFreezable(true);
    m_isInit = false;
}

testScene::testScene(ctShaderManager * t_shaderManager) : ctScene(t_shaderManager)
{
    SetIsFreezable(true);
    m_isInit = false;
}

testScene::testScene(ctShaderManager * t_shaderManager, QOpenGLContext * t_OpenGLContext) : ctScene(t_shaderManager, t_OpenGLContext)
{
    SetIsFreezable(true);
    m_isInit = false;
}

testScene::~testScene()
{

}

void testScene::Init()
{
    ctScene::Init();
    m_lastFPS = 0;
    SetDragMode(false);

    SetDefault(GetShaderManager(), 0, GetWindow()->GetOpenGLContext());
    m_lineBorders = new ctLineBorders(GetShaderManager(), this, GetOpenGLContext());

    m_ball = new Ball(GetShaderManager(), this, GetOpenGLContext());

    m_lineBorders->Init();
    m_lineBorders->AddDot(QVector3D(-500, 0, 0));
    m_lineBorders->AddDot(QVector3D(0, 300, 0));
    m_lineBorders->AddDot(QVector3D(500, 0, 0));
    m_lineBorders->AddDot(QVector3D(0, -300, 0));
    m_lineBorders->GenerateCompleteBuffer();

    m_ball->Init();
    m_ball->SetMovingVector(QVector3D(1,5,0));
    m_ball->SetSpeed(3.0f);

    m_ball->GetTransform()->Move(QVector3D(0,0,0));
    glClearColor(1.3f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, GetWindow()->width(), GetWindow()->height());
    m_frame = 0;

    AddObject(m_ball);
    AddObject(m_lineBorders);
    m_isInit = true;
}

void testScene::SetDefault(ctShaderManager * t_shaderManager, ctScene * t_scene, QOpenGLContext * t_OpenGLContext)
{
    ctScene::SetDefault(t_shaderManager, t_scene, t_OpenGLContext);

    GetShaderManager()->AddFragmentShader(fragmentShaderSource, "fragmentShaderSource");
    GetShaderManager()->AddVertexShader(vertexModelShaderSource, "vertexModelShaderSource");
    GetShaderManager()->AddFragmentShader(texturedFragmentShaderSource, "texturedFragmentShaderSource");
    //GetShaderManager()->AddVertexShader(texturedVertexShaderSource, "texturedVertexShaderSource");
    GetShaderManager()->AddVertexShader(texturedModelVertexShaderSource, "texturedModelVertexShaderSource");
    GetShaderManager()->AddFragmentShader(lineFragmentShaderSource, "lineFragmentShaderSource");
    GetShaderManager()->AddVertexShader(lineVertexShaderSource, "lineVertexShaderSource");
    GetShaderManager()->SetUpShaderProgram("vertexModelShaderSource", "fragmentShaderSource", "coloredShader");
    GetShaderManager()->SetUpShaderProgram("lineVertexShaderSource", "lineFragmentShaderSource", "lineShader");
    GetShaderManager()->SetUpShaderProgram("texturedModelVertexShaderSource", "texturedFragmentShaderSource", "texturedPlaneShader");
}

void testScene::BeginDraw()
{
    ctScene::BeginDraw();
    //-----------------TEMPORARY-SOLUTION-------------------------
    glViewport(0, 0, GetWindow()->GetWidth(), GetWindow()->GetHeight());
    //------------------------------------------------------------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0.5f,0,1);
    ++frameCounter;
    if(msecsCounter<1000)msecsCounter += ctTime::GetTime()->GetDeltaTime();
    else {m_lastFPS=frameCounter; msecsCounter = 0;frameCounter=0;}

    //DrawText(QPointF(30,30), QString::number(m_lastFPS));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void testScene::Draw()
{
    ctScene::Draw();

    QVector<ctObject*>::Iterator itr;
    for( itr = m_objects->begin(); itr != m_objects->end(); ++itr)
    {
        (*itr)->SetProjectionMatrix(matrix);
    }
}

void testScene::EndDraw()
{
    ctScene::EndDraw();
    m_frame+=1.0f;
    if(m_frame > 360) m_frame = m_frame - 360;
}
void testScene::Update()
{
    ctScene::Update();

    ctTime::GetTime()->Update();

    CalcColliedReaction();
}

void testScene::Freeze()
{
}

void testScene::Unfreeze()
{
}

void testScene::CalcColliedReaction()
{
    QVector<QVector3D> *dots = m_lineBorders->GetDots();

    for(int i = 0; i < dots->count(); ++i)
    {
        intersectInfo intersection;
        if(i < dots->count()-1)
        {
            intersection = m_lineBorders->intersection(m_ball->GetTransform()->GetGlobalPos(),
                                                       m_ball->GetTransform()->GetGlobalPos() + m_ball->GetMovingVector(),
                                                       dots->at(i), dots->at(i+1));
        }
        else
        {
            intersection = m_lineBorders->intersection(m_ball->GetTransform()->GetGlobalPos(),
                                                       m_ball->GetTransform()->GetGlobalPos() + m_ball->GetMovingVector(),
                                                       dots->at(i), dots->at(0));
        }
        if(intersection.isIntesect)
        {
            if((intersection.pointOfIntersection - m_ball->GetTransform()->GetGlobalPos()).length() <
                    (intersection.pointOfIntersection - (m_ball->GetTransform()->GetGlobalPos()+m_ball->GetMovingVector())).length())
                continue;
            else if((intersection.pointOfIntersection - m_ball->GetTransform()->GetGlobalPos()).length()-m_ball->GetRadius() <=
                    (m_ball->GetMovingVector()*m_ball->GetSpeed()).length())
            {
                QVector2D vec1 = (intersection.Vec2Begin - intersection.pointOfIntersection).toVector2D();
                QVector2D vec2 = (m_ball->GetTransform()->GetGlobalPos() - intersection.pointOfIntersection).toVector2D();
                float dotProd = QVector2D::dotProduct(vec1,vec2);
                float angle = acos(dotProd/(vec1.length()*vec2.length()))*(180/3.14);
                qDebug()<<"Hit"<<angle;
                float newAngle = 0;
                if(angle > 90){ newAngle =  -((angle-90)*2);}
                else if(angle < 90){ newAngle =(90-angle)*2;}
                newAngle*=(3.14/180);
                QVector2D newVec;
                newVec.setX(vec2.x()*cos(newAngle) - vec2.y()*sin(newAngle));
                newVec.setY(vec2.x()*sin(newAngle) + vec2.y()*cos(newAngle));
                newVec.normalize();
                m_ball->SetMovingVector(newVec);
            }
        }
    }
}
