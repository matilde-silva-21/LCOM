#include "game.h"

extern int mouse_hookid, timer_hookid;
extern uint8_t keyboard_scancode, keyboard_statuscode;
extern uint8_t mouse_scancode, mouse_statuscode;
extern int ih_error;
extern int kbd_hookid; //, hookid, timer_counter;
extern int timer_counter;
//extern Mouse *mouse;

extern void *video_mem;
extern void *display_mem;

//extern Ship *ship;
extern int shipYPosition;

bool menuDisplay = true;
bool gameOver = false;

int (game_loop)() {

    bool exit = false;
    //mouse->x = 0; mouse->y = 0;
    uint16_t mode = 0x105;
    initMenuXpm();

    int mouse_bit_no;
    uint8_t kbd_bit_no;
    uint8_t timer_bit_no;
    vbe_mode_info_t info;

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
    xpm_image_t mouse_img;
    xpm_load(mouse_xpm, XPM_INDEXED, &mouse_img);
    Mouse mouse = {50, 50, mouse_img};

    xpm_image_t background = loadBackground();

    KeyActivity key;

    Ship *ship = createShip(512, SHIP_YPOS, 15);
    if (drawMenu(button))
        return 1;

    drawMouse(&mouse);

    while (keyboard_scancode != ESC_BREAK && !exit) {
        displayScreen();
        if (!menuDisplay) {
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
                            mouse_print_packet(&pp);
                            printf("x = %d     y = %d\n", mouse.x, mouse.y);
                            //eraseMouse(&mouse);
                            ///====================MENU====================
                            if (menuDisplay) {
                                updateMouseCoordinates(&pp, &mouse);
                                //fprintf(stderr, "x = %d     y = %d\n", mouse.x, mouse.y);
                                button = getButton(mouse.x, mouse.y);
                                printf("%d", button);
                                if (drawMenu(button)) {
                                    return 1;
                                }
                                drawMouse(&mouse);
                                if (pp.lb) {
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
                            ///=============================================
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
                            drawBackground(background);
                            drawShip(ship);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

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
