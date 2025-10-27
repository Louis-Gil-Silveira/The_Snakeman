#ifndef RESSOURCES_H
#define RESSOURCES_H

#include <raylib.h>
#include <stdlib.h>

#include "game.h"
#include "snake.h"
#include "food.h"
#include "hint.h"

// === CONSTANTS ===
#define FRUIT_NUMBER 5     // Number of fruit types
#define SOUND_NUMBER 2     // Number of sound effects
#define MUSIC_NUMBER 6     // Number of music tracks

// === SCREEN SETTINGS ===
extern const int screenWidth;   // Game window width
extern const int screenHeight;  // Game window height
extern const int tileSize;      // Size of each tile in the grid
extern const int whiteHeight;   // Height of the white top HUD bar

// === COLORS ===
extern Color lightGreen;              // Light green color for tiles
extern Color darkGreen;               // Dark green color for tiles
extern Color black;                   // Black color for text
extern Color semiTransparentBlack;    // Semi-transparent black color

// === AUDIO STATE VARIABLES ===
extern bool firstFrameTitle;          // Flag for first frame of title screen
extern int playMusicPause;            // Index of pause music (-1 if none)
extern int playMusicGameplay;         // Index of gameplay music (-1 if none)
extern int playMusicEnding;           // Index of ending music (-1 if none)

// === AUDIO RESOURCES ===
extern Music gameMusic[6]; // Array of music tracks
extern Sound gameSound[2]; // Array of sound effects

// === TEXTURES AND FONT ===
extern Texture2D headTexture;                        // Snake head texture
extern Texture2D bodyTexture;                        // Snake body texture
extern Texture2D legsTexture;                        // Snake tail/legs texture
extern Texture2D fruitTextures[FRUIT_NUMBER];        // Array of fruit textures
extern Texture2D fenceTexture;                       // Fence texture
extern Font myFont;                                  // Font used for on-screen text

// === HEAD SETTINGS ===
extern int headAngle;             // Rotation angle of the snake head
extern Vector2 origin;            // Rotation origin of the head
extern Rectangle sourceRec;       // Source rectangle for head texture

// === FUNCTIONS ===

// Load all textures, fonts, sounds, and music
void LoadGameRessources(void);

// Initialize audio resources (load and set volume)
void SetAudio(void);

// Initialize textures and font resources
void SetGameTextures(void);

// Draw the green checkerboard background tiles
void DrawGreenTiles(int screenHeight, int screenWidth, int tileSize, Color lightGreen, Color darkGreen);

// Draw title screen text
void DrawTitleText(void);

// Draw gameplay screen text (score, high score, etc.)
void DrawGameplayText(void);

// Draw pause screen text
void DrawPauseText(void);

// Draw ending screen text
void DrawEndingText(void);

// Play title screen audio
void PlayTitleAudio(void);

// Play gameplay audio
void PlayGameplayAudio(void);

// Play pause screen audio
void PlayPauseAudio(void);

// Play ending screen audio
void PlayEndingAudio(void);

// Free/unload all music and sound resources
void FreeMusic(void);

// Unload all textures to free memory
void UnloadGameTextures(void);

#endif // RESSOURCES_H
