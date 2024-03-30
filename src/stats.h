#pragma once

#include <QList>
#include <QMap>

class Stats {
    QMap<int, QList<int>> cache;

  public:
    const QList<int> &getStats(int numCells);
    void trimStats(QList<int> &stats);
    void saveStats(int numCells, QList<int> stats);
    void addStats(int numCells, int decisecs);
};
