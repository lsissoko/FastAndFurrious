
#include <stdlib.h>
#include "myLib.h"
#include "images/sprites2.h"
#include "images/instructionsScreen.h"
#include "images/startScreen.h"
#include "images/pauseScreen.h"
#include "images/winScreen.h"
#include "images/loseScreen.h"
#include "images/bg.h"
#include "images/clouds.h"
#include "sounds/BgSong.h"
#include "sounds/victory.h"
#include "sounds/explosion.h"
#include "sounds/cat_screech.h"


OBJ_ATTR shadowOAM[128];

Sprite player;
int health;
Sprite enemy[1];
int numEnemies = 1;
Sprite hp;
Sprite hpAmount;

int selector;
int selectRow = 70;
int selectCol = 10;

int vblankCounter = 0;

int soundAStart = 0;
int soundALooping = 0;
char *soundA;
int soundALength = 0;
int soundAFreq = 0;
int soundAPlaying = 0;
int soundADuration = 0;

int soundBStart = 0;
int soundBLooping = 0;
char *soundB;
int soundBLength = 0;
int soundBFreq = 0;
int soundBPlaying = 0;
int soundBDuration = 0;

int state;
int rows[ ] = {115, 100, 85, 70, 55, 40};

unsigned int buttons;
unsigned int oldButtons;

// prototypes
void init();
void start();
void game();
void pause();
void win();
void lose();
void instructions();
void setupSounds();
void playSoundA(const unsigned char* sound, int length, int frequency, int isLooping);
void playSoundB(const unsigned char* sound, int length, int frequency, int isLooping);
void stopSounds(int soundA, int soundB);
void setupInterrupts();
void interruptHandler();
void collide();
void hideSprites();
void clearShadowOAM();

int main() {
    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;

    REG_BG0CNT = CBB(0) | SBB(27) | BG_SIZE1 | COLOR256;
    REG_BG1CNT = CBB(1) | SBB(29) | BG_SIZE1 | COLOR256;

    state = START;

    DMANow(3, (unsigned int*) sprites2Tiles, &CHARBLOCKBASE[4], sprites2TilesLen / 2);
    DMANow(3, (unsigned int*) sprites2Pal, SPRITE_PALETTE, sprites2PalLen / 2);

    setupInterrupts();
    setupSounds();

    oldButtons = buttons;
    buttons = BUTTONS;

    while (1) {
        switch (state) {
            case START:
                start();
                break;
            case INSTRUCTIONS:
                instructions();
                break;
            case GAME:
                game();
                break;
            case LOSE:
                lose();
                break;
            case WIN:
                win();
                break;
            case PAUSE:
                pause();
                break;
        }
    }
}

void init() {
    clearShadowOAM();

    player.row = 95;
    player.col = 10;
    player.rdel = 3;
    player.cdel = 3;
    player.width = 6 * 8;
    player.height = 5 * 8;
    player.attributes = &shadowOAM[0];

    health = 3;

    int i;
    for (i = 0; i < numEnemies; i++) {
        enemy[i].row = rows[rand() % 6];
        enemy[i].col = 200;
        enemy[i].cdel = 4;
        enemy[i].width = 6 * 8;
        enemy[i].height = 5 * 8;
        enemy[i].attributes = &shadowOAM[i + 1];
    }

    hp.row = 5;
    hp.col = 5;
    hp.width = 64;
    hp.height = 32;
    hp.attributes = &shadowOAM[3];

    hpAmount.row = 5;
    hpAmount.col = hp.col + hp.width + 2;
    hpAmount.width = 16;
    hpAmount.height = 32;
    hpAmount.attributes = &shadowOAM[4];
}

void start() {
    REG_BG0HOFS = 0;
    REG_BG1HOFS = 0;

    loadPalette(startScreenPal);
    DMANow(3, (unsigned int*) startScreenTiles, &CHARBLOCKBASE[0], startScreenTilesLen / 2);
    DMANow(3, (unsigned int*) startScreenMap, &SCREENBLOCKBASE[27], startScreenMapLen / 2);

    hideSprites();
    clearShadowOAM();

    selector = 0;
    selectRow = 108;
    selectCol = 35;

    while (state == START) {
        oldButtons = buttons;
        buttons = BUTTONS;

        if (BUTTON_PRESSED(BUTTON_UP)) {
            if (selector == 1) {
                selector = 0;
                selectRow = 108;
                selectCol = 35;
            }
        }

        if (BUTTON_PRESSED(BUTTON_DOWN)) {
            if (selector == 0) {
                selector = 1;
                selectRow = 125;
                selectCol = 28;
            }
        }

        if (BUTTON_PRESSED(BUTTON_START)) {
            if (selector == 0) {
                init(); // init sprite positions
                state = GAME;
            }
            if (selector == 1) {
                state = INSTRUCTIONS;
            }
        }

        shadowOAM[2].attr0 = (ROWMASK & selectRow) | ATTR0_8BPP | ATTR0_SQUARE;
        shadowOAM[2].attr1 = (COLMASK & selectCol) | ATTR1_SIZE8;
        shadowOAM[2].attr2 = SPRITEOFFSET16(16, 0);

        // copy shadow OAM into regular OAM
        DMANow(3, shadowOAM, OAM, 128);
    }
}

void game() {
    REG_BG0HOFS = 0;
    REG_BG1HOFS = 0;

    loadPalette(bgPal);
    DMANow(3, (unsigned int*) cloudsTiles, &CHARBLOCKBASE[0], cloudsTilesLen / 2);
    DMANow(3, (unsigned int*) bgTiles, &CHARBLOCKBASE[1], bgTilesLen / 2);
    DMANow(3, (unsigned int*) cloudsMap, &SCREENBLOCKBASE[27], cloudsMapLen / 2);
    DMANow(3, (unsigned int*) bgMap, &SCREENBLOCKBASE[29], bgMapLen / 2);

    //start background song
    if (!soundBPlaying)
        playSoundB(BgSong, BGSONGLEN, BGSONGFREQ, 1);

    int hOffBg = 0;
    int hOffCloud = 0;

    hideSprites();

    // remove selector icon from shadow OAM
    shadowOAM[2].attr0 = 0;
    shadowOAM[2].attr1 = 0;
    shadowOAM[2].attr2 = 0;

    int animCount = 0;

    while (state == GAME) {
        oldButtons = buttons;
        buttons = BUTTONS;

        hOffCloud += 1;
        hOffBg += 2;

        int i;
        for (i = 0; i < numEnemies; i++) {
            // move enemy
            if (enemy[i].col <= -50) {
                enemy[i].col = 350;
                enemy[i].row = rows[rand() % 6];
            } else if (enemy[i].col > -50) {
                enemy[i].col -= enemy[i].cdel;
            }
            // cheat: freeze enemy
            if (BUTTON_HELD(BUTTON_B) && BUTTON_HELD(BUTTON_A)) {
                enemy[i].cdel = 0;
            }
        }

        // update player positions in shadowOAM
        player.attributes->attr0 = (ROWMASK & player.row) | ATTR0_8BPP | ATTR0_SQUARE;
        player.attributes->attr1 = (COLMASK & player.col) | ATTR1_SIZE64;
        if (animCount >= 10)
            player.attributes->attr2 = SPRITEOFFSET16(0, 16);
        else
            player.attributes->attr2 = SPRITEOFFSET16(0, 0);

        // player movement
        if (BUTTON_HELD(BUTTON_RIGHT)) {
            if (player.col + 64 <= 240)
                player.col += player.cdel;
        }
        if (BUTTON_HELD(BUTTON_LEFT)) {
            if (player.col >= 1)
                player.col -= player.cdel;
        }
        if (BUTTON_HELD(BUTTON_DOWN)) {
            player.row = (player.row + player.height > SCREENHEIGHT) ?
                    (SCREENHEIGHT - player.height) : (player.row + player.rdel);
        }
        if (BUTTON_HELD(BUTTON_UP)) {
            player.row = (player.row < 60) ? 40 : (player.row - player.rdel);
        }

        // update enemy positions in shadowOAM
        for (i = 0; i < numEnemies; i++) {
            enemy[i].attributes->attr0 = (ROWMASK & enemy[i].row) | ATTR0_8BPP | ATTR0_SQUARE;
            enemy[i].attributes->attr1 = (COLMASK & enemy[i].col) | ATTR1_SIZE64;
            if (animCount >= 10)
                enemy[i].attributes->attr2 = SPRITEOFFSET16(8, 16);
            else
                enemy[i].attributes->attr2 = SPRITEOFFSET16(8, 0);
        }

        // increment, and wrap around, sprite animation counter
        animCount++;
        if (animCount == 20)
            animCount = 0;

        // check for player/enemy collision
        collide();

        // update hp display in shadowOAM
        hp.attributes->attr0 = (ROWMASK & hp.row) | ATTR0_8BPP | ATTR0_WIDE;
        hp.attributes->attr1 = (COLMASK & hp.col) | ATTR1_SIZE64;
        hp.attributes->attr2 = SPRITEOFFSET16(17, 0);
        hpAmount.attributes->attr0 = (ROWMASK & hpAmount.row) | ATTR0_8BPP | ATTR0_SQUARE;
        hpAmount.attributes->attr1 = (COLMASK & hpAmount.col) | ATTR1_SIZE32;
        hpAmount.attributes->attr2 = SPRITEOFFSET16(21, 4 * (health - 1)*2);

        waitForVblank();

        // copy shadowOAM into OAM
        DMANow(3, shadowOAM, OAM, 128);

        REG_BG0HOFS = hOffCloud;
        REG_BG1HOFS = hOffBg;

        // win if player travelled far enough
        if (hOffBg >= 2500) {
            stopSounds(1, 1);
            state = WIN;
        }

        // pause the game
        if (BUTTON_PRESSED(BUTTON_SELECT)) {
            hideSprites();
            state = PAUSE;
        }
    }
}

void pause() {
    stopSounds(1, 0);

    REG_BG0HOFS = 0;
    REG_BG1HOFS = 0;

    hideSprites();

    loadPalette(pauseScreenPal);
    DMANow(3, (unsigned int*) pauseScreenTiles, &CHARBLOCKBASE[0], pauseScreenTilesLen / 2);
    DMANow(3, (unsigned int*) pauseScreenMap, &SCREENBLOCKBASE[27], pauseScreenMapLen / 2);

    while (state == PAUSE) {
        oldButtons = buttons;
        buttons = BUTTONS;

        if (BUTTON_PRESSED(BUTTON_SELECT)) {
            state = GAME;
        }

        if (BUTTON_HELD(BUTTON_L) && BUTTON_HELD(BUTTON_R)) {
            stopSounds(1, 1);
            state = START;
        }
    }
}

void win() {
    REG_BG0HOFS = 0;
    REG_BG1HOFS = 0;

    hideSprites();

    loadPalette(winScreenPal);
    DMANow(3, (unsigned int*) winScreenTiles, &CHARBLOCKBASE[0], winScreenTilesLen / 2);
    DMANow(3, (unsigned int*) winScreenMap, &SCREENBLOCKBASE[27], winScreenMapLen / 2);

    stopSounds(1, 1);
    playSoundA(victory, VICTORYLEN, VICTORYFREQ, 0);

    while (state == WIN) {
        oldButtons = buttons;
        buttons = BUTTONS;

        if (BUTTON_PRESSED(BUTTON_START)) {
            stopSounds(1, 0); // stop victory music
            state = START;
        }
    }
}

void lose() {
    REG_BG0HOFS = 0;
    REG_BG1HOFS = 0;

    hideSprites();

    loadPalette(loseScreenPal);
    DMANow(3, (unsigned int*) loseScreenTiles, &CHARBLOCKBASE[0], loseScreenTilesLen / 2);
    DMANow(3, (unsigned int*) loseScreenMap, &SCREENBLOCKBASE[27], loseScreenMapLen / 2);

    stopSounds(1, 1);
    //playSoundA(explosion, EXPLOSIONLEN, EXPLOSIONFREQ, 0);
    playSoundA(cat_screech, CAT_SCREECHLEN, CAT_SCREECHFREQ, 0);

    while (state == LOSE) {
        oldButtons = buttons;
        buttons = BUTTONS;

        if (BUTTON_PRESSED(BUTTON_START)) {
            stopSounds(1, 0);
            state = START;
        }
    }
}

void instructions() {
    REG_BG0HOFS = 0;
    REG_BG1HOFS = 0;

    hideSprites();

    loadPalette(instructionsScreenPal);
    DMANow(3, (unsigned int*) instructionsScreenTiles, &CHARBLOCKBASE[0], instructionsScreenTilesLen / 2);
    DMANow(3, (unsigned int*) instructionsScreenMap, &SCREENBLOCKBASE[27], instructionsScreenMapLen / 2);

    while (state == INSTRUCTIONS) {
        oldButtons = buttons;
        buttons = BUTTONS;

        if (BUTTON_PRESSED(BUTTON_START)) {
            state = START;
        }
    }
}

void setupSounds() {
    REG_SOUNDCNT_X = SND_ENABLED;

    REG_SOUNDCNT_H = SND_OUTPUT_RATIO_100 |
            DSA_OUTPUT_RATIO_100 |
            DSA_OUTPUT_TO_BOTH |
            DSA_TIMER0 |
            DSA_FIFO_RESET |
            DSB_OUTPUT_RATIO_100 |
            DSB_OUTPUT_TO_BOTH |
            DSB_TIMER1 |
            DSB_FIFO_RESET;

    REG_SOUNDCNT_L = 0;
}

void playSoundA(const unsigned char* sound, int length, int frequency, int isLooping) {
    if (soundAPlaying) {
        dma[1].cnt = 0;
        REG_TM0CNT = 0;
    }

    int timerInterval = 16777216 / frequency;

    DMANow(1,
            (unsigned int) sound,
            (unsigned int) REG_FIFO_A,
            DMA_32 |
            DMA_REPEAT |
            DMA_DESTINATION_FIXED |
            DMA_AT_REFRESH |
            DMA_SOURCE_INCREMENT);

    REG_TM0CNT = 0;
    REG_TM0D = -timerInterval;
    REG_TM0CNT = TIMER_ON;

    soundAStart = vblankCounter;
    soundALooping = isLooping;
    soundA = sound;
    soundALength = length;
    soundADuration = ((length * 60) / frequency) - ((length / frequency)*3) - 1;
    soundAFreq = frequency;
    soundAPlaying = 1;
}

void playSoundB(const unsigned char* sound, int length, int frequency, int isLooping) {
    if (soundBPlaying) {
        dma[2].cnt = 0;
        REG_TM1CNT = 0;
    }

    int timerInterval = 16777216 / frequency;

    DMANow(2,
            (unsigned int) sound,
            (unsigned int) REG_FIFO_B,
            DMA_32 |
            DMA_REPEAT |
            DMA_DESTINATION_FIXED |
            DMA_AT_REFRESH |
            DMA_SOURCE_INCREMENT);

    REG_TM1CNT = 0;
    REG_TM1D = -timerInterval;
    REG_TM1CNT = TIMER_ON;

    soundBStart = vblankCounter;
    soundBLooping = isLooping;
    soundB = sound;
    soundBLength = length;
    soundBDuration = ((length * 60) / frequency) - ((length / frequency)*3) - 1;
    soundBFreq = frequency;
    soundBPlaying = 1;
}

void stopSounds(int soundA, int soundB) {
    if (soundA) {
        dma[1].cnt = 0;
        REG_TM0CNT = 0;
        soundAPlaying = 0;
    }
    if (soundB) {
        dma[2].cnt = 0;
        REG_TM1CNT = 0;
        soundBPlaying = 0;
    }
}

void setupInterrupts() {
    REG_IME = 0;
    REG_INTERRUPT = (unsigned int) interruptHandler;
    REG_IE |= INT_VBLANK;
    REG_DISPSTAT |= INT_VBLANK_ENABLE;
    REG_IME = 1;
}

void interruptHandler() {
    REG_IME = 0;

    if (REG_IF & INT_VBLANK) {
        vblankCounter++;
        if ((vblankCounter > soundAStart + soundADuration) && soundAPlaying) {
            stopSounds(1, 0);

            if (soundALooping) {
                playSoundA(soundA, soundALength, soundAFreq, soundALooping);
            }
        }

        if ((vblankCounter > soundBStart + soundBDuration) && soundBPlaying) {
            stopSounds(0, 1);

            if (soundBLooping) {
                playSoundB(soundB, soundBLength, soundBFreq, soundBLooping);
            }
        }

        REG_IF = INT_VBLANK;
    }
    REG_IME = 1;
}

void hideSprites() {
    int i;
    for (i = 0; i < 128; i++) {
        OAM[i].attr0 = ATTR0_HIDE;
    }
}

void collide() {
    int i;
    for (i = 0; i < numEnemies; i++) {
        if (player.row + player.height >= enemy[i].row &&
                player.row <= enemy[i].row + enemy[i].height &&
                player.col + player.width >= enemy[i].col &&
                player.col <= enemy[i].col + enemy[i].width) {
            //playSoundA(explosion, EXPLOSIONLEN, EXPLOSIONFREQ, 0);
            playSoundA(cat_screech, CAT_SCREECHLEN, CAT_SCREECHFREQ, 0);
            if (health > 0)
                health--;
            if (health == 0)
                state = LOSE;
            enemy[i].col = 350;
            enemy[i].row = rows[rand() % 6];
        }
    }
}

void clearShadowOAM() {
    int i;
    for (i = 0; i < 128; i++) {
        shadowOAM[i].attr0 = 0;
        shadowOAM[i].attr1 = 0;
        shadowOAM[i].attr2 = 0;
    }
}