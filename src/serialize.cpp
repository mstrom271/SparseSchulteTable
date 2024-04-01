#include "serialize.h"
#include "settings.h"

QString serializeTableStyle(TableStyleT TableStyle) {
    if (TableStyle == TableStyleT::ClassicTable)
        return "Classic";
    else
        return "Sparse";
}

TableStyleT deSerializeTableStyle(QString str) {
    if (str == "Classic")
        return TableStyleT::ClassicTable;
    else
        return TableStyleT::SparseTable;
}

QString serializeCentralPointStyle(CentralPointStyleT CentralPointStyle) {
    if (CentralPointStyle == CentralPointStyleT::None)
        return "None";
    else if (CentralPointStyle == CentralPointStyleT::GreenDot)
        return "GreenDot";
    else
        return "EyePic";
}

CentralPointStyleT deSerializeCentralPointStyle(QString str) {
    if (str == "None")
        return CentralPointStyleT::None;
    else if (str == "GreenDot")
        return CentralPointStyleT::GreenDot;
    else
        return CentralPointStyleT::EyePic;
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
