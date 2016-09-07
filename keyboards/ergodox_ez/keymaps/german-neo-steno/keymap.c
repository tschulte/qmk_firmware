/*
 * This layout tries to enable the NEO2 layout (http://neo-layout.org/) in hardware.
 * Since Neo2 is optimized to enable input of German texts, I assume, that the PC is
 * set to German keyboard layout (QWERTZ) with dead keys. I want this, because I want
 * to be able to just connect the keybord to any PC without having to install the Neo2
 * keyboard layout.
 *
 * Neo2 uses non-standard shifted keys, like <shift>+6 for $ (<shift>+4
 * on standard QWERTZ), <shift>+7 for € (<AltGr>+E), but also allows input of chars not
 * inputable on QWERTZ.
 *
 * This is a port of my existing sollution using the firmware of Ben Blazak
 * (https://github.com/tschulte/ergodox-firmware/blob/neo2/firmware/keyboard/ergodox/layout/neo2--tschulte.c)
 * As Oleg points out in his layout file, non-standard shifted keys are problematic.
 *
 * In the above mentioned implementation I already tried this by not using <shift> but
 * instead defining one layer for the lower-case letters, and a second layer which
 * example, I can not use the real shift-key, but instead must define one layer for the
 * lowercase letters, and the next layer for the upper-case letters.
 *
 * But this has problems:
 * - at least the second layer needs to define functions for everey key (Press shift, press key,
 *   release key, release shift)
 * - Keyboard shortcuts that need shift do not work
 * - Keyboard repeating does not work
 *
 * In the end, I did not use this idea, and have not implemented Neo2 in full detail.
 * I also opted to use macros even for the lower case letters (pressing a key sends
 * key press, directly followed by key release), thus disabling keyboard repeating.
 * This was done to prevent problems with [, ], {, }, (, ), < and >, for example I
 * was not able to type "[]" or "<>".
 *
 * As I can see, the tmk firmware is a little bit less capable of defining macros
 * (everything must be defined using FN-keys, which is a bit cumbersome). So why
 * am I doing it? I want to learn Plover (http://www.openstenoproject.org/), and
 * for this I need NKRO, which is not supported by Ben Blazak's firmware.
 */

#include "ergodox.h"
#include "debug.h"
#include "action_layer.h"
#include "sendchar.h"
#include "virtser.h"
#include "keymap_neo2.h"

// Layers

#define QWERTZ_NEO2 0 // Neo2 when PC is set to QWERTZ
#define NEO2_NEO2 1 // Neo2 when PC is set to Neo2
#define SYMBOLS 2 // SYMBOLS and function keys
#define NUMBERS 3 // Numbers
#define FUNCTIONS 4 // keyboard funktions
#define FKEYS 5 // F1-F12
#define TXBOLT 6 // TxBolt Steno Virtual Serial

// Utilities for macros

#define MOD(mod, kc) MACRO(I(15), DOWN(mod), TYPE(kc), UP(mod), END)
#define TPE(kc) MACRO(I(15), TYPE(kc), END)
#define MOD_ND(mod, kc) MACRO(I(15), DOWN(mod), TYPE(kc), UP(mod), TYPE(KC_SPC), END)
#define TPE_ND(kc) MACRO(I(15), TYPE(kc), TYPE(KC_SPC), END)

// Macro IDs

#define CIRC 0
#define GRV 1

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * Keymap: Neo2 when PC is set to QWERTZ
     * (ê, é and è stand for the dead accent key)
     *
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |   1  |   2  |   3  |   4  |   5  |   ê  |           |   é   |   6  |   7  |   8  |   9  |   0  |   è   |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |   x  |   v  |   l  |   c  |   w  | PgUp |           | Enter|   k  |   h  |   g  |   f  |   q  |   ß    |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * | Tab    |   u  |   i  |   a  |   e  |   o  |------|           |------|   s  |   n  |   r  |   t  |   d  |   y    |
     * |--------+------+------+------+------+------| PgDn |           |  Del |------+------+------+------+------+--------|
     * |        |   ü  |   ö  |   ä  |   p  |   z  |      |           |      |   b  |   m  |   ,  |   .  |   j  | Backsp |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   | App  | LGui |      |      |  L3  |                                       |   L4 |      |  ESC | RGui |      |
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |  L6  |  L5  |       |  L5  |  L1  |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      | LAlt |       | RAlt |      |      |
     *                                 |  L2  |LShift|------|       |------|RShift| Space|
     *                                 |      |      | LCtrl|       | RCtrl|      |      |
     *                                 `--------------------'       `--------------------'
     */
     [QWERTZ_NEO2] = KEYMAP(
        // left hand
        KC_NO,          KC_1,         KC_2,   KC_3,   KC_4,   KC_5,   DE_CIRC,
        KC_NO,          DE_X,         DE_V,   DE_L,   DE_C,   DE_W,   KC_PGUP,
        KC_TAB,         DE_U,         DE_I,   DE_A,   DE_E,   DE_O,
        KC_NO,          DE_UE,        DE_OE,  DE_AE,  DE_P,   DE_Z,   KC_PGDN,
        KC_APP,         KC_LGUI,      KC_NO,  KC_NO,  MO(NUMBERS),
                                              TG(TXBOLT),  MO(FKEYS),
                                                              KC_LALT,
                                               MO(SYMBOLS),KC_LSFT,KC_LCTL,
        // right hand
             DE_ACUT,     KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   DE_GRV,
             KC_ENT,      DE_K,   DE_H,   DE_G,   DE_F,   DE_Q,   DE_SS,
                          DE_S,   DE_N,   DE_R,   DE_T,   DE_D,   DE_Y,
             KC_DEL,      DE_B,   DE_M,   KC_COMM,KC_DOT, DE_J,   KC_BSPC,
                                  MO(FUNCTIONS), KC_NO, KC_ESC, KC_RGUI, KC_NO,
             MO(FKEYS), TG(NEO2_NEO2),
             KC_RALT,
             KC_RCTL,KC_RSFT, KC_SPC
    ),
    /*
     * Keymap: Layer 1: Neo2 when PC is set to Neo2
     *
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |      |      |      |      |      |  ê   |           |   é  |      |      |      |      |      |   è    |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |   Q  |   W  |   E  |   R  |   T  |      |           |      |   Z  |   U  |   I  |   O  |   P  |   UE   |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |  Tab   |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   OE |   AE   |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |   Y  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |   -  | Backsp |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      | RALT |                                       | RALT |      |       |      |      |
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |      |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 | CAPS |      |------|       |------|      |      |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     */
    [NEO2_NEO2] = KEYMAP(
        // left hand
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS, DE_CIRC,
        KC_TRNS,   NEO_X,     NEO_V,     NEO_L,     NEO_C,     NEO_W,   KC_TRNS,
        KC_TRNS,   NEO_U,     NEO_I,     NEO_A,     NEO_E,     NEO_O,
        KC_TRNS,   NEO_UE,    NEO_OE,    NEO_AE,    NEO_P,     NEO_Z,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_RALT,
                                         KC_TRNS,  KC_TRNS,
                                                   KC_TRNS,
                                KC_CAPS, KC_TRNS,  KC_TRNS,
        // right hand
            KC_RBRC,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS, DE_ACUT,
            KC_TRNS,   NEO_K,     NEO_H,     NEO_G,     NEO_F,     NEO_Q,     NEO_SS,
                       NEO_S,     NEO_N,     NEO_R,     NEO_T,     NEO_D,     NEO_Y,
            KC_TRNS,   NEO_B,     NEO_M,     NEO_COMM,  NEO_DOT,   NEO_J,     KC_TRNS,
                                  KC_RALT,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS,
        KC_TRNS,KC_TRNS, KC_TRNS
    ),
    /*
     * Keymap: Layer 2: Symbols and function keys
     *
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |      |   _  |   [  |   ]  |   ^  |      |           |      |   !  |   <  |   >  |   =  |   &  |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |   \  |   /  |   {  |   }  |   *  |------|           |------|   ?  |   (  |   )  |   -  |   :  |   @    |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |   #  |   $  |   |  |   ~  |   `  |      |           |      |   +  |   %  |   "  |   '  |   ;  |        |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      |      |                                       |      |      |      |      |      |
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |      |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 |      |      |------|       |------|      |      |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     */
    [SYMBOLS] = KEYMAP(
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, DE_UNDS, DE_LBRC, DE_RBRC, M(CIRC), KC_TRNS,
       KC_TRNS, DE_BSLS, DE_SLSH, DE_LCBR, DE_RCBR, DE_ASTR,
       KC_TRNS, DE_HASH, DE_DLR,  DE_PIPE, DE_TILD, M(GRV),  KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  DE_EXLM, DE_LESS, DE_MORE, DE_EQL,  DE_AMPR, KC_TRNS,
                 DE_QST,  DE_LPRN, DE_RPRN, DE_MINS, DE_COLN, DE_AT,
       KC_TRNS,  DE_PLUS, DE_PERC, DE_DQOT, DE_QUOT, DE_SCLN, KC_TRNS,
                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
    ),
    /*
     * Keymap: Layer 3: Numbers
     *
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |      |      |      |      |      |      |           |      |      |  7   |  8   |  9   |   +  |    -   |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |      |      |      |      |      |------|           |------|      |  4   |  5   |  6   |   ,  |    .   |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |      |      |      |      |      |      |           |      |      |  1   |  2   |  3   |   ;  |        |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      |      |                                       |  0   |      |      |      |      |
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |      |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 |      |      |------|       |------|      |      |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     */
    [NUMBERS] = KEYMAP(
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_7,    KC_8,    KC_9,    DE_PLUS, DE_MINS,
                 KC_TRNS, KC_4,    KC_5,    KC_6,    DE_COMM, DE_DOT,
       KC_TRNS,  KC_TRNS, KC_1,    KC_2,    KC_3,    DE_SCLN, KC_TRNS,
                          KC_0,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
    ),
    /*
     * Keymap: Layer 4: Keyboard functions
     *
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        | PgUp | Backs|  Up  |  Del | PgDn |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        | Home | Left | Down | Right|  End |------|           |------|      |      |      |      |      |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |      |  Tab |      | Enter|      |      |           |      |      |      |      |      |      |        |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      |      |                                       |  L0  |      |      |      |      |
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |      |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 |      |      |------|       |------|      |      |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     */
    [FUNCTIONS] = KEYMAP(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_PGUP, KC_BSPC, KC_UP,   KC_DELT, KC_PGDN, KC_TRNS,
        KC_TRNS, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END,
        KC_TRNS, KC_TRNS, KC_TAB,  KC_TRNS, KC_ENT,  KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                            KC_TRNS, KC_TRNS,
                                                     KC_TRNS,
                                   KC_TRNS, KC_TRNS, KC_TRNS,
     // right hand
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS
    ),
    /*
     * Keymap: Layer 5: F1-F12
     *
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * | Teensy |   F1 |   F2 |   F3 |   F4 |   F5 |  F6  |           |  F7  |  F8  |  F9  | F10  |  F11 |  F12 |        |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |      |      |      |      |      |------|           |------|      |      |      |      |      |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      |      |                                       |      |      |      |      |      |
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |  L0  |       |  L0  |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 |      |      |------|       |------|      |      |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     */
    [FKEYS] = KEYMAP(
        RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                            KC_TRNS, KC_TRNS,
                                                     KC_TRNS,
                                   KC_TRNS, KC_TRNS, KC_TRNS,
     // right hand
        KC_F7,    KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS
   ),
// TxBolt Codes
#define Sl 0b00000001
#define Tl 0b00000010
#define Kl 0b00000100
#define Pl 0b00001000
#define Wl 0b00010000
#define Hl 0b00100000
#define Rl 0b01000001
#define Al 0b01000010
#define Ol 0b01000100
#define X  0b01001000
#define Er 0b01010000
#define Ur 0b01100000
#define Fr 0b10000001
#define Rr 0b10000010
#define Pr 0b10000100
#define Br 0b10001000
#define Lr 0b10010000
#define Gr 0b10100000
#define Tr 0b11000001
#define Sr 0b11000010
#define Dr 0b11000100
#define Zr 0b11001000
#define NM 0b11010000
#define GRPMASK 0b11000000
#define GRP0 0b00000000
#define GRP1 0b01000000
#define GRP2 0b10000000
#define GRP3 0b11000000
    /* Keymap Layer 6: TxBolt (Serial)
     *
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |   #  |   #  |   #  |   #  |   #  |      |           |      |   #  |   #  |   #  |   #  |   #  |   #    |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |   S  |   T  |   P  |   H  |   *  |------|           |------|   *  |   F  |   P  |   L  |   T  |   D    |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |   S  |   K  |   W  |   R  |   *  |      |           |      |   *  |   R  |   B  |   G  |   S  |   Z    |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      |      |                                       |      |      |      |      |      |
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |      |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 |   A  |   O  |------|       |------|   E  |   U  |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     */
    // TxBolt over Serial
    [TXBOLT] = KEYMAP(
       KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
       KC_NO,   M(NM),   M(NM),   M(NM),   M(NM),   M(NM),  KC_NO,
       KC_NO,   M(Sl),   M(Tl),   M(Pl),   M(Hl),   M(X),
       KC_NO,   M(Sl),   M(Kl),   M(Wl),   M(Rl),   M(X),   KC_NO,
       KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                           KC_TRNS, KC_NO,
                                                    KC_NO,
                                  M(Al),   M(Ol),   KC_NO,
    // right hand
       KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
       KC_NO,  M(NM),   M(NM),   M(NM),   M(NM),   M(NM),   M(NM),
               M(X),    M(Fr),   M(Pr),   M(Lr),   M(Tr),   M(Dr),
       KC_NO,  M(X),    M(Rr),   M(Br),   M(Gr),   M(Sr),   M(Zr),
                        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
       KC_NO,   KC_NO,
       KC_NO,
       KC_NO,   M(Er),   M(Ur)
    )
};

const uint16_t PROGMEM fn_actions[] = {
};

uint8_t chord[4] = {0,0,0,0};
uint8_t pressed_count = 0;
uint16_t scan_no = 0;

void send_chord(void)
{
  for(uint8_t i = 0; i < 4; i++)
  {
    if(chord[i])
      virtser_send(chord[i]);
  }
  virtser_send(0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  // We need to track keypresses in all modes, in case the user
  // changes mode whilst pressing other keys.
  if (record->event.pressed)
    pressed_count++;
  else
    pressed_count--;
  return true;
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
  uint8_t layer = biton32(layer_state);
  if (record->event.pressed) {
    if (layer == TXBOLT) {
      uint8_t grp = (id & GRPMASK) >> 6;
      chord[grp] |= id;
    } else {
      switch (id) {
        case CIRC: return TPE_ND(DE_CIRC);
        case GRV: return MOD_ND(KC_LSFT, DE_ACUT);
      }
    }
  }
  else {
    if (pressed_count == 0 && layer == TXBOLT) {
      send_chord();
      chord[0] = chord[1] = chord[2] = chord[3] = 0;
    }
  }
  return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    if (layer == NEO2_NEO2)
        scan_no++;
    else
        scan_no = 0;
    switch (layer) {
        case NEO2_NEO2:
            if ((scan_no % 1000) < 500) {
                // blink ergodox_led
                ergodox_board_led_on();
            }
            break;
        case TXBOLT:
            ergodox_board_led_on();
            break;
        default:
            // none
            break;
    }

};
