#pragma once

#include <QWidget>

class StatsWidget : public QWidget {
    Q_OBJECT

    QString msg_min, msg_max, msg_last, msg_sec, msg_nostat, msg_deletelast,
        msg_deleteall;

    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

  public:
    StatsWidget(QWidget *wgt = nullptr);
};
