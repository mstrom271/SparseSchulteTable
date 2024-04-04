#include "helpwidget.h"
#include "language.h"
#include "settings.h"
#include "theme.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QScrollBar>
#include <QScroller>

void HelpWidget::onThemeChange() {}

void HelpWidget::onLanguageChange() {
    QString helpmsg;

    QFile file(":/rcc/help_" + Language::getEffectiveLanguage() + ".html");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        helpmsg += file.readAll();
    helpLabel->setText(helpmsg);
    helpLabel->adjustSize();

    okBtn->setText(tr("Ok"));

    update();
}

bool HelpWidget::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QWidget::event(event);
}

void HelpWidget::resizeEvent(QResizeEvent *event) {
    okBtn->setMinimumHeight(Settings::getLogicalDPI() * 0.15);
    okBtn->setMaximumHeight(Settings::getLogicalDPI() * 0.25);

    QWidget::resizeEvent(event);
}

void HelpWidget::showEvent(QShowEvent *event) {
    okBtn->setFocus();
    scrollArea->verticalScrollBar()->setValue(0);

    QWidget::showEvent(event);
}

HelpWidget::HelpWidget(QWidget *wgt) : QWidget(wgt) {
    vLayout = new QVBoxLayout;
    scrollArea = new QScrollArea;
    helpLabel = new QLabel;
    helpLabel->setFont(Settings::getFont3());
    helpLabel->setWordWrap(true);
    helpLabel->setOpenExternalLinks(true);
    helpLabel->setAttribute(Qt::WA_TranslucentBackground);
    scrollArea->setWidget(helpLabel);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setAttribute(Qt::WA_TranslucentBackground);
#ifdef Q_OS_ANDROID
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScroller::grabGesture(scrollArea->viewport(),
                           QScroller::LeftMouseButtonGesture);
#endif
    vLayout->addWidget(scrollArea, 17);

    okBtn = new QPushButton;
    okBtn->setFont(Settings::getFont3());
    connect(okBtn, SIGNAL(clicked()), SIGNAL(ok()));
    vLayout->addWidget(okBtn, 1);
    setLayout(vLayout);
}
