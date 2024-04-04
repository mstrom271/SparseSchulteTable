#include "mainwidget.h"
#include "language.h"
#include "settings.h"
#include "sound.h"
#include "theme.h"
#include <QApplication>
#include <QKeyEvent>
#include <QPushButton>
// #ifdef Q_OS_ANDROID
// #include "QtAndroid"
// #endif

MainWidget::MainWidget(QWidget *wgt) : QWidget(wgt) {
    vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);

    stackLayout = new QStackedLayout;
    vLayout->addLayout(stackLayout);
    setupWgt = new SetupWidget;
    connect(setupWgt, SIGNAL(showSettings()), SLOT(showSettings()));
    connect(setupWgt, SIGNAL(showHelp()), SLOT(showHelp()));
    connect(setupWgt, SIGNAL(startExercise()), SLOT(startExercise()));
    stackLayout->addWidget(setupWgt);

    exerciseWgt = new ExerciseWidget;
    connect(exerciseWgt, SIGNAL(doneSignal()), SLOT(showSetup()));
    stackLayout->addWidget(exerciseWgt);

    helpWgt = new HelpWidget;
    connect(helpWgt, SIGNAL(ok()), SLOT(showSetup()));
    stackLayout->addWidget(helpWgt);

    settingsWgt = new SettingsWidget;
    connect(settingsWgt, SIGNAL(ok()), SLOT(showSetup()));
    stackLayout->addWidget(settingsWgt);
    stackLayout->setCurrentWidget(setupWgt);
    setLayout(vLayout);

    stackLayout->setCurrentWidget(setupWgt);
    Theme::applyTheme();
    Language::applyLanguage();

    setObjectName("MainWidget");
}

void MainWidget::startExercise() {
    Sound::switching();

    stackLayout->setCurrentWidget(exerciseWgt);
}

void MainWidget::showSetup() {
    Sound::switching();

    stackLayout->setCurrentWidget(setupWgt);
}

void MainWidget::showSettings() {
    Sound::switching();

    stackLayout->setCurrentWidget(settingsWgt);
}

void MainWidget::showHelp() {
    Sound::switching();

    stackLayout->setCurrentWidget(helpWgt);
}

void MainWidget::onThemeChange() {
    QString style;
    if (Theme::getEffectiveTheme() == "DarkTheme") {
        style += "QWidget {background-color: black; color: white;}";
        style += "QPushButton {\
                    border: 1px solid #6f6f71;\
                    border-radius: 10px;\
                    background-color: qlineargradient(x1: 0.5, y1: 1, x2: 0.5, y2: 0,\
                                                      stop: 0 #111111, stop: 1 #222222);\
                 }";
    } else if (Theme::getEffectiveTheme() == "GreyTheme") {
        style +=
            "QWidget {background-color: qlineargradient(x1: 0.5, y1: 1, x2: 0.5, y2: 0,\
                                                            stop: 0 #777777, stop: 1 #555555);\
                           color: black;}";
        style += "QPushButton {\
                    border: 1px solid #666666;\
                    border-radius: 10px;\
                    background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,\
                                                      stop: 0 #AAAAAA, stop: 1 #888888);\
                 }";
    } else if (Theme::getEffectiveTheme() == "LightTheme") {
        style += "QWidget {background-color: #EEEEEE; color: black;}";
        style += "QPushButton {\
                    border: 1px solid #AAAAAA;\
                    border-radius: 10px;\
                    background-color: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1,\
                                                      stop: 0 #CCCCCC, stop: 1 #DDDDDD);\
                 }";
    }

    setStyleSheet(style);
}

void MainWidget::onLanguageChange() {}

bool MainWidget::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QWidget::event(event);
}

void MainWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Back || event->key() == Qt::Key_Backspace) {
        if (stackLayout->currentWidget() != setupWgt) {
            Sound::switching();
            stackLayout->setCurrentWidget(setupWgt);
        } else
            QApplication::quit();
    }
}

void MainWidget::showEvent(QShowEvent *event) {
    // TODO
    // #ifdef Q_OS_ANDROID
    //     QTimer::singleShot(600, []{QtAndroid::hideSplashScreen();});
    // #endif

    QWidget::showEvent(event);
}
