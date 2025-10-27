#pragma once

#include <raylib.h>

// === Forward declaration of the snake ===
struct Segment;
extern struct Segment* head;

// === Global game variables ===
extern int score;
extern int highScore;
extern int lastScore;
extern int frameCounter;
extern float moveDelay;
extern int fps;
extern int headAngle;

// === Snake direction variables ===
extern Vector2 direction;
extern Vector2 nextDirection;

// === Screen and grid size ===
extern const int screenWidth;
extern const int screenHeight;
extern const int tileSize;
extern const int whiteHeight;

// === Textures and font ===
extern Texture2D headTexture;
extern Texture2D bodyTexture;
extern Texture2D legsTexture;
extern Texture2D fruitTexture;
extern Texture2D fruitTextures[5]; // FRUIT_NUMBER
extern Texture2D fenceTexture;
extern Font myFont;
extern Vector2 origin;
extern Rectangle sourceRec;

// === Colors ===
extern Color lightGreen;
extern Color darkGreen;
extern Color black;
extern Color semiTransparentBlack;

// === Audio ===
extern Music gameMusic[6];
extern Sound gameSound[2];
extern bool firstFrameTitle;
extern int playMusicPause;
extern int playMusicGameplay;
extern int playMusicEnding;
