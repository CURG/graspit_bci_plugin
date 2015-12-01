#ifndef BCISERVICE_H
#define BCISERVICE_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <vector>

#include "BCI/worldController.h"
#include "BCIOnlinePlanner.h"
#include "BCI/controller_scene/controller_scene_mgr.h"
#include "BCI/rosclient.h"

#include <Inventor/nodes/SoOrthographicCamera.h>
#include<Inventor/Qt/SoQtRenderArea.h>

class QImage;
class QString;

class GraspableBody;
class GraspPlanningState;
class BCIControlWindow;
class ControllerSceneManager;


using namespace bci_experiment;

class BCIService:public QObject
{

    Q_OBJECT

public:
    ~BCIService(){}

    BCIOnlinePlanner * currentPlanner;
    ControllerSceneManager * csm;
    QTimer *timer;

    //these are the change state signals used by the bci state machine states
    void emitGoToNextState1(){emit goToNextState1();}
    void emitGoToNextState2(){emit goToNextState2();}
    void emitGoToPreviousState(){emit goToPreviousState();}
    void emitExec(){emit exec();}
    void emitNext(){emit next();}
    void emitRotLat(){emit rotLat();}
    void emitRotLong(){emit rotLong();}

    //these are emitted by the bci emg device
    void emitGoToStateLow(){emit goToStateLow();}
    void emitGoToStateMedium(){emit goToStateMedium();}
    void emitGoToStateHigh(){emit goToStateHigh();}

    //void emitAnalyzeGrasp(const GraspPlanningState * gps) {emit analyzeGrasp(gps); }
    //void emitAnalyzeNextGrasp() {emit analyzeNextGrasp(); }
    void emitAnalyzeApproachDir(GraspPlanningState * gs){emit analyzeApproachDir(gs);}

    //ros server calls
    bool runObjectRetreival(QObject *callbackReceiver, const char *slot);

    bool runObjectRecognition(QObject * callbackReceiver , const char * slot);

    bool getCameraOrigin(QObject * callbackReceiver, const char * slot);

    bool checkGraspReachability(const GraspPlanningState * state,
                                            QObject * callbackReceiver,
                                            const char * slot);


    bool executeGrasp(const GraspPlanningState * gps,
                                  QObject * callbackReceiver,
                                  const char * slot);


    static BCIService* getInstance();

    void init(BCIControlWindow *bciControlWindow);
    SoQtRenderArea *bciRenderArea;

    void setCurrentPlanner(BCIOnlinePlanner *p){currentPlanner = p;}
    BCIOnlinePlanner * getCurrentPlanner(){return currentPlanner;}



public slots:
    //called when active planner is updated
    void onPlannerUpdated(){emit plannerUpdated();}
    void emitProcessWorldPlanner(int i){emit processWorldPlanner(i);}

    void updateControlSceneState0();
    void updateControlSceneState1();
    void updateControlSceneState2();

    void updateControlScene();


signals:

    // state machine transition signals
    void goToNextState1();
    void goToNextState2();
    void goToPreviousState();
    void exec();
    void next();
    void rotLong();
    void rotLat();

    void plannerUpdated();

    void runObjectRecognitionSignal();
    void getCameraOriginSignal();
    void checkGraspReachabilitySignal();

    //! Signal that planner grasps should be processed or sent out for execution
    void processWorldPlanner(int solutionIndex);

    void analyzeApproachDir(GraspPlanningState * gps);

    void goToStateLow();
    void goToStateMedium();
    void goToStateHigh();

private:
        //singleton pattern, single static instance of the class
        static BCIService * bciServiceInstance;

        //this is singleton, so constructor must be private.
        BCIService();

        RosClient *rosClient;
        SoOrthographicCamera * pcam;

        static QMutex createLock;

};

#endif // BCISERVICE_H
