#include "HexGridWidget.h"
#include <QPainter>
#include <QtMath>

using namespace Akropolis;

HexGridWidget::HexGridWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true); // Important pour suivre la souris sans cliquer
    offset = QPointF(400, 300); // Centre arbitraire
}

void HexGridWidget::setCite(const Cite* cite) {
    citeALire = cite;
    update(); // Redessine le widget
}

// Conversion des couleurs de votre enum vers Qt
QColor HexGridWidget::typeToColor(Couleur c) const {
    switch(c) {
    case Couleur::BLEU: return QColor(33, 150, 243); // Habitation
    case Couleur::JAUNE: return QColor(255, 235, 59); // Place
    case Couleur::ROUGE: return QColor(244, 67, 54);  // Caserne
    case Couleur::VIOLET: return QColor(156, 39, 176); // Temple
    case Couleur::VERT: return QColor(76, 175, 80);   // Parc
    case Couleur::GRIS: return QColor(158, 158, 158); // Carrière
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

    // Arrondi axial simple (pourrait être amélioré pour plus de précision aux bords)
    return CoordHex(qRound(q), qRound(r));
}

void HexGridWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(240, 240, 240)); // Fond gris clair

    if (!citeALire) return;

    // 1. DESSINER LE PLATEAU EXISTANT
    // On itère sur la map du plateau de la classe Cité
    for (auto const& [coord, paire] : citeALire->getPlateau()) {
        HexagoneConstruction* hex = paire.first;
        int hauteur = paire.second;
        dessinerHexagone(painter, coord, hex, hauteur);
    }

    // 2. DESSINER LA TUILE FANTÔME (SI ACTIVE)
    // Ici, vous ajouteriez la logique pour dessiner la tuile qui suit la souris
}

void HexGridWidget::dessinerHexagone(QPainter& painter, CoordHex coord, HexagoneConstruction* hex, int hauteur) {
    QPointF center = cubeToPixel(coord);
    QPolygonF poly;
    for (int i = 0; i < 6; ++i) {
        double angle = M_PI / 180 * (60 * i - 30);
        poly << center + QPointF(hexSize * cos(angle), hexSize * sin(angle));
    }

    // Couleur de fond selon le Type
    QColor baseColor = typeToColor(hex->getType().getCouleur());

    // Effet de hauteur (plus foncé si plus bas)
    if (hauteur > 1) {
        // Ajouter un effet d'ombre ou bordure épaisse pour la hauteur
        painter.setPen(QPen(Qt::black, 3));
    } else {
        painter.setPen(QPen(Qt::gray, 1));
    }

    painter.setBrush(baseColor);
    painter.drawPolygon(poly);

    // Dessin des étoiles
    if (hex->getEtoiles() > 0) {
        painter.setPen(Qt::black);
        painter.drawText(QRectF(center.x()-10, center.y()-10, 20, 20), Qt::AlignCenter,
                         QString("⭐").repeated(hex->getEtoiles()));
    }

    // Debug : Coordonnées
    // painter.setPen(Qt::black);
    // painter.drawText(center, QString("%1,%2").arg(coord.getQ()).arg(coord.getR()));
}

void HexGridWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        CoordHex hex = pixelToHex(event->position());
        emit hexClicked(hex); // Envoie le signal à la MainWindow
    }
    // Gestion du déplacement (pan) avec clic droit ou milieu
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
