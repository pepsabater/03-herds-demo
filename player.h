#ifndef PLAYER_H
#define PLAYER_H

#include "global.h"

// si belluga i interacciona directament
#define PLAYER_MODE_STATIC    0 // no es mou per l'escena
#define PLAYER_MODE_DYNAMIC   1 // es mou per l'escena

#define PLAYER_STATUS_DEAD    0 // és mort
#define PLAYER_STATUS_LIVE    1 // és viu

// accions i esdeveniments
#define PLAYER_ACTION_GO        0   // anar endavant
#define PLAYER_ACTION_TLEFT     1   // tombrar a l'esquerra
#define PLAYER_ACTION_TRIGHT    2   // tombar a la dreta
#define PLAYER_ACTION_REVERSE   3   // girar cua
#define PLAYER_ACTION_WAIT      4   // esperar-se
#define PLAYER_ACTIONS          5   // nombre d'accions

class Player 
{
protected:
// característiques i estats
int type;       // de quina mena és
int status;     // estat
bool active;    // si és dinàmic o estàtic
bool onPlay;    // si hi és al joc o no

// control de posició i orientació
Compass compass;

// imatge associada
QGraphicsPixmapItem* pixmapItem;
QString pixmapPath;

public:

    Player();
    // característiques i estats   
    int getType(){return type;};
    int getStatus(){return status;};
    bool isActive(){return active;};
    bool isOnPlay(){return onPlay;};
    // imatge
    void setPixmapPath(QString pixpath){pixmapPath=pixpath;};
    void setPixmap(QGraphicsPixmapItem* qpixmap){pixmapItem=qpixmap;};
    QGraphicsPixmapItem* getPixmap(){return pixmapItem;};
    QString getPixmapPath(){return pixmapPath;};
    // posicionament i orientació
    void pointTo(){compass.pointTo();};
    int getCompassAngle(){return compass.getCompassAngle();};
    void setPos(int xpos, int ypos){compass.setXPos(xpos);compass.setYPos(ypos);};
    void setXPos (int xpos){compass.setXPos(xpos);};
    int getXPos(){return compass.getXPos();};
    void setYPos (int ypos){compass.setYPos(ypos);};
    int getYPos(){return compass.getYPos();};
    void setCompass(int direction){compass.setSense(direction);};
    int getCompass(){return compass.getSense();};
};

#endif // PLAYER_H
