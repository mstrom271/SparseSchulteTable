#pragma once

#include "settings.h"
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QWidget>

class HelpWidget : public QWidget {
    Q_OBJECT

    Settings *settings;

    QVBoxLayout *vLayout;
    QScrollArea *scrollArea;
    QLabel *helpLabel;
    QPushButton *okBtn;

    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

  public:
    HelpWidget(Settings *newSettings, QWidget *wgt = nullptr);

  signals:
    void ok();
};
