#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include "global.h"
#include "mygameview.h"
#include "arrow.h"

class GameController: public QObject
{
protected:

    Q_OBJECT

QGraphicsScene* gameScene;
GameView* gameView;
QPixmap* gameBackImage;
QBrush* gameBrush;
QTimer* gameTamTam;

std::vector<Player*>playersPool;   // col·lecció de jugadors

Player* gamePlayer0=nullptr;

QString backPixmapPath="../imatges/brown-background01-1440x900.png";
QString arrow0PixmapPath="../imatges/bug-red40.png";
QString arrowPixmapPath="../imatges/bug-orange40.png";


    // afegir jugador
    bool playerAdd(Player*);
    // accionador del jugador segons l'acció sol·licitada
    bool playerMove(Player*, int);
    // dur el jugador cap a endavant
    void playerGo(Player*);
    // dur el jugador cap a l'esquerra
    void playerGoLeft(Player*);
    // dur el jugador cap a la dreta
    void playerGoRight(Player*);
    // el jugador fa mitja volta
    void playerGoReverse(Player*);
    // el jugador fa un retruc
    void playerRebound();
    // dibuixem el jugador
    void playerDraw(Player*, bool);
    // busca el primer jugador de la llista de la mena sol·licitada
    Player* playerFinder(int);

public:

    GameController(QGraphicsScene*, GameView*);
    void gStartTamTam(){gameTamTam->start();}
    void gSetInit();
    void gSetRestart();
    void gSetTheatre();
    void gMakePlayers(int); // la mena de jugadors

public slots:
    // connectors events->mètodes
    // gestor d'actualizació del joc en temps real
    void onTamTam();
    // gestor de teclat, events de teclat
    void touchMe(QKeyEvent*);
};

#endif // GAMECONTROL_H
