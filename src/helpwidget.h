#pragma once

#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QWidget>

class HelpWidget : public QWidget {
    Q_OBJECT

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
    HelpWidget(QWidget *wgt = nullptr);

  signals:
    void ok();
};
