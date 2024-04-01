#pragma once

#include <QString>

enum class TableStyleT { SparseTable, ClassicTable };
enum class CentralPointStyleT { None, GreenDot, EyePic };

QString serializeTableStyle(TableStyleT TableStyle);
TableStyleT deSerializeTableStyle(QString str);
QString serializeCentralPointStyle(CentralPointStyleT CentralPointStyle);
CentralPointStyleT deSerializeCentralPointStyle(QString str);
QString serializeTableStat(QList<int> TableStat);
QList<int> deSerializeTableStat(QString str);
