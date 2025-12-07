#include "MainWindow.h"
#include <QPainter>
#include <QMouseEvent>
#include "HexCons_Carr_Quart_Place.h"
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

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

void RiviereWidget::dessinerTuile(QPainter& painter, TuileCite* tuile, int xOffset, int yOffset, bool hovered) {
    QPoint offsets[3] = {QPoint(0,0), QPoint(30, -18), QPoint(60, 0)};

    for(int i=0; i<3; i++) {
        HexagoneConstruction* hex = tuile->getHexagone(i);
        QColor col = Qt::white;

        if (auto* q = dynamic_cast<Quartier*>(hex))
            col = typeToColor(q->getType().getCouleur());
        else if (auto* p = dynamic_cast<Place*>(hex))
            col = typeToColor(p->getType().getCouleur());
        else if (dynamic_cast<Carriere*>(hex))
            col = typeToColor(Couleur::gris);

        int x = xOffset + offsets[i].x();
        int y = yOffset + offsets[i].y();

        // Ombre
        if (hovered) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(0, 0, 0, 60));
            painter.drawEllipse(x+2, y+2, 26, 26);
        }

        // Hexagone avec dégradé
        QRadialGradient grad(x+13, y+13, 15);
        grad.setColorAt(0, col.lighter(120));
        grad.setColorAt(1, col);

        painter.setBrush(grad);
        painter.setPen(QPen(col.darker(130), hovered ? 3 : 2));
        painter.drawEllipse(x, y, 26, 26);    }
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
    if (!choixActuel) return;

    auto tuiles = choixActuel->getTuilesDisponibles();
    if (tuiles.empty()) {
        painter.drawText(rect(), Qt::AlignCenter, "Aucune tuile disponible");
        return;
    };

    int largeurCase = width() / tuiles.size();

    for(size_t i=0; i < tuiles.size(); ++i) {
        int x = i * largeurCase/2 - 30;
        int y = height() / 2;

        bool hovered = (static_cast<int>(i) == hoveredIndex);

        // Cadre de sélection
        if (hovered) {
            painter.setPen(QPen(QColor(33, 150, 243), 3));
            painter.setBrush(QColor(33, 150, 243, 30));
            painter.drawRoundedRect(x-10, y-40, 80, 120, 8, 8);
        }

        dessinerTuile(painter, tuiles[i], x, y);

        // Afficher le coût
        int cout = choixActuel->calculerCout(i);
        painter.setPen(Qt::NoPen);
        painter.setBrush(cout > 0 ? QColor(244, 67, 54) : QColor(76, 175, 80));
        painter.drawRoundedRect(x+20, y+50, 40, 24, 12, 12);

        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(QRect(x+20, y+50, 40, 24), Qt::AlignCenter,
                         QString("%1").arg(cout));    }
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 1. Initialisation du Jeu
    partie = new Partie(1, ModeJeu::SOLO);
    partie->ajouterJoueur("Joueur 1", 1);
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


    infoLabel = new QLabel("Joueur 1");
    infoLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50;"
);
    panelLayout->addWidget(infoLabel);

    statutLabel = new QLabel("Choisissez une tuile");
    statutLabel->setStyleSheet("font-size: 14px; color: #7f8c8d;");
    panelLayout->addWidget(statutLabel);

    progressBar = new QProgressBar;
    progressBar->setRange(0, 100);
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
}

MainWindow::~MainWindow() {
    delete partie;
}
void MainWindow::onTuileChoisie(int index) {
    if (tuileSelectionnee) return;

    Joueur* j = partie->getJoueurActuel();
    ChoixTuile* choix = partie->getChoixTuile();

    int cout = choix->calculerCout(index);
    if (j->peutPayerPierres(cout)) {
        j->retirerPierres(cout);

        // On récupère la tuile et on la sort de la rivière
        tuileSelectionnee = choix->choisirTuile(j, index);
        rotationActuelle = 0;
        etatActuel = EtatJeu::PLACEMENT_TUILE;

        mettreAJourInterface();
    } else {
        QMessageBox::warning(this, "Impossible",
                             QString("Vous avez besoin de %1 pierres. Vous n'en avez que %2.")
                                 .arg(cout).arg(j->getNbPierres()));
    }
}

void MainWindow::onRotationClicked() {
    if (!tuileSelectionnee) return;
    gridWidget->rotateFantome();
    rotationActuelle = gridWidget->getFantomeRotation();
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
                cite->placerTuile(tuileSelectionnee, c);
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
        
        partie->passerTour();
        passerAuJoueurSuivant();
    } else {
        QMessageBox::information(this, "Info", "Placement invalide (ancre/rotation ne correspond pas à un coup légal).");
    }
}

void MainWindow::onHexClicked(CoordHex coord) {
    if (!tuileSelectionnee || etatActuel != EtatJeu::PLACEMENT_TUILE) return;

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
            passerAuJoueurSuivant();
        } else {
            // Feedback silencieux ou sonore (coup invalide)
            QMessageBox::information(this, "Info", "Placement invalide ici.");
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Erreur", e.what());
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
    etatActuel = EtatJeu::CHOIX_RIVIERE;

    // Animation de transition
    animerTransition();

    mettreAJourInterface();
}

void MainWindow::verifierFinPartie() {
    // Vérifier si la pioche est vide (à adapter selon votre logique)
    if (partie->getChoixTuile()->getTuilesDisponibles().empty()) {
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

    /* Progress bar (exemple: nombre de tours)
    int progression = (partie->getJoueurActuel() * 100) / 12; // Supposons 12 tours max
    progressBar->setValue(progression);*/

    // Mise à jour de la grille
    gridWidget->setCite(j->getCite());

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

