#include "mainwidget.h"
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
    setupWgt = new SetupWidget(&settings);
    connect(setupWgt, SIGNAL(showSettings()), SLOT(showSettings()));
    connect(setupWgt, SIGNAL(showHelp()), SLOT(showHelp()));
    connect(setupWgt, SIGNAL(startExercise()), SLOT(startExercise()));
    stackLayout->addWidget(setupWgt);

    exerciseWgt = new ExerciseWidget(&settings);
    connect(exerciseWgt, SIGNAL(doneSignal()), SLOT(showSetup()));
    stackLayout->addWidget(exerciseWgt);

    helpWgt = new HelpWidget(&settings);
    connect(helpWgt, SIGNAL(ok()), SLOT(showSetup()));
    stackLayout->addWidget(helpWgt);

    settingsWgt = new SettingsWidget(&settings);
    connect(settingsWgt, SIGNAL(ok()), SLOT(showSetup()));
    stackLayout->addWidget(settingsWgt);
    stackLayout->setCurrentWidget(setupWgt);
    setLayout(vLayout);

    stackLayout->setCurrentWidget(setupWgt);
    settings.getTheme().notifyAll();
    settings.getLanguage().notifyAll();

    setObjectName("MainWidget");
}

void MainWidget::startExercise() {
    settings.getSound().switching();

    stackLayout->setCurrentWidget(exerciseWgt);
}

void MainWidget::showSetup() {
    settings.getSound().switching();

    stackLayout->setCurrentWidget(setupWgt);
}

void MainWidget::showSettings() {
    settings.getSound().switching();

    stackLayout->setCurrentWidget(settingsWgt);
}

void MainWidget::showHelp() {
    settings.getSound().switching();

    stackLayout->setCurrentWidget(helpWgt);
}

void MainWidget::onThemeChange() {
    QString style;
    if (settings.getTheme().getTheme() == "DarkTheme") {
        style += "QWidget {background-color: black; color: white;}";
        style += "QPushButton {\
                    border: 1px solid #6f6f71;\
                    border-radius: 10px;\
                    background-color: qlineargradient(x1: 0.5, y1: 1, x2: 0.5, y2: 0,\
                                                      stop: 0 #111111, stop: 1 #222222);\
                 }";
    } else if (settings.getTheme().getTheme() == "GreyTheme") {
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
    } else if (settings.getTheme().getTheme() == "LightTheme") {
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
#ifdef QT_QML_DEBUG
    if (event->key() == Qt::Key_L) {
        if (settings.getLanguage().getLanguage() == "en")
            settings.getLanguage().setLanguage("ru");
        else
            settings.getLanguage().setLanguage("en");
        settings.getLanguage().notifyAll();
    } else if (event->key() == Qt::Key_T) {
        if (settings.getTheme().getTheme() == "DarkTheme")
            settings.getTheme().setTheme("GreyTheme");
        else if (settings.getTheme().getTheme() == "GreyTheme")
            settings.getTheme().setTheme("LightTheme");
        else
            settings.getTheme().setTheme("DarkTheme");
        settings.getTheme().notifyAll();
    } else if (event->key() == Qt::Key_1)
        stackLayout->setCurrentWidget(setupWgt);
    else if (event->key() == Qt::Key_2)
        stackLayout->setCurrentWidget(exerciseWgt);
    else if (event->key() == Qt::Key_3)
        stackLayout->setCurrentWidget(settingsWgt);
    else if (event->key() == Qt::Key_4)
        stackLayout->setCurrentWidget(helpWgt);
#endif

    if (event->key() == Qt::Key_Back || event->key() == Qt::Key_Backspace) {
        if (stackLayout->currentWidget() != setupWgt) {
            settings.getSound().switching();
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
