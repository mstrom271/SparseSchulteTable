#include "exercisewidget.h"
#include <chrono>

using deciseconds = std::chrono::duration<long long, std::ratio<1, 10>>;

void ExerciseWidget::onThemeChange() {}

void ExerciseWidget::onLanguageChange() { cancelBtn->setText(tr("Cancel")); }

bool ExerciseWidget::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QWidget::event(event);
}

void ExerciseWidget::resizeEvent(QResizeEvent *event) {
    cancelBtn->setMinimumHeight(settings->getLogicalDPI() * 0.15);
    cancelBtn->setMaximumHeight(settings->getLogicalDPI() * 0.25);

    doneBtn->setMinimumHeight(settings->getLogicalDPI() * 0.15);
    doneBtn->setMaximumHeight(settings->getLogicalDPI() * 0.25);

    QWidget::resizeEvent(event);
}

void ExerciseWidget::showEvent(QShowEvent *event) {
    if (!keepAwakeHelper)
        keepAwakeHelper = new KeepAwakeHelper;

    timer.start(50);
    const auto now = std::chrono::steady_clock::now();
    startTime =
        std::chrono::duration_cast<deciseconds>(now.time_since_epoch()).count();

    doneBtn->setText(tr("Done") + ": " + QString::number(0) + "/" +
                     QString::number(settings->getNumCells()));
    schulteWgt->generate(settings->getSeedForSchulte());

    QWidget::showEvent(event);
}

ExerciseWidget::ExerciseWidget(Settings *newSettings, QWidget *wgt)
    : QWidget(wgt), settings(newSettings) {
    vLayout = new QVBoxLayout;
    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spacer->setAttribute(Qt::WA_TranslucentBackground);
    vLayout->addWidget(spacer);

    timerLabel = new QLabel;
    timerLabel->setFont(settings->getFont3());
    timerLabel->setAttribute(Qt::WA_TranslucentBackground);
    timerLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    vLayout->addWidget(timerLabel, 1);

    cancelBtn = new QPushButton;
    cancelBtn->setFont(settings->getFont3());
    connect(cancelBtn, SIGNAL(clicked()), SLOT(cancelSlot()));
    vLayout->addWidget(cancelBtn, 1);

    schulteWgt = new SchulteWidget(settings, false);
    connect(schulteWgt, SIGNAL(pressedNumber(int)), SLOT(pressedNumber(int)));
    connect(schulteWgt, SIGNAL(done()), SLOT(doneSlot()));
    vLayout->addWidget(schulteWgt, 15);

    doneBtn = new QPushButton;
    doneBtn->setFont(settings->getFont3());
    connect(doneBtn, SIGNAL(clicked()), SLOT(doneSlot()));
    vLayout->addWidget(doneBtn, 1);
    setLayout(vLayout);

    QObject::connect(&timer, SIGNAL(timeout()), SLOT(updateTime()));
}

void ExerciseWidget::updateTime() {
    if (settings->getShowTimer()) {
        const auto now = std::chrono::steady_clock::now();
        long long elapsed =
            std::chrono::duration_cast<deciseconds>(now.time_since_epoch())
                .count() -
            startTime;
        timerLabel->setText(QString().number(elapsed / 10) + "," +
                            QString().number(elapsed % 10) + " " + tr("sec"));
    } else
        timerLabel->setText(tr("Timer is off"));
}

void ExerciseWidget::pressedNumber(int number) {
    doneBtn->setText(tr("Done") + ": " + QString::number(number) + "/" +
                     QString::number(settings->getNumCells()));
}

void ExerciseWidget::cancelSlot() {
    if (keepAwakeHelper) {
        delete keepAwakeHelper;
        keepAwakeHelper = nullptr;
    }

    timer.stop();

    emit doneSignal();
}

void ExerciseWidget::doneSlot() {
    if (keepAwakeHelper) {
        delete keepAwakeHelper;
        keepAwakeHelper = nullptr;
    }

    timer.stop();

    const auto now = std::chrono::steady_clock::now();
    long long elapsed =
        std::chrono::duration_cast<deciseconds>(now.time_since_epoch())
            .count() -
        startTime;

    settings->getStats().addStats(settings->getNumCells(), elapsed);

    emit doneSignal();
}
