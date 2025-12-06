cmake_minimum_required(VERSION 3.5)

project(akropolis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. AJOUTEZ CES LIGNES POUR ACTIVER QT
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Trouve les composants Widgets de Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(akropolis
    main.cpp
    MainWindow.h MainWindow.cpp
    HexGridWidget.h HexGridWidget.cpp
    main.cpp
    Cite.cpp
    CoordHex.cpp
    GameExcep_Enums.cpp
    HexCons_Carr_Quart_Place.cpp
    Joueur.cpp
    Partie_Variante.cpp
    Pioche_ChoixTuile.cpp
    Score.cpp
    TuileCite_TuileDep.cpp
    Type.cpp
    fabrique_tuiles.cpp
    HexGridWidget.cpp
    HexGridWidget.h
    MainWindow.cpp
    MainWindow.h    Cite.cpp Cite.h
    CoordHex.cpp CoordHex.h
    fabrique_tuiles.cpp
    GameExcep_Enums.cpp GameExcep_Enums.h
    HexCons_Carr_Quart_Place.cpp HexCons_Carr_Quart_Place.h
    Joueur.cpp Joueur.h
    Partie_Variante.cpp Partie_Variante.h
    Pioche_ChoixTuile.cpp Pioche_ChoixTuile.h
    Score.cpp Score.h
    TuileCite_TuileDep.cpp TuileCite_TuileDep.h
    Type.cpp Type.h
        MenuConfiguration.h
        MenuConfiguration.cpp
        FonctionsAide.h
        FonctionsAide.cpp
)

# 2. LIEZ LA BIBLIOTHÈQUE QT A VOTRE EXÉCUTABLE
target_link_libraries(akropolis PRIVATE Qt6::Widgets)
