#include "game.h"

extern int mouse_hookid, timer_hookid;
extern uint8_t keyboard_scancode, keyboard_statuscode;
extern uint8_t mouse_scancode, mouse_statuscode;
extern int ih_error;
extern int kbd_hookid; //, hookid, timer_counter;
extern int timer_counter;

extern void *video_mem;
extern void *display_mem;

extern int shipYPosition;

bool menuDisplay = false;
bool gameOver = false;
int row = 0;

int (game_loop)() {

    bool exit = false;
    uint16_t mode = 0x105;
    initMenuXpm();

    int mouse_bit_no;
    uint8_t kbd_bit_no;
    uint8_t timer_bit_no;
    vbe_mode_info_t info;

    int ipf = ((int) sys_hz()) / 60, speed = 2; // 60 = frame rate

    bool mov_img = false, right_mov = true;

    int frame_counter = 0, frames_per_state = 20;

    if (vg_get_mode_info(&mode, &info)) {
        return 1;
    }

    if (vg_set_mode(&mode)) {
        return 1;
    }

    if (mouse_subscribe_int(&mouse_bit_no)) {
        return 1;
    }

    if (send_mouse_command(ENABLE_MOUSE)) {
        return 1;
    }

    if (kbd_subscribe_int(&kbd_bit_no)) {
        return 1;
    }

    if (timer_subscribe_int(&timer_bit_no)) {
        return 1;
    }

    int ipc_status, r; //, pack_count = 0;
    message msg;
    struct packet pp;
    int currentByte = 1;
    uint8_t mouseBytes[3];
    int size = 1;
    uint8_t kbdBytes[2];

    Button button = Initial;

    xpm_image_t background = loadBackground();
    //xpm_image_t shipBullet_img = loadShipBulletXpm();

    KeyActivity key;

    Mouse *mouse = createMouse(50, 50);
    Ship *ship = createShip(512, SHIP_YPOS, 15);
    ShipBullet *shipBullets[MAX_SHIP_BULLETS]; // array with all the ship bullets in action
    initShipBullets(shipBullets);
    Alien aliens[] = {
            createAlien(24, 20, alien1, alien1_m),
            createAlien(300, 20, alien2, alien2_m),
            createAlien(600, 20, alien3, alien3_m)
    };


    if (drawMenu(button))
        return 1;

    drawMouse(mouse);

    while (keyboard_scancode != ESC_BREAK && !exit) {
        displayScreen();
        if (!menuDisplay) {
            drawBackground(background);
            drawShip(ship);
        }
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & BIT(mouse_bit_no)) {
                        mouse_ih(); // read 1 byte per interrupt -> a packet has 3 bytes
                        if (ih_error)
                            continue;
                        if (currentByte == 1) {
                            // 1º byte do packet
                            if ((mouse_scancode & BIT(3)) == 0) {
                                continue; // if the bit(3) is different from 1 wait for the next interrupt
                            }
                            currentByte = 2; // for the next iteration
                            mouseBytes[0] = mouse_scancode;
                        } else if (currentByte == 2) {
                            currentByte = 3;
                            mouseBytes[1] = mouse_scancode;
                        } else if (currentByte == 3) {
                            currentByte = 1;
                            mouseBytes[2] = mouse_scancode;
                            getMousePacket(&pp, mouseBytes);
                            //mouse_print_packet(&pp);
                            printf("x = %d     y = %d\n", mouse->x, mouse->y);
                            ///====================MENU====================
                            updateMouse(&pp,
                                        mouse); // updates mouse coordinates and rb_pressed variable according to the given packet
                            if (menuDisplay) {
                                button = getButton(mouse->x, mouse->y);
                                if (drawMenu(button)) {
                                    return 1;
                                }
                                drawMouse(mouse);
                                if (mouse->lb_pressed) {
                                    switch (button) {
                                        case StartButton:
                                            ///start game
                                            menuDisplay = false;
                                            drawBackground(background);
                                            drawShip(ship);
                                            break;
                                        case InstructionsButton:
                                            displayInstructions();
                                            break;
                                        case HallOfFameButton:
                                            break;
                                        case ExitButton:
                                            exit = true;
                                            break;
                                        case Initial:
                                            break;
                                    }
                                }
                            }
                            ///====================SHIP SHOOT====================
                            /*
                            else {
                                if (mouse->lb_pressed && ship->canShoot) {
                                    ShipBullet *shipBullet = createShipBullet(ship->x + ship->img.width / 2, ship->y,
                                                                              SHIP_BULLET_SPEED, shipBullet_img);
                                    shipShoot(shipBullets, shipBullet);
                                    ship->canShoot = false;
                                    drawShipBullets(shipBullets);
                                }
                            }
                             */
                        }
                    }
                    if (msg.m_notify.interrupts & BIT(kbd_bit_no)) {
                        kbc_ih();
                        if (twoBytes(keyboard_scancode)) {
                            size = 2;
                            continue;
                        }
                        if (size == 1) {
                            kbdBytes[0] = keyboard_scancode;
                        } else {
                            kbdBytes[0] = 0xE0;
                            kbdBytes[1] = keyboard_scancode;
                        }
                        size = 1;
                        if (!menuDisplay) {
                            switch (keyboard_scancode) {
                                case 0x1E:
                                    key = A_Pressed;
                                    break;
                                case 0x9E:
                                    key = A_Released;
                                    break;
                                case 0x20:
                                    key = D_Pressed;
                                    break;
                                case 0xAE:
                                    key = D_Released;
                                    break;
                                default:
                                    key = Invalid_key;
                                    break;
                            }
                            ship = updateShipPosition(ship, key);
                            //drawBackground(background);
                            //drawShip(ship);
                        }
                    }
                    if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
                        //mouse->lb_pressed = false;
                        if (timer_counter % 30 == 0) {
                            ship->canShoot = true;
                        }
                        timer_int_handler();
                        /*
                        if (!menuDisplay) {
                            drawShipBullets(shipBullets);
                            updateShipBulletPosition(shipBullets);
                        }
                         */
                    }
                    break;
                default:
                    break;
            }
        } else {}
        if (!gameOver && !menuDisplay) {
            if (timer_counter >= ipf) {

                frame_counter++;
                drawBackground(background);
                drawShip(ship);

                for (int i = 0; i < sizeOfAliens; i++) {
                    Alien *a = &aliens[i];
                    if (right_mov) {
                        change_alien_x_coordinates(a, speed);
                        drawAlien(a, mov_img);
                        if ((a->x + a->width) >= x_right_border) {
                            right_mov = false;
                            change_all_y(aliens, 20, sizeOfAliens);
                            if(row % 3 == 0) {
                                speed++;
                                frames_per_state--;
                            }
                            row++;
                        }
                    } else {
                        change_alien_x_coordinates(a, -speed);
                        drawAlien(a, mov_img);
                        if (a->x <= x_left_border) {
                            change_all_y(aliens, 20, sizeOfAliens);
                            right_mov = true;
                            if(row % 3 == 0) {
                                speed++;
                                frames_per_state--;
                            }
                            row++;
                        }
                    }

                    if ((a->y + a->height) >= territory) {
                        gameOver = true;
                        //drawBackground(img);
                    }
                }
                timer_counter = 0;
                if (frame_counter >= frames_per_state) {
                    if (mov_img) { mov_img = false; }
                    else { mov_img = true; }
                    frame_counter = 0;
                }
            }
        }
    }

    destroyMouse(mouse);
    destroyShip(ship);

    if (timer_unsubscribe_int()) {
        return 1;
    }

    if (send_mouse_command(DISABLE_MOUSE))
        return 1;

    if (mouse_unsubscribe_int())
        return 1;

    if (kbd_unsubscribe_int()) {
        return 1;
    }

    if (vg_exit())
        return 1;
    return 0;

}
