#include "serialize.h"

QVector<QString> getTableStyleList() {
    return QVector<QString>{"Sparse", "Classic"};
}

QString serializeTableStyle(TableStyleT TableStyle) {
    return getTableStyleList()[static_cast<int>(TableStyle)];
}

TableStyleT deSerializeTableStyle(QString str) {
    auto index = getTableStyleList().indexOf(str);
    if (index == -1)
        index = 0;
    return TableStyleT(index);
}

QVector<QString> getCentralPointStyleList() {
    return QVector<QString>{"GreenDot", "EyePic", "None"};
}

QString serializeCentralPointStyle(CentralPointStyleT CentralPointStyle) {
    return getCentralPointStyleList()[static_cast<int>(CentralPointStyle)];
}

CentralPointStyleT deSerializeCentralPointStyle(QString str) {
    auto index = getCentralPointStyleList().indexOf(str);
    if (index == -1)
        index = 0;
    return CentralPointStyleT(index);
}

QString serializeTableStat(QList<int> TableStat) {
    QString s;
    for (auto &i : TableStat)
        s += QString::number(i) + ":";
    return s;
}

QList<int> deSerializeTableStat(QString str) {
    QStringList numbers = str.split(":");

    QList<int> result;
    for (const QString &number : numbers) {
        bool ok;
        int score = number.toInt(&ok);
        if (ok) {
            result.push_back(score);
        }
    }
    return result;
}
