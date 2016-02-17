#include "androidutils.h"

#include <deque>
#include <memory>
#include <mutex>

#include <QtAndroid>
#include <QSemaphore>
#include <QAndroidJniEnvironment>

namespace KDAB {
namespace Android {

static std::deque<Runnable> s_pendingRunnables;
static std::mutex s_pendingRunnablesMutex;

void runOnAndroidThread(const Runnable &runnable)
{
    s_pendingRunnablesMutex.lock();
    bool triggerRun = s_pendingRunnables.empty();
    s_pendingRunnables.push_back(runnable);
    s_pendingRunnablesMutex.unlock();
    if (triggerRun) {
        QtAndroid::androidActivity().callMethod<void>("runOnUiThread",
                                                      "(Ljava/lang/Runnable;)V",
                                                      QAndroidJniObject("com/kdab/android/utils/Runnable").object());
    }
}

void runOnAndroidThreadSync(const Runnable &runnable, int waitMs)
{
    std::shared_ptr<QSemaphore> sem = std::make_shared<QSemaphore>();
    runOnAndroidThread([sem, &runnable](){
        runnable();
        sem->release();
    });
    sem->tryAcquire(1, waitMs);
}

extern "C" JNIEXPORT void JNICALL Java_com_kdab_android_utils_Runnable_runPendingCppRunnables(JNIEnv */*env*/, jobject /*obj*/)
{
    for (;;) { // run all posted runnables
        s_pendingRunnablesMutex.lock();
        if (s_pendingRunnables.empty()) {
            s_pendingRunnablesMutex.unlock();
            break;
        }
        Runnable runnable(std::move(s_pendingRunnables.front()));
        s_pendingRunnables.pop_front();
        s_pendingRunnablesMutex.unlock();
        runnable();
    }
}

} // namespace Android
} // KDAB
