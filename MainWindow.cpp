#include "MainWindow.h"
#include <QPainter>
#include <QMouseEvent>
#include "HexCons_Carr_Quart_Place.h"
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QInputDialog>
#include <QString>
#include <QTimer>

using namespace Akropolis;
RiviereWidget::RiviereWidget(QWidget *parent) : QWidget(parent) {
    setMinimumHeight(200);
    setMouseTracking(true);

    setStyleSheet(
        "RiviereWidget {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "              stop:0 #ffffff, stop:1 #f5f7fa);"
        "  border: 2px solid #e0e0e0;"
        "  border-radius: 12px;"
        "}"
        );
}

void RiviereWidget::setChoixTuile(Akropolis::ChoixTuile* choix) {
    choixActuel = choix;
    update();
}

void RiviereWidget::enterEvent(QEnterEvent *event) {
    setCursor(Qt::PointingHandCursor);
}

void RiviereWidget::leaveEvent(QEvent *event) {
    hoveredIndex = -1;
    update();
}

void RiviereWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!choixActuel) return;

    int nbTuiles = choixActuel->getTuilesDisponibles().size();
    if (nbTuiles == 0) return;

    int largeurCase = width() / nbTuiles;
    int newIndex = static_cast<int>(event->position().x()) / largeurCase;

    if (newIndex != hoveredIndex && newIndex >= 0 && newIndex < nbTuiles) {
        hoveredIndex = newIndex;
        update();
    }
}


void RiviereWidget::mousePressEvent(QMouseEvent *event) {
    if (!choixActuel) return;
    int nbTuiles = choixActuel->getTuilesDisponibles().size();
    if (nbTuiles == 0) return;

    int largeurCase = width() / nbTuiles;
    int index = static_cast<int>(event->position().x()) / largeurCase;
    if (index >= 0 && index < nbTuiles) {
        emit tuileChoisie(index);
    }
}

QColor RiviereWidget::typeToColor(Couleur c) const {
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

const double HEX_SIZE_RIV = 15.0;
const double HEX_SPACING = HEX_SIZE_RIV * 1.732;
const double TUILE_WIDTH_ESTIMEE = HEX_SPACING * 3.5;

void RiviereWidget::dessinerTuile(QPainter& painter, TuileCite* tuile, int xCenterOffset, int yCenter, bool hovered) {

   
    QPointF offsets[3] = {
        QPointF(0, 0),                        // Hexagone 0 (Ancre/Pivot)
        QPointF(-HEX_SPACING, 0),             // Hexagone 1 (Gauche)
        QPointF(-HEX_SPACING / 2.0, HEX_SIZE_RIV * 1.5) // Hexagone 2 (Bas-Gauche)
    };

    // Le point le plus à gauche du dessin est -HEX_SPACING.
    const double L_SHAPE_MIDPOINT_X = -HEX_SPACING / 2.0;

    
    const double MARGIN_OFFSET_RIGHT = HEX_SPACING *1.5; // Décaler d'un demi-espacement

    // Décalage final pour centrer la forme et ajouter la marge droite
    const double drawingOffsetX = -L_SHAPE_MIDPOINT_X + MARGIN_OFFSET_RIGHT;

    double drawingOffsetY = -HEX_SIZE_RIV * 0.75; // Ajustement vertical


    //  Dessin de chaque hexagone de la tuile
    for(int i = 0; i < 3; i++) {
        HexagoneConstruction* hex = tuile->getHexagone(i);
        QColor col = Qt::white;

        // Détermination de la couleur (code omis pour la clarté)
        if (auto* q = dynamic_cast<Quartier*>(hex))
            col = typeToColor(q->getType().getCouleur());
        else if (auto* p = dynamic_cast<Place*>(hex))
            col = typeToColor(p->getType().getCouleur());
        else if (dynamic_cast<Carriere*>(hex))
            col = typeToColor(Couleur::gris);

        // Calcul du centre absolu
        QPointF center = QPointF(xCenterOffset + offsets[i].x() + drawingOffsetX,
                                 yCenter + offsets[i].y() + drawingOffsetY);

        // --- Dessin de la forme (Hexagone) ---
        QPolygonF poly;
        for (int j = 0; j < 6; ++j) {
            double angle_deg = 60 * j - 30;
            double angle_rad = M_PI / 180 * angle_deg;
            poly << center + QPointF(HEX_SIZE_RIV * cos(angle_rad), HEX_SIZE_RIV * sin(angle_rad));
        }

        // Ombre / Effet 3D (code omis)
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 40));
        painter.drawPolygon(poly.translated(1, 1));

        // Hexagone avec dégradé (code omis)
        QRadialGradient grad(center, HEX_SIZE_RIV);
        grad.setColorAt(0, col.lighter(120));
        grad.setColorAt(1, col);

        painter.setBrush(grad);
        painter.setPen(QPen(col.darker(130), hovered ? 3 : 2));
        painter.drawPolygon(poly);

        // Affichage des étoiles (code omis)
        if (auto* p = dynamic_cast<Place*>(hex)) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(255, 215, 0));
            painter.drawEllipse(center, 5, 5);
            painter.setPen(Qt::black);
            painter.drawText(QRectF(center.x() - 10, center.y() - 10, 20, 20),
                             Qt::AlignCenter, QString::number(p->getNbEtoile()));
        }
    }
}
void RiviereWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(220, 220, 220)); // Fond gris clair

    // Titre
    painter.setPen(QColor(60, 60, 60));
    QFont titleFont = painter.font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 25, "Marché (Rivière)");

    if (!choixActuel || choixActuel->getTuilesDisponibles().empty()) {
        painter.drawText(rect(), Qt::AlignCenter, "Aucune tuile disponible");
        return;
    };

    auto tuiles = choixActuel->getTuilesDisponibles();
    int nbTuiles = tuiles.size();

    // Largeur totale disponible moins les marges
    int totalWidth = width() - 20;

    // Largeur allouée par tuile, incluant l'espace entre elles
    double largeurParTuile = totalWidth / (double)nbTuiles;

    // Position de départ (pour centrer la rivière)
    double currentX = 10;

    for(size_t i = 0; i < nbTuiles; ++i) {

        // Calculer le centre de la zone de la tuile
        double xCenter = currentX + (largeurParTuile / 2.0);
        double yCenter = height() / 2.0;

        // Décalage pour aligner le centre de la tuile dessinée sur le centre de la zone allouée
        double drawingX = xCenter - (TUILE_WIDTH_ESTIMEE / 2.0);

        bool hovered = (static_cast<int>(i) == hoveredIndex);

        // Cadre de sélection
        if (hovered) {
            painter.setPen(QPen(QColor(33, 150, 243), 3));
            painter.setBrush(QColor(33, 150, 243, 30));
            // Le cadre couvre toute la zone allouée
            painter.drawRoundedRect(currentX, 40, largeurParTuile, height() - 50, 8, 8);
        }

        // Dessin de la tuile
        dessinerTuile(painter, tuiles[i], (int)drawingX, (int)yCenter, hovered);

        // Afficher le coût
        int cout = choixActuel->calculerCout(i);

        // Position du coût: centré horizontalement dans la zone (près du bas)
        double costX = xCenter - 20;
        double costY = height() - 40;

        painter.setPen(Qt::NoPen);
        painter.setBrush(cout > 0 ? QColor(244, 67, 54) : QColor(76, 175, 80));
        painter.drawRoundedRect(costX, costY, 40, 24, 12, 12);

        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(QRectF(costX, costY, 40, 24), Qt::AlignCenter,
                         QString("%1").arg(cout));

        currentX += largeurParTuile;
    }
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 1. Initialisation du Jeu :
    // Choix du Mode (Solo ou Multi)
    QStringList modes;
    modes << "Solo (vs IA)" << "Multijoueur (Local)";
    bool ok;
    QString modeChoisi = QInputDialog::getItem(this, "Bienvenue sur Akropolis",
        "Choisissez le mode de jeu :",
        modes, 0, false, &ok);
    if (!ok) modeChoisi = "Solo (vs IA)";
    if (modeChoisi == "Solo (vs IA)") {
        // --- CONFIGURATION SOLO ---
        partie = new Partie(1, ModeJeu::SOLO);

        // Ajout du joueur humain
        partie->ajouterJoueur("Joueur Humain", 1);

        // Ajout de l'IA (Illustre Constructeur) - paramètre estIA = true
        partie->ajouterJoueur("Illustre Constructeur", 2, true);

        // Choix difficulté (pour le score de l'IA)
        QStringList diffs; diffs << "Facile" << "Moyen" << "Difficile";
        QString diff = QInputDialog::getItem(this, "Difficulté", "Niveau de l'IA :", diffs, 1, false);

        if (diff == "Difficile") partie->setDifficulte(NiveauDifficulte::DIFFICILE);
        else if (diff == "Moyen") partie->setDifficulte(NiveauDifficulte::MOYEN);
        else partie->setDifficulte(NiveauDifficulte::FACILE);}
    //Choix du nombre de joueurs
    else{
        partie = new Partie(1, ModeJeu::SOLO);
        int nbJoueurs = QInputDialog::getInt(this, "Multijoueurs",
            "Nombre de joueurs (2-4) :",
            2, 2, 4, 1, &ok);
        if (!ok) nbJoueurs = 2; // Par défaut si l'utilisateur annule

    // Création dynamique des joueurs
        for (int i = 0; i < nbJoueurs; ++i) {
            std::string nom = "Joueur " + std::to_string(i + 1);
            partie->ajouterJoueur(nom, i + 1);}
    }
    partie->initialiserTuiles();

    // 2. Initialisation
    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    layoutPrincipal = new QHBoxLayout(centralWidget);
    layoutPrincipal->setSpacing(20);
    layoutPrincipal->setContentsMargins(20, 20, 20, 20);

    // Partie Gauche : La Grille
    gridWidget = new HexGridWidget;
    gridWidget->setMinimumSize(600, 600);
    layoutPrincipal->addWidget(gridWidget, 2);

    // Partie Droite : Contrôles
    QWidget *sidePanel = new QWidget;
    sidePanel->setMaximumWidth(350);
    QVBoxLayout *sideLayout = new QVBoxLayout(sidePanel);
    sideLayout->setSpacing(15);

    // === PANEL JOUEUR ===
    panelJoueur = new QFrame;
    panelJoueur->setStyleSheet(
        "QFrame {"
        "  background: white;"
        "  border: 2px solid #e0e0e0;"
        "  border-radius: 12px;"
        "  padding: 15px;"
        "}"
        );
    QVBoxLayout *panelLayout = new QVBoxLayout(panelJoueur);


    infoLabel = new QLabel("Info Joueur");
    infoLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50;");
    panelLayout->addWidget(infoLabel);

    statutLabel = new QLabel("Choisissez une tuile");
    statutLabel->setStyleSheet("font-size: 14px; color: #7f8c8d;");
    panelLayout->addWidget(statutLabel);

    progressBar = new QProgressBar;
    int nbPilesTotal = partie->getPioche()->getNbPilesRestantes();
    progressBar->setRange(0, nbPilesTotal);
    progressBar->setValue(0);
    progressBar->setStyleSheet(
        "QProgressBar {"
        "  border: 2px solid #e0e0e0;"
        "  border-radius: 8px;"
        "  text-align: center;"
        "  height: 25px;"
        "}"
        "QProgressBar::chunk {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "              stop:0 #4CAF50, stop:1 #8BC34A);"
        "  border-radius: 6px;"
        "}"
        );
    progressBar->setFormat("Progression de la partie");
    panelLayout->addWidget(progressBar);

    sideLayout->addWidget(panelJoueur);

    // Widget Rivière
    QLabel *labelRiviere = new QLabel("Marché");
    labelRiviere->setStyleSheet("font-size: 16px; font-weight: bold; color: #34495e;");
    sideLayout->addWidget(labelRiviere);

    riviereWidget = new RiviereWidget;
    sideLayout->addWidget(riviereWidget);


    QLabel *labelControles = new QLabel("Contrôles");
    labelControles->setStyleSheet("font-size: 16px; font-weight: bold; color: #34495e;");
    sideLayout->addWidget(labelControles);

    btnRotation = new ModernButton("Pivoter (R)");
    btnRotation->setEnabled(false);
    sideLayout->addWidget(btnRotation);

    btnValidation = new ModernButton("Valider Placement");
    btnValidation->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2980b9, stop:1 #3498db);"
        "  color: white; border: none; border-radius: 8px; padding: 12px 24px;"
        "  font-size: 14px; font-weight: bold; min-width: 120px;"
        "}"
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3498db, stop:1 #2980b9); }"
        "QPushButton:pressed { background: #206d9b; }"
        "QPushButton:disabled { background: #cccccc; color: #666666; }"
    );
    btnValidation->setEnabled(false);
    sideLayout->addWidget(btnValidation);

    btnUndo = new ModernButton("Annuler");
    btnUndo->setEnabled(false);
    sideLayout->addWidget(btnUndo);

    btnRecentrer = new ModernButton("Recentrer");
    sideLayout->addWidget(btnRecentrer);

    sideLayout->addStretch();
    layoutPrincipal->addWidget(sidePanel, 0);

    setStyleSheet("QMainWindow { background: #f5f7fa; }");
    setWindowTitle("Akropolis - Édition Moderne");
    resize(1200, 800);

    // 3. Connexions
    connect(gridWidget, &HexGridWidget::hexClicked, this, &MainWindow::onHexClicked);
    connect(btnRotation, &QPushButton::clicked, this, &MainWindow::onRotationClicked);
    connect(btnValidation, &QPushButton::clicked, this, &MainWindow::onValidationButtonClicked);
    connect(btnRecentrer, &QPushButton::clicked, this, &MainWindow::onRecentrerClicked);
    connect(btnUndo, &QPushButton::clicked, this, &MainWindow::onUndoClicked);
    connect(riviereWidget, &RiviereWidget::tuileChoisie, this, &MainWindow::onTuileChoisie);

    // 4. Démarrage
    etatActuel = EtatJeu::CHOIX_RIVIERE;
    mettreAJourInterface();
    // Message de bienvenue
    QMessageBox::information(this, "C'est parti !",
        QString("La partie commence avec %1 joueurs !\nC'est au tour de %2."));

}

MainWindow::~MainWindow() {
    delete partie;
}
void MainWindow::onTuileChoisie(int index) {
    if (tuileSelectionnee) return;

    Joueur* j = partie->getJoueurActuel();
    ChoixTuile* choix = partie->getChoixTuile();

    int cout = choix->calculerCout(index);
    try{ (j->peutPayerPierres(cout));
        j->retirerPierres(cout);

        // On récupère la tuile et on la sort de la rivière
        tuileSelectionnee = choix->choisirTuile(j, index);
        rotationActuelle = 0;
        etatActuel = EtatJeu::PLACEMENT_TUILE;

        mettreAJourInterface();
    } catch(const GameException& e) {
        QMessageBox::warning(this, "Impossible",
                             QString("Vous avez besoin de %1 pierres. Vous n'en avez que %2.")
                                 .arg(cout).arg(j->getNbPierres()));
    }
}

void MainWindow::onRotationClicked() {
    // Vérification de base pour s'assurer qu'une tuile est bien en prévisualisation
    if (!tuileSelectionnee || !btnRotation->isEnabled()) return;

    int currentRot = gridWidget->getFantomeRotation();
    
    // 1. Incrémenter la rotation de 60 degrés (de 0 à 5)
    int nextRot = (currentRot + 1) % 6;
    
    // 2. Appliquer la nouvelle rotation au widget (le dessin change immédiatement)
    gridWidget->setFantomeRotation(nextRot); 
    rotationActuelle = nextRot; 
    
    // 3. Maintenir le bouton Valider activé et mettre à jour le statut
    btnValidation->setEnabled(true); 
    statutLabel->setText(QString("Rotation %1. Cliquez sur Valider pour placer.")
                         .arg(nextRot * 60));
}

void MainWindow::onValidationButtonClicked() {
    if (!tuileSelectionnee || etatActuel != EtatJeu::PLACEMENT_TUILE) return;

    Joueur* j = partie->getJoueurActuel();
    Cite* cite = j->getCite();
    
    // 1. Déterminer la rotation finale
    int finalRotation = gridWidget->getFantomeRotation(); 
    
    // 2. Chercher le coup validé par le moteur de jeu qui correspond à l'ancre et à la rotation UI
    auto coupsValides = cite->genererCoupsValides(*tuileSelectionnee);
    bool valide = false;
    
    // La rotation UI (0-5) correspond aux 6 permutations de CoupPossible.
    for(const auto& c : coupsValides) {
        // Le CoupPossible stocke un ID de rotation qui contient la forme (V ou ^) ET la permutation
        // Comme nous ne savons pas si l'ancre correspond à la forme V (0-2) ou ^ (3-5),
        // nous testons si une des deux formes correspond à l'ancre et à la permutation choisie (finalRotation % 3)
        
        // C'est le CoupPossible que nous devons appliquer (il contient hauteur et recouvrement)
        if(c.ancre == ancreSelectionnee && c.rotation == finalRotation) { 
            try {
                // 3. Appliquer le coup validé
                j->placerTuile(tuileSelectionnee, c);
                valide = true;
                break;
            } catch (const std::exception& e) {
                QMessageBox::warning(this, "Erreur de placement", e.what());
                return;
            }
        }
    }

    if(valide) {
        // 4. Nettoyer l'état et passer au joueur suivant
        tuileSelectionnee = nullptr;
        gridWidget->clearTuileFantome();
        
        btnRotation->setEnabled(false);
        btnValidation->setEnabled(false);
        
        passerAuJoueurSuivant();
    } else {
        QMessageBox::information(this, "Info", "Placement invalide (ancre/rotation ne correspond pas à un coup légal).");
    }
}

void MainWindow::onHexClicked(CoordHex coord) {
    if (!tuileSelectionnee || etatActuel != EtatJeu::PLACEMENT_TUILE) return;

    Joueur* j = partie->getJoueurActuel();
    Cite* cite = j->getCite();
    
    // 1. Vérifier si cette coordonnée est l'ancre d'au moins UN coup légal (nécessaire pour activer la prévisualisation)
    bool ancreLegale = false;
    auto coupsValides = cite->genererCoupsValides(*tuileSelectionnee);
    
    for (const auto& coup : coupsValides) {
        if (coup.ancre == coord) {
            ancreLegale = true;
            break;
        }
    }

    if (ancreLegale) {
        // Ancre valide : on active la prévisualisation.
        ancreSelectionnee = coord;
        
        // La rotation est initialisée à 0 sans vérification de légalité
        int initialRotation = 0; 
        
        // Initialisation de la prévisualisation
        gridWidget->setTuileFantome(tuileSelectionnee, ancreSelectionnee);
        gridWidget->setFantomeRotation(initialRotation);
        rotationActuelle = initialRotation;
            
        btnRotation->setEnabled(true);
        btnValidation->setEnabled(true);
        statutLabel->setText("Rotationnez ou Validez le placement.");
    } else {
        // Clic sur une case illégale
        gridWidget->clearTuileFantome(); 
        btnRotation->setEnabled(false);
        btnValidation->setEnabled(false);
        statutLabel->setText("Emplacement invalide. Cliquez sur une case légale.");
    }
}

void MainWindow::passerAuJoueurSuivant() {
    // Vérifier fin de partie
    verifierFinPartie();

    if (etatActuel == EtatJeu::FIN_PARTIE) {
        afficherScore();
        return;
    }

    // Passer au joueur suivant
    partie->passerTour();
    Joueur* nouveauJoueur = partie->getJoueurActuel();

    etatActuel = EtatJeu::CHOIX_RIVIERE;
    tuileSelectionnee = nullptr;
    animerTransition();

    mettreAJourInterface();

    if (nouveauJoueur->isIA()) {
        riviereWidget->setEnabled(false);
        statutLabel->setText("L'Illustre Constructeur réfléchit...");

        QTimer::singleShot(1000, this, [this, nouveauJoueur]() {
            ChoixTuile* choix = partie->getChoixTuile();
            const auto& dispos = choix->getTuilesDisponibles();

            int indexChoisi = -1;
            int coutMin = 999;
            for (size_t i = 0; i < dispos.size(); ++i) {
                int cout = choix->calculerCout(i);
                if (dispos[i]->contientPlace() && nouveauJoueur->peutPayerPierres(cout)) {
                    if (cout < coutMin) { coutMin = cout; indexChoisi = i; }
                }
            }
            if (indexChoisi == -1) indexChoisi = 0;

            TuileCite* tuile = choix->choisirTuile(nouveauJoueur, indexChoisi);
            nouveauJoueur->recupererTuileIA(tuile);

            QMessageBox::information(this, "Tour de l'IA",
                                     QString("L'Illustre Constructeur a pris la tuile n°%1.").arg(indexChoisi + 1));
            passerAuJoueurSuivant();
        });

    } else {
        // C'est à l'humain : on réactive tout
        riviereWidget->setEnabled(true);
        QMessageBox::information(this, "À vous !",
                                 QString("C'est à %1 de jouer.").arg(QString::fromStdString(nouveauJoueur->getNom())));
    }
}

void MainWindow::verifierFinPartie() {
    // Règle : Pioche vide ET max 1 tuile sur la rivière
    bool piocheVide = partie->getPioche()->estVide();
    int tuilesRestantes = partie->getChoixTuile()->getNombreTuiles();

    if (piocheVide && tuilesRestantes <= 1) {
        etatActuel = EtatJeu::FIN_PARTIE;
    }
}

void MainWindow::afficherScore() {
    QString message = " FIN DE PARTIE \n\n";

    for (auto* joueur : partie->getJoueurs()) {
        message += QString("%1 : %2 points\n")
        .arg(QString::fromStdString(joueur->getNom()))
            .arg(joueur->calculerScore());
    }

    QMessageBox::information(this, "Partie terminée", message);
}

void MainWindow::animerTransition() {
    // Effet de fondu sur le panel joueur
    auto* effect = new QGraphicsOpacityEffect(panelJoueur);
    panelJoueur->setGraphicsEffect(effect);

    auto* anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(500);
    anim->setStartValue(1.0);
    anim->setKeyValueAt(0.5, 0.3);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::mettreAJourInterface() {
    Joueur* j = partie->getJoueurActuel();
    infoLabel->setText(QString("Tour de : %1\nPierres: %2\nScore: %3\nÉtat: %4")
                           .arg(QString::fromStdString(j->getNom()))
                           .arg(j->getNbPierres())
                           .arg(j->calculerScore())
                           .arg(etatActuel == EtatJeu::CHOIX_RIVIERE ? "Choisir une tuile" : "Placer la tuile"));

    QString statut = (etatActuel == EtatJeu::CHOIX_RIVIERE)
                         ? "Choisissez une tuile dans le marché"
                         : "Placez la tuile sur la grille";
    statutLabel->setText(statut);

    // Changement de la cité affichée dans le widget
    gridWidget->setCite(j->getCite());
    gridWidget->update();

    if (partie && partie->getPioche()) {
        int pilesRestantes = partie->getPioche()->getNbPilesRestantes();
        int totalInitial = progressBar->maximum(); // On récupère le max qu'on a set au début

        // La valeur correspond à ce qu'on a déjà joué
        int pilesConsommees = totalInitial - pilesRestantes;

        progressBar->setValue(pilesConsommees);
    }



    // Mise à jour de la rivière
    partie->remplirChoixTuile();
    riviereWidget->setChoixTuile(partie->getChoixTuile());

    // Activation des boutons
    btnRotation->setEnabled(etatActuel == EtatJeu::PLACEMENT_TUILE);
    btnUndo->setEnabled(false); // À implémenter
    riviereWidget->setEnabled(etatActuel == EtatJeu::CHOIX_RIVIERE);
}

void MainWindow::onUndoClicked() {
    // À implémenter : annuler le dernier coup
}

void MainWindow::onRecentrerClicked() {
    // Recentrer la vue (à implémenter dans HexGridWidget)
    gridWidget->update();
}

