#include "schultewidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <cmath>

constexpr int emptyCell = 999;

int SchulteWidget::getQuad_side() const { return quad_side; }

void SchulteWidget::onThemeChange() {
    eyePixmap.load(":/rcc/" + settings->getTheme().getTheme() +
                   "/eye_icon.png");
    greenDotPixmap.load(":/rcc/" + settings->getTheme().getTheme() +
                        "/greendot_icon.png");

    if (settings->getTheme().getTheme() == "DarkTheme") {
        cellBrush = QColor(0x1A, 0x1A, 0x1A);
        numbersPen = QColor(Qt::white);
        borderPen = QColor(Qt::white);
        borderPen.setWidth(1);
    } else if (settings->getTheme().getTheme() == "GreyTheme") {
        cellBrush = QColor(0x88, 0x88, 0x88);
        numbersPen = QColor(Qt::black);
        borderPen = QColor(Qt::black);
        borderPen.setWidth(3);
    } else if (settings->getTheme().getTheme() == "LightTheme") {
        cellBrush = QColor(0xDD, 0xDD, 0xDD);
        numbersPen = QColor(Qt::black);
        borderPen = QColor(Qt::black);
        borderPen.setWidth(2);
    }
}

void SchulteWidget::onLanguageChange() {}

bool SchulteWidget::event(QEvent *event) {
    if (event->type() == ThemeChangeEvent::type) {
        onThemeChange();
        return true;
    } else if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QWidget::event(event);
}

void SchulteWidget::resizeEvent(QResizeEvent *event) {
    setMaximumHeight(settings->getWindowSize().width());

    border = settings->getLogicalDPI() * 0.025;
    maxSide = std::min(width() - border * 2, height() - border * 2);
    side = maxSide * settings->getTableScale() / 100.0;
    maxCorner = QPointF((width() - (maxSide + border * 2)) / 2.0 + border,
                        (height() - (maxSide + border * 2)) / 2.0 + border);
    corner = QPointF((width() - (side + border * 2)) / 2.0 + border,
                     (height() - (side + border * 2)) / 2.0 + border);

    cellSide = side / quad_side;
    cellborder = settings->getLogicalDPI() * 0.02;

    QWidget::resizeEvent(event);
}

void SchulteWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    side = maxSide * settings->getTableScale() / 100.0;
    corner = QPointF((width() - (side + border * 2)) / 2.0 + border,
                     (height() - (side + border * 2)) / 2.0 + border);
    cellSide = side / quad_side;

    // Central Point
    double picSize = 0;
    double clipX = 0;
    double clipY = 0;
    if (settings->getCentralPointStyle() == CentralPointStyle::EyePic) {
        picSize = std::min(cellSide * 0.8, settings->getLogicalDPI() * 0.2);
        clipX = picSize * 1.2;
        clipY = picSize * 0.8;
        painter.drawPixmap(corner.x() + side / 2 - picSize / 2,
                           corner.y() + side / 2 - picSize / 2, picSize,
                           picSize, eyePixmap);

    } else if (settings->getCentralPointStyle() ==
               CentralPointStyle::GreenDot) {
        picSize = std::min(cellSide * 0.6, settings->getLogicalDPI() * 0.15);
        clipX = picSize * 1.2;
        clipY = picSize * 1.2;
        painter.drawPixmap(corner.x() + side / 2 - picSize / 2,
                           corner.y() + side / 2 - picSize / 2, picSize,
                           picSize, greenDotPixmap);
    }

    // Clip region around central point
    if (settings->getCentralPointStyle() != CentralPointStyle::None) {
        QRegion r1(rect());
        QRegion r2(QRect(corner.x() + side / 2 - clipX / 2,
                         corner.y() + side / 2 - clipY / 2, clipX, clipY),
                   QRegion::Ellipse);
        QRegion r3 = r1.subtracted(r2);
        painter.setClipRegion(r3);
    }

    // Draw quads
    painter.setBrush(cellBrush);

    int tableIndex = 0;
    for (int j = 0; j < quad_side; j++)
        for (int i = 0; i < quad_side; i++) {
            if (quad_side % 2 != 0 &&
                settings->getCentralPointStyle() != CentralPointStyle::None &&
                (i == j) && (i == quad_side / 2)) {
                continue;
            } else {
                if (table_sequence[tableIndex] != emptyCell) {
                    if (!fake &&
                        (table_sequence[tableIndex] == settings->getNumCells()))
                        painter.setPen(borderPen);
                    else
                        painter.setPen(Qt::NoPen);

                    painter.drawRoundedRect(
                        corner.x() + cellborder + i * cellSide,
                        corner.y() + cellborder + j * cellSide,
                        cellSide - cellborder * 2, cellSide - cellborder * 2,
                        (cellSide - cellborder) * 0.05,
                        (cellSide - cellborder) * 0.05);
                }
                tableIndex++;
            }
        }

    // Draw numbers
    if (!fake) {
        painter.setPen(numbersPen);
        QFont font = settings->getFont4();
        if (settings->getFontMaxSize() == 0)
            font.setPixelSize(cellSide / 2);
        else
            font.setPixelSize(
                std::min<int>(cellSide / 2, settings->getFontMaxSize()));
        painter.setFont(font);
        QFontMetrics fm(font);

        tableIndex = 0;
        for (int j = 0; j < quad_side; j++)
            for (int i = 0; i < quad_side; i++) {
                if (quad_side % 2 != 0 &&
                    settings->getCentralPointStyle() !=
                        CentralPointStyle::None &&
                    (i == j) && (i == quad_side / 2)) {
                    continue;
                } else {
                    if (table_sequence[tableIndex] != emptyCell) {
                        QString text =
                            QString::number(table_sequence[tableIndex]);
                        painter.drawText(
                            corner.x() + i * cellSide + cellSide / 2 -
                                fm.horizontalAdvance(text) / 2,
                            corner.y() + j * cellSide + cellSide / 2 -
                                fm.descent() + fm.lineSpacing() / 2,
                            text);
                    }
                    tableIndex++;
                }
            }
    }

    QWidget::paintEvent(event);
}

double length(QPointF point) {
    return std::sqrt(point.x() * point.x() + point.y() * point.y());
}

void SchulteWidget::mousePressEvent(QMouseEvent *event) {
    if (!fake) {
        int min = *std::min_element(std::begin(table_sequence),
                                    std::end(table_sequence));

        int i = std::floor((event->pos().x() - corner.x()) / cellSide);
        int j = std::floor((event->pos().y() - corner.y()) / cellSide);

        if (i < quad_side && i >= 0 && j < quad_side && j >= 0 &&
            !(quad_side % 2 != 0 &&
              settings->getCentralPointStyle() != CentralPointStyle::None &&
              (i == j) && (i == quad_side / 2))) {

            int index = j * quad_side + i;
            if (quad_side % 2 != 0 &&
                settings->getCentralPointStyle() != CentralPointStyle::None &&
                ((j > quad_side / 2) ||
                 ((j == quad_side / 2) && (i > quad_side / 2))))
                index--;

            if (table_sequence[index] == min) {
                emit pressedNumber(table_sequence[index]);
                if (table_sequence[index] == settings->getNumCells())
                    emit done();
                else
                    settings->getSound().click();
                table_sequence[index] = emptyCell;
                update();
            }
        }
    }

    return QWidget::mousePressEvent(event);
}

void SchulteWidget::mouseMoveEvent(QMouseEvent *event) {

    return QWidget::mouseMoveEvent(event);
}

void SchulteWidget::mouseReleaseEvent(QMouseEvent *event) {

    return QWidget::mouseReleaseEvent(event);
}

SchulteWidget::SchulteWidget(Settings *newSettings, bool fake, QWidget *wgt)
    : QWidget(wgt), settings(newSettings), fake(fake) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setContentsMargins(0, 0, 0, 0);
}

void SchulteWidget::generate(int seed) {

    // find Max elemts and Quad side in the table including empty cells
    QVector<int> elements;
    int max_elements;
    if (settings->getCentralPointStyle() == CentralPointStyle::None)
        elements = {4, 9, 16, 25, 36, 49, 64, 81, 100, 121};
    else
        elements = {4, 8, 16, 24, 36, 48, 64, 80, 100, 120};
    auto it = std::lower_bound(std::begin(elements), std::end(elements),
                               settings->getNumCells());
    max_elements = (it != std::end(elements) ? *it : elements.last());
    quad_side = 2 + it - std::begin(elements);

    // fill table with numbers, where 0 is empty cell
    table_sequence.resize(max_elements);
    table_sequence.fill(emptyCell);
    for (int i = 1; i <= settings->getNumCells(); i++)
        table_sequence[i - 1] = i;

    // shuffle numbers and empty cells
    generator.seed(seed);
    std::shuffle(std::begin(table_sequence), std::end(table_sequence),
                 generator);

    update();
}
