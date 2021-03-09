#pragma once

#include "WindowsWrapper.h"

extern RECT pauseView;

int Call_Pause(unsigned int mode_action);
int Call_Options(unsigned int mode_action);
int Call_Tweaks(unsigned int mode_action);

static const char* menu_strings_table[98] = {
#ifdef JAPANESE
    "\x8E\x6E\x82\xDF\x82\xA9\x82\xE7", /* 始めから */
    "\x91\xB1\x82\xAB\x82\xA9\x82\xE7", /* 続きから */
    "\x90\xDD\x92\xE8", /* 設定 */
    "\x90\xDD\x92\xE8", /* 設定 */
    "\x91\x80\x8D\xEC", /* 操作 */
    "\x91\x80\x8D\xEC", /* 操作 */
    "\x91\x80\x8D\xEC\x81\x69\x83\x4C\x81\x5B\x83\x7B\x81\x5B\x83\x68\x81\x6A", /* 操作（キーボード） */
    "\x91\x80\x8D\xEC\x81\x69\x83\x4C\x81\x5B\x83\x7B\x81\x5B\x83\x68\x81\x6A", /* 操作（キーボード） */
    "\x91\x80\x8D\xEC\x81\x69\x83\x52\x83\x93\x83\x67\x83\x8D\x81\x5B\x83\x89\x81\x5B\x81\x6A", /* 操作（コントローラー） */
    "\x91\x80\x8D\xEC\x81\x69\x83\x52\x83\x93\x83\x67\x83\x8D\x81\x5B\x83\x89\x81\x5B\x81\x6A", /* 操作（コントローラー） */
    "\x8F\xE3", /* 上 */
    "\x89\xBA", /* 下 */
    "\x8D\xB6", /* 左 */
    "\x89\x45", /* 右 */
    "\x82\x6E\x82\x6A", /* ＯＫ */
    "\x83\x4C\x83\x83\x83\x93\x83\x5A\x83\x8B", /* キャンセル */
    "\x83\x57\x83\x83\x83\x93\x83\x76", /* ジャンプ */
    "\x94\xAD\x96\x43", /* 発砲 */
    "\x8E\x9F\x82\xCC\x95\x90\x8A\xED", /* 次の武器 */
    "\x91\x4F\x82\xCC\x95\x90\x8A\xED", /* 前の武器 */
    "\x93\xB9\x8B\xEF", /* 道具 */
    "\x83\x7D\x83\x62\x83\x76", /* マップ */
    "\x83\x7C\x81\x5B\x83\x59", /* ポーズ */
    "\x8C\xFC\x82\xAB\x82\xF0\x95\xCF\x82\xA6\x82\xB8\x82\xC9\x88\xDA\x93\xAE", /* 向きを変えずに移動 */
    "\x81\x6D\x96\xB3\x82\xB5\x81\x6E", /* ［無し］ */
    "\x82\xB1\x82\xCC\x93\xAE\x8D\xEC\x82\xC9\x8A\x84\x82\xE8\x93\x96\x82\xC4\x82\xE9\x83\x4C\x81\x5B\x82\xF0\x89\x9F\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3\x82\xA2", /* この動作に割り当てるキーを押してください */
    "\x82\xB1\x82\xCC\x93\xAE\x8D\xEC\x82\xC9\x8A\x84\x82\xE8\x93\x96\x82\xC4\x82\xE9\x83\x7B\x83\x5E\x83\x93\x82\xF0\x89\x9F\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3\x82\xA2", /* この動作に割り当てるボタンを押してください */
    "\x83\x52\x83\x93\x83\x67\x83\x8D\x81\x5B\x83\x89\x81\x5B\x8E\x67\x97\x70", /* コントローラー使用 */
    "\x83\x49\x83\x74", /* オフ */
    "\x83\x49\x83\x93", /* オン */
    "\x83\x54\x83\x45\x83\x93\x83\x68\x83\x67\x83\x89\x83\x62\x83\x4E", /* サウンドトラック */
    "\x83\x54\x83\x45\x83\x93\x83\x68\x83\x67\x83\x89\x83\x62\x83\x4E\x91\x49\x91\xF0", /* サウンドトラック選択 */
    "\x83\x74\x83\x8C\x81\x5B\x83\x80\x83\x8C\x81\x5B\x83\x67", /* フレームレート */
    "\x82\x54\x82\x4F\x82\x65\x82\x6F\x82\x72", /* ５０ＦＰＳ */
    "\x82\x55\x82\x4F\x82\x65\x82\x6F\x82\x72", /* ６０ＦＰＳ */
    "\x82\x75\x81\x5D\x82\x72\x82\x78\x82\x6D\x82\x62", /* Ｖ‐ＳＹＮＣ */
    "\x89\xF0\x91\x9C\x93\x78", /* 解像度 */
    "\x91\x53\x89\xE6\x96\xCA\x83\x82\x81\x5B\x83\x68", /* 全画面モード */
    "\x83\x45\x83\x42\x83\x93\x83\x68\x83\x45\x83\x82\x81\x5B\x83\x68\x81\x40\x82\x53\x82\x51\x82\x55\x82\x98\x82\x51\x82\x53\x82\x4F", /* ウィンドウモード　４２６ｘ２４０ */
    "\x83\x45\x83\x42\x83\x93\x83\x68\x83\x45\x83\x82\x81\x5B\x83\x68\x81\x40\x82\x57\x82\x54\x82\x51\x82\x98\x82\x53\x82\x57\x82\x4F", /* ウィンドウモード　８５２ｘ４８０ */
    "\x83\x45\x83\x42\x83\x93\x83\x68\x83\x45\x83\x82\x81\x5B\x83\x68\x81\x40\x82\x50\x82\x51\x82\x56\x82\x57\x82\x98\x82\x56\x82\x51\x82\x4F", /* ウィンドウモード　１２７８ｘ７２０ */
    "\x83\x45\x83\x42\x83\x93\x83\x68\x83\x45\x83\x82\x81\x5B\x83\x68\x81\x40\x82\x50\x82\x56\x82\x4F\x82\x53\x82\x98\x82\x58\x82\x55\x82\x4F", /* ウィンドウモード　１７０４ｘ９６０ */
    "\x83\x58\x83\x80\x81\x5B\x83\x59\x83\x58\x83\x4E\x83\x8D\x81\x5B\x83\x8B", /* スムーズスクロール */
    "\x89\xE6\x96\xCA\x90\x6B\x93\xAE\x8C\xF8\x89\xCA", /* 画面震動効果 */
    "\x83\x4A\x83\x89\x81\x5B\x83\x74\x83\x42\x83\x8B\x83\x5E\x81\x5B", /* カラーフィルター */
    "\x96\xB3\x82\xB5", /* 無し */
    "\x90\x46\x8A\x6F\x88\xD9\x8F\xED\x81\x69\x82\x50\x8C\x5E\x82\x52\x90\x46\x8A\x6F\x81\x41\x90\xD4\x90\x46\x8E\xE3\x81\x6A", /* 色覚異常（１型３色覚、赤色弱） */
    "\x90\x46\x8A\x6F\x88\xD9\x8F\xED\x81\x69\x82\x50\x8C\x5E\x82\x51\x90\x46\x8A\x6F\x81\x41\x90\xD4\x90\x46\x96\xD3\x81\x6A", /* 色覚異常（１型２色覚、赤色盲） */
    "\x90\x46\x8A\x6F\x88\xD9\x8F\xED\x81\x69\x82\x51\x8C\x5E\x82\x52\x90\x46\x8A\x6F\x81\x41\x97\xCE\x90\x46\x8E\xE3\x81\x6A", /* 色覚異常（２型３色覚、緑色弱） */
    "\x90\x46\x8A\x6F\x88\xD9\x8F\xED\x81\x69\x82\x51\x8C\x5E\x82\x51\x90\x46\x8A\x6F\x81\x41\x97\xCE\x90\x46\x96\xD3\x81\x6A", /* 色覚異常（２型２色覚、緑色盲） */
    "\x90\x46\x8A\x6F\x88\xD9\x8F\xED\x81\x69\x82\x52\x8C\x5E\x82\x52\x90\x46\x8A\x6F\x81\x41\x90\xC2\x90\x46\x8E\xE3\x81\x6A", /* 色覚異常（３型３色覚、青色弱） */
    "\x90\x46\x8A\x6F\x88\xD9\x8F\xED\x81\x69\x82\x52\x8C\x5E\x82\x51\x90\x46\x8A\x6F\x81\x41\x90\xC2\x90\x46\x96\xD3\x81\x6A", /* 色覚異常（３型２色覚、青色盲） */
    "\x83\x82\x83\x6D\x83\x4E\x83\x8D", /* モノクロ */
    "\x83\x81\x83\x4B\x83\x68\x83\x89\x83\x43\x83\x75", /* メガドライブ */
    "\x82\x6C\x82\x72\x82\x77", /* ＭＳＸ */
    "\x83\x51\x81\x5B\x83\x80\x83\x7B\x81\x5B\x83\x43", /* ゲームボーイ */
    "\x89\xE6\x96\xCA\x83\x74\x83\x89\x83\x62\x83\x56\x83\x85\x8C\xF8\x89\xCA", /* 画面フラッシュ効果 */
    "\x82\x51\x82\x4F\x82\x4F\x81\x93", /* ２００％ */
    "\x82\x50\x82\x54\x82\x4F\x81\x93", /* １５０％ */
    "\x82\x50\x82\x53\x82\x4F\x81\x93", /* １４０％ */
    "\x82\x50\x82\x52\x82\x4F\x81\x93", /* １３０％ */
    "\x82\x50\x82\x51\x82\x4F\x81\x93", /* １２０％ */
    "\x82\x50\x82\x50\x82\x4F\x81\x93", /* １１０％ */
    "\x82\x50\x82\x4F\x82\x4F\x81\x93", /* １００％ */
    "\x82\x58\x82\x4F\x81\x93", /* ９０％ */
    "\x82\x57\x82\x4F\x81\x93", /* ８０％ */
    "\x82\x56\x82\x4F\x81\x93", /* ７０％ */
    "\x82\x55\x82\x4F\x81\x93", /* ６０％ */
    "\x82\x54\x82\x4F\x81\x93", /* ５０％ */
    "\x82\x4F\x81\x93", /* ５０％ */
    "\x8A\x67\x92\xA3\x83\x43\x83\x93\x83\x5E\x83\x74\x83\x46\x81\x5B\x83\x58", /* 拡張インタフェース */ 
    "\x92\xB2\x90\xAE", /* 調整 */
    "\x92\xB2\x90\xAE", /* 調整 */
    "\x8E\xF3\x82\xAF\x82\xE9\x83\x5F\x83\x81\x81\x5B\x83\x57", /* 受けるダメージ */
    "\x88\xEA\x8C\x82\x82\xC5\x93\x7C\x82\xEA\x82\xE9", /* 一撃で倒れる */
    "\x83\x7B\x83\x58\x82\x67\x82\x6F", /* ボスＨＰ */
    "\x82\x50\x82\x67\x82\x6F", /* １ＨＰ */
    "\x98\x41\x8E\xCB\x8B\x40\x94\x5C", /* 連射機能 */
    "\x92\xB7\x89\x9F\x82\xB5", /* 長押し */
    "\x93\xFC\x82\xEA\x91\xD6\x82\xA6", /* 入れ替え */
    "\x96\xB3\x8C\xC0\x92\x65\x96\xF2", /* 無限弾薬 */
    "\x83\x66\x83\x42\x83\x74\x83\x48\x83\x8B\x83\x67\x83\x75\x81\x5B\x83\x58\x83\x5E\x81\x5B", /* ディフォルトブースター */
    "\x96\xB3\x82\xB5", /* 無し */
    "\x83\x75\x81\x5B\x83\x58\x83\x5E\x81\x5B\x82\x96\x82\x4F\x81\x44\x82\x57", /* ブースターｖ０．８ */
    "\x83\x75\x81\x5B\x83\x58\x83\x5E\x81\x5B\x82\x96\x82\x51\x81\x44\x82\x4F", /* ブースターｖ２．０ */
    "\x96\xB3\x8C\xC0\x83\x75\x81\x5B\x83\x58\x83\x5E\x81\x5B", /* 無限ブースター */
    "\x83\x57\x83\x83\x83\x93\x83\x76\x97\x5D\x97\x54", /* ジャンプ余裕 */
    "\x8C\x6F\x8C\xB1\x92\x6C\x82\xAA\x97\x8E\x82\xC6\x82\xB3\x82\xEA\x82\xC8\x82\xA2", /* 経験値が落とされない */
    "\x83\x51\x81\x5B\x83\x80\x91\xAC\x93\x78", /* ゲーム速度 */
    "\x83\x51\x81\x5B\x83\x80\x8F\x49\x97\xB9", /* ゲーム終了 */
    "\x82\xE6\x82\xEB\x82\xB5\x82\xA2\x82\xC5\x82\xB7\x82\xA9\x81\x48", /* よろしいですか？ */
    "\x83\x5A\x81\x5B\x83\x75\x82\xB3\x82\xEA\x82\xC4\x82\xA2\x82\xC8\x82\xA2\x90\x69\x8D\x73\x93\xE0\x97\x65\x82\xAA\x8E\xB8\x82\xED\x82\xEA\x82\xDC\x82\xB7\x81\x42", /* セーブされていない進行内容が失われます。 */
    "\x82\xCD\x82\xA2", /* はい */
    "\x82\xA2\x82\xA2\x82\xA6", /* いいえ */
    "\x82\x6F\x82\x60\x82\x74\x82\x72\x82\x64", /* ＰＡＵＳＥ */
    "\x91\xB1\x82\xAF\x82\xE9", /* 続ける */
    "\x83\x8A\x83\x5A\x83\x62\x83\x67", /* リセット */
    "\x81\x69\x83\x51\x81\x5B\x83\x80\x8D\xC4\x8B\x4E\x93\xAE\x95\x4B\x97\x76\x81\x6A" /* （ゲーム再起動必要） */
#elif defined(SPANISH)
    "Nueva Partida",
    "Continuar",
    "Opciones",
    "OPCIONES",
    "Controles",
    "CONTROLES",
    "Controles (Teclado)",
    "CONTROLES (TECLADO)",
    "Controles (Gamepad)",
    "CONTROLES (GAMEPAD)",
    "Arriba",
    "Abajo",
    "Izquierda",
    "Derecha",
    "Aceptar",
    "Cancelar",
    "Saltar",
    "Disparar",
    "Arma Siguiente",
    "Arma Anterior",
    "Inventario",
    "Mapa",
    "Pausa",
    "Bloq. Dirección",
    "[No asignado]",
    "Presiona una tecla para asignarla a esta acción",
    "Presiona un bot󮠰ara asignarlo a esta acción",
    "Usar Gamepad",
    "Off",
    "On",
    "Soundtrack",
    "SELECCIONAR SOUNDTRACK",
    "Framerate",
    "50 FPS", 
    "60 FPS",
    "V-sync",
    "Resolución",
    "Pantalla Completa",
    "Ventana 426x240",
    "Ventana 852x480",
    "Ventana 1278x720",
    "Ventana 1704x960",
    "Scroll Continuo",
    "Sacudidas de Pantalla",
    "Filtros de color",
    "Ninguno",
    "Protanopia (debil)",
    "Protanopia (fuerte)",
    "Deuteranopia (débill)",
    "Deuteranopia (fuerte)",
    "Tritanopia (débil)",
    "Tritanopia (fuerte)",
    "Monocromo",
    "Sega Genesis",
    "MSX",
    "Game Boy",
    "Screen Flashing",
    "200%",
    "150%",
    "140%",
    "130%",
    "120%",
    "110%",
    "100%",
    "90%",
    "80%",
    "70%",
    "60%",
    "50%",
    "0%",
    "UI Extendida",
    "Ajustes",
    "AJUSTES",
    "Daño Recibido",
    "One-hit KO",
    "HP de Jefes",
    "1 HP",
    "Autofire",
    "Hold",
    "Switch",
    "Munición Infinita",
    "Booster por defecto",
    "Ninguno",
    "Booster 0.8",
    "Booster 2.0",
    "Booster Infinito",
    "Grace Jumps",
    "Desh. Experiencia",
    "Velocidad",
    "Salir",
    "Estás Seguro/a?",
    "Se perderá el progreso que no hayas guardado",
    "Sí",
    "No",
    "PAUSA",
    "Retomar",
    "Reiniciar",
    "REQUIERE UN REINICIO"
#else
    "New Game",
    "Continue",
    "Options",
    "OPTIONS",
    "Controls",
    "CONTROLS",
    "Controls (Keyboard)",
    "CONTROLS (KEYBOARD)",
    "Controls (Gamepad)",
    "CONTROLS (GAMEPAD)",
    "Up",
    "Down",
    "Left",
    "Right",
    "OK",
    "Cancel",
    "Jump",
    "Shoot",
    "Next Weapon",
    "Previous Weapon",
    "Inventory",
    "Map",
    "Pause",
    "Strafe",
    "[Unbound]",
    "Press a key to bind to this action",
    "Press a button to bind to this action",
    "Enable Gamepad",
    "Off",
    "On",
    "Soundtrack",
    "SELECT SOUNDTRACK",
    "Framerate",
    "50 FPS", 
    "60 FPS",
    "V-sync",
    "Resolution",
    "Full Screen",
    "Windowed 426x240",
    "Windowed 852x480",
    "Windowed 1278x720",
    "Windowed 1704x960",
    "Smooth Scrolling",
    "Screen Shake",
    "Color Filters",
    "None",
    "Protanomaly",
    "Protanopia",
    "Deuteranomaly",
    "Deuteranopia",
    "Tritanomaly",
    "Tritanopia",
    "Grayscale",
    "Genesis",
    "MSX",
    "Game Boy",
    "Screen Flashing",
    "200%",
    "150%",
    "140%",
    "130%",
    "120%",
    "110%",
    "100%",
    "90%",
    "80%",
    "70%",
    "60%",
    "50%",
    "0%",
    "Extended UI",
    "Tweaks",
    "TWEAKS",
    "Damage Received",
    "One-hit KO",
    "Boss Health",
    "1 HP",
    "Autofire",
    "Hold",
    "Switch",
    "Infinite Ammo",
    "Default Booster",
    "None",
    "Booster 0.8",
    "Booster 2.0",
    "Infinite Booster",
    "Grace Jumps",
    "No Exp Drops",
    "Game Speed",
    "Quit",
    "ARE YOU SURE?",
    "Unsaved Progress will be lost",
    "Yes", 
    "No",
    "PAUSED",
    "Resume",
    "Reset",
    "RESTART REQUIRED"
#endif
};

typedef enum PauseMenuStrings {
    NEW_GAME,
    CONTINUE,
    OPTIONS,
    OPTIONS_TITLE,
    CONTROLS,
    CONTROLS_TITLE,
    CONTROLS_KEYBOARD,
    CONTROLS_KEYBOARD_TITLE,
    CONTROLS_GAMEPAD,
    CONTROLS_GAMEPAD_TITLE,
    CONTROLS_UP,
    CONTROLS_DOWN,
    CONTROLS_LEFT,
    CONTROLS_RIGHT,
    CONTROLS_OK,
    CONTROLS_CANCEL,
    CONTROLS_JUMP,
    CONTROLS_SHOOT,
    CONTROLS_NEXT_WEAPON,
    CONTROLS_PREV_WEAPON,
    CONTROLS_INVENTORY,
    CONTROLS_MAP,
    CONTROLS_PAUSE,
    CONTROLS_STRAFE,
    CONTROLS_UNBOUND,
    CONTROLS_PUSH_KEY,
    CONTROLS_PUSH_BUTTON,
    CONTROLS_ENABLE_GAMEPAD,
    OFF,
    ON,
    SOUNDTRACK,
    SELECT_SOUNDTRACK,
    FRAMERATE,
    FRAMERATE_50,
    FRAMERATE_60,
    V_SYNC,
    RESOLUTION,
    RESOLUTION_FULL,
    RESOLUTION_240,
    RESOLUTION_480,
    RESOLUTION_720,
    RESOLUTION_960,
    SMOOTH_SCROLLING,
    SCREEN_SHAKE,
    COLOR_FILTERS,
    COLOR_NONE,
    COLOR_PROTANOMALY,
    COLOR_PROTANOPIA,
    COLOR_DEUTERANOMALY,
    COLOR_DEUTERANOPIA,
    COLOR_TRITANOMALY,
    COLOR_TRITANOPIA,
    COLOR_GRAYSCALE,
    COLOR_GENESIS,
    COLOR_MSX,
    COLOR_GAME_BOY,
    SCREEN_FLASHING,
    PERCENT_200,
    PERCENT_150,
    PERCENT_140,
    PERCENT_130,
    PERCENT_120,
    PERCENT_110,
    PERCENT_100,
    PERCENT_90,
    PERCENT_80,
    PERCENT_70,
    PERCENT_60,
    PERCENT_50,
    PERCENT_0,
    EXTENDED_UI,
    TWEAKS,
    TWEAKS_TITLE,
    DAMAGE_RECEIVED,
    DAMAGE_OHKO,
    BOSS_HP,
    BOSS_1_HP,
    AUTOFIRE,
    AUTOFIRE_HOLD,
    AUTOFIRE_SWITCH,
    INFINITE_AMMO,
    DEFAULT_BOOSTER,
    DEFAULT_BOOSTER_NONE,
    DEFAULT_BOOSTER_08,
    DEFAULT_BOOSTER_20,
    INFINITE_BOOSTER,
    GRACE_JUMPS,
    NO_EXPERIENCE_DROPS,
    GAME_SPEED,
    QUIT,
    ARE_YOU_SURE,
    UNSAVED_PROGRESS,
    YES,
    NO,
    PAUSED,
    RESUME,
    RESET,
    RESTART_REQUIRED
} PauseMenuStrings;
