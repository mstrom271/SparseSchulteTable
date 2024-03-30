#pragma once

#include <QSoundEffect>

class Sound {
    bool isClickSound;

    QSoundEffect *clickSound = nullptr, *switchingSound = nullptr;

  public:
    Sound();
    ~Sound();
    void click();
    void switching();

    bool getIsClickSound() const;
    void setIsClickSound(bool newIsClickSound);
};
