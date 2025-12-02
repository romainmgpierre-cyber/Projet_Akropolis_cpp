#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QProgressBar>
#include <QFrame>
#include "HexGridWidget.h"
#include "Partie_Variante.h"

enum class EtatJeu {
    CHOIX_RIVIERE,
    PLACEMENT_TUILE,
    FIN_PARTIE
};


class RiviereWidget : public QWidget {
    Q_OBJECT
public:
    explicit RiviereWidget(QWidget *parent = nullptr);
    void setChoixTuile(Akropolis::ChoixTuile* choix);

signals:
    void tuileChoisie(int index); // Émet l'index de la tuile cliquée

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Akropolis::ChoixTuile* choixActuel = nullptr;
    double scale = 30.0; // Taille des hexagones dans la rivière

    // Helper dessin
    void dessinerTuile(QPainter& painter, Akropolis::TuileCite* tuile, int x, int y);
    QColor typeToColor(Akropolis::Couleur c) const;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onHexClicked(Akropolis::CoordHex coord);
    void onRotationClicked();
    void onPasserTourClicked();
    void onTuileChoisie(int index);

private:
    void mettreAJourInterface();

    HexGridWidget *gridWidget;
    QLabel *infoLabel;
    QPushButton *btnRotation;

    Akropolis::Partie* partie;

    // État temporaire pour le tour en cours
    Akropolis::TuileCite* tuileSelectionnee = nullptr;
    int rotationActuelle = 0;
    EtatJeu etatActuel;
    RiviereWidget *riviereWidget;
};

#endif
