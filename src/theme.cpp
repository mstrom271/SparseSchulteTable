#include "theme.h"
#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <QWidget>

Theme::Theme() {
    QSettings settings;
    currentTheme = settings.value("/theme", list[1]).toString();
    if (!list.contains(currentTheme))
        currentTheme = list[1];
}

void Theme::setTheme(QString newThemeName) {
    if (!list.contains(newThemeName))
        newThemeName = list[1];

    QSettings settings;
    settings.setValue("/theme", newThemeName);

    currentTheme = newThemeName;
}

QString Theme::getTheme() { return currentTheme; }

QStringList Theme::getList() { return list; }

// notify all widgets to change a theme
void Theme::notifyAll() {
    const QWidgetList allWidgets = QApplication::allWidgets();
    for (auto widget : allWidgets) {
        QApplication::instance()->postEvent(widget, new ThemeChangeEvent);
    }
}
