#pragma once

#include <QSoundEffect>

class Sound {
    QSoundEffect *clickSound = nullptr, *switchingSound = nullptr;

    Sound();
    Sound(const Sound &) = delete;
    Sound &operator=(const Sound &) = delete;

  public:
    ~Sound();
    static Sound &getInstance();

    static void click();
    static void switching();
};
