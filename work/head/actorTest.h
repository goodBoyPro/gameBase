#if !defined(ACTORTEST)
#define ACTORTEST

#include "GActor.h"
#include <random>

class actorTest : public GActor {
  private:
  public:
    struct GSource {
        sf::Texture tex1;
        sf::Texture tex2;
        sf::Texture tex3;
        sf::Texture tex4;
        sf::Texture tex5;
        GSource() { tex1.loadFromFile("res/a.png"); }
        static GSource gs;
    };

    /* data */
  public:
    actorTest(/* args */) {
        delayTaskPtr = &dtp;
        spr.setTexture(GSource::gs.tex1);
        spr.setScale(2, 2);
        spr.setOrigin(24,48);
        posInWs = {0, 0};
        static float x = 0;
        x += 1.f;
        velocity = {cosf(x) * 4.f, sinf(x) * 4.f};

        //actorEvent();
    };
    int isAsyncCanceled = 0;
    int isDead = 0;
    FVector velocity;
    canRun t;
    xlib::Timer20240522::DelayTask *delayTaskPtr = nullptr;
    static xlib::Timer20240522::DelayTask dtp;
    std::mutex mutAsuncTask;
   
    bool flag = 0;
    // 所有事件写在此处
    void createLoop() {
        //std::unique_lock lk(mutAsuncTask);
       
        // if (delayTaskPtr->isTaskValid) {
        //     delayTaskPtr = delay(1, [this]() {
        //         {
        //             //threadSleep(1);
        //             posInWs += velocity;
        //             createLoop();
        //         }
        //     });
        // }
         delayTaskPtr=delay(1, [this]() {
                {
                    //threadSleep(1);
                    posInWs += velocity;
                    createLoop();
                }
            });
    };

    void stopAllAsyncTask() {
        std::unique_lock lk(mutAsuncTask);
        isAsyncCanceled = 1;
        delayTaskPtr->isTaskValid = 0;
    }
    void destroy() {

        stopAllAsyncTask();

        delay(0, [this]() {
            std::unique_lock lk(mutAsuncTask);
            //destroyActor();
        });
    }
    void actorEvent() {
        //delay(rand() % 2000 + 1000, [this]() { destroy(); });
        createLoop();
    }

    virtual void dataLoop() {
        // if (t.delay(2)) {
        //     posInWs += velocity;
        // }
    }
    virtual void drawLoop() {
        GActor::drawLoop();
        // printf("%d\n", actors.size());对速度拖累很明显
        // printNum((int)actors.size());
    }
};

#endif // ACTORTEST
