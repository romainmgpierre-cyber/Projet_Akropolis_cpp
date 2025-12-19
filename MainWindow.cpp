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
#include <QApplication>

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

    
    const double L_SHAPE_MIDPOINT_X = -HEX_SPACING / 2.0;

    
    const double MARGIN_OFFSET_RIGHT = HEX_SPACING *1.5;

    // Décalage final pour centrer la forme et ajouter la marge droite
    const double drawingOffsetX = -L_SHAPE_MIDPOINT_X + MARGIN_OFFSET_RIGHT;

    double drawingOffsetY = -HEX_SIZE_RIV * 0.75; 


    //  Dessin de chaque hexagone de la tuile
    for(int i = 0; i < 3; i++) {
        HexagoneConstruction* hex = tuile->getHexagone(i);
        QColor col = Qt::white;

        // Détermination de la couleur
        if (auto* q = dynamic_cast<Quartier*>(hex))
            col = typeToColor(q->getType().getCouleur());
        else if (auto* p = dynamic_cast<Place*>(hex))
            col = typeToColor(p->getType().getCouleur());
        else if (dynamic_cast<Carriere*>(hex))
            col = typeToColor(Couleur::gris);

        // Calcul du centre
        QPointF center = QPointF(xCenterOffset + offsets[i].x() + drawingOffsetX,
                                 yCenter + offsets[i].y() + drawingOffsetY);

        // dessin de la forme
        QPolygonF poly;
        for (int j = 0; j < 6; ++j) {
            double angle_deg = 60 * j - 30;
            double angle_rad = M_PI / 180 * angle_deg;
            poly << center + QPointF(HEX_SIZE_RIV * cos(angle_rad), HEX_SIZE_RIV * sin(angle_rad));
        }

        
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 40));
        painter.drawPolygon(poly.translated(1, 1));

      
        QRadialGradient grad(center, HEX_SIZE_RIV);
        grad.setColorAt(0, col.lighter(120));
        grad.setColorAt(1, col);

        painter.setBrush(grad);
        painter.setPen(QPen(col.darker(130), hovered ? 3 : 2));
        painter.drawPolygon(poly);

        
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

    painter.fillRect(rect(), QColor(220, 220, 220)); 

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

    // Largeur allouée par tuile
    double largeurParTuile = totalWidth / (double)nbTuiles;

    // Position de départ
    double currentX = 10;

    for(size_t i = 0; i < nbTuiles; ++i) {

        // Calculer le centre de la zone de la tuile
        double xCenter = currentX + (largeurParTuile / 2.0);
        double yCenter = height() / 2.0;

        
        double drawingX = xCenter - (TUILE_WIDTH_ESTIMEE / 2.0);

        bool hovered = (static_cast<int>(i) == hoveredIndex);

        // Cadre de sélection
        if (hovered) {
            painter.setPen(QPen(QColor(33, 150, 243), 3));
            painter.setBrush(QColor(33, 150, 243, 30));
            // Le cadre couvre toute la zone allouée
            painter.drawRoundedRect(currentX, 40, largeurParTuile, height() - 50, 8, 8);
        }

        
        dessinerTuile(painter, tuiles[i], (int)drawingX, (int)yCenter, hovered);

        
        int cout = choixActuel->calculerCout(i);

        
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
    : QMainWindow(parent){
    setStyleSheet("QMainWindow { background: #f5f7fa; }");
    setWindowTitle("Akropolis");
    resize(1200, 800);
    QTimer::singleShot(0, this, &MainWindow::demarrerPartie);

}
void MainWindow::demarrerPartie(){
    // Initialisation du Jeu
    
    QStringList optionsInitiales;
    optionsInitiales << "Créer une nouvelle partie" << "Charger une partie existante";

    bool ok;
    QString choixInitial = QInputDialog::getItem(this, "Akropolis - Menu",
                                                 "Que souhaitez-vous faire ?",
                                                 optionsInitiales, 0, false, &ok);

    if (!ok) {
        qApp->quit(); // Quitter si l'utilisateur annule
        return;
    }

    if (choixInitial == "Charger une partie existante") {
        QMessageBox::information(this, "Chargement", "A FAIRE");
        return;
    }

    QStringList modes;
    modes << "Solo (vs IA)" << "Multijoueur (Local)";
    QString modeChoisi = QInputDialog::getItem(this, "Bienvenue sur Akropolis",
        "Choisissez le mode de jeu :",
        modes, 0, false, &ok);
    if (!ok) modeChoisi = "Solo (vs IA)";
    if (modeChoisi == "Solo (vs IA)") {
        // mode solo
        partie = new Partie(1, ModeJeu::SOLO);

        // Ajout du joueur humain
        QString nomJoueur = QInputDialog::getText(this, "Nom du Joueur",
                                                  "Entrez votre nom :",
                                                  QLineEdit::Normal, "Joueur", &ok);

        
        if (!ok || nomJoueur.isEmpty()) nomJoueur = "Joueur Humain";
        partie->ajouterJoueur(nomJoueur.toStdString(), 1);

        
        partie->ajouterJoueur("Illustre Constructeur", 2, true);

        // Choix difficulté
        QStringList diffs; diffs << "Facile" << "Moyen" << "Difficile";
        QString diff = QInputDialog::getItem(this, "Difficulté", "Niveau de l'IA :", diffs, 1, false);

        if (diff == "Difficile") partie->setDifficulte(NiveauDifficulte::DIFFICILE);
        else if (diff == "Moyen") partie->setDifficulte(NiveauDifficulte::MOYEN);
        else partie->setDifficulte(NiveauDifficulte::FACILE);}
    //Choix du nombre de joueurs
    else{
        partie = new Partie(1, ModeJeu::MULTIJOUEUR);
        int nbJoueurs = QInputDialog::getInt(this, "Multijoueurs",
            "Nombre de joueurs (2-4) :",
            2, 2, 4, 1, &ok);
        if (!ok) nbJoueurs = 2; // Par défaut si l'utilisateur annule

    for (int i = 0; i < nbJoueurs; ++i) {
            QString defaut = QString("Joueur %1").arg(i + 1);
            QString label = QString("Nom du Joueur %1 :").arg(i + 1);

            QString nom = QInputDialog::getText(this, "Configuration des joueurs",
                                                label, QLineEdit::Normal, defaut, &ok);

            if (!ok || nom.isEmpty()) nom = defaut;

            partie->ajouterJoueur(nom.toStdString(), i + 1);
        }
    }
    partie->initialiserTuiles();

    
    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    layoutPrincipal = new QHBoxLayout(centralWidget);
    layoutPrincipal->setSpacing(20);
    layoutPrincipal->setContentsMargins(20, 20, 20, 20);

    // Partie Gauche : la grille
    gridWidget = new HexGridWidget;
    gridWidget->setMinimumSize(600, 600);
    layoutPrincipal->addWidget(gridWidget, 2);

    // Partie Droite : les contrôles
    QWidget *sidePanel = new QWidget;
    sidePanel->setMaximumWidth(350);
    QVBoxLayout *sideLayout = new QVBoxLayout(sidePanel);
    sideLayout->setSpacing(10);

    
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

    
    QLabel *labelRiviere = new QLabel("Marché");
    labelRiviere->setStyleSheet("font-size: 16px; font-weight: bold; color: #34495e;");
    sideLayout->addWidget(labelRiviere);

    riviereWidget = new RiviereWidget;
    sideLayout->addWidget(riviereWidget);


    QLabel *labelControles = new QLabel("Contrôles");
    labelControles->setStyleSheet("font-size: 16px; font-weight: bold; color: #34495e;");
    sideLayout->addWidget(labelControles);

    btnRotation = new ModernButton("Pivoter");
    btnRotation->setEnabled(false);
    sideLayout->addWidget(btnRotation);

    btnPivot = new ModernButton("Changer Pivot");
    sideLayout->addWidget(btnPivot);

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


    // connexions
    connect(gridWidget, &HexGridWidget::hexClicked, this, &MainWindow::onHexClicked);
    connect(btnRotation, &QPushButton::clicked, this, &MainWindow::onRotationClicked);
    connect(btnValidation, &QPushButton::clicked, this, &MainWindow::onValidationButtonClicked);
    connect(btnRecentrer, &QPushButton::clicked, this, &MainWindow::onRecentrerClicked);
    connect(btnUndo, &QPushButton::clicked, this, &MainWindow::onUndoClicked);
    connect(riviereWidget, &RiviereWidget::tuileChoisie, this, &MainWindow::onTuileChoisie);
    connect(btnPivot, &QPushButton::clicked, this, &MainWindow::onPivotClicked);

    // démarrage
    etatActuel = EtatJeu::CHOIX_RIVIERE;
    mettreAJourInterface();
    QMessageBox::information(this, "C'est parti !",
        QString("La partie commence !"));

}

MainWindow::~MainWindow() {
    delete partie;
}

void MainWindow::onTuileChoisie(int index) {
    if (tuileSelectionnee) return;

    Joueur* j = partie->getJoueurActuel();
    ChoixTuile* choix = partie->getChoixTuile();
    try {
        tuileSelectionnee = choix->choisirTuile(j, index);

        indexSourceTuile = index;
        etatActuel = EtatJeu::PLACEMENT_TUILE;

        mettreAJourInterface();

    } catch(const GameException& e) {
        int cout = choix->calculerCout(index);
        QMessageBox::warning(this, "Impossible",
                             QString("Vous avez besoin de %1 pierres. Vous n'en avez que %2.")
                                 .arg(cout).arg(j->getNbPierres()));
    }
}

void MainWindow::onRotationClicked() {
    // Vérification pour s'assurer qu'une tuile est bien en prévisualisation
    if (!tuileSelectionnee || !btnRotation->isEnabled()) return;

    int currentRot = gridWidget->getFantomeRotation();
    
    // Incrémenter la rotation de 60 degrés (de 0 à 5)
    int nextRot = (currentRot + 1) % 6;
    
    // Appliquer la nouvelle rotation au widget
    gridWidget->setFantomeRotation(nextRot); 
    rotationActuelle = nextRot; 
    
    
    btnValidation->setEnabled(true); 
    statutLabel->setText(QString("Rotation %1. Cliquez sur Valider pour placer.")
                         .arg(nextRot * 60));
}

void MainWindow::onValidationButtonClicked() {
    if (!tuileSelectionnee || etatActuel != EtatJeu::PLACEMENT_TUILE) return;

    Joueur* j = partie->getJoueurActuel();
    Cite* cite = j->getCite();

    //1. L'ÉTAT VISUEL
    // On détermine quelle partie de la tuile (index 0, 1 ou 2) est à quelle coordonnée sur la grille.

    int rotUI = gridWidget->getFantomeRotation();
    int pivotUI = this->pivotActuel; // Index de l'hexagone servant de pivot (0, 1 ou 2)
    CoordHex ancreVisuelle = ancreSelectionnee; // La coordonnée de la grille où l'utilisateur a cliqué

    // Positions locales relatives dans la tuile
    const std::array<CoordHex, 3> localPos = {CoordHex(0,0), CoordHex(-1,0), CoordHex(0,-1)};


    CoordHex offsetPivot = localPos[pivotUI].rotate(rotUI);

    std::vector<CoordHex> posVisuelles(3);

    for(int i=0; i<3; ++i) {
        posVisuelles[i] = ancreVisuelle + (localPos[i].rotate(rotUI) - offsetPivot);
    }

    //  2. RECHERCHE D'UN COUP LOGIQUE ÉQUIVALENT
    auto coupsValides = cite->genererCoupsValides(*tuileSelectionnee);
    Cite::CoupPossible coupFinal;
    bool trouve = false;

    for(const auto& c : coupsValides) {
        // Décodage du coup logique (Forme + Permutation)
        int forme = c.rotation / 3;
        int perm = c.rotation % 3;

        // Géométrie relative de la forme utilisée par le moteur
        CoordHex rel1, rel2;
        if (forme == 0) { rel1 = CoordHex(-1, 0); rel2 = CoordHex(0, -1); }
        else { rel1 = CoordHex(1, 0);  rel2 = CoordHex(0, 1); }

        std::array<CoordHex, 3> slotsLogiques;
        slotsLogiques[0] = c.ancre;
        slotsLogiques[1] = c.ancre + rel1;
        slotsLogiques[2] = c.ancre + rel2;

        // Calcul du contenu : Quel hexagone original (0,1,2) se trouve dans quel slot ?
        // La rotationHoraire (perm) décale les contenus : 0->1, 1->2, 2->0 dans le tableau
        // Cela signifie que le Slot 0 reçoit le contenu qui était en position X...

        int indexHexInSlot[3];
        if (perm == 0) { indexHexInSlot[0]=0; indexHexInSlot[1]=1; indexHexInSlot[2]=2; }
        else if (perm == 1){ indexHexInSlot[0]=2; indexHexInSlot[1]=0; indexHexInSlot[2]=1; }
        else { indexHexInSlot[0]=1; indexHexInSlot[1]=2; indexHexInSlot[2]=0; }

        // Le coup est valide si pour chaque hexagone (0,1,2), sa position visuelle est identique à sa position logique.
        bool match = true;
        for(int slot=0; slot<3; ++slot) {
            int originalIndex = indexHexInSlot[slot]; // Quel hexagone est ici ?
            CoordHex posLogique = slotsLogiques[slot]; // Où est-il selon le moteur ?

            // Est-ce que cela correspond à ce qu'on voit ?
            if (!(posVisuelles[originalIndex] == posLogique)) {
                match = false;
                break;
            }
        }

        if (match) {
            coupFinal = c;
            trouve = true;
            break;
        }
    }

    // 3. APPLICATION OU REJET
    if(trouve) {
        try {
            int pierres = j->placerTuile(tuileSelectionnee, coupFinal);

            if (pierres > 0) {
                QMessageBox::information(this, "Recouvrement",
                                         QString("Vous avez recouvert des carrières et gagné %1 pierre(s) !").arg(pierres));
            }

            tuileSelectionnee = nullptr;
            gridWidget->clearTuileFantome();
            btnRotation->setEnabled(false);
            btnValidation->setEnabled(false);
            passerAuJoueurSuivant();
        } catch (...) {}
    } else {
        QMessageBox::information(this, "Placement Invalide",
                                 "Ce placement n'est pas valide (vérifiez l'adjacence ou la superposition).");
    }

}

void MainWindow::onHexClicked(CoordHex coord) {
    if (!tuileSelectionnee || etatActuel != EtatJeu::PLACEMENT_TUILE) return;

    Joueur* j = partie->getJoueurActuel();
    Cite* cite = j->getCite();
    
    // Vérifier si cette coordonnée est l'ancre d'au moins un coup léga
    bool ancreLegale = false;
    auto coupsValides = cite->genererCoupsValides(*tuileSelectionnee);
    
    for (const auto& coup : coupsValides) {
        if (coup.ancre == coord) {
            ancreLegale = true;
            break;
        }
    }

    if (ancreLegale) {
        // on active la prévisualisation.
        ancreSelectionnee = coord;
        
        // La rotation est initialisée à 0
        int initialRotation = 0; 
        
        
        gridWidget->setTuileFantome(tuileSelectionnee, ancreSelectionnee);
        gridWidget->setFantomeRotation(initialRotation);
        rotationActuelle = initialRotation;
            
        btnRotation->setEnabled(true);
        btnValidation->setEnabled(true);
        statutLabel->setText("Rotationnez ou Validez le placement.");
    } else {
        
        gridWidget->clearTuileFantome(); 
        btnRotation->setEnabled(false);
        btnValidation->setEnabled(false);
        statutLabel->setText("Emplacement invalide. Cliquez sur une case légale.");
    }
}

void MainWindow::passerAuJoueurSuivant() {
    
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

            auto coups = nouveauJoueur->getCite()->genererCoupsValides(*tuile);
            if (!coups.empty()) {
                nouveauJoueur->placerTuile(tuile, coups[0]); 
            } else {
                nouveauJoueur->recupererTuileIA(tuile);
            }
            nouveauJoueur->recupererTuileIA(tuile);


            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Tour de l'IA");
            msgBox.setText(QString("L'Illustre Constructeur a pris la tuile n°%1.").arg(indexChoisi + 1));

            QPushButton *voirBtn = msgBox.addButton("Visualiser sa cité", QMessageBox::ActionRole);
            QPushButton *passerBtn = msgBox.addButton("Passer au tour suivant", QMessageBox::AcceptRole);

            msgBox.exec(); // Affichage

            if (msgBox.clickedButton() == voirBtn) {
                gridWidget->setCite(nouveauJoueur->getCite());
                gridWidget->update();

                // Pour permettre de revenir à sa cité
                QMessageBox::information(this, "Vue de l'IA",
                                         "Vous visualisez la cité de l'IA.\nCliquez sur OK pour revenir à votre cité et jouer.");

            }

            passerAuJoueurSuivant();
        });

    } else {
        // C'est à l'humain donc on réactive tout
        riviereWidget->setEnabled(true);
        QMessageBox::information(this, "À vous !",
            QString("C'est à %1 de jouer.").arg(QString::fromStdString(nouveauJoueur->getNom())));
    }
}

void MainWindow::verifierFinPartie() {
    // Vérification des conditions de fin de partie
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
                         : "Placez la tuile sur la grille (ou Annulez)";
    statutLabel->setText(statut);

    // Mise à jour graphique
    gridWidget->setCite(j->getCite());
    gridWidget->update();

    if (partie && partie->getPioche()) {
        int pilesRestantes = partie->getPioche()->getNbPilesRestantes();
        int totalInitial = progressBar->maximum();
        int pilesConsommees = totalInitial - pilesRestantes;
        progressBar->setValue(pilesConsommees);
    }

    // Mise à jour de la rivière
    partie->remplirChoixTuile();
    riviereWidget->setChoixTuile(partie->getChoixTuile());

    
    // Rotation seulement si on est en train de placer une tuile
    btnRotation->setEnabled(etatActuel == EtatJeu::PLACEMENT_TUILE);
    
    // Annuler seulement si on a une tuile en main et qu'on est en mode placement
    bool peutAnnuler = (etatActuel == EtatJeu::PLACEMENT_TUILE && tuileSelectionnee != nullptr);
    btnUndo->setEnabled(peutAnnuler);

    // Rivière seulement si on doit choisir
    riviereWidget->setEnabled(etatActuel == EtatJeu::CHOIX_RIVIERE);
    
    
    if (etatActuel != EtatJeu::PLACEMENT_TUILE) {
        btnValidation->setEnabled(false);
    }
}




void MainWindow::onUndoClicked() {
    // Sécurité
    if (!tuileSelectionnee || etatActuel != EtatJeu::PLACEMENT_TUILE) return;

    Joueur* j = partie->getJoueurActuel();
    ChoixTuile* choix = partie->getChoixTuile();

    // Rembourser le joueur
    if (indexSourceTuile != -1) {
        
        j->ajouterPierres(indexSourceTuile);
    }

    // Remettre la tuile dans la rivière
    if (choix && tuileSelectionnee) {
        choix->remettreTuile(tuileSelectionnee, indexSourceTuile);
    }

    
    tuileSelectionnee = nullptr;     
    indexSourceTuile = -1;           
    
    
    gridWidget->clearTuileFantome(); 
    
   
    etatActuel = EtatJeu::CHOIX_RIVIERE;
    
    
    mettreAJourInterface();
    
    statutLabel->setText("Sélection annulée. Pierres remboursées.");
}

void MainWindow::onRecentrerClicked() {
    // Recentrer la vue
    gridWidget->update();
}

void MainWindow::onPivotClicked() {
    if (!tuileSelectionnee) return;

    
    pivotActuel = (pivotActuel + 1) % 3;
    gridWidget->setFantomePivot(pivotActuel);

    
    onHexClicked(ancreSelectionnee);

    statutLabel->setText(QString("Pivot changé sur l'hexagone %1").arg(pivotActuel));
}
