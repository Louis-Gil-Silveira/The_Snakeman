#ifndef FOOD_H
#define FOOD_H

#include <raylib.h>
#include <stdlib.h>

#include "game.h"
#include "ressources.h"
#include "snake.h"

// === CONSTANTS ===
#define FRUIT_NUMBER 5       // Total number of fruit types
#define MAX_FENCES 100       // Maximum number of fences on the field

// === FRUIT STRUCT ===
typedef struct Fruit {
    Vector2 position;        // Position of the fruit on the grid
    int type;                // Type of the fruit (matches FruitType enum)
    bool active;             // Whether this fruit is currently active/spawned
} Fruit;

// Array of fruits currently in the game
extern Fruit fruit[FRUIT_NUMBER];

// === FRUIT TYPES ENUM ===
typedef enum FruitType
{
    NORMAL_FRUIT,  // Normal fruit
    RED_FRUIT,     // Red fruit
    BLUE_FRUIT,    // Blue fruit
    ORANGE_FRUIT,  // Orange fruit
    PURPLE_FRUIT,  // Purple fruit
    FRUIT_COUNT    // Total number of fruit types
} FruitType;

// Current fruit type in use
extern FruitType fruitType;

// === FENCE VARIABLES ===
extern Vector2 fencePositions[MAX_FENCES];  // Positions of all fences
extern int fenceCount;                      // Current number of fences on the field

// === GLOBAL VARIABLES FOR FRUITS ===
extern Vector2 fruitPosition;               // Current active fruit position
extern Vector2 fencePosition;               // Current fence being placed
extern bool fruitActive;                    // Flag: is a fruit active on screen
extern Texture2D fruitTextures[FRUIT_NUMBER]; // Array of fruit textures
extern Texture2D fenceTexture;             // Fence texture

// === FUNCTION PROTOTYPES ===

// Initialize fruit states and setup
void InitFruit(void);

// Determine a valid position for fruit placement based on snake position
void FruitPlacement(Segment* snakeHead);

// Spawn a fruit at a valid location on the grid
void FruitSpawn(Segment* snakeHead);

// Check and handle collision between the snake and the fruit
void FruitColision(void);

// Draw the fruit(s) on screen
void DrawFruit(void);

// Draw the fences on screen
void DrawFences(void);

// Handle collision between the snake and fences
void FenceColision(void);

// Reset all fences (clear positions and count)
void ResetFences(void);

#endif // FOOD_H
