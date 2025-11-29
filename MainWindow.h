#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "HexGridWidget.h"
#include "Partie_Variante.h"

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

private:
    void mettreAJourInterface();

    HexGridWidget *gridWidget;
    QLabel *infoLabel;
    QPushButton *btnRotation;

    Akropolis::Partie* partie;

    // État temporaire pour le tour en cours
    Akropolis::TuileCite* tuileSelectionnee = nullptr;
    int rotationActuelle = 0;
};
#endif
