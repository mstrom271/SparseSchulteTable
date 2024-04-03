#include "config.h"
#include "mainwidget.h"
#include <QApplication>
#include <QScreen>

int main(int argc, char **argv) {
    QApplication *app = new QApplication(argc, argv);
    QApplication::setOrganizationName(ORGANIZATION_NAME);
    QApplication::setApplicationName(PROJECT_NAME);

    MainWidget *wgt = new MainWidget;

#if defined(Q_OS_WINDOWS) || defined(Q_OS_LINUX)
    wgt->resize(500, 1000);
    wgt->move(app->primaryScreen()->size().width() / 2 - 250, 0);
#endif

    wgt->show();
    return app->exec();
}
