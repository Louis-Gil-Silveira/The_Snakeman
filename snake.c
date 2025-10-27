#include <raylib.h>
#include <stdlib.h>

#include "snake.h"
#include "ressources.h"
#include "game.h"
#include "food.h"
#include "hint.h"

// === GLOBAL VARIABLES FOR SNAKE ===
Segment* head = NULL;           // Pointer to the head of the snake linked list
Vector2 direction = { 0 };      // Current movement direction of the snake
Vector2 nextDirection = { 0 };  // Next direction based on player input

// === CREATE INITIAL SNAKE ===
// Creates a snake with three segments: head -> body -> tail (legs)
Segment* CreateSnake(void)
{
    // Allocate memory for each segment
    Segment* head = malloc(sizeof(Segment));    // Head segment
    Segment* body = malloc(sizeof(Segment)); // Body segment
    Segment* legs = malloc(sizeof(Segment)); // Tail segment

    // Check for allocation failure
    if (!head || !body || !legs)
    {
        if (head) free(head);      // Free head if allocated
        if (body) free(body); // Free body if allocated
        if (legs) free(legs); // Free legs if allocated
        return NULL;          // Return NULL to indicate failure
    }

    // Set initial positions for each segment (x, y)
    head->position = (Vector2){ 256, 512 };   // Head starts in the middle of screen
    body->position = (Vector2){ 192, 512 }; // Body behind head
    legs->position = (Vector2){ 128, 512 }; // Tail behind body

    // Link segments in a singly linked list
    head->next = body;  // Head points to body
    body->next = legs; // Body points to tail
    legs->next = NULL; // Tail is the last segment

    return head; // Return pointer to head of snake
}

// === INITIALIZE SNAKE ===
// Sets direction and creates snake for the first time
void InitializeSnake(void)
{
    head = CreateSnake(); // Create the initial snake
    direction = (Vector2){ (float)tileSize, 0.0f }; // Initially move right
    nextDirection = direction; // Next direction same as initial
}

// === HANDLE PLAYER INPUT FOR SNAKE DIRECTION ===
void SnakeDirectionInput(void)
{
    // Prevent 180-degree turns by checking current direction
    if (IsKeyPressed(KEY_RIGHT) && direction.x == 0) // Can only move right if not moving horizontally
    {
        nextDirection = (Vector2){ (float)tileSize, 0.0f }; // Set next direction right
		headAngle = 90; // Rotate head sprite right     cf l. 172
    }
    else if (IsKeyPressed(KEY_LEFT) && direction.x == 0)
    {
        nextDirection = (Vector2){ -(float)tileSize, 0.0f }; // Move left
        headAngle = 270; // Rotate head sprite left
    }
    else if (IsKeyPressed(KEY_UP) && direction.y == 0)
    {
        nextDirection = (Vector2){ 0.0f, -(float)tileSize }; // Move up
        headAngle = 0; // Rotate head sprite up
    }
    else if (IsKeyPressed(KEY_DOWN) && direction.y == 0)
    {
        nextDirection = (Vector2){ 0.0f, (float)tileSize }; // Move down
        headAngle = 180; // Rotate head sprite down
    }
}

// === MOVE SNAKE ===
// Moves the snake based on current direction every moveDelay frames
void SnakeMovement(void)
{
    // Only move snake every 'moveDelay' frames
    if (frameCounter % (int)moveDelay == 0)
    {
        direction = nextDirection; // Apply the chosen next direction

        Vector2 previousPosition = head->position; // Save head's current position
        head->position.x += direction.x;           // Update head X position
        head->position.y += direction.y;           // Update head Y position

        // Move each segment to the previous segment's position
        Segment* current = head->next; // Start with body
        while (current != NULL)
        {
            Vector2 tmp = current->position;  // Save current segment position
            current->position = previousPosition; // Set current segment to previous
            previousPosition = tmp;           // Update previous position
            current = current->next;          // Move to next segment
        }
    }
}

// === CHECK SELF-COLLISION ===
// Ends the game if snake head collides with any body segment
void SelfColision(void)
{
    Segment* current = head->next; // Skip head segment
    while (current != NULL)
    {
        // Cast positions to int to compare tiles
        if ((int)head->position.x == (int)current->position.x &&
            (int)head->position.y == (int)current->position.y)
        {
            currentScreen = ENDING; // Game over
            break; // Stop checking
        }
        current = current->next; // Move to next segment
    }
}

// === CHECK BORDER COLLISION ===
// Ends the game if snake hits screen borders
void BorderColision(void)
{
    if (head->position.x >= screenWidth || head->position.x < 0 ||
        head->position.y >= screenHeight || head->position.y < whiteHeight)
    {
        currentScreen = ENDING; // Snake hits wall
    }
}

// === CHECK COLLISION WITH FENCES ===
// Ends the game if snake hits a fence
void FenceColision(void)
{
    for (int i = 0; i < fenceCount; i++) // Loop over all fences
    {
        // Compare positions (cast to int to match tile positions)
        if ((int)head->position.x == (int)fencePositions[i].x &&
            (int)head->position.y == (int)fencePositions[i].y)
        {
            currentScreen = ENDING; // Snake hits fence
            break; // Stop checking
        }
    }
}

// === DRAW SNAKE ===
// Draws head, body, and tail with proper rotation
void DrawSnake(void)
{
    // --- Draw head ---
    Rectangle destRec = {
        head->position.x + (float)tileSize / 2.0f, // Center X
        head->position.y + (float)tileSize / 2.0f, // Center Y
        (float)tileSize,                           // Width
        (float)tileSize                            // Height
    };
    DrawTexturePro(headTexture, sourceRec, destRec, origin, (float)headAngle, WHITE);

    // --- Draw body and tail ---
    Segment* prev = head;           // Previous segment
    Segment* current = head->next;  // Current segment
    while (current != NULL)
    {
        Vector2 diff = { current->position.x - prev->position.x,
                         current->position.y - prev->position.y }; // Calculate difference to determine rotation

        int angle = 0; // Default rotation
        if (diff.x > 0) angle = 270;    // Moving left
        else if (diff.x < 0) angle = 90; // Moving right
        else if (diff.y > 0) angle = 0;  // Moving up
        else if (diff.y < 0) angle = 180; // Moving down

        Texture2D tex = (current->next != NULL) ? bodyTexture : legsTexture; // Tail uses legsTexture
        Rectangle destRecSeg = {
            current->position.x + (float)tileSize / 2.0f,
            current->position.y + (float)tileSize / 2.0f,
            (float)tileSize,
            (float)tileSize
        };

        DrawTexturePro(tex, sourceRec, destRecSeg, origin, (float)angle, WHITE);

        prev = current;       // Move to next segment
        current = current->next;
    }
}

// === FREE SNAKE MEMORY ===
// Frees all segments of the snake linked list
void FreeSnake(void)
{
    Segment* current = head;
    while (current != NULL)
    {
        Segment* next = current->next; // Save next segment
        free(current);                 // Free current segment
        current = next;                // Move to next
    }
    head = NULL; // Reset head pointer
}
