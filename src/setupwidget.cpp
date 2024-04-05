#include "setupwidget.h"
#include "language.h"
#include "settings.h"
#include "sound.h"
#include "theme.h"
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QStyle>

SetupWidget::SetupWidget(QWidget *wgt) : QWidget(wgt) {
    vLayout = new QVBoxLayout;
    statsWgt = new StatsWidget;
    vLayout->addWidget(statsWgt, 3);

    hLayout1 = new QHBoxLayout;
    deleteAllStatsBtn = new QPushButton;
    connect(deleteAllStatsBtn, SIGNAL(clicked()), SLOT(deleteAllStats()));
    hLayout1->addWidget(deleteAllStatsBtn);

    deleteLastStatsBtn = new QPushButton;
    connect(deleteLastStatsBtn, SIGNAL(clicked()), SLOT(deleteLastStats()));
    hLayout1->addWidget(deleteLastStatsBtn);

    settingsBtn = new QPushButton;
    connect(settingsBtn, SIGNAL(clicked()), SIGNAL(showSettings()));
    hLayout1->addWidget(settingsBtn);

    helpBtn = new QPushButton;
    connect(helpBtn, SIGNAL(clicked()), SIGNAL(showHelp()));
    hLayout1->addWidget(helpBtn);

    likeBtn = new QPushButton;
    connect(likeBtn, SIGNAL(clicked()), SLOT(likeApp()));
    hLayout1->addWidget(likeBtn);
    vLayout->addLayout(hLayout1, 1);

    hLayout2 = new QHBoxLayout;
    btn_dec = new QPushButton;
    connect(btn_dec, SIGNAL(clicked()), SLOT(decBtn()));
    hLayout2->addWidget(btn_dec);

    label = new QLabel;
    label->setFont(Settings::getFont3());
    label->setAlignment(Qt::AlignCenter);
    label->setAttribute(Qt::WA_TranslucentBackground);
    hLayout2->addWidget(label);

    btn_inc = new QPushButton;
    connect(btn_inc, SIGNAL(clicked()), SLOT(incBtn()));
    hLayout2->addWidget(btn_inc);
    vLayout->addLayout(hLayout2, 1);

    slider = new QSlider(Qt::Horizontal);
    connect(slider, SIGNAL(valueChanged(int)), SLOT(sliderChanged(int)));
    vLayout->addWidget(slider);

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spacer->setAttribute(Qt::WA_TranslucentBackground);
    vLayout->addWidget(spacer);

    schulteWgt = new SchulteWidget(true);
    vLayout->addWidget(schulteWgt, 10);

    startBtn = new QPushButton;
    startBtn->setFont(Settings::getFont3());
    connect(startBtn, SIGNAL(clicked()), SIGNAL(startExercise()));
    vLayout->addWidget(startBtn, 1);
    setLayout(vLayout);
}

void SetupWidget::deleteAllStats() {
    Sound::switching();

    MessageBox *msgBox = new MessageBox;
    msgBox->setText(msgDeleteAllStats);
    if (msgBox->exec() == QDialog::DialogCode::Accepted) {
        Settings::removeAllTableStats(Settings::getNumCells());
    }
    delete msgBox;

    Sound::switching();
    statsWgt->update();
}

void SetupWidget::deleteLastStats() {
    Sound::switching();

    MessageBox *msgBox = new MessageBox;
    msgBox->setText(msgDeleteLastStats);
    if (msgBox->exec() == QDialog::DialogCode::Accepted)
        Settings::removeLastTableStats(Settings::getNumCells());
    delete msgBox;

    Sound::switching();
    statsWgt->update();
}

void SetupWidget::likeApp() {
    Sound::switching();
    QDesktopServices::openUrl(QUrl("https://play.google.com/store/apps/"
                                   "details?id=org.tapmyapp.sparseshulte"));
}

void SetupWidget::decBtn() {
    Sound::click();

    slider->setValue(slider->value() - 1);
}

void SetupWidget::incBtn() {
    Sound::click();

    slider->setValue(slider->value() + 1);
}

void SetupWidget::sliderChanged(int value) {
    numCellsChanged(Settings::getNumCellsRangeCache()[value]);
}

void SetupWidget::numCellsChanged(int newNumCells) {
    Settings::setNumCells(newNumCells);

    Settings::generateSeedForSchulte();
    schulteWgt->generate(Settings::getSeedForSchulte());

    statsWgt->update();

    if (Settings::getTableStyle() == TableStyleT::SparseTable)
        label->setText(QString::number(newNumCells));
    else
        label->setText(QString::number(schulteWgt->getQuad_side()) + "x" +
                       QString::number(schulteWgt->getQuad_side()));

    emit numCellsChanged();
}

void SetupWidget::setQSliderStyle() {
    QString styleQSlider;
    QString slider_fullwidth =
        QString::number(static_cast<int>(slider->height() * 0.8));
    QString slider_halfwidth =
        QString::number(static_cast<int>(slider->height() * 0.4));
    QString slider_quartwidth =
        QString::number(static_cast<int>(slider->height() * 0.225));

    if (Theme::getEffectiveTheme() == "DarkTheme") {
        styleQSlider = "\
            QSlider::groove:horizontal {\
                border: 1px solid #262626;\
                height: 5px;\
                background: #393939;\
                margin: 0 " +
                       slider_quartwidth + "px;\
            }\
            QSlider::handle:horizontal {\
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);;\
                border: 1px solid #8f8f8f;\
                border-radius: " +
                       slider_halfwidth + "px;\
                width: " +
                       slider_fullwidth + "px;\
                margin: -" +
                       slider_halfwidth + "px -" + slider_quartwidth + "px;\
            }";
    } else if (Theme::getEffectiveTheme() == "GreyTheme") {
        styleQSlider = "QSlider{background: transparent;}\
            QSlider::groove:horizontal {\
                border: 1px solid #262626;\
                height: 5px;\
                background: #393939;\
                margin: 0 " +
                       slider_quartwidth + "px;\
            }\
            QSlider::handle:horizontal {\
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #BBBBBB, stop:1 #999999);\
                border: 1px solid #666666;\
                border-radius: " +
                       slider_halfwidth + "px;\
                width: " +
                       slider_fullwidth + "px;\
                margin: -" +
                       slider_halfwidth + "px -" + slider_quartwidth + "px;\
            }";
    } else if (Theme::getEffectiveTheme() == "LightTheme") {
        styleQSlider = "\
            QSlider::groove:horizontal {\
                border: 1px solid #262626;\
                height: 5px;\
                background: #393939;\
                margin: 0 " +
                       slider_quartwidth + "px;\
            }\
            QSlider::handle:horizontal {\
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f6f7fa, stop:1 #dadbde);;\
                border: 1px solid #8f8f8f;\
                border-radius: " +
                       slider_halfwidth + "px;\
                width: " +
                       slider_fullwidth + "px;\
                margin: -" +
                       slider_halfwidth + "px -" + slider_quartwidth + "px;\
        }";
    }

    slider->setStyleSheet(styleQSlider);
}

void SetupWidget::onThemeChange() {
    deleteAllStatsBtn->setIcon(
        QIcon(":/rcc/" + Theme::getEffectiveTheme() + "/delete_all_icon.png"));
    deleteLastStatsBtn->setIcon(
        QIcon(":/rcc/" + Theme::getEffectiveTheme() + "/delete_last_icon.png"));
    settingsBtn->setIcon(
        QIcon(":/rcc/" + Theme::getEffectiveTheme() + "/settings_icon.png"));
    helpBtn->setIcon(
        QIcon(":/rcc/" + Theme::getEffectiveTheme() + "/help_icon.png"));
    likeBtn->setIcon(
        QIcon(":/rcc/" + Theme::getEffectiveTheme() + "/like_icon.png"));
    btn_dec->setIcon(
        QIcon(":/rcc/" + Theme::getEffectiveTheme() + "/left_icon.png"));
    btn_inc->setIcon(
        QIcon(":/rcc/" + Theme::getEffectiveTheme() + "/right_icon.png"));

    setQSliderStyle();
}

void SetupWidget::onLanguageChange() {
    startBtn->setText(tr("Start"));
    msgDeleteAllStats = tr("Delete all records for this table?");
    msgDeleteLastStats = tr("Delete last record from statistics?");
}

bool SetupWidget::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QWidget::event(event);
}

void SetupWidget::resizeEvent(QResizeEvent *event) {
    // Min and Max sizes of widgets
    for (auto wgt : std::initializer_list<QWidget *>{
             deleteAllStatsBtn, deleteLastStatsBtn, settingsBtn, helpBtn,
             likeBtn, btn_dec, label, btn_inc, slider, startBtn}) {
        wgt->setMinimumHeight(Settings::getLogicalDPI() * 0.15);
        wgt->setMaximumHeight(Settings::getLogicalDPI() * 0.25);
    }

    setQSliderStyle();

    QWidget::resizeEvent(event);
}

void SetupWidget::showEvent(QShowEvent *event) {
    startBtn->setFocus();

    slider->blockSignals(true);
    slider->setRange(0, Settings::getNumCellsRangeCache().size() - 1);
    slider->blockSignals(false);
    for (int i = 0; i < Settings::getNumCellsRangeCache().size(); i++) {
        if (Settings::getNumCellsRangeCache()[i] == Settings::getNumCells()) {
            slider->setValue(3);
            slider->setValue(121);
            slider->setValue(i);
            break;
        }
    }

    Settings::generateSeedForSchulte();
    schulteWgt->generate(Settings::getSeedForSchulte());

    QWidget::showEvent(event);
}

void SetupWidget::paintEvent(QPaintEvent *event) {
    // icon sizes
    double iconScale = 0.5;
    for (auto btn : {deleteAllStatsBtn, deleteLastStatsBtn, settingsBtn,
                     helpBtn, likeBtn, btn_dec, btn_inc}) {
        btn->setIconSize(QSize(btn->size().height() * iconScale,
                               btn->size().height() * iconScale));
    }

    QWidget::paintEvent(event);
}

void MessageBox::onThemeChange() {
    QString style;
    if (Theme::getEffectiveTheme() == "DarkTheme") {
        style += "QWidget {background-color: black; color: white;}";
        style += "QPushButton {\
                    border: 1px solid #6f6f71;\
                    border-radius: 10px;\
                    background-color: qlineargradient(x1: 0.5, y1: 1, x2: 0.5, y2: 0,\
                                                      stop: 0 #111111, stop: 1 #222222);\
                 }";
        style += "QDialog{border:1px solid gray}";
    } else if (Theme::getEffectiveTheme() == "GreyTheme") {
        style +=
            "QWidget {background-color: qlineargradient(x1: 0.5, y1: 1, x2: 0.5, y2: 0,\
                                                            stop: 0 #999999, stop: 1 #777777);\
                           color: black;}";
        style += "QPushButton {\
                    border: 1px solid #666666;\
                    border-radius: 10px;\
                    background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,\
                                                      stop: 0 #AAAAAA, stop: 1 #BBBBBB);\
                 }";
        style += "QDialog{border:1px solid black}";
    } else if (Theme::getEffectiveTheme() == "LightTheme") {
        style += "QWidget {background-color: #EEEEEE; color: black;}";
        style += "QPushButton {\
                    border: 1px solid #AAAAAA;\
                    border-radius: 10px;\
                    background-color: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1,\
                                                      stop: 0 #CCCCCC, stop: 1 #DDDDDD);\
                 }";
        style += "QDialog{border:1px solid gray}";
    }

    setStyleSheet(style);
}

void MessageBox::onLanguageChange() {
    okBtn->setText(tr("Ok"));
    cancelBtn->setText(tr("Cancel"));
}

bool MessageBox::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QDialog::event(event);
}

void MessageBox::resizeEvent(QResizeEvent *event) {
#ifdef Q_OS_ANDROID
    setMinimumWidth(QApplication::primaryScreen()->size().width() * 0.8);
#endif

    okBtn->setMinimumHeight(Settings::getLogicalDPI() * 0.25);
    okBtn->setMaximumHeight(Settings::getLogicalDPI() * 0.30);

    cancelBtn->setMinimumHeight(Settings::getLogicalDPI() * 0.25);
    cancelBtn->setMaximumHeight(Settings::getLogicalDPI() * 0.30);

    QDialog::resizeEvent(event);
}

MessageBox::MessageBox(QWidget *wgt) : QDialog(wgt) {
    vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(
        Settings::getLogicalDPI() * 0.05, Settings::getLogicalDPI() * 0.05,
        Settings::getLogicalDPI() * 0.05, Settings::getLogicalDPI() * 0.05);
    lbl = new QLabel;
    lbl->setFont(Settings::getFont3());
    lbl->setWordWrap(true);
    lbl->setAttribute(Qt::WA_TranslucentBackground);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFontMetrics fm(Settings::getFont3());
    lbl->setMinimumHeight(fm.lineSpacing() * 3);
    vLayout->addWidget(lbl);

    vLayout->addSpacing(Settings::getLogicalDPI() * 0.20);

    hLayout = new QHBoxLayout;
    okBtn = new QPushButton;
    okBtn->setFont(Settings::getFont3());
    connect(okBtn, SIGNAL(clicked()), SLOT(accept()));
    hLayout->addWidget(okBtn);

    cancelBtn = new QPushButton;
    cancelBtn->setFont(Settings::getFont3());
    connect(cancelBtn, SIGNAL(clicked()), SLOT(reject()));
    hLayout->addWidget(cancelBtn);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);

    onThemeChange();
    onLanguageChange();
}

void MessageBox::setText(QString text) { lbl->setText(text); }
