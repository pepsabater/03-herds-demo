#include "global.h"
#include "player.h"

Player::Player()
{
    // característiques i estats
    type=0;
    status=PLAYER_STATUS_LIVE;
    active=true;
    onPlay=true;

    // posicionament i orientació
    compass.setPos(0, 0);
    compass.setSense(COMPASS_N);
};
