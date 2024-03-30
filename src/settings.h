#pragma once

#include "language.h"
#include "sound.h"
#include "stats.h"
#include "theme.h"
#include <QFont>

enum class TableStyle { SparseTable, ClassicTable };
QString serializeTableStyle(TableStyle tableStyle);
TableStyle deSerializeTableStyle(QString s);

enum class CentralPointStyle { None, GreenDot, EyePic };
QString serializeCentralPointStyle(CentralPointStyle centralPointStyle);
CentralPointStyle deSerializeCentralPointStyle(QString s);

class Settings {
    Language language;
    Theme theme;
    Stats stats;
    Sound sound;

    int numCells;
    int tableScale;

    bool firstRun;
    int fontMaxSize;
    bool keepAwake;
    bool showTimer;
    TableStyle tableStyle;
    CentralPointStyle centralPointStyle;

    QVector<int> numCellsRangeCache;
    QFont font1, font2, font3, font4;
    double logicalDPI;
    long long seedForSchulte;

  public:
    Settings();
    void upgradeOldSettings();

    Language &getLanguage();
    Theme &getTheme();
    Stats &getStats();
    Sound &getSound();

    int getNumCells() const;
    void setNumCells(int newNumCells);
    QVector<int> &getNumCellsRange();
    void initNumCellsRange();
    int correctNumCells(int numCells);

    bool getFirstRun() const;
    int getFontMaxSize() const;
    void setFontMaxSize(int newFontMaxSize);
    bool getShowTimer() const;
    void setShowTimer(bool newShowTimer);
    TableStyle getTableStyle() const;
    void setTableStyle(TableStyle newTableStyle);
    bool getKeepAwake() const;
    void setKeepAwake(bool newKeepAwake);

    const QFont &getFont1() const;
    void setFont1(const QFont &newFont1);
    const QFont &getFont2() const;
    void setFont2(const QFont &newFont2);
    const QFont &getFont3() const;
    void setFont3(const QFont &newFont3);
    const QFont &getFont4() const;
    void setFont4(const QFont &newFont4);
    double getLogicalDPI() const;
    void setLogicalDPI(double newLogicalDPI);
    CentralPointStyle getCentralPointStyle() const;
    void setCentralPointStyle(CentralPointStyle newCentralPointStyle);
    QSize getWindowSize();
    int getTableScale() const;
    void setTableScale(int newTableScale);
    long long getSeedForSchulte() const;
    void setSeedForSchulte(long long newSeedForSchulte);
    void generateSeedForSchulte();
};
