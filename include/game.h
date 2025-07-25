#pragma once

#include "constants.h"
#include "model.h"

typedef struct {
    Model head;
    ivec2 positions[GRID_SIZE * GRID_SIZE];
    Model body[(GRID_SIZE * GRID_SIZE) - 1];
    size_t num_body_parts;
} Snake;

enum CELL {
    BACKGROUND,
    SNAKE,
    FOOD
};

typedef struct {
    enum CELL grid[GRID_SIZE][GRID_SIZE];
    int width, height;
    Model background[GRID_SIZE][GRID_SIZE];

    Model food;
    ivec2 food_pos;

    Snake snake;
    ivec2 last_pos;
    int score;
    bool finished;
} Game;

void game_create(Game* grid, int width, int height);

void game_draw(Game* grid, Shader shader);

void game_move_snake(Game* grid, ivec2 new_pos);

void game_place_food(Game* grid);

void game_destroy(Game* grid);

