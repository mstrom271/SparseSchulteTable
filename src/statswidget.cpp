#include "statswidget.h"
#include <QPainter>
#include <algorithm>

void StatsWidget::onThemeChange() {}

void StatsWidget::onLanguageChange() {
    msg_min = tr("min");
    msg_max = tr("max");
    msg_last = tr("last");
    msg_sec = tr("sec");
    msg_nostat = tr("There is no statistics for this table");
    msg_deletelast = tr("Delete last record from statistics?");
    msg_deleteall = tr("Delete all records for this table?");

    update();
}

bool StatsWidget::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QWidget::event(event);
}

void StatsWidget::resizeEvent(QResizeEvent *event) {
    setMaximumHeight(settings->getWindowSize().width() / 3.5);

    QWidget::resizeEvent(event);
}

void StatsWidget::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.setFont(settings->getFont4());
    QFontMetrics fm(settings->getFont4());

    const QList<int> &stats =
        settings->getStats().getStats(settings->getNumCells());
    int num = stats.size();

    if (num > 0) {
        int min = stats.first();
        int max = stats.first();
        int last = stats.last();
        for (int decisec : stats) {
            if (decisec > max)
                max = decisec;
            else if (decisec < min)
                min = decisec;
        }

        if (height() > width() / 12.0) {
            double dot_radius = width() / 100.0;
            double top = fm.lineSpacing() + dot_radius;
            double bottom = height() - dot_radius;
            double side = width() / 30.0;

            double stepY =
                (bottom - top) / (((max - min) > 0) ? (max - min) : 1);
            double stepX =
                (width() - side * 2) / ((num - 1) > 0 ? (num - 1) : 1);

            p.setPen(QPen(p.pen().color(), 3, Qt::DashLine));
            p.drawLine(side, bottom, width() - side, bottom);
            p.drawLine(side, top, width() - side, top);
            p.drawLine(width() - side, top, width() - side, bottom);
            p.drawLine(side, top, side, bottom);

            p.setPen(QPen(p.pen().color(), 3, Qt::SolidLine));
            double x = width() - side;
            for (int i = num - 1; i > 0; i--) {
                p.drawLine(x, bottom - (stats[i] - min) * stepY, x - stepX,
                           bottom - (stats[i - 1] - min) * stepY);
                x -= stepX;
            }
            x = width() - side;
            p.setBrush(p.pen().color());
            for (int i = num - 1; i >= 0; i--) {
                p.drawEllipse(x - dot_radius,
                              bottom - (stats[i] - min) * stepY - dot_radius,
                              dot_radius * 2, dot_radius * 2);
                x -= stepX;
            }
        }

        p.setPen(QPen(p.pen().color(), 3, Qt::SolidLine));
        QString smax(msg_max + ": " + QString().number(max / 10) + "," +
                     QString().number(max % 10) + " " + msg_sec);
        QString smin(msg_min + ": " + QString().number(min / 10) + "," +
                     QString().number(min % 10) + " " + msg_sec);
        QString slast(msg_last + ": " + QString().number(last / 10) + "," +
                      QString().number(last % 10) + " " + msg_sec);
        p.drawText(0, fm.ascent(), smax);
        p.drawText(width() / 2 - fm.horizontalAdvance(smin) / 2, fm.ascent(),
                   smin);
        p.drawText(width() - fm.horizontalAdvance(slast), fm.ascent(), slast);
    } else
        p.drawText(width() / 2 - fm.horizontalAdvance(msg_nostat) / 2,
                   height() / 2 + fm.lineSpacing() / 2, msg_nostat);

    QWidget::paintEvent(event);
}

StatsWidget::StatsWidget(Settings *newSettings, QWidget *wgt)
    : QWidget(wgt), settings(newSettings) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}
