#include "settings.h"
#include <QApplication>
#include <QScreen>
#include <QSettings>
#include <QWidget>

void Settings::upgradeOldSettings() {
    QSettings settings;

    bool firsthelp = settings.value("/firsthelp", false).toBool();
    QString prevVersion = settings.value("/version", "").toString();

    if (firsthelp == false && prevVersion == "") { // first run at all
    } else if (firsthelp == true &&
               prevVersion == "") { // first run after upgrade to 0.4
        settings.remove("/firsthelp");

        if (settings.value("/nightMode", true).toBool())
            theme.setTheme(theme.getList()[0]);
        else
            theme.setTheme(theme.getList()[2]);
        settings.remove("/nightMode");

        sound.setIsClickSound(false);

        settings.setValue("/firstrun", false);
        settings.setValue("/centralpointstyle",
                          serializeCentralPointStyle(CentralPointStyle::None));
    } else if (prevVersion == "0.4") { // next run of 0.4
    }
}

Settings::Settings() {
    upgradeOldSettings();

    QSettings settings;

    // TODO
    settings.setValue("/version", "0.5" /* VERSION_NAME */);

    firstRun = settings.value("/firstrun", true).toBool();
    if (firstRun)
        settings.setValue("/firstrun", false);

    fontMaxSize = settings.value("/fontmaxsize", 0).toInt();
    keepAwake = settings.value("/keepawake", true).toBool();
    showTimer = settings.value("/showtimer", true).toBool();
    tableStyle = deSerializeTableStyle(
        settings
            .value("/tablestyle", serializeTableStyle(TableStyle::SparseTable))
            .toString());
    centralPointStyle = deSerializeCentralPointStyle(
        settings
            .value("/centralpointstyle",
                   serializeCentralPointStyle(CentralPointStyle::GreenDot))
            .toString());
    tableScale = settings.value("/tablescale", 100).toInt();

    numCells = std::clamp(settings.value("/numcells", 16).toInt(), 3, 121);
    initNumCellsRange();

    font1 = font2 = font3 = font4 = QApplication::font();
#ifdef Q_OS_ANDROID
    font1.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.4);
    font2.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.25);
    font3.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.14);
    font4.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.09);
    logicalDPI = QApplication::primaryScreen()->physicalDotsPerInch() * 1.0;
#else
    font1.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.45);
    font2.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.30);
    font3.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.20);
    font4.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.13);
    logicalDPI = QApplication::primaryScreen()->physicalDotsPerInch() * 1.5;
#endif
}

Language &Settings::getLanguage() { return language; }
Theme &Settings::getTheme() { return theme; }
Stats &Settings::getStats() { return stats; }
Sound &Settings::getSound() { return sound; }

int Settings::getNumCells() const { return numCells; }
void Settings::setNumCells(int newNumCells) {
    numCells = correctNumCells(newNumCells);

    QSettings settings;
    settings.setValue("/numcells", numCells);
}

QVector<int> &Settings::getNumCellsRange() { return numCellsRangeCache; }
void Settings::initNumCellsRange() {
    numCellsRangeCache.clear();

    if (getTableStyle() == TableStyle::SparseTable) {
        if (getCentralPointStyle() == CentralPointStyle::None)
            for (int i = 3; i <= 121; i++)
                numCellsRangeCache.push_back(i);
        else
            for (int i = 3; i <= 120; i++)
                numCellsRangeCache.push_back(i);
    } else if (getTableStyle() == TableStyle::ClassicTable) {
        if (getCentralPointStyle() == CentralPointStyle::None)
            numCellsRangeCache = {4, 9, 16, 25, 36, 49, 64, 81, 100, 121};
        else
            numCellsRangeCache = {4, 8, 16, 24, 36, 48, 64, 80, 100, 120};
    }

    setNumCells(numCells);
}

int Settings::correctNumCells(int newNumCells) {
    auto it = std::lower_bound(std::begin(numCellsRangeCache),
                               std::end(numCellsRangeCache), newNumCells);
    return (it != std::end(numCellsRangeCache) ? *it
                                               : numCellsRangeCache.last());
}

bool Settings::getFirstRun() const { return firstRun; }

int Settings::getFontMaxSize() const { return fontMaxSize; }
void Settings::setFontMaxSize(int newFontMaxSize) {
    fontMaxSize = newFontMaxSize;

    QSettings settings;
    settings.setValue("/fontmaxsize", fontMaxSize);
}

bool Settings::getKeepAwake() const { return keepAwake; }
void Settings::setKeepAwake(bool newKeepAwake) {
    keepAwake = newKeepAwake;

    QSettings settings;
    settings.setValue("/keepawake", keepAwake);
}

bool Settings::getShowTimer() const { return showTimer; }
void Settings::setShowTimer(bool newShowTimer) {
    showTimer = newShowTimer;

    QSettings settings;
    settings.setValue("/showtimer", showTimer);
}

TableStyle Settings::getTableStyle() const { return tableStyle; }
void Settings::setTableStyle(TableStyle newTableStyle) {
    tableStyle = newTableStyle;

    QSettings settings;
    settings.setValue("/tablestyle", serializeTableStyle(tableStyle));

    initNumCellsRange();
}

CentralPointStyle Settings::getCentralPointStyle() const {
    return centralPointStyle;
}
void Settings::setCentralPointStyle(CentralPointStyle newCentralPointStyle) {
    centralPointStyle = newCentralPointStyle;

    QSettings settings;
    settings.setValue("/centralpointstyle",
                      serializeCentralPointStyle(centralPointStyle));

    initNumCellsRange();
}

const QFont &Settings::getFont1() const { return font1; }
void Settings::setFont1(const QFont &newFont1) { font1 = newFont1; }
const QFont &Settings::getFont2() const { return font2; }
void Settings::setFont2(const QFont &newFont2) { font2 = newFont2; }
const QFont &Settings::getFont3() const { return font3; }
void Settings::setFont3(const QFont &newFont3) { font3 = newFont3; }
const QFont &Settings::getFont4() const { return font4; }
void Settings::setFont4(const QFont &newFont4) { font4 = newFont4; }
double Settings::getLogicalDPI() const { return logicalDPI; }
void Settings::setLogicalDPI(double newLogicalDPI) {
    logicalDPI = newLogicalDPI;
}

long long Settings::getSeedForSchulte() const { return seedForSchulte; }
void Settings::setSeedForSchulte(long long newSeedForSchulte) {
    seedForSchulte = newSeedForSchulte;
}
void Settings::generateSeedForSchulte() {
    const auto now = std::chrono::steady_clock::now();
    setSeedForSchulte(now.time_since_epoch().count());
}

QSize Settings::getWindowSize() {
    static QWidget *mainWgt = nullptr;
    if (!mainWgt) {
        const QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
        for (auto wgt : topLevelWidgets)
            if (wgt->objectName() == "MainWidget") {
                mainWgt = wgt;
                break;
            }
    }

    if (mainWgt)
        return mainWgt->size();
    else
        return QSize(600, 800);
}

int Settings::getTableScale() const { return tableScale; }
void Settings::setTableScale(int newTableScale) {
    tableScale = newTableScale;

    QSettings settings;
    settings.setValue("/tablescale", tableScale);
}

QString serializeTableStyle(TableStyle tableStyle) {
    if (tableStyle == TableStyle::ClassicTable)
        return "Classic";
    else
        return "Sparse";
}

TableStyle deSerializeTableStyle(QString s) {
    if (s == "Classic")
        return TableStyle::ClassicTable;
    else
        return TableStyle::SparseTable;
}

QString serializeCentralPointStyle(CentralPointStyle centralPointStyle) {
    if (centralPointStyle == CentralPointStyle::None)
        return "None";
    else if (centralPointStyle == CentralPointStyle::GreenDot)
        return "GreenDot";
    else
        return "EyePic";
}

CentralPointStyle deSerializeCentralPointStyle(QString s) {
    if (s == "None")
        return CentralPointStyle::None;
    else if (s == "GreenDot")
        return CentralPointStyle::GreenDot;
    else
        return CentralPointStyle::EyePic;
}
