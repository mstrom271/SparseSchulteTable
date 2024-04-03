#include "theme.h"
#include <QApplication>
#include <QStringList>
#include <QWidget>

Theme::Theme() {}

Theme &Theme::getInstance() {
    static Theme instance;
    return instance;
}

// notify all widgets to change a theme
void Theme::notifyAll() {
    const QWidgetList allWidgets = QApplication::allWidgets();
    for (auto widget : allWidgets) {
        QApplication::instance()->postEvent(widget, new ThemeChangeEvent);
    }
}

void Theme::applyTheme(QString theme) {
    // TODO:
    // static_cast<QApplication
    // *>(QApplication::instance())->setStyleSheet(style);
}

QString Theme::getSystemTheme() {
    QPalette palette = QApplication::palette();
    QColor windowColor = palette.color(QPalette::Window);
    if (windowColor.value() < 128)
        return correct("DarkTheme");
    else
        return correct("LightTheme");
}

QStringList Theme::getThemeList() {
    return {"DarkTheme", "GreyTheme", "LightTheme"};
}

// check if theme is allowed. Return default, if theme is incorrect
QString Theme::correct(QString theme) {
    QStringList themeAllowed = getThemeList();
    if (!themeAllowed.contains(theme))
        return themeAllowed.front();
    return theme;
}
