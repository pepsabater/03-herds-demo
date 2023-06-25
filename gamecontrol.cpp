#include "global.h"
#include "gamecontrol.h"

GameController::GameController(QGraphicsScene* scene, GameView* view)
{
    // assignem l'escena i la vista heretades
    gameScene=scene;
    gameView=view;

    // connectem els events de teclat i un temporitzador a la classe
    connect(view, SIGNAL(keyPressEvent(QKeyEvent*)), this, SLOT(touchMe(QKeyEvent*)));
    gameTamTam = new QTimer();
    connect(gameTamTam, SIGNAL(timeout()), this, SLOT(onTamTam()));
}

void GameController::gSetInit ()
{
    gameScene->setSceneRect(0,0,VIEW_WIDTH,VIEW_HEIGHT);
    gameView->setScene(gameScene);
    // fixa les mides i bloca els marcs
    gameView->setFixedSize(VIEW_WIDTH,VIEW_HEIGHT);
    gameView->setFrameStyle(0);
    // defineix la mida de l'escena
    gameScene->setSceneRect(0,0,VIEW_WIDTH,VIEW_HEIGHT);
    gameView->setStyleSheet("background-color:black;");
    gameView->setWindowTitle("herds_demo");

    // programa el temporitzador
    gameTamTam->setInterval(GAME_WORLD_TIMER_TIME);
}

void GameController::gSetTheatre ()
{
    // crea un QPixmap amb la imatge de fons
    gameBackImage=new QPixmap(backPixmapPath);
    // crea un pincell utilitzant el QPixmap com a textura
    gameBrush=new QBrush(*gameBackImage);
    // estableix el pincell (brush) com a fons de l'escena
    gameView->setBackgroundBrush(*gameBrush);
    // afegim els jugadors
    gMakePlayers(PLAYER_TYPE_ARROW);
}

void GameController::gMakePlayers(int playerType)
{
int counter;
Player* newPlayer=nullptr;

    if (playerType==PLAYER_TYPE_ARROW)
        for (counter=0; counter<INITIAL_ARROWS; counter++)
        {
            newPlayer=new Arrow();
            if (counter==0)
             {
                newPlayer->setPixmapPath(arrow0PixmapPath);
                newPlayer->setPos(VIEW_WIDTH/2, VIEW_HEIGHT/2);
             }
             else
                newPlayer->setPixmapPath(arrowPixmapPath);
            playerAdd(newPlayer);
        }
}

void GameController::onTamTam()
{
int counter;

    // belluguem els jugadors, el jugador 0 ens el reservem pel control de teclat...
    for (counter=1; (unsigned long)counter < playersPool.size(); counter++)
    {   // movem els jugadors
        if (playersPool[counter]->isActive()==true
                && playersPool[counter]->getStatus() == PLAYER_STATUS_LIVE
                && playersPool[counter]->isOnPlay()==true)
            // forcem un moviment aleatori, per ara
            playerMove(playersPool[counter], ((alNumGen(0,3,1) < 3)? PLAYER_ACTION_GO : alNumGen(0, 2, 1)));
    }
}

//---------------------
// belluga el jugador
//---------------------

bool GameController::playerMove (Player* player, int action)
{
bool retval=false;

    // si és un jugador vàlid
    if (player!=nullptr)
    {   // hi haurà moviment
        retval=true;
        // mirem cap a on assenyala la bruïxola
        switch (action)
        {   // i n'adjustem les noves coordinades
        case PLAYER_ACTION_GO:
            playerGo(player);
            break;
        case PLAYER_ACTION_REVERSE:
            playerGoReverse(player);
            break;
        case PLAYER_ACTION_TLEFT:
            playerGoLeft(player);
            break;
        case PLAYER_ACTION_TRIGHT:
            playerGoRight(player);
            break;
        case PLAYER_ACTION_WAIT:
            break;
        default:    // no sap on vol anar!
            retval=false;
        }
    }
    return retval;
}

//-----------------------
// el jugador va endavant
//-----------------------

void GameController::playerGo(Player* player)
{
int newXPos=0;
int newYPos=0;

    if (player==nullptr)
        return;

    // enregistrem la seva posicio
    newXPos=player->getXPos();
    newYPos=player->getYPos();

    switch(player->getCompass())
    {   // recalculem la posició segon la brúixola
    case COMPASS_N:
        newYPos-=PLAYER_XDRIFT;
        break;
    case COMPASS_NE:
        newXPos+=PLAYER_XDRIFT;
        newYPos-=PLAYER_YDRIFT;
        break;
    case COMPASS_E:
        newXPos+=PLAYER_XDRIFT;
        break;
    case COMPASS_SE:
        newXPos+=PLAYER_XDRIFT;
        newYPos+=PLAYER_YDRIFT;
        break;
    case COMPASS_S:
        newYPos+=PLAYER_YDRIFT;
        break;
    case COMPASS_SW:
        newXPos-=PLAYER_XDRIFT;
        newYPos+=PLAYER_YDRIFT;
        break;
    case COMPASS_W:
        newXPos-=PLAYER_XDRIFT;
        break;
    case COMPASS_NW:
        newXPos-=PLAYER_XDRIFT;
        newYPos-=PLAYER_YDRIFT;
        break;
    default:
        break;
    }

    // el desplacem al nou lloc?
    // si la nova posició és dins l'escena
    if ((newXPos>=0 && newXPos <= VIEW_WIDTH-PIXMAP_WIDTH)
        && (newYPos >= 0 && newYPos <= VIEW_HEIGHT-PIXMAP_HEIGHT))
    {   // posem al'hora les coordinades del jugador
        player->setPos(newXPos, newYPos);
        // i el redibuixem
        playerDraw (player, false);
    }
    else    // si no, retruc!
        switch (player->getCompass())
        {
        case COMPASS_N:
        case COMPASS_E:
        case COMPASS_S:
        case COMPASS_W:
            playerGoReverse(player);
            break;
        case COMPASS_NE:
            if (newXPos > VIEW_WIDTH-PIXMAP_WIDTH)
                playerGoLeft(player);
            else
                playerGoRight(player);
            break;
        case COMPASS_SE:
            if (newXPos > VIEW_WIDTH-PIXMAP_WIDTH)
                playerGoRight(player);
            else
                playerGoLeft(player);
            break;
        case COMPASS_SW:
            if (newXPos < 0)
                playerGoLeft(player);
            else
                playerGoRight(player);
            break;
        case COMPASS_NW:
            if (newXPos < 0)
                playerGoRight(player);
            else
                playerGoLeft(player);
            break;
        }
}

//--------------------------------
// el jugador tomba a l'esquerra
//--------------------------------

void GameController::playerGoLeft(Player* player)
{
int leftDir;

    if (player!=nullptr)
    {   // un tomb a l'esquerra tanca l'angle
        leftDir=player->getCompass() - 1;
        (leftDir < 0)? player->setCompass(COMPASS_STEPS - 1) : player->setCompass(leftDir);
        player->pointTo();
        playerDraw(player, true);
    }
}

//----------------------
// el jugador gira cua
//----------------------

void GameController::playerGoReverse(Player* player)
{
int newDir;

    if (player!=nullptr)
    {
        newDir=player->getCompass() + COMPASS_STEPS/2;
        (newDir >= COMPASS_STEPS)? player->setCompass(newDir-COMPASS_STEPS) : player->setCompass(newDir);
        player->pointTo();
        playerDraw (player, true);
    }
}

//----------------------------
// el jugador tomba a la dreta
//----------------------------
void GameController::playerGoRight(Player* player)
{
int rightDir;

    if (player!=nullptr)
    {   // un tomb a la dreta obre l'angle
        rightDir=player->getCompass() + 1;
        (rightDir >= COMPASS_STEPS)? player->setCompass(0) : player->setCompass(rightDir);
        player->pointTo();
        playerDraw(player, true);
    }
}

//----------------------------------------
// afegueix el jugador i l'hi busca lloc
//----------------------------------------
// fa servir el vector de jugadors
bool GameController::playerAdd(Player* newPlayer)
{
QGraphicsPixmapItem* facePixmap;

bool retval=false;

    if (newPlayer!=nullptr)
    {   // creem i associem l'imatge
        facePixmap=new QGraphicsPixmapItem(QPixmap(newPlayer->getPixmapPath()));
        facePixmap->setFlag(QGraphicsItem::ItemIsMovable, true);
        // i al propi objecte Player
        newPlayer->setPixmap(facePixmap);
        facePixmap->setPos(newPlayer->getXPos(),newPlayer->getYPos());
        // afegim el jugador a la llista de jugadors
        playersPool.push_back(newPlayer);
        // i l'afegim a l'escena
        gameScene->addItem(facePixmap);

        while (gameScene->collidingItems(facePixmap).size()!=0)
        {   // el busquem un lloc a l'escena, repetim fins trobar-ne un de lliure
            newPlayer->setPos(alNumGen(0,VIEW_WIDTH-PIXMAP_WIDTH,PLAYER_XDRIFT),alNumGen(0,VIEW_HEIGHT-PIXMAP_HEIGHT,PLAYER_YDRIFT));
            playerDraw(newPlayer, false);
        }
        retval=true;
    }
    return retval;
}

//-----------------------------------------------
// control del joc per teclat (per proves)
//-----------------------------------------------

void GameController::touchMe (QKeyEvent* keyEvent)
{
static Player* gamePlayer0=nullptr;
int playerAction=0;

    if(gamePlayer0 == nullptr)
    {
        gamePlayer0=playerFinder(PLAYER_TYPE_ARROW);

        if (gamePlayer0 == nullptr)
            return;
    }

    switch (keyEvent->key())
    {
    case Qt::Key_Up:
        playerAction=PLAYER_ACTION_GO;
        break;
    case Qt::Key_Left:
        playerAction=PLAYER_ACTION_TLEFT;
        break;
    case Qt::Key_Down:
        playerAction=PLAYER_ACTION_REVERSE;
        break;
    case Qt::Key_Right:
        playerAction=PLAYER_ACTION_TRIGHT;
        break;
    case Qt::Key_Space: // arrenca/atura el temporitzador
//            gamePlayer0->setCompass(COMPASS_N);
//            gamePlayer0->pointToCompass();
        (gameTamTam->isActive()==true)? gameTamTam->stop(): gameTamTam->start();
    default:
        return;
    }
    playerMove(gamePlayer0, playerAction);
}

//-----------------------------------------------
// dibuixem el jugador
//-----------------------------------------------
// si rotation = true, és que cal girar-lo
// si rotation = false, és un desplaçament lineal
//-----------------------------------------------

void GameController::playerDraw(Player *player, bool rotation)
{
QGraphicsPixmapItem* pixmapItem;

    pixmapItem=player->getPixmap();

    if (player==nullptr || player->getPixmap() == nullptr)
        return;

    if (rotation==true)
    {   // fixem el punt de gir fem la rotació
        pixmapItem->setTransformOriginPoint(pixmapItem->boundingRect().center());
        pixmapItem->setRotation(player->getCompassAngle());
        pixmapItem->update();
    }
    else
        // el posicionem a l'escena
        pixmapItem->setPos(player->getXPos(), player->getYPos());
}

Player* GameController::playerFinder(int playerType)
{
int counter;
Player* foundPlayer=nullptr;

    for (counter=0; (unsigned long)counter<playersPool.size(); counter++)
        if (playersPool[counter]->getType()==playerType)
        {
            foundPlayer = playersPool[counter];
            break;
        }
    return foundPlayer;
}

