#include "MainWindow.h"
#include <QMessageBox>

using namespace Akropolis;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 1. Initialisation du Jeu
    partie = new Partie(1, ModeJeu::SOLO);
    partie->ajouterJoueur("Joueur GUI");
    partie->initialiserTuiles();
    // partie->lancerPartie(); // ATTENTION: Ne pas appeler ça si ça contient une boucle while(true) !

    // 2. Initialisation
    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Partie Gauche : La Grille
    gridWidget = new HexGridWidget;
    mainLayout->addWidget(gridWidget, 1); // Stretch factor 1

    // Partie Droite : Contrôles
    QVBoxLayout *controlsLayout = new QVBoxLayout;

    infoLabel = new QLabel("Au tour de : Joueur 1");
    infoLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    controlsLayout->addWidget(infoLabel);

    btnRotation = new QPushButton("Pivoter Tuile (R)");
    controlsLayout->addWidget(btnRotation);

    mainLayout->addLayout(controlsLayout, 0);

    // 3. Connexions
    connect(gridWidget, &HexGridWidget::hexClicked, this, &MainWindow::onHexClicked);
    connect(btnRotation, &QPushButton::clicked, this, &MainWindow::onRotationClicked);

    // Initialiser la vue avec la cité du premier joueur
    gridWidget->setCite(partie->getJoueurActuel()->getCite());
}

MainWindow::~MainWindow() {
    delete partie;
}

void MainWindow::onRotationClicked() {
    // Logique de rotation de la tuile en main (à implémenter dans Partie ou ici)
    // tuileSelectionnee->rotationHoraire();
    gridWidget->update();
}

void MainWindow::onHexClicked(CoordHex coord) {
    try {
        Joueur* j = partie->getJoueurActuel();
        Cite* cite = j->getCite();

        // Exemple simplifié : On essaie de placer la tuile courante là où on clique
        // Il faudrait récupérer la tuile depuis la main du joueur (ChoixTuile)

        // adapter la méthode placerTuile pour qu'elle prenne
        // une coordonnée brute ou construire un objet CoupPossible ici.

        // Simulation d'un coup (A adapter selon Pioche)
        // cite->placerTuile(tuileSelectionnee, coord, rotationActuelle);

        QMessageBox::information(this, "Action",
                                 QString("Clic en Q=%1, R=%2").arg(coord.getQ()).arg(coord.getR()));

        gridWidget->update(); // Redessiner
        mettreAJourInterface(); // Passer au joueur suivant si nécessaire

    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Erreur", e.what());
    }
}

void MainWindow::mettreAJourInterface() {
    Joueur* j = partie->getJoueurActuel();
    infoLabel->setText(QString("Cité de : %1\nScore: %2")
                           .arg(QString::fromStdString(j->getNom()))
                           .arg(j->calculerScore()));

    // On change la cité affichée dans le widget
    gridWidget->setCite(j->getCite());
}
void MainWindow::onPasserTourClicked() {
    // Pour l'instant, on ne fait rien ou on met à jour l'interface
    // C'est ici que vous mettrez la logique : partie->joueurSuivant();
    mettreAJourInterface();
}
