#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QWidget>

class SettingsWidget : public QWidget {
    Q_OBJECT

    QVBoxLayout *vLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QGridLayout *gridLayout;
    QLabel *lbl_caption;

    QLabel *lbl_language;
    QLabel *lbl_language_detailed;
    QComboBox *cmb_language;

    QLabel *lbl_theme;
    QLabel *lbl_theme_detailed;
    QComboBox *cmb_theme;

    QLabel *lbl_clickSound;
    QLabel *lbl_clickSound_detailed;
    QCheckBox *ckb_clickSound;

    QLabel *lbl_tableStyle;
    QLabel *lbl_tableStyle_detailed;
    QComboBox *cmb_tableStyle;

    QLabel *lbl_centralPointStyle;
    QLabel *lbl_centralPointStyle_detailed;
    QComboBox *cmb_centralPointStyle;

    QLabel *lbl_fontMaxSize;
    QLabel *lbl_fontMaxSize_detailed;
    QComboBox *cmb_fontMaxSize;

    QLabel *lbl_tableScale;
    QLabel *lbl_tableScale_detailed;
    QComboBox *cmb_tableScale;

    QLabel *lbl_showTimer;
    QLabel *lbl_showTimer_detailed;
    QCheckBox *ckb_showTimer;

    QLabel *lbl_keepAwake;
    QLabel *lbl_keepAwake_detailed;
    QCheckBox *ckb_keepAwake;
    QPushButton *okBtn;

    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

  public:
    SettingsWidget(QWidget *wgt = nullptr);

  public slots:
    void languageChange(QString s);
    void themeChange(QString s);
    void clickSoundChange(int state);
    void tableStyleChange(QString s);
    void centralPointStyleChange(QString s);
    void fontMaxSizeChange(QString s);
    void tableScaleChange(QString s);
    void showTimerChange(int state);
    void keepAwakeChange(int state);

  signals:
    void ok();
};
