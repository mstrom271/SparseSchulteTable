#pragma once

#ifdef __ANDROID__
// glitch workaround. Should be:
// #ifdef Q_OS_ANDROID
#include <QJniObject>
#endif

class KeepAwakeHelper {
  public:
    KeepAwakeHelper();
    virtual ~KeepAwakeHelper();

  private:
#ifdef Q_OS_ANDROID
    QJniObject m_wakeLock;
#endif
};
