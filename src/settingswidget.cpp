#include "settingswidget.h"
#include "language.h"
#include "serialize.h"
#include "settings.h"
#include "theme.h"
#include <QAbstractItemView>
#include <QLayout>
#include <QPushButton>
#include <QScrollBar>
#include <QScroller>
#include <QStyledItemDelegate>

SettingsWidget::SettingsWidget(QWidget *wgt) : QWidget(wgt) {
    vLayout = new QVBoxLayout;
    scrollArea = new QScrollArea;
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAttribute(Qt::WA_TranslucentBackground);
#ifdef Q_OS_ANDROID
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScroller::grabGesture(scrollArea->viewport(),
                           QScroller::LeftMouseButtonGesture);
#endif
    scrollWidget = new QWidget;
    scrollWidget->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout = new QGridLayout;
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setContentsMargins(
        Settings::getLogicalDPI() * 0.1, Settings::getLogicalDPI() * 0.1,
        Settings::getLogicalDPI() * 0.1, Settings::getLogicalDPI() * 0.1);
    int level = 0;
    lbl_caption = new QLabel;
    lbl_caption->setWordWrap(true);
    lbl_caption->setAttribute(Qt::WA_TranslucentBackground);
    lbl_caption->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    gridLayout->addWidget(lbl_caption, level, 0, 1, 2);
    level += 1;

    lbl_language = new QLabel;
    lbl_language->setWordWrap(true);
    lbl_language->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_language, level, 0,
                          Qt::AlignBottom | Qt::AlignLeft);
    lbl_language_detailed = new QLabel;
    lbl_language_detailed->setWordWrap(true);
    lbl_language_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_language_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    cmb_language = new QComboBox;
    cmb_language->setFocusPolicy(Qt::NoFocus);
    cmb_language->setMinimumContentsLength(10);
    cmb_language->setItemDelegate(
        new QStyledItemDelegate()); // to force stylesheets to work
    QObject::connect(cmb_language, SIGNAL(textActivated(QString)),
                     SLOT(languageChange(QString)));
    cmb_language->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(cmb_language, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_theme = new QLabel;
    lbl_theme->setWordWrap(true);
    lbl_theme->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_theme, level, 0, Qt::AlignBottom | Qt::AlignLeft);
    lbl_theme_detailed = new QLabel;
    lbl_theme_detailed->setWordWrap(true);
    lbl_theme_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_theme_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    cmb_theme = new QComboBox;
    cmb_theme->setFocusPolicy(Qt::NoFocus);
    cmb_theme->setMinimumContentsLength(10);
    cmb_theme->setItemDelegate(
        new QStyledItemDelegate()); // to force stylesheets to work
    QObject::connect(cmb_theme, SIGNAL(textActivated(QString)),
                     SLOT(themeChange(QString)));
    cmb_theme->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(cmb_theme, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_soundState = new QLabel;
    lbl_soundState->setWordWrap(true);
    lbl_soundState->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_soundState, level, 0,
                          Qt::AlignBottom | Qt::AlignLeft);
    lbl_soundState_detailed = new QLabel;
    lbl_soundState_detailed->setWordWrap(true);
    lbl_soundState_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_soundState_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    ckb_soundState = new QCheckBox;
    ckb_soundState->setFocusPolicy(Qt::NoFocus);
    ckb_soundState->setCheckState(Settings::getSoundState()
                                      ? Qt::CheckState::Checked
                                      : Qt::CheckState::Unchecked);
    ckb_soundState->setAttribute(Qt::WA_TranslucentBackground);
    QObject::connect(ckb_soundState, SIGNAL(stateChanged(int)),
                     SLOT(soundStateChange(int)));
    gridLayout->addWidget(ckb_soundState, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_tableStyle = new QLabel;
    lbl_tableStyle->setWordWrap(true);
    lbl_tableStyle->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_tableStyle, level, 0,
                          Qt::AlignBottom | Qt::AlignLeft);
    lbl_tableStyle_detailed = new QLabel;
    lbl_tableStyle_detailed->setWordWrap(true);
    lbl_tableStyle_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_tableStyle_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    cmb_tableStyle = new QComboBox;
    cmb_tableStyle->setFocusPolicy(Qt::NoFocus);
    cmb_tableStyle->setMinimumContentsLength(10);
    cmb_tableStyle->setItemDelegate(
        new QStyledItemDelegate()); // to force stylesheets to work
    QObject::connect(cmb_tableStyle, SIGNAL(textActivated(QString)),
                     SLOT(tableStyleChange(QString)));
    cmb_tableStyle->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(cmb_tableStyle, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_centralPointStyle = new QLabel;
    lbl_centralPointStyle->setWordWrap(true);
    lbl_centralPointStyle->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_centralPointStyle, level, 0,
                          Qt::AlignBottom | Qt::AlignLeft);
    lbl_centralPointStyle_detailed = new QLabel;
    lbl_centralPointStyle_detailed->setWordWrap(true);
    lbl_centralPointStyle_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_centralPointStyle_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    cmb_centralPointStyle = new QComboBox;
    cmb_centralPointStyle->setFocusPolicy(Qt::NoFocus);
    cmb_centralPointStyle->setMinimumContentsLength(10);
    cmb_centralPointStyle->setItemDelegate(
        new QStyledItemDelegate()); // to force stylesheets to work
    QObject::connect(cmb_centralPointStyle, SIGNAL(textActivated(QString)),
                     SLOT(centralPointStyleChange(QString)));
    cmb_centralPointStyle->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(cmb_centralPointStyle, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_fontMaxSize = new QLabel;
    lbl_fontMaxSize->setWordWrap(true);
    lbl_fontMaxSize->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_fontMaxSize, level, 0,
                          Qt::AlignBottom | Qt::AlignLeft);
    lbl_fontMaxSize_detailed = new QLabel;
    lbl_fontMaxSize_detailed->setWordWrap(true);
    lbl_fontMaxSize_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_fontMaxSize_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    cmb_fontMaxSize = new QComboBox;
    cmb_fontMaxSize->setFocusPolicy(Qt::NoFocus);
    for (int i = 0; i < 200; i += 10)
        cmb_fontMaxSize->addItem(QString::number(i));
    cmb_fontMaxSize->setCurrentText(
        QString::number(Settings::getFontMaxSize()));
    cmb_fontMaxSize->setMinimumContentsLength(10);
    cmb_fontMaxSize->setItemDelegate(
        new QStyledItemDelegate()); // to force stylesheets to work
    QObject::connect(cmb_fontMaxSize, SIGNAL(textActivated(QString)),
                     SLOT(fontMaxSizeChange(QString)));
    cmb_fontMaxSize->setAttribute(Qt::WA_TranslucentBackground);
    cmb_fontMaxSize->setMaxVisibleItems(5);
    QScroller::grabGesture(cmb_fontMaxSize->view()->viewport(),
                           QScroller::LeftMouseButtonGesture);
    cmb_fontMaxSize->view()->setVerticalScrollMode(
        QAbstractItemView::ScrollPerPixel);
    gridLayout->addWidget(cmb_fontMaxSize, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_tableScale = new QLabel;
    lbl_tableScale->setWordWrap(true);
    lbl_tableScale->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_tableScale, level, 0,
                          Qt::AlignBottom | Qt::AlignLeft);
    lbl_tableScale_detailed = new QLabel;
    lbl_tableScale_detailed->setWordWrap(true);
    lbl_tableScale_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_tableScale_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    cmb_tableScale = new QComboBox;
    cmb_tableScale->setFocusPolicy(Qt::NoFocus);
    for (int i = 10; i <= 100; i += 10)
        cmb_tableScale->addItem(QString::number(i) + "%");
    cmb_tableScale->setCurrentText(QString::number(Settings::getTableScale()) +
                                   "%");
    cmb_tableScale->setMinimumContentsLength(10);
    cmb_tableScale->setItemDelegate(
        new QStyledItemDelegate()); // to force stylesheets to work
    QObject::connect(cmb_tableScale, SIGNAL(textActivated(QString)),
                     SLOT(tableScaleChange(QString)));
    cmb_tableScale->setAttribute(Qt::WA_TranslucentBackground);
    cmb_tableScale->setMaxVisibleItems(5);
    QScroller::grabGesture(cmb_tableScale->view()->viewport(),
                           QScroller::LeftMouseButtonGesture);
    cmb_tableScale->view()->setVerticalScrollMode(
        QAbstractItemView::ScrollPerPixel);
    gridLayout->addWidget(cmb_tableScale, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_showTimer = new QLabel;
    lbl_showTimer->setWordWrap(true);
    lbl_showTimer->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_showTimer, level, 0,
                          Qt::AlignBottom | Qt::AlignLeft);
    lbl_showTimer_detailed = new QLabel;
    lbl_showTimer_detailed->setWordWrap(true);
    lbl_showTimer_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_showTimer_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    ckb_showTimer = new QCheckBox;
    ckb_showTimer->setFocusPolicy(Qt::NoFocus);
    ckb_showTimer->setCheckState(Settings::getShowTimer()
                                     ? Qt::CheckState::Checked
                                     : Qt::CheckState::Unchecked);
    ckb_showTimer->setAttribute(Qt::WA_TranslucentBackground);
    QObject::connect(ckb_showTimer, SIGNAL(stateChanged(int)),
                     SLOT(showTimerChange(int)));
    gridLayout->addWidget(ckb_showTimer, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;

    lbl_keepAwake = new QLabel;
    lbl_keepAwake->setWordWrap(true);
    lbl_keepAwake->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_keepAwake, level, 0,
                          Qt::AlignBottom | Qt::AlignLeft);
    lbl_keepAwake_detailed = new QLabel;
    lbl_keepAwake_detailed->setWordWrap(true);
    lbl_keepAwake_detailed->setAttribute(Qt::WA_TranslucentBackground);
    gridLayout->addWidget(lbl_keepAwake_detailed, level + 1, 0,
                          Qt::AlignTop | Qt::AlignLeft);

    ckb_keepAwake = new QCheckBox;
    ckb_keepAwake->setFocusPolicy(Qt::NoFocus);
    ckb_keepAwake->setCheckState(Settings::getKeepAwake()
                                     ? Qt::CheckState::Checked
                                     : Qt::CheckState::Unchecked);
    ckb_keepAwake->setAttribute(Qt::WA_TranslucentBackground);
    QObject::connect(ckb_keepAwake, SIGNAL(stateChanged(int)),
                     SLOT(keepAwakeChange(int)));
    gridLayout->addWidget(ckb_keepAwake, level, 1, 2, 1,
                          Qt::AlignVCenter | Qt::AlignLeft);
    level += 2;
    scrollWidget->setLayout(gridLayout);
    scrollWidget->setMinimumHeight(Settings::getLogicalDPI() * 0.5 * level);
    scrollArea->setWidget(scrollWidget);
    vLayout->addWidget(scrollArea, 17);

    okBtn = new QPushButton;
    okBtn->setFont(Settings::getFont3());
    connect(okBtn, SIGNAL(clicked()), SIGNAL(ok()));
    vLayout->addWidget(okBtn, 1);
    setLayout(vLayout);
}

void SettingsWidget::onThemeChange() {
#ifdef Q_OS_ANDROID
    QColor bgColor;
    QColor inversebgColor;
    if (Theme::getEffectiveTheme() == "DarkTheme") {
        bgColor = Qt::black;
        inversebgColor = Qt::white;
    } else if (Theme::getEffectiveTheme() == "GreyTheme") {
        bgColor = QColor("#777777");
        inversebgColor = Qt::black;
    } else {
        bgColor = QColor("#EEEEEE");
        inversebgColor = Qt::black;
    }

    QString ckbStyle =
        "QCheckBox {background-color: transparent; border: 1px solid " +
        inversebgColor.name() +
        "; }"
        "QCheckBox::indicator {\
                                width: " +
        QString::number(static_cast<int>(Settings::getLogicalDPI() * 0.15)) +
        "px;\
                                height: " +
        QString::number(static_cast<int>(Settings::getLogicalDPI() * 0.15)) +
        "px;\
                           }";
    ckb_soundState->setStyleSheet(ckbStyle);
    ckb_showTimer->setStyleSheet(ckbStyle);
    ckb_keepAwake->setStyleSheet(ckbStyle);

    QFontMetrics fm(Settings::getFont3());
    QString cmbStyle =
        "QComboBox QAbstractItemView::item { min-height: " +
        QString::number(fm.lineSpacing() * 3) + "px;}" +
        "QComboBox {background-color: " + bgColor.name() + ";}" +
        "QComboBox {combobox-popup: 0}" + // setMaxVisibleItems don't work
                                          // without this
        "QWidget {border: 1px solid " + inversebgColor.name() + ";}" +
        "QWidget:item:selected {color: " + inversebgColor.name() +
        "; background-color: " + bgColor.name() + ";}";
    cmb_language->setStyleSheet(cmbStyle);
    cmb_theme->setStyleSheet(cmbStyle);
    cmb_tableStyle->setStyleSheet(cmbStyle);
    cmb_centralPointStyle->setStyleSheet(cmbStyle);
    cmb_fontMaxSize->setStyleSheet(cmbStyle);
    cmb_tableScale->setStyleSheet(cmbStyle);
#endif

    cmb_language->clear();
    for (auto &lang : Language::getLanguageList())
        cmb_language->addItem(QIcon(":/rcc/" + Theme::getEffectiveTheme() +
                                    "/" + lang + "_icon.png"),
                              lang);
    cmb_language->setCurrentText(Settings::getLanguage());

    cmb_theme->clear();
    for (auto &theme : Theme::getThemeList())
        cmb_theme->addItem(QIcon(":/rcc/" + Theme::getEffectiveTheme() + "/" +
                                 theme + "_icon.png"),
                           theme);
    cmb_theme->setCurrentText(Settings::getTheme());

    cmb_tableStyle->clear();
    for (auto &tableType : {serializeTableStyle(TableStyleT::ClassicTable),
                            serializeTableStyle(TableStyleT::SparseTable)})
        cmb_tableStyle->addItem(QIcon(":/rcc/" + Theme::getEffectiveTheme() +
                                      "/" + tableType + "_icon.png"),
                                tableType);
    cmb_tableStyle->setCurrentText(
        serializeTableStyle(Settings::getTableStyle()));

    cmb_centralPointStyle->clear();
    cmb_centralPointStyle->addItem(
        QIcon(":/rcc/" + Theme::getEffectiveTheme() + "/eye_icon.png"),
        serializeCentralPointStyle(CentralPointStyleT::EyePic));
    cmb_centralPointStyle->addItem(
        QIcon(":/rcc/" + Theme::getEffectiveTheme() + "/greendot_icon.png"),
        serializeCentralPointStyle(CentralPointStyleT::GreenDot));
    cmb_centralPointStyle->addItem(
        QIcon(":/rcc/" + Theme::getEffectiveTheme() + "/none_icon.png"),
        serializeCentralPointStyle(CentralPointStyleT::None));
    cmb_centralPointStyle->setCurrentText(
        serializeCentralPointStyle(Settings::getCentralPointStyle()));
}

void SettingsWidget::onLanguageChange() {
    lbl_caption->setText(tr("Settings"));

    lbl_language->setText(tr("Language"));
    lbl_language_detailed->setText(
        tr("To choose a language of the application"));

    lbl_theme->setText(tr("Themes"));
    lbl_theme_detailed->setText(tr("Color of interface elements"));

    lbl_soundState->setText(tr("Sound"));
    lbl_soundState_detailed->setText(tr("Sound of button clicks"));

    lbl_tableStyle->setText(tr("Table Style"));
    lbl_tableStyle_detailed->setText(tr("Classic or Sparse table"));

    lbl_centralPointStyle->setText(tr("Central Point"));
    lbl_centralPointStyle_detailed->setText(tr("Point for concentration"));

    lbl_fontMaxSize->setText(tr("Max Font"));
    lbl_fontMaxSize_detailed->setText(
        tr("Table's max font in pixels. 0 is auto"));

    lbl_tableScale->setText(tr("Table Size"));
    lbl_tableScale_detailed->setText(
        tr("Table size in %. Useful for big screens of tablets"));

    lbl_showTimer->setText(tr("Show Timer"));
    lbl_showTimer_detailed->setText(tr("Timer during exercise"));

    lbl_keepAwake->setText(tr("Keep Awake"));
    lbl_keepAwake_detailed->setText(
        tr("Block fading of screen during exercise"));

    okBtn->setText(tr("Ok"));

    cmb_language->setCurrentText(Settings::getLanguage());
}

bool SettingsWidget::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QWidget::event(event);
}

void SettingsWidget::resizeEvent(QResizeEvent *event) {
    QFontMetrics fm(Settings::getFont3());

    lbl_caption->setFont(Settings::getFont2());

    lbl_language->setFont(Settings::getFont3());
    lbl_language_detailed->setFont(Settings::getFont4());
    cmb_language->setFont(Settings::getFont3());
    cmb_language->setIconSize(QSize(fm.lineSpacing(), fm.lineSpacing()));
    cmb_language->view()->setIconSize(
        QSize(fm.lineSpacing() * 3, fm.lineSpacing() * 3));

    lbl_theme->setFont(Settings::getFont3());
    lbl_theme_detailed->setFont(Settings::getFont4());
    cmb_theme->setFont(Settings::getFont3());
    cmb_theme->setIconSize(QSize(fm.lineSpacing(), fm.lineSpacing()));
    cmb_theme->view()->setIconSize(
        QSize(fm.lineSpacing() * 3, fm.lineSpacing() * 3));

    lbl_soundState->setFont(Settings::getFont3());
    lbl_soundState_detailed->setFont(Settings::getFont4());
    ckb_soundState->setFont(Settings::getFont3());

    lbl_tableStyle->setFont(Settings::getFont3());
    lbl_tableStyle_detailed->setFont(Settings::getFont4());
    cmb_tableStyle->setFont(Settings::getFont3());
    cmb_tableStyle->setIconSize(QSize(fm.lineSpacing(), fm.lineSpacing()));
    cmb_tableStyle->view()->setIconSize(
        QSize(fm.lineSpacing() * 3, fm.lineSpacing() * 3));

    lbl_centralPointStyle->setFont(Settings::getFont3());
    lbl_centralPointStyle_detailed->setFont(Settings::getFont4());
    cmb_centralPointStyle->setFont(Settings::getFont3());
    cmb_centralPointStyle->setIconSize(
        QSize(fm.lineSpacing(), fm.lineSpacing()));
    cmb_centralPointStyle->view()->setIconSize(
        QSize(fm.lineSpacing() * 3, fm.lineSpacing() * 3));

    lbl_fontMaxSize->setFont(Settings::getFont3());
    lbl_fontMaxSize_detailed->setFont(Settings::getFont4());
    cmb_fontMaxSize->setFont(Settings::getFont3());

    lbl_tableScale->setFont(Settings::getFont3());
    lbl_tableScale_detailed->setFont(Settings::getFont4());
    cmb_tableScale->setFont(Settings::getFont3());

    lbl_showTimer->setFont(Settings::getFont3());
    lbl_showTimer_detailed->setFont(Settings::getFont4());
    ckb_showTimer->setFont(Settings::getFont3());

    lbl_keepAwake->setFont(Settings::getFont3());
    lbl_keepAwake_detailed->setFont(Settings::getFont4());
    ckb_keepAwake->setFont(Settings::getFont3());

    okBtn->setMinimumHeight(Settings::getLogicalDPI() * 0.15);
    okBtn->setMaximumHeight(Settings::getLogicalDPI() * 0.25);

    QWidget::resizeEvent(event);
}

void SettingsWidget::showEvent(QShowEvent *event) {
    okBtn->setFocus();
    scrollArea->verticalScrollBar()->setValue(0);

    QWidget::showEvent(event);
}

void SettingsWidget::languageChange(QString s) {
    Settings::setLanguage(s);
    Language::applyLanguage();
}

void SettingsWidget::themeChange(QString s) {
    Settings::setTheme(s);
    Theme::applyTheme();
}

void SettingsWidget::soundStateChange(int state) {
    if (state == Qt::Checked)
        Settings::setSoundState(true);
    else if (state == Qt::Unchecked)
        Settings::setSoundState(false);
}

void SettingsWidget::tableStyleChange(QString s) {
    Settings::setTableStyle(deSerializeTableStyle(s));
}

void SettingsWidget::centralPointStyleChange(QString s) {
    Settings::setCentralPointStyle(deSerializeCentralPointStyle(s));
}

void SettingsWidget::fontMaxSizeChange(QString s) {
    Settings::setFontMaxSize(s.toInt());
}

void SettingsWidget::tableScaleChange(QString s) {
    s.resize(s.size() - 1);
    Settings::setTableScale(s.toInt());
}

void SettingsWidget::showTimerChange(int state) {
    if (state == Qt::Checked)
        Settings::setShowTimer(true);
    else if (state == Qt::Unchecked)
        Settings::setShowTimer(false);
}

void SettingsWidget::keepAwakeChange(int state) {
    if (state == Qt::Checked)
        Settings::setKeepAwake(true);
    else if (state == Qt::Unchecked)
        Settings::setKeepAwake(false);
}
