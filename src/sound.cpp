#include "sound.h"
#include "settings.h"

Sound::Sound() {
    clickSound = new QSoundEffect;
    switchingSound = new QSoundEffect;

    clickSound->setSource(QUrl("qrc:/rcc/Sounds/clickSound.wav"));
    switchingSound->setSource(QUrl("qrc:/rcc/Sounds/clickSound.wav"));
}

Sound::~Sound() {
    delete clickSound;
    delete switchingSound;
}

Sound &Sound::getInstance() {
    static Sound instance;
    return instance;
}

void Sound::click() {
    if (Settings::getClickSound()) {
        Sound::getInstance().clickSound->stop();
        Sound::getInstance().clickSound->play();
    }
}

void Sound::switching() {
    if (Settings::getClickSound()) {
        Sound::getInstance().switchingSound->stop();
        Sound::getInstance().switchingSound->play();
    }
}
