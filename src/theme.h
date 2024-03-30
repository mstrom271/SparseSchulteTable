#pragma once

#include <QEvent>
#include <QString>
#include <QStringList>

class ThemeChangeEvent : public QEvent {
  public:
    static const QEvent::Type type =
        static_cast<QEvent::Type>(QEvent::User + 200);
    ThemeChangeEvent() : QEvent(static_cast<QEvent::Type>(type)) {}
};

class Theme {
    QString currentTheme;
    QStringList list{"DarkTheme", "GreyTheme", "LightTheme"};

  public:
    Theme();
    void setTheme(QString newThemeName);
    QString getTheme();
    QStringList getList();
    void notifyAll();
};
