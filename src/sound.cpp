#include "sound.h"
#include <QSettings>

bool Sound::getIsClickSound() const { return isClickSound; }

void Sound::setIsClickSound(bool newIsClickSound) {
    isClickSound = newIsClickSound;

    QSettings settings;
    settings.setValue("/clickSound", isClickSound);
}

Sound::Sound() {
    QSettings settings;
    isClickSound = settings.value("/clickSound", true).toBool();

    clickSound = new QSoundEffect;
    switchingSound = new QSoundEffect;

    clickSound->setSource(QUrl("qrc:/rcc/Sounds/clickSound.wav"));
    switchingSound->setSource(QUrl("qrc:/rcc/Sounds/clickSound.wav"));
}

Sound::~Sound() {
    delete clickSound;
    delete switchingSound;
}

void Sound::click() {
    if (isClickSound) {
        clickSound->stop();
        clickSound->play();
    }
}

void Sound::switching() {
    if (isClickSound) {
        switchingSound->stop();
        switchingSound->play();
    }
}
