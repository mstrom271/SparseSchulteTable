#include "language.h"
#include <QApplication>
#include <QSettings>
#include <QWidget>

Language::Language() {
    language = loadSettings();
    setLanguage(language);
}

// notify all widgets to change a language
void Language::notifyAll() {
    QApplication::instance()->installTranslator(&translator);
    const QWidgetList allWidgets = QApplication::allWidgets();
    for (auto widget : allWidgets) {
        QApplication::instance()->postEvent(widget, new LanguageChangeEvent);
    }
}

// load translator for respective lang, update settings
void Language::setLanguage(QString lang) {
    language = correct(lang);

    bool isOk = translator.load(":/rcc/translation_" + language + ".qm");

    QSettings settings;
    settings.setValue("/language", language);
}

QString Language::getLanguage() { return language; }

QString Language::loadSettings() {
    QSettings settings;
    language = settings.value("/language", "").toString();

    // no language in the settings, first launch
    if (language == "") {
        switch (QLocale::system().language()) {
        case QLocale::Russian:
            language = "ru";
            break;
        case QLocale::English:
            language = "en";
            break;
        default:
            language = "en";
            break;
        }
    }
    language = correct(language);
    settings.setValue("/language", language);

    return language;
}

// check if lang is allowed. Return default, if lang is incorrect
QString Language::correct(QString lang) {
    if (lang != "ru" && lang != "en")
        lang = "en";
    return lang;
}
