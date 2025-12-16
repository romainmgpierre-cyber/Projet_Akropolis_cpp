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

class ModernButton : public QPushButton {
public:
    explicit ModernButton(const QString& text, QWidget* parent = nullptr)
        : QPushButton(text, parent) {
        setStyleSheet(
            "QPushButton {"
            "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
            "              stop:0 #4CAF50, stop:1 #45a049);"
            "  color: white;"
            "  border: none;"
            "  border-radius: 8px;"
            "  padding: 12px 24px;"
            "  font-size: 14px;"
            "  font-weight: bold;"
            "  min-width: 120px;"
            "}"
            "QPushButton:hover {"
            "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
            "              stop:0 #5CBF60, stop:1 #4CAF50);"
            "}"
            "QPushButton:pressed {"
            "  background: #3d8b40;"
            "}"
            "QPushButton:disabled {"
            "  background: #cccccc;"
            "  color: #666666;"
            "}"
            );
        setCursor(Qt::PointingHandCursor);
    }
};

class RiviereWidget : public QWidget {
    Q_OBJECT
public:
    explicit RiviereWidget(QWidget *parent = nullptr);
    void setChoixTuile(Akropolis::ChoixTuile* choix);

signals:
    void tuileChoisie(int index);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Akropolis::ChoixTuile* choixActuel = nullptr;
    int hoveredIndex = -1;
    double scale = 30.0;

    void dessinerTuile(QPainter& painter, Akropolis::TuileCite* tuile, int x, int y, bool hovered = false);
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
    void onUndoClicked();
    void onRecentrerClicked();
    void onTuileChoisie(int index);
    void onValidationButtonClicked();
    void onPivotClicked();

private:
    void mettreAJourInterface();
    void passerAuJoueurSuivant();
    void verifierFinPartie();
    void afficherScore();
    void animerTransition();

    HexGridWidget *gridWidget;
    QLabel *infoLabel;
    QLabel *statutLabel;
    QProgressBar *progressBar;
    ModernButton *btnRotation;
    ModernButton *btnUndo;
    ModernButton *btnRecentrer;
    ModernButton *btnValidation;
    ModernButton *btnPivot;

    Akropolis::Partie* partie;
    Akropolis::TuileCite* tuileSelectionnee = nullptr;
    int rotationActuelle = 0;
    int indexSourceTuile = -1;
    int pivotActuel = 0;
    EtatJeu etatActuel;
    RiviereWidget *riviereWidget;
    Akropolis::CoordHex ancreSelectionnee;

    QFrame *panelJoueur;
    QHBoxLayout *layoutPrincipal;
};

#endif
