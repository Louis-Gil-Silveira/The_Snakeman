#include <raylib.h>
#include <stdlib.h>

#include "game.h"
#include "ressources.h"
#include "snake.h"
#include "food.h"
#include "hint.h"

// === MAIN ENTRY POINT ===
// Initializes the game, runs the main loop, and frees resources on exit
int main(void)
{
    // Initialize the snake game: window, audio, textures, variables
    InitSnakeGame();

    // === MAIN GAME LOOP ===
    // Runs until the user closes the window
    while (!WindowShouldClose())
    {
        UpdateGame();  // Update the game based on the current screen
    }

    // Free all allocated memory and resources
    FreeSnakeGame();

    // Close audio and graphics devices properly
    CloseAudioDevice();
    CloseWindow();

    return 0;  // Exit successfully
}
