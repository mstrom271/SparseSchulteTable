#pragma once

#include "schultewidget.h"
#include "settings.h"
#include "statswidget.h"
#include <QDialog>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

class SetupWidget : public QWidget {
    Q_OBJECT

    Settings *settings;

    QVBoxLayout *vLayout;
    StatsWidget *statsWgt;
    QHBoxLayout *hLayout1;
    QPushButton *deleteAllStatsBtn, *deleteLastStatsBtn, *settingsBtn, *helpBtn,
        *likeBtn;
#ifdef FREE_VERSION
    QPushButton *proBtn;
#endif
    QHBoxLayout *hLayout2;
    QPushButton *btn_dec;
    QLabel *label;
    QPushButton *btn_inc;
    QSlider *slider;
    SchulteWidget *schulteWgt;
    QPushButton *startBtn;

    QString msgDeleteAllStats;
    QString msgDeleteLastStats;

    void setQSliderStyle();
    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

  public:
    SetupWidget(Settings *newSettings, QWidget *wgt = nullptr);

  public slots:
    void deleteAllStats();
    void deleteLastStats();
    void likeApp();
    void decBtn();
    void incBtn();
    void sliderChanged(int value);
    void numCellsChanged(int newNumCells);

  signals:
    void showSettings();
    void showHelp();
    void startExercise();
    void numCellsChanged();
};

class MessageBox : public QDialog {
    Q_OBJECT

    Settings *settings;

    QVBoxLayout *vLayout;
    QLabel *lbl;
    QHBoxLayout *hLayout;
    QPushButton *okBtn, *cancelBtn;

    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

  public:
    MessageBox(Settings *newSettings, QWidget *wgt = nullptr);
    void setText(QString text);
};
