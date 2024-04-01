#pragma once

#include "keepawakehelper.h"
#include "schultewidget.h"
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QTimer>
#include <QWidget>
#include <random>

class ExerciseWidget : public QWidget {
    Q_OBJECT

    QVBoxLayout *vLayout;
    QLabel *timerLabel;
    QPushButton *cancelBtn;
    SchulteWidget *schulteWgt;
    QPushButton *doneBtn;

    QTimer timer;
    long long startTime;

    KeepAwakeHelper *keepAwakeHelper = nullptr;

    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

  public:
    ExerciseWidget(QWidget *wgt = nullptr);

  public slots:
    void updateTime();
    void pressedNumber(int number);
    void cancelSlot();
    void doneSlot();

  signals:
    void doneSignal();
};
