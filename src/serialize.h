#pragma once

#include "settings.h"
#include <QString>

QString serializeTableStyle(TableStyleT TableStyle);
TableStyleT deSerializeTableStyle(QString str);
QString serializeCentralPointStyle(CentralPointStyleT CentralPointStyle);
CentralPointStyleT deSerializeCentralPointStyle(QString str);
QString serializeTableStat(QList<int> TableStat);
QList<int> deSerializeTableStat(QString str);
