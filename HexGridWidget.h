#ifndef HEXGRIDWIDGET_H
#define HEXGRIDWIDGET_H

#include <QWidget>
#include <QMap>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include "Cite.h"
#include "CoordHex.h"

class HexGridWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HexGridWidget(QWidget *parent = nullptr);
    void setCite(const Akropolis::Cite* cite);
    void setTuileFantome(Akropolis::TuileCite* tuile, Akropolis::CoordHex position);
    void clearTuileFantome();
    void setZoomLevel(double zoom);
    void animateHexPlacement(Akropolis::CoordHex coord);
    void rotateFantome();
    void setFantomeRotation(int rotation); 
    int getFantomeRotation() const { return rotFantome; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

signals:
    void hexClicked(Akropolis::CoordHex coord);
    void hexHovered(Akropolis::CoordHex coord);

private:
    const Akropolis::Cite* citeALire = nullptr;
    Akropolis::TuileCite* tuileFantome = nullptr;
    Akropolis::CoordHex posFantome;
    Akropolis::CoordHex hoveredHex;
    int rotFantome = 0;

    double hexSize = 40.0;
    double zoomFactor = 1.0;
    QPointF offset;
    QPointF velocity;
    bool isDragging = false;
    QPoint lastMousePos;

    // Animation
    QPropertyAnimation* placeAnimation = nullptr;
    double animationProgress = 0.0;
    Akropolis::CoordHex animatedHex;

    // Helpers de dessin
    QPointF cubeToPixel(Akropolis::CoordHex coord) const;
    Akropolis::CoordHex pixelToHex(QPointF point) const;
    QColor typeToColor(Akropolis::Couleur c) const;
    void dessinerHexagone(QPainter& painter, Akropolis::CoordHex coord,
                          Akropolis::HexagoneConstruction* hex, int hauteur,
                          bool isHovered = false);
    void dessinerEtoiles(QPainter& painter, QPointF center, int nbEtoiles);
    void drawGradientHex(QPainter& painter, const QPolygonF& poly, const QColor& baseColor);
};

#endif
