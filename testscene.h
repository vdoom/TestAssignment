#ifndef TESTSCENE_H
#define TESTSCENE_H
#include "ctScene.h"
#include "ctTimer.h"
//#include <QMediaPlayer>
#include <QVector>
#include <QPoint>

class ctPlane;
class ctLineBorders;
class Ball;

class testScene : public ctScene
{
public:
    enum GameStates
    {
        GS_GAME,
        GS_CREDITS,
        GS_MAINMENU
    };
private:
    float m_frame;
    int m_lastFPS;
    int frameCounter;
    int msecsCounter;
    ctLineBorders * m_lineBorders;
    Ball * m_ball;
    //------------------------------------
    int m_gameState;
    //--------------------------------
    bool m_isInit;

protected:
public:
    testScene();
    explicit testScene(ctShaderManager *);
    explicit testScene(ctShaderManager *, QOpenGLContext *);
    virtual ~testScene();

    virtual void Init();
    virtual void SetDefault(ctShaderManager *, ctScene *, QOpenGLContext *);

    virtual void BeginDraw();
    virtual void Draw();
    virtual void EndDraw();

    virtual void Freeze();
    virtual void Unfreeze();

    virtual void Update();

    void CalcColliedReaction();
};

#endif // TESTSCENE_H
