#include "MainWindow.h"
#include <QPainter>
#include <QMouseEvent>
#include "HexCons_Carr_Quart_Place.h"
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
    mainLayout->addWidget(gridWidget, 1);

    // Partie Droite : Contrôles
    QWidget *sidePanel = new QWidget;
    QVBoxLayout *sideLayout = new QVBoxLayout(sidePanel);

    infoLabel = new QLabel("Bienvenue dans Akropolis");
    infoLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    sideLayout->addWidget(infoLabel);

    // Widget Rivière
    riviereWidget = new RiviereWidget;
    sideLayout->addWidget(new QLabel("--- Rivière (Marché) ---"));
    sideLayout->addWidget(riviereWidget);

    btnRotation = new QPushButton("Pivoter (R)");
    btnRotation->setEnabled(false); // Désactivé tant qu'on n'a pas choisi de tuile
    sideLayout->addWidget(btnRotation);

    QPushButton* btnPasser = new QPushButton("Debug: Passer Tour"); // Au cas où
    sideLayout->addWidget(btnPasser);

    sideLayout->addStretch();
    mainLayout->addWidget(sidePanel, 0);

    // 3. Connexions
    connect(gridWidget, &HexGridWidget::hexClicked, this, &MainWindow::onHexClicked);
    connect(btnRotation, &QPushButton::clicked, this, &MainWindow::onRotationClicked);
    connect(riviereWidget, &RiviereWidget::tuileChoisie, this, &MainWindow::onTuileChoisie);

    // 4. Démarrage
    etatActuel = EtatJeu::CHOIX_RIVIERE;
    mettreAJourInterface();
}

MainWindow::~MainWindow() {
    delete partie;
}
void MainWindow::onTuileChoisie(int index) {
    if (tuileSelectionnee) return; // Déjà une tuile en main

    Joueur* j = partie->getJoueurActuel();
    ChoixTuile* choix = partie->getChoixTuile();

    int cout = choix->calculerCout(index);
    if (j->peutPayerPierres(cout)) {
        j->retirerPierres(cout);

        // On récupère la tuile et on la sort de la rivière
        tuileSelectionnee = choix->choisirTuile(j, index);
        rotationActuelle = 0;

        // Feedback visuel : on montre le fantôme sur la grille
        gridWidget->setTuileFantome(tuileSelectionnee, CoordHex(0,0), 0);

        mettreAJourInterface();
    } else {
        QMessageBox::warning(this, "Pas assez de pierres", "Vous ne pouvez pas payer cette tuile.");
    }
}
void MainWindow::onRotationClicked() {
    // Logique de rotation de la tuile en main (à implémenter dans Partie ou ici)
    // tuileSelectionnee->rotationHoraire();
    gridWidget->update();
}

void MainWindow::onHexClicked(CoordHex coord) {
    if (!tuileSelectionnee) return;

    Joueur* j = partie->getJoueurActuel();
    Cite* cite = j->getCite();
    try {

        Cite::CoupPossible coup;
        coup.ancre = coord;
        coup.rotation = 0;
        auto coups = cite->genererCoupsValides(*tuileSelectionnee);
        bool valide = false;
        for(auto& c : coups) {
            if(c.ancre == coord) { // On vérifie juste la position, la rotation est gérée manuellement
                // On applique le coup validé par le moteur
                cite->placerTuile(tuileSelectionnee, c);
                valide = true;
                break;
            }
        }

        if(valide) {
            tuileSelectionnee = nullptr;
            gridWidget->clearTuileFantome();
            partie->passerTour();
            mettreAJourInterface();
        } else {
            // Feedback silencieux ou sonore (coup invalide)
            QMessageBox::information(this, "Info", "Placement invalide ici.");
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Erreur", e.what());
    }
}

void MainWindow::mettreAJourInterface() {
    Joueur* j = partie->getJoueurActuel();
    infoLabel->setText(QString("Tour de : %1\nPierres: %2\nScore: %3\nÉtat: %4")
                           .arg(QString::fromStdString(j->getNom()))
                           .arg(j->getNbPierres())
                           .arg(j->calculerScore())
                           .arg(etatActuel == EtatJeu::CHOIX_RIVIERE ? "Choisir une tuile" : "Placer la tuile"));

    // On change la cité affichée dans le widget
    gridWidget->setCite(j->getCite());

    // Update Rivière
    partie->remplirChoixTuile();
    riviereWidget->setChoixTuile(partie->getChoixTuile());

    // Gestion activation boutons
    btnRotation->setEnabled(etatActuel == EtatJeu::PLACEMENT_TUILE);
    riviereWidget->setEnabled(etatActuel == EtatJeu::CHOIX_RIVIERE);
}
void MainWindow::onPasserTourClicked() {
    // Pour l'instant, on ne fait rien ou on met à jour l'interface
    // C'est ici que vous mettrez la logique : partie->joueurSuivant();
    mettreAJourInterface();
}


RiviereWidget::RiviereWidget(QWidget *parent) : QWidget(parent) {
    setMinimumHeight(150);
}

void RiviereWidget::setChoixTuile(Akropolis::ChoixTuile* choix) {
    choixActuel = choix;
    update();
}

void RiviereWidget::mousePressEvent(QMouseEvent *event) {
    if (!choixActuel) return;
    // Détection simple : on divise la largeur par le nombre de tuiles
    int nbTuiles = choixActuel->getTuilesDisponibles().size();
    if (nbTuiles == 0) return;

    int largeurCase = width() / nbTuiles;
    int index = static_cast<int>(event->position().x()) / largeurCase;
    if (index >= 0 && index < nbTuiles) {
        emit tuileChoisie(index);
    }
}

QColor RiviereWidget::typeToColor(Akropolis::Couleur c) const {
    // Même logique de couleur que HexGridWidget, simplifiée ici
    switch(c) {
    case Akropolis::Couleur::bleu: return QColor(33, 150, 243);
    case Akropolis::Couleur::jaune: return QColor(255, 235, 59);
    case Akropolis::Couleur::rouge: return QColor(244, 67, 54);
    case Akropolis::Couleur::violet: return QColor(156, 39, 176);
    case Akropolis::Couleur::vert: return QColor(76, 175, 80);
    case Akropolis::Couleur::gris: return QColor(158, 158, 158);
    default: return Qt::white;
    }
}

void RiviereWidget::dessinerTuile(QPainter& painter, Akropolis::TuileCite* tuile, int xOffset, int yOffset) {
    // Dessin simplifié des 3 hexagones d'une tuile (forme triangle)
    // Hex 0 (0,0), Hex 1 (0,-1), Hex 2 (-1, 0) par exemple, selon votre modèle
    // Ici on fait une représentation visuelle "à plat" pour l'interface

    QPoint offsets[3] = {QPoint(0,0), QPoint(25, -15), QPoint(50, 0)}; // Position arbitraire pour joli rendu

    for(int i=0; i<3; i++) {
        Akropolis::HexagoneConstruction* hex = tuile->getHexagone(i);
        QColor col = Qt::white;

        if (auto* q = dynamic_cast<Akropolis::Quartier*>(hex)) col = typeToColor(q->getType().getCouleur());
        else if (auto* p = dynamic_cast<Akropolis::Place*>(hex)) col = typeToColor(p->getType().getCouleur());
        else if (dynamic_cast<Akropolis::Carriere*>(hex)) col = typeToColor(Akropolis::Couleur::gris);

        painter.setBrush(col);
        painter.drawEllipse(xOffset + offsets[i].x(), yOffset + offsets[i].y(), 20, 20);
    }
}

void RiviereWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), QColor(220, 220, 220)); // Fond gris clair

    if (!choixActuel) return;

    auto tuiles = choixActuel->getTuilesDisponibles();
    if (tuiles.empty()) return;

    int largeurCase = width() / tuiles.size();

    for(size_t i=0; i < tuiles.size(); ++i) {
        int x = i * largeurCase + 20;
        int y = height() / 2;

        dessinerTuile(painter, tuiles[i], x, y);

        // Afficher le coût
        int cout = choixActuel->calculerCout(i);
        painter.setPen(Qt::black);
        painter.drawText(x, y + 40, QString("Coût: %1").arg(cout));
    }
}
