#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <stdlib.h>

#include "ressources.h" // Access to textures, music, and sounds
#include "snake.h"      // Access to snake structures and functions
#include "food.h"       // Access to fruit structures and functions
#include "hint.h"       // Access to hint functions

// === GAME STATES ===
// Enum representing the different game screens / states
typedef enum GameScreen
{
    TITLE,      // Title screen (main menu)
    GAMEPLAY,   // Gameplay screen (active game)
    ENDING,     // End of game screen
    PAUSE       // Pause screen
} GameScreen;

// === GLOBAL VARIABLES ===

// Frame counter used for timing and animations
extern int frameCounter;

// Delay between snake movements (controls speed)
extern float moveDelay;

// Frames per second of the game
extern int fps;

// Current score in the ongoing game
extern int score;

// Highest score recorded
extern int highScore;

// Score from the last finished game
extern int lastScore;

// Current screen / game state (TITLE, GAMEPLAY, PAUSE, ENDING)
extern GameScreen currentScreen;

// === FUNCTIONS ===

// Initialize all global game variables
void SetGameVariables(void);

// Initialize the entire game (window, state, etc.)
void InitSnakeGame(void);

// Initialize all game entities (snake, fruits, fences, etc.)
void InitGameEntities(void);

// Update the title screen (animations, keyboard input, etc.)
void UpdateTitleScreen(void);

// Update the gameplay screen (snake movement, collisions, scoring)
void UpdateGameplayScreen(void);

// Update the pause screen (display, keyboard input)
void UpdatePauseScreen(void);

// Update the ending screen (display final score, restart/quit options)
void UpdateEndingScreen(void);

// Update the game based on the current screen/state (called in main loop)
void UpdateGame(void);

// Load all game resources (textures, music, sounds)
void LoadGameRessources(void);

// Unload all textures to free memory
void UnloadGameTextures(void);

// Reset the game for a new round (score, snake, fruits)
void GameReset(void);

// Free all resources and clean up the game
void FreeSnakeGame(void);

#endif // GAME_H
