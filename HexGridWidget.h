#ifndef HEXGRIDWIDGET_H
#define HEXGRIDWIDGET_H

#include <QWidget>
#include <QMap>
#include <QMouseEvent>
#include "Cite.h"
#include "CoordHex.h"

class HexGridWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HexGridWidget(QWidget *parent = nullptr);

    // pointeur vers la cité à dessiner
    void setCite(const Akropolis::Cite* cite);

    // Pour visualiser la tuile en cours de placement (fantôme)
    void setTuileFantome(Akropolis::TuileCite* tuile, Akropolis::CoordHex position, int rotation);
    void clearTuileFantome();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override; // Pour le survol
    void mouseReleaseEvent(QMouseEvent *event) override;
signals:
    // Signal émis quand on clique sur une case (q, r)
    void hexClicked(Akropolis::CoordHex coord);

private:
    const Akropolis::Cite* citeALire = nullptr;

    // Variables pour la "Tuile Fantôme" (visualisation avant clic)
    Akropolis::TuileCite* tuileFantome = nullptr;
    Akropolis::CoordHex posFantome;
    int rotFantome = 0;

    double hexSize = 35.0;
    QPointF offset; // Pour scroller/centrer la vue
    bool isDragging = false;
    QPoint lastMousePos;

    // Helpers de dessin
    QPointF cubeToPixel(Akropolis::CoordHex coord) const;
    Akropolis::CoordHex pixelToHex(QPointF point) const;
    QColor typeToColor(Akropolis::Couleur c) const;
    void dessinerHexagone(QPainter& painter, Akropolis::CoordHex coord, Akropolis::HexagoneConstruction* hex, int hauteur);
};

#endif
