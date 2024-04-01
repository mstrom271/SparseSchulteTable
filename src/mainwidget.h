#pragma once

#include "exercisewidget.h"
#include "helpwidget.h"
#include "settingswidget.h"
#include "setupwidget.h"
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QStackedLayout>
#include <QWidget>

class MainWidget : public QWidget {
    Q_OBJECT

    QVBoxLayout *vLayout;
    QStackedLayout *stackLayout;
    SetupWidget *setupWgt;
    ExerciseWidget *exerciseWgt;
    HelpWidget *helpWgt;
    SettingsWidget *settingsWgt;

    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

  public:
    MainWidget(QWidget *wgt = nullptr);

  public slots:
    void startExercise();
    void showSetup();
    void showSettings();
    void showHelp();
};
