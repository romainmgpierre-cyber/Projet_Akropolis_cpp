#include "HexGridWidget.h"
#include <QPainter>
#include <QWheelEvent>
#include <QRadialGradient>
#include <QtMath>
#include <QGraphicsDropShadowEffect>
#include<QTime>
#include<QTimer>
#include "HexCons_Carr_Quart_Place.h"

using namespace Akropolis;

HexGridWidget::HexGridWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    offset = QPointF(width()/2, height()/2);
    setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                  "stop:0 #f5f7fa, stop:1 #e8eef5);");

    // Ombre portée pour effet de profondeur
    auto shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 30));
    shadow->setOffset(0, 5);
    setGraphicsEffect(shadow);
}

void HexGridWidget::setCite(const Cite* cite) {
    citeALire = cite;
    update();
}

QColor HexGridWidget::typeToColor(Couleur c) const {
    switch(c) {
    case Couleur::bleu: return QColor(33, 150, 243); // Habitation
    case Couleur::jaune: return QColor(255, 235, 59); // Place
    case Couleur::rouge: return QColor(244, 67, 54);  // Caserne
    case Couleur::violet: return QColor(156, 39, 176); // Temple
    case Couleur::vert: return QColor(76, 175, 80);   // Parc
    case Couleur::gris: return QColor(158, 158, 158); // Carrière
    default: return Qt::white;
    }
}

void HexGridWidget::drawGradientHex(QPainter& painter, const QPolygonF& poly, const QColor& baseColor) {
    QRectF bounds = poly.boundingRect();
    QRadialGradient gradient(bounds.center(), bounds.width() / 2);

    QColor lightColor = baseColor.lighter(120);
    QColor darkColor = baseColor.darker(110);

    gradient.setColorAt(0, lightColor);
    gradient.setColorAt(0.7, baseColor);
    gradient.setColorAt(1, darkColor);

    painter.setBrush(gradient);
    painter.drawPolygon(poly);
}

void HexGridWidget::dessinerEtoiles(QPainter& painter, QPointF center, int nbEtoiles) {
    const double starSize = 10.0;
    const double spacing = 14.0;
    double startX = center.x() - ((nbEtoiles - 1) * spacing) / 2.0;

    for (int i = 0; i < nbEtoiles; i++) {
        double sx = startX + i * spacing;

        // Dessin d'une étoile à 5 branches
        QPolygonF star;
        for (int j = 0; j < 10; j++) {
            double radius = (j % 2 == 0) ? starSize : starSize * 0.4;
            double angle = (M_PI * 2 * j) / 10.0 - M_PI / 2.0;
            star << QPointF(sx + cos(angle) * radius, center.y() + sin(angle) * radius);
        }

        // Ombre de l'étoile
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 50));
        painter.translate(1, 1);
        painter.drawPolygon(star);
        painter.restore();

        // Étoile avec dégradé
        QRadialGradient starGrad(sx, center.y(), starSize);
        starGrad.setColorAt(0, QColor(255, 223, 0)); // Or brillant
        starGrad.setColorAt(0.5, QColor(255, 215, 0));
        starGrad.setColorAt(1, QColor(255, 165, 0)); // Orange

        painter.setBrush(starGrad);
        painter.setPen(QPen(QColor(218, 165, 32), 1.5)); // Contour or foncé
        painter.drawPolygon(star);
    }
}

QPointF HexGridWidget::cubeToPixel(CoordHex coord) const {
    double effectiveSize = hexSize * zoomFactor;
    double x = effectiveSize  * qSqrt(3) * (coord.getQ() + coord.getR()/2.0);
    double y = effectiveSize  * (3.0/2.0) * coord.getR();
    return QPointF(x, y) + offset;
}

CoordHex HexGridWidget::pixelToHex(QPointF point) const {
    double effectiveSize = hexSize * zoomFactor;
    QPointF p = point - offset;
    double q = (qSqrt(3)/3.0 * p.x() - 1.0/3.0 * p.y()) / effectiveSize;
    double r = (2.0/3.0 * p.y()) / effectiveSize;
    return CoordHex(qRound(q), qRound(r));
}

void HexGridWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.fillRect(rect(), QColor(245, 247, 250));

    // Grille de points en arrière-plan (effet design moderne)
    painter.setPen(QPen(QColor(200, 200, 200, 100), 1));
    for (int x = 0; x < width(); x += 30) {
        for (int y = 0; y < height(); y += 30) {
            painter.drawPoint(x, y);
        }
    }

    if (!citeALire) return;

    for (auto const& [coord, paire] : citeALire->getPlateau()) {
        HexagoneConstruction* hex = paire.first;
        int hauteur = paire.second;
        bool isHovered = (coord == hoveredHex);
        dessinerHexagone(painter, coord, hex, hauteur, isHovered);
    }
    if (tuileFantome) {
        QPointF center = cubeToPixel(posFantome);

        painter.save();
        painter.setOpacity(0.6); // Semi-transparent

        /// Anneau pulsant
        QPen pen(QColor(76, 175, 80), 3);
        pen.setStyle(Qt::DashLine);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        double pulseSize = 20 + 5 * qSin(QTime::currentTime().msec() * 0.01);
        painter.drawEllipse(center, pulseSize, pulseSize);

        // Icône de placement
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(76, 175, 80));
        painter.drawEllipse(center, 15, 15);

        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setBold(true);
        font.setPointSize(12);
        painter.setFont(font);
        painter.drawText(QRectF(center.x()-10, center.y()-10, 20, 20),
                         Qt::AlignCenter, "+");

        painter.restore();
        QTimer::singleShot(50, this, [this](){ update(); });

    }
}

void HexGridWidget::dessinerHexagone(QPainter& painter, CoordHex coord, HexagoneConstruction* hex, int hauteur, bool isHovered) {
    QPointF center = cubeToPixel(coord);
    double effectiveSize = hexSize * zoomFactor;
    QPolygonF poly;
    for (int i = 0; i < 6; ++i) {
        double angle = M_PI / 180 * (60 * i - 30);
        poly << center + QPointF(effectiveSize  * cos(angle), effectiveSize  * sin(angle));
    }

    QColor baseColor = Qt::white;
    int nbEtoiles = 0;

    if (Quartier* q = dynamic_cast<Quartier*>(hex)) {
        baseColor = typeToColor(q->getType().getCouleur());
    }
    else if (Place* p = dynamic_cast<Place*>(hex)) {
        baseColor = typeToColor(p->getType().getCouleur());
        nbEtoiles = p->getNbEtoile();
    }
    else if (Carriere* c = dynamic_cast<Carriere*>(hex)) {
        baseColor = typeToColor(Couleur::gris);
    }
    // Ombre portée
    if (hauteur > 1 || isHovered) {
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 40));
        painter.translate(2, 2);
        painter.drawPolygon(poly);
        painter.restore();
    }

    // Dessin
    if (hauteur > 1) {
        painter.setPen(QPen(baseColor.darker(150), 4));
    } else {
        painter.setPen(QPen(QColor(180, 180, 180), 2));
    }
    drawGradientHex(painter, poly, baseColor);

    if (isHovered) {
        painter.setPen(QPen(QColor(33, 150, 243), 3));
        painter.setBrush(Qt::NoBrush);
        painter.drawPolygon(poly);
    }

    if (nbEtoiles > 0) {
        dessinerEtoiles(painter, center, nbEtoiles);
    }

    if (hauteur > 1) {
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setPointSize(10);
        font.setBold(true);
        painter.setFont(font);

        QRectF rect(center.x() + effectiveSize * 0.4, center.y() - effectiveSize * 0.4, 20, 20);
        painter.setBrush(QColor(0, 0, 0, 150));
        painter.drawEllipse(rect);
        painter.drawText(rect, Qt::AlignCenter, QString::number(hauteur));
    }
}

void HexGridWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        CoordHex hex = pixelToHex(event->position());
        emit hexClicked(hex);
    }
    if (event->button() == Qt::RightButton) {
        isDragging = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);

    }
}

void HexGridWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging) {
        QPoint delta = event->pos() - lastMousePos;
        offset += delta;
        lastMousePos = event->pos();
        update();
    } else {
        // Mise à jour du hover
        CoordHex newHovered = pixelToHex(event->position());
        if (!(newHovered == hoveredHex)) {
            hoveredHex = newHovered;
            emit hexHovered(hoveredHex);
            update();
        }
    }
}

void HexGridWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        isDragging = false;
        setCursor(Qt::OpenHandCursor);
    }
}

void HexGridWidget::wheelEvent(QWheelEvent *event) {
    double delta = event->angleDelta().y() / 1200.0;
    zoomFactor = qBound(0.5, zoomFactor + delta, 2.5);
    update();
}

void HexGridWidget::setTuileFantome(TuileCite* tuile, CoordHex position, int rotation) {
    this->tuileFantome = tuile;
    this->posFantome = position;
    this->rotFantome = rotation;
    update();
}

void HexGridWidget::clearTuileFantome() {
    this->tuileFantome = nullptr;
    update();
}
