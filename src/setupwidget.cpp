#include "setupwidget.h"
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QStyle>

SetupWidget::SetupWidget(Settings *newSettings, QWidget *wgt)
    : QWidget(wgt), settings(newSettings) {
    vLayout = new QVBoxLayout;
    statsWgt = new StatsWidget(settings);
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

#ifdef FREE_VERSION
    proBtn = new QPushButton;
    connect(proBtn, SIGNAL(clicked()), SLOT(showPRO()));
    hLayout1->addWidget(proBtn);
#endif
    vLayout->addLayout(hLayout1, 1);

    hLayout2 = new QHBoxLayout;
    btn_dec = new QPushButton;
    connect(btn_dec, SIGNAL(clicked()), SLOT(decBtn()));
    hLayout2->addWidget(btn_dec);

    label = new QLabel;
    label->setFont(settings->getFont3());
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

    schulteWgt = new SchulteWidget(settings, true);
    vLayout->addWidget(schulteWgt, 10);

    startBtn = new QPushButton;
    startBtn->setFont(settings->getFont3());
    connect(startBtn, SIGNAL(clicked()), SIGNAL(startExercise()));
    vLayout->addWidget(startBtn, 1);
    setLayout(vLayout);
}

void SetupWidget::deleteAllStats() {
    settings->getSound().switching();

    MessageBox *msgBox = new MessageBox(settings);
    msgBox->setText(msgDeleteAllStats);
    if (msgBox->exec() == QDialog::DialogCode::Accepted) {
        settings->getStats().saveStats(settings->getNumCells(), QList<int>());
    }
    delete msgBox;

    settings->getSound().switching();
    statsWgt->update();
}

void SetupWidget::deleteLastStats() {
    settings->getSound().switching();

    MessageBox *msgBox = new MessageBox(settings);
    msgBox->setText(msgDeleteLastStats);
    if (msgBox->exec() == QDialog::DialogCode::Accepted) {
        QList<int> stats =
            settings->getStats().getStats(settings->getNumCells());
        if (!stats.empty())
            stats.pop_back();
        settings->getStats().saveStats(settings->getNumCells(), stats);
    }
    delete msgBox;

    settings->getSound().switching();
    statsWgt->update();
}

void SetupWidget::likeApp() {
    settings->getSound().switching();
    // TODO
    // #ifdef FREE_VERSION
    //     QDesktopServices::openUrl(QUrl("https://play.google.com/store/apps/"
    //                                    "details?id=org.tapmyapp.sparseshulte"));
    // #else
    //     QDesktopServices::openUrl(
    //         QUrl("https://play.google.com/store/apps/"
    //              "details?id=org.tapmyapp.sparseschulte_pro"));
    // #endif
}

void SetupWidget::decBtn() {
    settings->getSound().click();

    slider->setValue(slider->value() - 1);
}

void SetupWidget::incBtn() {
    settings->getSound().click();

    slider->setValue(slider->value() + 1);
}

void SetupWidget::sliderChanged(int value) {
    numCellsChanged(settings->getNumCellsRange()[value]);
}

void SetupWidget::numCellsChanged(int newNumCells) {
    settings->setNumCells(newNumCells);

    settings->generateSeedForSchulte();
    schulteWgt->generate(settings->getSeedForSchulte());

    statsWgt->update();

    if (settings->getTableStyle() == TableStyle::SparseTable)
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

    if (settings->getTheme().getTheme() == "DarkTheme") {
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
    } else if (settings->getTheme().getTheme() == "GreyTheme") {
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
    } else if (settings->getTheme().getTheme() == "LightTheme") {
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
    deleteAllStatsBtn->setIcon(QIcon(
        ":/rcc/" + settings->getTheme().getTheme() + "/delete_all_icon.png"));
    deleteLastStatsBtn->setIcon(QIcon(
        ":/rcc/" + settings->getTheme().getTheme() + "/delete_last_icon.png"));
    settingsBtn->setIcon(QIcon(":/rcc/" + settings->getTheme().getTheme() +
                               "/settings_icon.png"));
    helpBtn->setIcon(
        QIcon(":/rcc/" + settings->getTheme().getTheme() + "/help_icon.png"));
    likeBtn->setIcon(
        QIcon(":/rcc/" + settings->getTheme().getTheme() + "/like_icon.png"));
#ifdef FREE_VERSION
    proBtn->setIcon(
        QIcon(":/rcc/" + settings->getTheme().getTheme() + "/pro_icon.png"));
#endif
    btn_dec->setIcon(
        QIcon(":/rcc/" + settings->getTheme().getTheme() + "/left_icon.png"));
    btn_inc->setIcon(
        QIcon(":/rcc/" + settings->getTheme().getTheme() + "/right_icon.png"));

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
             likeBtn,
#ifdef FREE_VERSION
             proBtn,
#endif
             btn_dec, label, btn_inc, slider, startBtn}) {
        wgt->setMinimumHeight(settings->getLogicalDPI() * 0.15);
        wgt->setMaximumHeight(settings->getLogicalDPI() * 0.25);
    }

    setQSliderStyle();

    QWidget::resizeEvent(event);
}

void SetupWidget::showEvent(QShowEvent *event) {
    startBtn->setFocus();

    slider->blockSignals(true);
    slider->setRange(0, settings->getNumCellsRange().size() - 1);
    slider->blockSignals(false);
    for (int i = 0; i < settings->getNumCellsRange().size(); i++) {
        if (settings->getNumCellsRange()[i] == settings->getNumCells()) {
            slider->setValue(3);
            slider->setValue(121);
            slider->setValue(i);
            break;
        }
    }

    settings->generateSeedForSchulte();
    schulteWgt->generate(settings->getSeedForSchulte());

    QWidget::showEvent(event);
}

void SetupWidget::paintEvent(QPaintEvent *event) {
    // icon sizes
    double iconScale = 0.5;
    for (auto btn :
         {deleteAllStatsBtn, deleteLastStatsBtn, settingsBtn, helpBtn, likeBtn,
#ifdef FREE_VERSION
          proBtn,
#endif
          btn_dec, btn_inc}) {
        btn->setIconSize(QSize(btn->size().height() * iconScale,
                               btn->size().height() * iconScale));
    }

    QWidget::paintEvent(event);
}

void MessageBox::onThemeChange() {
    QString style;
    if (settings->getTheme().getTheme() == "DarkTheme") {
        style += "QWidget {background-color: black; color: white;}";
        style += "QPushButton {\
                    border: 1px solid #6f6f71;\
                    border-radius: 10px;\
                    background-color: qlineargradient(x1: 0.5, y1: 1, x2: 0.5, y2: 0,\
                                                      stop: 0 #111111, stop: 1 #222222);\
                 }";
        style += "QDialog{border:1px solid gray}";
    } else if (settings->getTheme().getTheme() == "GreyTheme") {
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
    } else if (settings->getTheme().getTheme() == "LightTheme") {
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

    okBtn->setMinimumHeight(settings->getLogicalDPI() * 0.25);
    okBtn->setMaximumHeight(settings->getLogicalDPI() * 0.30);

    cancelBtn->setMinimumHeight(settings->getLogicalDPI() * 0.25);
    cancelBtn->setMaximumHeight(settings->getLogicalDPI() * 0.30);

    QDialog::resizeEvent(event);
}

MessageBox::MessageBox(Settings *newSettings, QWidget *wgt)
    : QDialog(wgt), settings(newSettings) {
    vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(
        settings->getLogicalDPI() * 0.05, settings->getLogicalDPI() * 0.05,
        settings->getLogicalDPI() * 0.05, settings->getLogicalDPI() * 0.05);
    lbl = new QLabel;
    lbl->setFont(settings->getFont3());
    lbl->setWordWrap(true);
    lbl->setAttribute(Qt::WA_TranslucentBackground);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFontMetrics fm(settings->getFont3());
    lbl->setMinimumHeight(fm.lineSpacing() * 3);
    vLayout->addWidget(lbl);

    vLayout->addSpacing(settings->getLogicalDPI() * 0.20);

    hLayout = new QHBoxLayout;
    okBtn = new QPushButton;
    okBtn->setFont(settings->getFont3());
    connect(okBtn, SIGNAL(clicked()), SLOT(accept()));
    hLayout->addWidget(okBtn);

    cancelBtn = new QPushButton;
    cancelBtn->setFont(settings->getFont3());
    connect(cancelBtn, SIGNAL(clicked()), SLOT(reject()));
    hLayout->addWidget(cancelBtn);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);

    onThemeChange();
    onLanguageChange();
}

void MessageBox::setText(QString text) { lbl->setText(text); }
