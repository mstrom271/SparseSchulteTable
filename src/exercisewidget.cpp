#include "exercisewidget.h"
#include "language.h"
#include "settings.h"
#include "theme.h"
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
    cancelBtn->setMinimumHeight(Settings::getInstance().getLogicalDPI() * 0.15);
    cancelBtn->setMaximumHeight(Settings::getInstance().getLogicalDPI() * 0.25);

    doneBtn->setMinimumHeight(Settings::getInstance().getLogicalDPI() * 0.15);
    doneBtn->setMaximumHeight(Settings::getInstance().getLogicalDPI() * 0.25);

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
                     QString::number(Settings::getInstance().getNumCells()));
    schulteWgt->generate(Settings::getInstance().getSeedForSchulte());

    QWidget::showEvent(event);
}

ExerciseWidget::ExerciseWidget(QWidget *wgt) : QWidget(wgt) {
    vLayout = new QVBoxLayout;
    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spacer->setAttribute(Qt::WA_TranslucentBackground);
    vLayout->addWidget(spacer);

    timerLabel = new QLabel;
    timerLabel->setFont(Settings::getInstance().getFont3());
    timerLabel->setAttribute(Qt::WA_TranslucentBackground);
    timerLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    vLayout->addWidget(timerLabel, 1);

    cancelBtn = new QPushButton;
    cancelBtn->setFont(Settings::getInstance().getFont3());
    connect(cancelBtn, SIGNAL(clicked()), SLOT(cancelSlot()));
    vLayout->addWidget(cancelBtn, 1);

    schulteWgt = new SchulteWidget(false);
    connect(schulteWgt, SIGNAL(pressedNumber(int)), SLOT(pressedNumber(int)));
    connect(schulteWgt, SIGNAL(done()), SLOT(doneSlot()));
    vLayout->addWidget(schulteWgt, 15);

    doneBtn = new QPushButton;
    doneBtn->setFont(Settings::getInstance().getFont3());
    connect(doneBtn, SIGNAL(clicked()), SLOT(doneSlot()));
    vLayout->addWidget(doneBtn, 1);
    setLayout(vLayout);

    QObject::connect(&timer, SIGNAL(timeout()), SLOT(updateTime()));
}

void ExerciseWidget::updateTime() {
    if (Settings::getInstance().getShowTimer()) {
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
                     QString::number(Settings::getInstance().getNumCells()));
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

    Settings::getInstance().addTableStat(Settings::getInstance().getNumCells(),
                                         elapsed);

    emit doneSignal();
}
