#ifndef ANDROIDUTILS_H
#define ANDROIDUTILS_H

#include <functional>

namespace KDAB {
namespace Android {

typedef std::function<void()> Runnable;

/// Posts a runnable on Android thread, then exists.
/// If you call runOnAndroidThread from Android UI thread,
/// it will execute the runnable immediately
void runOnAndroidThread(const Runnable &runnable);

/// Posts a runnable on Android thread then waits until it's executed.
void runOnAndroidThreadSync(const Runnable &runnable, int waitMs = INT_MAX);

} // namespace Android
} // KDAB

#endif // ANDROIDUTILS_H
