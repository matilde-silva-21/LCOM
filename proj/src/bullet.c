#include "bullet.h"

ShipBullet *createShipBullet(int x, int y, int speed) {
    ShipBullet *shipBullet = (ShipBullet *) malloc(sizeof(ShipBullet));

    shipBullet->x = x;
    shipBullet->y = y;
    shipBullet->speed = speed;

    xpm_image_t shipBullet_xpm;
    xpm_load(bullet, XPM_INDEXED, &shipBullet_xpm);

    shipBullet->img = shipBullet_xpm;

    return shipBullet;
}

AlienBullet *createAlienBullet(int x, int y, int speed) {
    AlienBullet *alienBullet = (AlienBullet *) malloc(sizeof(AlienBullet));

    alienBullet->x = x;
    alienBullet->y = y;
    alienBullet->speed = speed;

    xpm_image_t alienBullet_xpm;
    xpm_load(bullet, XPM_INDEXED, &alienBullet_xpm);

    alienBullet->img = alienBullet_xpm;

    return alienBullet;
}

void updateShipBulletPosition(ShipBullet *shipBullet) {
    shipBullet->y = shipBullet->y - shipBullet->speed;
}

void updateAlienBulletPosition(AlienBullet *alienBullet) {
    alienBullet->y = alienBullet->y - alienBullet->speed;
}
