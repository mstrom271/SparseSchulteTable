#include "settings.h"
#include "config.h"
#include "serialize.h"
#include <QApplication>
#include <QDataStream>
#include <QIODevice>
#include <QScreen>
#include <QWidget>

Settings::Settings() {
    upgradeOldSettings();

    FirstRun = settings.value("/FirstRun", true).toBool();
    if (FirstRun)
        settings.setValue("/FirstRun", false);

    Language = settings.value("/Language", "System").toString();
    Theme = settings.value("/Theme", "System").toString();

    for (int table = 3; table <= 121; table++)
        Stats[table] = deSerializeTableStat(
            settings.value("/Stats/" + QString::number(table)).toString());

    NumCells = std::clamp(settings.value("/NumCells", 16).toInt(), 3, 121);
    TableScale = settings.value("/TableScale", 100).toInt();
    FontMaxSize = settings.value("/FontMaxSize", 0).toInt();
    KeepAwake = settings.value("/KeepAwake", true).toBool();
    ShowTimer = settings.value("/ShowTimer", true).toBool();
    ClickSound = settings.value("/ClickSound", false).toBool();
    TableStyle = deSerializeTableStyle(
        settings
            .value("/TableStyle", serializeTableStyle(TableStyleT::SparseTable))
            .toString());

    CentralPointStyle = deSerializeCentralPointStyle(
        settings
            .value("/CentralPointStyle",
                   serializeCentralPointStyle(CentralPointStyleT::GreenDot))
            .toString());

    // Temporary settings
    initNumCellsRangeCache();
    Font1 = Font2 = Font3 = Font4 = QApplication::font();
#ifdef Q_OS_ANDROID
    Font1.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.4);
    Font2.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.25);
    Font3.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.14);
    Font4.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.09);
    LogicalDPI = QApplication::primaryScreen()->physicalDotsPerInch() * 1.0;
#else
    Font1.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.45);
    Font2.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.30);
    Font3.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.20);
    Font4.setPixelSize(QApplication::primaryScreen()->physicalDotsPerInch() *
                       0.13);
    LogicalDPI = QApplication::primaryScreen()->physicalDotsPerInch() * 1.5;
#endif
}

Settings &Settings::getInstance() {
    static Settings instance;
    return instance;
}

void Settings::upgradeOldSettings() {
    // evaluate from 0.1-0.3 to 0.4
    if (settings.contains("/firsthelp") && !settings.contains("/version")) {

        // remove /firsthelp
        settings.remove("/firsthelp");

        // create /version
        settings.setValue("/version", "0.4");

        // create /firstrun
        settings.setValue("/firstrun", false);

        // convert /nightMode to /theme
        if (settings.contains("/nightMode")) {
            if (Settings::getInstance()
                    .settings.value("/nightMode", true)
                    .toBool())
                settings.setValue("/theme", "DarkTheme");
            else
                settings.setValue("/theme", "LightTheme");
            settings.remove("/nightMode");
        }

        // create /centralpointstyle
        settings.setValue("/centralpointstyle",
                          serializeCentralPointStyle(CentralPointStyleT::None));
    }

    // evaluate from 0.4 to 0.5
    if (settings.value("/version", "") == "0.4") {
        // pairs to convert {old_name, new_name}
        QVector<QPair<QString, QString>> names = {
            // clang-format off
            {"/centralpointstyle",   "/CentralPointStyle"},
            {"/firstrun",            "/FirstRun"},
            {"/fontmaxsize",         "/FontMaxSize"},
            {"/keepawake",           "/KeepAwake"},
            {"/language",            "/Language"},
            {"/numcells",            "/NumCells"},
            {"/showtimer",           "/ShowTimer"},
            {"/clickSound",          "/ClickSound"},
            {"/tablescale",          "/TableScale"},
            {"/tablestyle",          "/TableStyle"},
            {"/theme",               "/Theme"},
            {"/version",             "/Version"},
            // clang-format on
        };
        for (int i = 3; i <= 121; i++)
            names.push_back({"/table" + QString::number(i),
                             "/Stats/" + QString::number(i)});

        // rename items
        for (auto item : names) {
            if (settings.contains(item.first)) {
                auto value = settings.value(item.first);

                // remove before set, due to case-insensitive windows approach
                settings.remove(item.first);
                settings.setValue(item.second, value);
            }
        }

        // update /version
        settings.setValue("/Version", "0.5");
    }
}

bool Settings::getFirstRun() { return Settings::getInstance().FirstRun; }

QString Settings::getLanguage() { return Settings::getInstance().Language; }
void Settings::setLanguage(QString newLanguage) {
    Settings::getInstance().settings.setValue("/Language", newLanguage);
    Settings::getInstance().Language = newLanguage;
};

QString Settings::getTheme() { return Settings::getInstance().Theme; }
void Settings::setTheme(QString newTheme) {
    Settings::getInstance().settings.setValue("/Theme", newTheme);
    Settings::getInstance().Theme = newTheme;
};

QList<int> Settings::getTableStats(int table) {
    return Settings::getInstance().Stats[table];
}
void Settings::setTableStats(int table, QList<int> newStat) {
    // convert to str
    Settings::getInstance().settings.setValue(
        "/Stats/" + QString::number(table), serializeTableStat(newStat));
    Settings::getInstance().Stats[table] = newStat;
};
void Settings::addTableStats(int table, int score) {
    auto stat = getTableStats(table);
    stat.push_back(score);

    // trim
    static constexpr int maxStats = 50;
    while (stat.size() > maxStats)
        stat.pop_front();

    setTableStats(table, stat);
}
void Settings::removeLastTableStats(int table) {
    auto stat = getTableStats(table);
    if (!stat.isEmpty())
        stat.pop_back();
    setTableStats(table, stat);
    if (stat.isEmpty())
        Settings::getInstance().settings.remove("/Stats/" +
                                                QString::number(table));
}
void Settings::removeAllTableStats(int table) {
    setTableStats(table, QList<int>());
    Settings::getInstance().settings.remove("/Stats/" + QString::number(table));
}

int Settings::getNumCells() { return Settings::getInstance().NumCells; }
void Settings::setNumCells(int newNumCells) {
    Settings::getInstance().correctNumCells();

    Settings::getInstance().settings.setValue("/NumCells", newNumCells);
    Settings::getInstance().NumCells = newNumCells;
};

int Settings::getTableScale() { return Settings::getInstance().TableScale; }
void Settings::setTableScale(int newTableScale) {
    Settings::getInstance().settings.setValue("/TableScale", newTableScale);
    Settings::getInstance().TableScale = newTableScale;
};

int Settings::getFontMaxSize() { return Settings::getInstance().FontMaxSize; }
void Settings::setFontMaxSize(int newFontMaxSize) {
    Settings::getInstance().settings.setValue("/FontMaxSize", newFontMaxSize);
    Settings::getInstance().FontMaxSize = newFontMaxSize;
};

bool Settings::getKeepAwake() { return Settings::getInstance().KeepAwake; }
void Settings::setKeepAwake(bool newKeepAwake) {
    Settings::getInstance().settings.setValue("/KeepAwake", newKeepAwake);
    Settings::getInstance().KeepAwake = newKeepAwake;
};

bool Settings::getShowTimer() { return Settings::getInstance().ShowTimer; }
void Settings::setShowTimer(bool newShowTimer) {
    Settings::getInstance().settings.setValue("/ShowTimer", newShowTimer);
    Settings::getInstance().ShowTimer = newShowTimer;
};

bool Settings::getClickSound() { return Settings::getInstance().ClickSound; }
void Settings::setClickSound(bool newClickSound) {
    Settings::getInstance().settings.setValue("/ClickSound", newClickSound);
    Settings::getInstance().ClickSound = newClickSound;
};

TableStyleT Settings::getTableStyle() {
    return Settings::getInstance().TableStyle;
}
void Settings::setTableStyle(TableStyleT newTableStyle) {
    Settings::getInstance().settings.setValue(
        "/TableStyle", serializeTableStyle(newTableStyle));
    Settings::getInstance().TableStyle = newTableStyle;

    Settings::getInstance().initNumCellsRangeCache();
    Settings::correctNumCells();
};

CentralPointStyleT Settings::getCentralPointStyle() {
    return Settings::getInstance().CentralPointStyle;
}
void Settings::setCentralPointStyle(CentralPointStyleT newCentralPointStyle) {
    Settings::getInstance().settings.setValue(
        "/CentralPointStyle", serializeCentralPointStyle(newCentralPointStyle));
    Settings::getInstance().CentralPointStyle = newCentralPointStyle;

    Settings::getInstance().initNumCellsRangeCache();
    Settings::correctNumCells();
};

// Temporary settings
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

QVector<int> Settings::getNumCellsRangeCache() {
    return Settings::getInstance().NumCellsRangeCache;
}
void Settings::initNumCellsRangeCache() {
    NumCellsRangeCache.clear();

    if (TableStyle == TableStyleT::SparseTable) {
        if (CentralPointStyle == CentralPointStyleT::None)
            for (int i = 3; i <= 121; i++)
                NumCellsRangeCache.push_back(i);
        else
            for (int i = 3; i <= 120; i++)
                NumCellsRangeCache.push_back(i);
    } else if (TableStyle == TableStyleT::ClassicTable) {
        if (CentralPointStyle == CentralPointStyleT::None)
            NumCellsRangeCache = {4, 9, 16, 25, 36, 49, 64, 81, 100, 121};
        else
            NumCellsRangeCache = {4, 8, 16, 24, 36, 48, 64, 80, 100, 120};
    }

    settings.setValue("/NumCells", NumCells);
}
void Settings::correctNumCells() {
    auto it =
        std::lower_bound(std::begin(Settings::getInstance().NumCellsRangeCache),
                         std::end(Settings::getInstance().NumCellsRangeCache),
                         Settings::getInstance().NumCells);
    Settings::getInstance().NumCells =
        (it != std::end(Settings::getInstance().NumCellsRangeCache)
             ? *it
             : Settings::getInstance().NumCellsRangeCache.last());
}

QFont Settings::getFont1() { return Settings::getInstance().Font1; }
void Settings::setFont1(QFont newFont1) {
    Settings::getInstance().Font1 = newFont1;
};
QFont Settings::getFont2() { return Settings::getInstance().Font2; }
void Settings::setFont2(QFont newFont2) {
    Settings::getInstance().Font2 = newFont2;
};
QFont Settings::getFont3() { return Settings::getInstance().Font3; }
void Settings::setFont3(QFont newFont3) {
    Settings::getInstance().Font3 = newFont3;
};
QFont Settings::getFont4() { return Settings::getInstance().Font4; }
void Settings::setFont4(QFont newFont4) {
    Settings::getInstance().Font4 = newFont4;
};
double Settings::getLogicalDPI() { return Settings::getInstance().LogicalDPI; }
void Settings::setLogicalDPI(double newLogicalDPI) {
    Settings::getInstance().LogicalDPI = newLogicalDPI;
};

long long Settings::getSeedForSchulte() {
    return Settings::getInstance().SeedForSchulte;
}
void Settings::setSeedForSchulte(long long newSeedForSchulte) {
    Settings::getInstance().SeedForSchulte = newSeedForSchulte;
};
void Settings::generateSeedForSchulte() {
    const auto now = std::chrono::steady_clock::now();
    Settings::getInstance().SeedForSchulte = now.time_since_epoch().count();
}