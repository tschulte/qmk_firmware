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

#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "keymap_steno.h"
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
#define LESS 2
#define MORE 3

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
    *                                        |  L6  |  L5  |       |      |  L1  |
    *                                 ,------|------|------|       |------+------+------.
    *                                 |      |      | LAlt |       | RAlt |      |      |
    *                                 |  L2  |LShift|------|       |------|RShift| Space|
    *                                 |      |      | LCtrl|       | RCtrl|      |      |
    *                                 `--------------------'       `--------------------'
    */
    [QWERTZ_NEO2] = LAYOUT_ergodox(
      // left hand
      KC_NO,     KC_1,     KC_2,  KC_3,   KC_4,        KC_5,  DE_CIRC,
      KC_NO,     DE_X,     DE_V,  DE_L,   DE_C,        DE_W,  KC_PGUP,
      KC_TAB,    DE_U,     DE_I,  DE_A,   DE_E,        DE_O,
      KC_NO,     DE_UE,    DE_OE, DE_AE,  DE_P,        DE_Z,  KC_PGDN,
      KC_APP,    KC_LGUI,  KC_NO, KC_NO,  MO(NUMBERS),
                                                        TG(TXBOLT),  MO(FKEYS),
                                                                     KC_LALT,
                                           MO(SYMBOLS), KC_LSFT,     KC_LCTL,
      // right hand
            DE_ACUT,     KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   DE_GRV,
            KC_ENT,      DE_K,   DE_H,   DE_G,   DE_F,   DE_Q,   DE_SS,
                        DE_S,   DE_N,   DE_R,   DE_T,   DE_D,   DE_Y,
            KC_DEL,      DE_B,   DE_M,   KC_COMM,KC_DOT, DE_J,   KC_BSPC,
                                MO(FUNCTIONS), KC_NO, KC_ESC, KC_RGUI, KC_NO,
            KC_NO,   TG(NEO2_NEO2),
            KC_RALT,
            KC_RCTL, KC_RSFT,       KC_SPC
  ),
  /*
    * Keymap: Layer 1: Neo2 when PC is set to Neo2
    *
    * ,--------------------------------------------------.           ,--------------------------------------------------.
    * |        |   1  |   2  |   3  |   4  |   5  |   ê  |           |   é   |   6  |   7  |   8  |   9  |   0  |   è   |
    * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    * |        |   x  |   v  |   l  |   c  |   w  |      |           |      |   k  |   h  |   g  |   f  |   q  |   ß    |
    * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    * | Tab    |   u  |   i  |   a  |   e  |   o  |------|           |------|   s  |   n  |   r  |   t  |   d  |   y    |
    * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    * |        |   ü  |   ö  |   ä  |   p  |   z  |      |           |      |   b  |   m  |   ,  |   .  |   j  |        |
    * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
    *   |      |      |      |      |      |                                       |      |      |      |      |      |
    *   `----------------------------------'                                       `----------------------------------'
    *                                        ,-------------.       ,-------------.
    *                                        |      |      |       |      |      |
    *                                 ,------|------|------|       |------+------+------.
    *                                 |      |      |      |       |      |      |      |
    *                                 | CAPS |      |------|       |------|      |      |
    *                                 |      |      |      |       |      |      |      |
    *                                 `--------------------'       `--------------------'
    */
  [NEO2_NEO2] = LAYOUT_ergodox(
      // left hand
      KC_TRNS, NEO_1,   NEO_2,   NEO_3,   NEO_4,   NEO_5, NEO_CIRC,
      KC_TRNS, NEO_X,   NEO_V,   NEO_L,   NEO_C,   NEO_W, KC_TRNS,
      KC_TRNS, NEO_U,   NEO_I,   NEO_A,   NEO_E,   NEO_O,
      KC_TRNS, NEO_UE,  NEO_OE,  NEO_AE,  NEO_P,   NEO_Z, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                                    KC_TRNS,  KC_TRNS,
                                                              KC_TRNS,
                                          NEO_L1_L, KC_TRNS,  KC_TRNS,
      // right hand
                  NEO_ACUT,  NEO_6,   NEO_7,   NEO_8,    NEO_9,   NEO_0,   NEO_GRV,
                  KC_TRNS,   NEO_K,   NEO_H,   NEO_G,    NEO_F,   NEO_Q,   NEO_SS,
                             NEO_S,   NEO_N,   NEO_R,    NEO_T,   NEO_D,   NEO_Y,
                  KC_TRNS,   NEO_B,   NEO_M,   NEO_COMM, NEO_DOT, NEO_J,   KC_TRNS,
                                      KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,
      KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS
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
  [SYMBOLS] = LAYOUT_ergodox(
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, DE_UNDS, DE_LBRC, DE_RBRC, M(CIRC), KC_TRNS,
      KC_TRNS, DE_BSLS, DE_SLSH, DE_LCBR, DE_RCBR, DE_ASTR,
      KC_TRNS, DE_HASH, DE_DLR,  DE_PIPE, DE_TILD, M(GRV),  KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                                      KC_TRNS, KC_TRNS,
                                                               KC_TRNS,
                                            KC_TRNS,  KC_TRNS, KC_TRNS,
  // right hand
      KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS,  DE_EXLM, M(LESS), M(MORE), DE_EQL,  DE_AMPR, KC_TRNS,
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
  [NUMBERS] = LAYOUT_ergodox(
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                                    KC_TRNS, KC_TRNS,
                                                             KC_TRNS,
                                          KC_TRNS,  KC_TRNS, KC_TRNS,
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
  [FUNCTIONS] = LAYOUT_ergodox(
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
    * | Teensy |      |      |      |      |      |      |           |      |  F10  |  F11  | F12  |    |      |        |
    * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    * |        |      |      |      |      |      |      |           |      |  F7  |  F8  |  F9  |      |      |        |
    * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    * |        |      |      |      |      |      |------|           |------|  F4  |  F5  |  F6  |      |      |        |
    * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    * |        |      |      |      |      |      |      |           |      |  F1  |   F2 |  F3  |      |      |        |
    * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
    *   |      |      |      |      |      |                                       |      |      |      |      |      |
    *   `----------------------------------'                                       `----------------------------------'
    *                                        ,-------------.       ,-------------.
    *                                        |      |  L0  |       |      |      |
    *                                 ,------|------|------|       |------+------+------.
    *                                 |      |      |      |       |      |      |      |
    *                                 |      |      |------|       |------|      |      |
    *                                 |      |      |      |       |      |      |      |
    *                                 `--------------------'       `--------------------'
    */
  [FKEYS] = LAYOUT_ergodox(
      RESET,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                                     KC_TRNS, KC_TRNS,
                                                              KC_TRNS,
                                            KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
      KC_TRNS,  KC_TRNS,  KC_F10,  KC_F11,  KC_F12,   KC_TRNS, KC_TRNS,
      KC_TRNS,  KC_TRNS,   KC_F7,   KC_F8,   KC_F9,   KC_TRNS, KC_TRNS,
                KC_TRNS,   KC_F4,   KC_F5,   KC_F6,   KC_TRNS, KC_TRNS,
      KC_TRNS,  KC_TRNS,   KC_F1,   KC_F2,   KC_F3,   KC_TRNS, KC_TRNS,
                           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,
      KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS
  ),

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
  [TXBOLT] = LAYOUT_ergodox(
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
      KC_NO,   STN_N1,  STN_N2,  STN_N3,  STN_N4,  STN_N5,  KC_NO,
      KC_NO,   STN_S1,  STN_TL,  STN_PL,  STN_HL,  STN_ST1,
      KC_NO,   STN_S2,  STN_KL,  STN_WL,  STN_RL,  STN_ST2, KC_NO,
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                                     KC_TRNS, KC_NO,
                                                              KC_NO,
                                            STN_A,   STN_O,   KC_NO,
  // right hand
      KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
      KC_NO,    STN_N6,  STN_N7,  STN_N8,  STN_N9,  STN_NA,  STN_NB,
                STN_ST3, STN_FR,  STN_PR,  STN_LR,  STN_TR,  STN_DR,
      KC_NO,    STN_ST4, STN_RR,  STN_BR,  STN_GR,  STN_SR,  STN_ZR,
                        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
      KC_NO,   KC_NO,
      KC_NO,
      KC_NO,   STN_E,   STN_U
  )
};

uint16_t scan_no = 0;

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
  if (record->event.pressed) {
    switch (id) {
      case CIRC: return TPE_ND(DE_CIRC);
      case GRV: return MOD_ND(KC_LSFT, DE_ACUT);
      case LESS: return TPE(DE_LESS);
      case MORE: return MOD(KC_LSFT, DE_LESS);
    }
  }
  return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

  steno_set_mode(STENO_MODE_BOLT);
  // or
  // steno_set_mode(STENO_MODE_GEMINI);
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  uint8_t layer = biton32(layer_state);

  ergodox_board_led_off();
  ergodox_right_led_1_off();
  ergodox_right_led_2_off();
  ergodox_right_led_3_off();
  if (layer == NEO2_NEO2) {
    scan_no++;
  } else {
    scan_no = 0;
  }
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
