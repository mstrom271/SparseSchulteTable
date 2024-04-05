#pragma once

#include <QList>
#include <QString>
#include <QVector>

enum class TableStyleT { SparseTable, ClassicTable };
QVector<QString> getTableStyleList();
QString serializeTableStyle(TableStyleT TableStyle);
TableStyleT deSerializeTableStyle(QString str);

enum class CentralPointStyleT { GreenDot, EyePic, None };
QVector<QString> getCentralPointStyleList();
QString serializeCentralPointStyle(CentralPointStyleT CentralPointStyle);
CentralPointStyleT deSerializeCentralPointStyle(QString str);

QString serializeTableStat(QList<int> TableStat);
QList<int> deSerializeTableStat(QString str);
