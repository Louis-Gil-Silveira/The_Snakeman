#include <raylib.h>
#include <stdlib.h>

#include "food.h"
#include "snake.h"
#include "ressources.h"
#include "game.h"
#include "hint.h"

// === GLOBAL VARIABLES FOR FRUITS AND FENCES ===
Fruit fruit[FRUIT_NUMBER];         // Array holding all possible fruits
FruitType fruitType = NORMAL_FRUIT; // Current active fruit type
Vector2 fruitPosition = { 0, 0 };  // Current fruit position on screen
Vector2 fencePosition = { 0, 0 };  // Current temporary fence position
bool fruitActive = false;           // Whether a fruit is currently active

// Fence positions array and count
Vector2 fencePositions[MAX_FENCES]; // Array holding positions of fences
int fenceCount = 0;                 // Current number of fences on screen

// === INITIALIZE FRUITS ===
// Resets fruit variables at game start
void InitFruit(void)
{
    fruitActive = false;              // No active fruit
    fruitPosition = (Vector2){ 0, 0 }; // Reset fruit position
    fencePosition = (Vector2){ 0, 0 }; // Reset fence position
    fruitType = NORMAL_FRUIT;         // Default fruit type
}

// === PLACE FRUIT ON A VALID TILE ===
// Avoids snake and fences
void FruitPlacement(Segment* snakeHead)
{
    bool validPosition = false;       // Flag for valid position

    // Loop until a valid position is found
    while (!validPosition)
    {
        // Randomly select tile coordinates for fruit
        fruitPosition.x = (float)GetRandomValue(0, (screenWidth / tileSize) - 1) * (float)tileSize;
        fruitPosition.y = (float)GetRandomValue(whiteHeight / tileSize, (screenHeight / tileSize) - 1) * (float)tileSize;

        validPosition = true; // Assume valid

        // Check against snake segments
        Segment* current = snakeHead;
        while (current != NULL)
        {
            if ((int)fruitPosition.x == (int)current->position.x &&
                (int)fruitPosition.y == (int)current->position.y)
            {
                validPosition = false; // Overlaps snake
                break;                 // Stop checking snake
            }
            current = current->next;
        }
    }
}

// === SPAWN FRUIT ===
// Called each frame to spawn a fruit if none is active
void FruitSpawn(Segment* snakeHead)
{
    if (fruitActive) return; // Already a fruit on screen

    bool validPosition = false;        // Flag for valid fruit position
    Vector2 newPos = { 0, 0 };         // Temporary position for new fruit

    // Loop until a valid position is found
    while (!validPosition)
    {
        // Generate random x and y tile coordinates
        newPos.x = (float)GetRandomValue(0, (screenWidth / tileSize) - 1) * tileSize;
        newPos.y = (float)GetRandomValue(whiteHeight / tileSize, (screenHeight / tileSize) - 1) * tileSize;

        validPosition = true; // Assume valid

        // Check overlap with snake
        Segment* current = snakeHead;
        while (current != NULL)
        {
            if ((int)current->position.x == (int)newPos.x &&
                (int)current->position.y == (int)newPos.y)
            {
                validPosition = false; // Collides with snake
                break;
            }
            current = current->next;
        }

        // Check overlap with fences
        for (int i = 0; i < fenceCount; i++)
        {
            if ((int)fencePositions[i].x == (int)newPos.x &&
                (int)fencePositions[i].y == (int)newPos.y)
            {
                validPosition = false; // Collides with fence
                break;
            }
        }
    }

    // Decide fruit type randomly (1 in 4 chance for special)
    int chance = GetRandomValue(0, 3); // 0..3
    if (chance == 0)
    {
        // Random special fruit type (RED, BLUE, ORANGE, PURPLE)
        fruitType = GetRandomValue(RED_FRUIT, PURPLE_FRUIT);
    }
    else
    {
        fruitType = NORMAL_FRUIT; // Normal fruit
    }

    fruitPosition = newPos; // Assign new position
    fruitActive = true;     // Mark fruit as active
}

// === CHECK COLLISION WITH FRUIT ===
// Handles eating fruit, adding segments, applying effects
void FruitColision(void)
{
    if (!fruitActive) return; // No fruit to eat

    // Compare head and fruit positions
    if ((int)head->position.x == (int)fruitPosition.x && 
        (int)head->position.y == (int)fruitPosition.y)
    {
        fruitActive = false; // Fruit eaten

        // --- ADD SEGMENT FOR NORMAL FRUIT ---
        Segment* newSegment = malloc(sizeof(Segment)); // Allocate new segment
        if (newSegment != NULL)
        {
            // Find last and previous segments
            Segment* last = head;
            Segment* prev = NULL;
            while (last->next != NULL)
            {
                prev = last;
                last = last->next;
            }

            if (prev == NULL) prev = head; // Only head exists

            // Position new segment based on tail direction
            newSegment->position.x = last->position.x + (last->position.x - prev->position.x);
            newSegment->position.y = last->position.y + (last->position.y - prev->position.y);
            newSegment->next = NULL;
            last->next = newSegment; // Attach to snake
        }

        // --- APPLY FRUIT TYPE EFFECTS ---
        switch (fruitType)
        {
        case NORMAL_FRUIT:
            score++;              // Increase score
            if (moveDelay > 1.0f) moveDelay -= 0.2f; // Slight speed up
            PlaySound(gameSound[0]); // Play eating sound
            break;
        case RED_FRUIT: // Speed up
            score++;
            if (moveDelay > 1.0f) moveDelay -= 1.4f;
            PlaySound(gameSound[1]);
            break;
        case BLUE_FRUIT: // Slow down
            score++;
            moveDelay += 1.4f;
            PlaySound(gameSound[1]);
            break;
        case ORANGE_FRUIT: // Add 3 segments
            score += 3;
            for (int i = 0; i < 3; i++)
            {
                Segment* newSeg = malloc(sizeof(Segment));
                if (newSeg != NULL)
                {
                    Segment* last = head;
                    Segment* prev = NULL;
                    while (last->next != NULL)
                    {
                        prev = last;
                        last = last->next;
                    }
                    if (prev == NULL) prev = head;

                    newSeg->position.x = last->position.x + (last->position.x - prev->position.x);
                    newSeg->position.y = last->position.y + (last->position.y - prev->position.y);
                    newSeg->next = NULL;
                    last->next = newSeg;
                }
            }
            PlaySound(gameSound[1]);
            break;
        case PURPLE_FRUIT: // Remove 3 segments before tail if possible
        {
            int segmentsToRemove = 3;
            while (segmentsToRemove > 0 && head != NULL && head->next != NULL && head->next->next != NULL)
            {
                Segment* current = head;
                while (current->next->next->next != NULL)
                {
                    current = current->next; // Move to segment before last
                }

                Segment* toDelete = current->next; // Segment to remove
                current->next = toDelete->next;    // Skip deleted segment
                free(toDelete);                    // Free memory
                segmentsToRemove--;
            }
            score -= 3;          // Decrease score
            if (score < 0) score = 0;
            PlaySound(gameSound[1]);
        }
        break;
        default:
            break;
        }

        // --- PLACE NEW FENCE AFTER EATING ---
        if (fenceCount < MAX_FENCES)
        {
            bool validPosition = false;  // Flag for fence placement
            Vector2 newFence = { 0, 0 }; // Temporary position

            // Find valid fence position
            while (!validPosition)
            {
                newFence.x = (float)GetRandomValue(0, (screenWidth / tileSize) - 1) * (float)tileSize;
                newFence.y = (float)GetRandomValue(whiteHeight / tileSize, (screenHeight / tileSize) - 1) * (float)tileSize;

                validPosition = true;

                // Check overlap with snake
                Segment* current = head;
                while (current != NULL)
                {
                    if (newFence.x == current->position.x && newFence.y == current->position.y)
                    {
                        validPosition = false; break;
                    }
                    if ((int)fencePosition.x == (int)fruitPosition.x && (int)fencePosition.y == (int)fruitPosition.y)
                    {
                        validPosition = false; break;
                    }
                    current = current->next;
                }

                // Check overlap with existing fences
                for (int i = 0; i < fenceCount; i++)
                {
                    if (newFence.x == fencePositions[i].x && newFence.y == fencePositions[i].y)
                    {
                        validPosition = false;
                        break;
                    }
                }
            }

            fencePositions[fenceCount] = newFence; // Save new fence position
            fenceCount++;                          // Increment fence count
        }
    }
}

// === DRAW FRUIT ===
void DrawFruit(void)
{
    if (fruitActive)
    {
        // Draw texture corresponding to current fruit type
        DrawTexture(fruitTextures[fruitType], (int)fruitPosition.x, (int)fruitPosition.y, WHITE);
    }
}

void DrawFences(void)
{
    for (int i = 0; i < fenceCount; i++)
    {
        DrawTexture(fenceTexture, (int)fencePositions[i].x, (int)fencePositions[i].y, WHITE);
    }
}

// === RESET FENCES ===
void ResetFences(void)
{
    fenceCount = 0;               // Clear fence count
    for (int i = 0; i < MAX_FENCES; i++)
    {
        fencePositions[i] = (Vector2){ 0, 0 }; // Clear positions
    }
}
