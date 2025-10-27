#ifndef SNAKE_H
#define SNAKE_H

#include <raylib.h>

// === SNAKE SEGMENT STRUCTURE ===
typedef struct Segment
{
    Vector2 position;        // Position of this segment on the grid
    struct Segment* next;    // Pointer to the next segment in the snake
} Segment;

// === GLOBAL VARIABLES FOR THE SNAKE ===
extern Segment* head;        // Pointer to the head segment of the snake
extern Vector2 direction;    // Current movement direction of the snake
extern Vector2 nextDirection; // Next direction the snake will move (based on input)

// === FUNCTION PROTOTYPES ===

// Creates a new snake with head, body, and tail segments
Segment* CreateSnake(void);

// Initializes the snake at the start of the game
void InitializeSnake(void);

// Handles player input to change the snake's direction
void SnakeDirectionInput(void);

// Moves the snake based on the current direction
void SnakeMovement(void);

// Checks for collision with itself (snake biting its own body)
void SelfColision(void);

// Checks for collision with screen borders
void BorderColision(void);

// Draws the snake on the screen using textures
void DrawSnake(void);

// Frees memory used by the snake's linked list
void FreeSnake(void);

#endif // SNAKE_H
