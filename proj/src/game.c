#include "game.h"

extern int mouse_hookid, timer_hookid, kbd_hookid;

extern uint8_t keyboard_scancode, keyboard_statuscode;
extern uint8_t mouse_scancode, mouse_statuscode;

extern int ih_error;
extern int timer_counter;

extern void *video_mem;
extern void *display_mem;

bool menuDisplay = false;
bool instructionDisplay = false;
extern ShipBullet *shipBullets[MAX_SHIP_BULLETS];
//extern Alien *aliens[ROW_ALIENS * COL_ALIENS];

bool gameOver = false;
int row = 0;

int (game_loop)() {

    int killCount = 0;
    bool exit = false;
    uint16_t mode = 0x105;
    initMenuXpm();

    int mouse_bit_no;
    uint8_t kbd_bit_no;
    uint8_t timer_bit_no;
    vbe_mode_info_t info;

    int ipf = ((int) sys_hz()) / 60, speed = 1; // 60 = frame rate

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
/*
    if (send_mouse_command(ENABLE_MOUSE)) {
        return 1;
    }
*/
    mouse_enable_data_reporting();
    if (kbd_subscribe_int(&kbd_bit_no)) {
        return 1;
    }

    if (timer_subscribe_int(&timer_bit_no)) {
        return 1;
    }

    int ipc_status, r;
    message msg;
    struct packet pp;
    int currentByte = 1;
    uint8_t mouseBytes[3];
    int size = 1;
    uint8_t kbdBytes[2];

    Button button = Initial;
    bool instruction_button = false;

    xpm_image_t background = loadBackground();
    xpm_image_t shipBullet_img = loadShipBulletXpm();

    KeyActivity key;

    Mouse *mouse = createMouse(50, 50);
    Ship *ship = createShip(512, SHIP_YPOS, 15);
    initShipBullets(shipBullets);
    //Alien aliens[sizeOfAliens];
    //createAliens();

    Alien aliens[sizeOfAliens]; /*= {
            createAlien(24, 20, alien1, alien1_m),
            createAlien(300, 20, alien2, alien2_m),
            createAlien(600, 20, alien3, alien3_m)
    };*/

    int xi = 24, yi = 20;

    int indice = 0;

    for (int column = 0; column < COL_ALIENS; column++) {
        for (int row = 0; row < ROW_ALIENS; row++) {
            if (row == 0) {
                printf("row = %d; col = %d; ind = %d; x = %d; y = %d\n", row, column, indice, xi, yi);
                aliens[indice] = createAlien(xi, yi, alien3, alien3_m);
            } else if (row == 1) {
                printf("row = %d; col = %d; ind = %d; x = %d; y = %d\n", row, column, indice, xi, yi);
                aliens[indice] = createAlien(xi, yi, alien1, alien1_m);
            } else {
                printf("row = %d; col = %d; ind = %d; x = %d; y = %d\n", row, column, indice, xi, yi);
                aliens[indice] = createAlien(xi, yi, alien2, alien2_m);
            }
            yi += 60;
            indice++;
        }

        yi = 20;
        xi += 80;
    }

    if (drawMenu(button))
        return 1;

    drawMouse(mouse);

    while (keyboard_scancode != ESC_BREAK && !exit && !gameOver) {
        displayScreen();
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
                            // 1 byte do packet
                            if ((mouse_scancode & BIT(3)) == 0) {
                                continue; // if the bit(3) is different from 1 wait for the next interrupt
                            }
                            currentByte = 2;
                            mouseBytes[0] = mouse_scancode;
                        } else if (currentByte == 2) {
                            // 2 byte do packet
                            currentByte = 3;
                            mouseBytes[1] = mouse_scancode;
                        } else if (currentByte == 3) {
                            // 3 byte do packet
                            currentByte = 1;
                            mouseBytes[2] = mouse_scancode;
                            getMousePacket(&pp, mouseBytes);
                            //mouse_print_packet(&pp);
                
                            ///====================MENU====================
                            updateMouse(&pp,mouse); // updates mouse coordinates and rb_pressed variable according to the given packet
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
                                            menuDisplay = false;
                                            instructionDisplay = true;
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

                            ///====================Instructions====================
                            if(instructionDisplay){
                                instruction_button = getInstructionButton(mouse->x, mouse->y);
                                if(drawInstructions(instruction_button)){
                                    return 1;
                                }
                                drawMouse(mouse);
                                if (mouse->lb_pressed) {
                                    if(instruction_button){
                                        menuDisplay = true;
                                        instructionDisplay = false;
                                        button = Initial;
                                        drawMenu(button);
                                    }
                                
                                }
                                
                            }
                            
                            ///====================SHIP SHOOT====================
                            else {
                                //printf("Not in menu plz help\n");
                                if (mouse->lb_pressed){// && ship->canShoot) {
                                    createShipBullet(ship->x + ship->img.width / 2, ship->y,SHIP_BULLET_SPEED, shipBullet_img);
                                    printf("Creating bullet\n");
                                    //shipShoot(shipBullets, shipBullet);
                                    //ship->canShoot = false;
                                    //drawShipBullets();
                                    mouse->lb_pressed = false;
                                }
                                else{
                                    printf("Not creating bullet\n");
                                }
                            }
                        }
                    }
                    else if(mouse_statuscode & OBF_BIT){
                        util_sys_inb(OUT_BUF, &mouse_scancode);
                        ih_error = 0;
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
                        if (!menuDisplay && !instructionDisplay) {
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
                        }
                    }
                    if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
                        updateShipBulletPosition();
                        timer_int_handler();
                        if (!menuDisplay && !instructionDisplay) {

                            drawBackground(background);
                            drawShip(ship);
                            drawShipBullets();
                        }

                    }
                    break;
                default:
                    break;
            }
        }
        if (!gameOver && !menuDisplay && !instructionDisplay) {
            //displayScreen();
            if (timer_counter >= ipf) {

                updateShipBulletPosition();
                frame_counter++;
                drawBackground(background);
                drawShip(ship);
                drawShipBullets();
                drawLives(ship);

                printf("\nkill count: %d", killCount);

                for (int i = 0; i < sizeOfAliens; i++) {
                    Alien *a = &aliens[i];
                    if(!(a->alive)) {continue;}
                    if (right_mov) {
                        change_alien_x_coordinates(a, speed);
                        verifyAlienAndBulletCollision(a, &killCount);
                        printf("\nkill count: %d", killCount);
                        drawAlien(a, mov_img);
                        if ((a->x + a->width) >= x_right_border) {
                            right_mov = false;
                            change_all_y(aliens, 20, sizeOfAliens);
                            if (row % 3 == 0) {
                                speed++;
                                frames_per_state--;
                            }
                        }
                    } else {
                        change_alien_x_coordinates(a, -speed);
                        verifyAlienAndBulletCollision(a, &killCount);
                        printf("\nkill count: %d", killCount);
                        drawAlien(a, mov_img);
                        if (a->x <= x_left_border) {
                            change_all_y(aliens, 20, sizeOfAliens);
                            right_mov = true;
                            if (row % 3 == 0) {
                                speed++;
                                frames_per_state--;
                            }
                        }
                    }
                    row++;
                    if ((a->y + a->height) >= territory || killCount == sizeOfAliens) {
                        gameOver = true;
                        //drawBackground(img);
                    }
                }

                timer_counter = 0;
                if (frame_counter >= frames_per_state) {
                    mov_img = !mov_img;
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



