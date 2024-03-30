
#include "keepawakehelper.h"
#include <QApplication>
#include <QDebug>

KeepAwakeHelper::KeepAwakeHelper() {
#ifdef Q_OS_ANDROID
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    if (activity.isValid()) {
        QJniObject serviceName = QJniObject::getStaticObjectField<jstring>(
            "android/content/Context", "POWER_SERVICE");
        if (serviceName.isValid()) {
            QJniObject powerMgr = activity.callObjectMethod(
                "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;",
                serviceName.object<jobject>());
            if (powerMgr.isValid()) {
                jint levelAndFlags = QJniObject::getStaticField<jint>(
                    "android/os/PowerManager", "FULL_WAKE_LOCK");
                QJniObject tag = QJniObject::fromString("My Tag");
                m_wakeLock = powerMgr.callObjectMethod(
                    "newWakeLock",
                    "(ILjava/lang/String;)Landroid/os/"
                    "PowerManager$WakeLock;",
                    levelAndFlags, tag.object<jstring>());
            }
        }
    }

    if (m_wakeLock.isValid()) {
        m_wakeLock.callMethod<void>("acquire", "()V");
        qDebug() << "Locked device, can't go to standby anymore";
    } else {
        Q_ASSERT(false);
    }
#endif
}

KeepAwakeHelper::~KeepAwakeHelper() {
#ifdef Q_OS_ANDROID
    if (m_wakeLock.isValid()) {
        m_wakeLock.callMethod<void>("release", "()V");
        qDebug() << "Unlocked device, can now go to standby";
    }
#endif
}
