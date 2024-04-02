#pragma once

#include <QFont>
#include <QMap>
#include <QSettings>
#include <QVector>

enum class TableStyleT { SparseTable, ClassicTable };
enum class CentralPointStyleT { None, GreenDot, EyePic };

class Settings {
    QSettings settings;

    bool FirstRun;
    QString Language;
    QString Theme;
    QMap<int, QList<int>> Stats;
    int NumCells;
    int TableScale;
    int FontMaxSize;
    bool KeepAwake;
    bool ShowTimer;
    bool ClickSound;
    TableStyleT TableStyle;
    CentralPointStyleT CentralPointStyle;

    // Temporary settings
    QVector<int> NumCellsRangeCache;
    double LogicalDPI;
    QFont Font1, Font2, Font3, Font4;
    long long SeedForSchulte;

    Settings();
    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;
    void upgradeOldSettings();

  public:
    static Settings &getInstance();

    static bool getFirstRun();

    static QString getLanguage();
    static void setLanguage(QString newLanguage);

    static QString getTheme();
    static void setTheme(QString newTheme);

    static QList<int> getTableStats(int table);
    static void setTableStats(int table, QList<int> newStat);
    static void addTableStats(int table, int score);
    static void removeLastTableStats(int table);
    static void removeAllTableStats(int table);

    static int getNumCells();
    static void setNumCells(int newNumCells);

    static int getTableScale();
    static void setTableScale(int newTableScale);

    static int getFontMaxSize();
    static void setFontMaxSize(int newFontMaxSize);

    static bool getKeepAwake();
    static void setKeepAwake(bool newKeepAwake);

    static bool getShowTimer();
    static void setShowTimer(bool newShowTimer);

    static bool getClickSound();
    static void setClickSound(bool newClickSound);

    static TableStyleT getTableStyle();
    static void setTableStyle(TableStyleT newTableStyle);

    static CentralPointStyleT getCentralPointStyle();
    static void setCentralPointStyle(CentralPointStyleT newCentralPointStyle);

    // Temporary settings
    static QSize getWindowSize();

    static QVector<int> getNumCellsRangeCache();
    void initNumCellsRangeCache();
    static int correctNumCells(int newNumCells);

    static double getLogicalDPI();
    static void setLogicalDPI(double newLogicalDPI);

    static QFont getFont1();
    static void setFont1(QFont newFont1);
    static QFont getFont2();
    static void setFont2(QFont newFont2);
    static QFont getFont3();
    static void setFont3(QFont newFont3);
    static QFont getFont4();
    static void setFont4(QFont newFont4);

    static long long getSeedForSchulte();
    static void setSeedForSchulte(long long newSeedForSchulte);
    static void generateSeedForSchulte();
};