#include "stats.h"
#include <QtCore>
#include <iostream>
#include <sstream>

QList<int> parse_str(const QString &s) {
    QList<int> tokens;
    std::stringstream check(s.toStdString());
    std::string intermediate;
    while (getline(check, intermediate, ':')) {
        int result;
        try {
            result = std::stoi(intermediate);
        } catch (...) {
            result = 0;
        }
        tokens.push_back(result);
    }
    return tokens;
}

const QList<int> &Stats::getStats(int numCells) {
    QSettings settings;
    if (cache.contains(numCells))
        return cache[numCells];
    else {
        cache[numCells] =
            parse_str(settings.value("/table" + QString::number(numCells), "")
                          .toString());
        return cache[numCells];
    }
}

static constexpr int maxStats = 50;
void Stats::trimStats(QList<int> &stats) {
    while (stats.size() > maxStats)
        stats.pop_front();
}

void Stats::saveStats(int numCells, QList<int> stats) {
    trimStats(stats);
    cache[numCells] = stats;

    QString s;
    for (auto &i : stats)
        s += QString().number(i) + ":";

    QSettings settings;
    settings.setValue("/table" + QString::number(numCells), s);
}

void Stats::addStats(int numCells, int decisecs) {
    QList<int> stats = getStats(numCells);
    stats.push_back(decisecs);
    saveStats(numCells, stats);
}
