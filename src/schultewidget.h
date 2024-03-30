#pragma once

#include "settings.h"
#include <QPen>
#include <QWidget>
#include <random>

class SchulteWidget : public QWidget {
    Q_OBJECT

    Settings *settings;
    bool fake;

    TableStyle tableStyle;
    CentralPointStyle centralPointStyle;

    QVector<int> table_sequence;
    int quad_side;

    double maxSide;
    double side;
    QPointF maxCorner;
    QPointF corner;
    double border;
    double cellSide;
    double cellborder;

    std::default_random_engine generator;
    QPixmap eyePixmap, greenDotPixmap;
    QBrush cellBrush;
    QPen numbersPen;
    QPen borderPen;

    void onThemeChange();
    void onLanguageChange();

  protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

  public:
    SchulteWidget(Settings *newSettings, bool fake, QWidget *wgt = nullptr);

    int getQuad_side() const;

  public slots:
    void generate(int seed);

  signals:
    void done();
    void pressedNumber(int);
};
