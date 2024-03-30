#pragma once

#include <QEvent>
#include <QTranslator>

class LanguageChangeEvent : public QEvent {
  public:
    static const QEvent::Type type =
        static_cast<QEvent::Type>(QEvent::User + 201);
    LanguageChangeEvent() : QEvent(static_cast<QEvent::Type>(type)) {}
};

class Language {
    QTranslator translator;
    QString language;

  public:
    Language();
    void setLanguage(QString newLanguage);
    QString getLanguage();
    void notifyAll();

  private:
    QString loadSettings();
    QString correct(QString language);
};
