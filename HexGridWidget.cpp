#include "HexGridWidget.h"
#include <QPainter>
#include <QtMath>
// Il faut inclure les sous-classes pour pouvoir les détecter
#include "HexCons_Carr_Quart_Place.h"

using namespace Akropolis;

HexGridWidget::HexGridWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    offset = QPointF(400, 300); // Centre arbitraire initial
}

void HexGridWidget::setCite(const Cite* cite) {
    citeALire = cite;
    update();
}

QColor HexGridWidget::typeToColor(Couleur c) const {
    // CORRECTION : Utilisation des minuscules comme dans votre GameExcep_Enums.h
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

QPointF HexGridWidget::cubeToPixel(CoordHex coord) const {
    double x = hexSize * qSqrt(3) * (coord.getQ() + coord.getR()/2.0);
    double y = hexSize * (3.0/2.0) * coord.getR();
    return QPointF(x, y) + offset;
}

CoordHex HexGridWidget::pixelToHex(QPointF point) const {
    QPointF p = point - offset;
    double q = (qSqrt(3)/3.0 * p.x() - 1.0/3.0 * p.y()) / hexSize;
    double r = (2.0/3.0 * p.y()) / hexSize;
    return CoordHex(qRound(q), qRound(r));
}

void HexGridWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(240, 240, 240));

    if (!citeALire) return;

    for (auto const& [coord, paire] : citeALire->getPlateau()) {
        HexagoneConstruction* hex = paire.first;
        int hauteur = paire.second;
        dessinerHexagone(painter, coord, hex, hauteur);
    }
}

void HexGridWidget::dessinerHexagone(QPainter& painter, CoordHex coord, HexagoneConstruction* hex, int hauteur) {
    QPointF center = cubeToPixel(coord);
    QPolygonF poly;
    for (int i = 0; i < 6; ++i) {
        double angle = M_PI / 180 * (60 * i - 30);
        poly << center + QPointF(hexSize * cos(angle), hexSize * sin(angle));
    }

    // --- CORRECTION MAJEURE : DÉTECTION DU TYPE ---
    QColor baseColor = Qt::white;
    int nbEtoiles = 0;

    // On vérifie le type réel de l'objet (Polymorphisme)
    if (Quartier* q = dynamic_cast<Quartier*>(hex)) {
        baseColor = typeToColor(q->getType().getCouleur());
    }
    else if (Place* p = dynamic_cast<Place*>(hex)) {
        baseColor = typeToColor(p->getType().getCouleur());
        nbEtoiles = p->getNbEtoile(); // CORRECTION : getNbEtoile() et non getEtoiles()
    }
    else if (Carriere* c = dynamic_cast<Carriere*>(hex)) {
        baseColor = typeToColor(Couleur::gris);
    }

    // Dessin
    if (hauteur > 1) {
        painter.setPen(QPen(Qt::black, 3)); // Bordure épaisse si en hauteur
    } else {
        painter.setPen(QPen(Qt::gray, 1));
    }

    painter.setBrush(baseColor);
    painter.drawPolygon(poly);

    // Dessin des étoiles si nécessaire
    if (nbEtoiles > 0) {
        painter.setPen(Qt::black);
        painter.drawText(QRectF(center.x()-15, center.y()-10, 30, 20), Qt::AlignCenter,
                         QString("⭐").repeated(nbEtoiles));
    }

    // Debug : Coordonnées
    painter.setPen(Qt::darkGray);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);
    // painter.drawText(center + QPointF(0, 15), QString("%1,%2").arg(coord.getQ()).arg(coord.getR()));
}

void HexGridWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        CoordHex hex = pixelToHex(event->position());
        emit hexClicked(hex);
    }
    if (event->button() == Qt::RightButton) {
        isDragging = true;
        lastMousePos = event->pos();
    }
}

void HexGridWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging) {
        QPoint delta = event->pos() - lastMousePos;
        offset += delta;
        lastMousePos = event->pos();
        update();
    }
}

void HexGridWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        isDragging = false;
    }
}
