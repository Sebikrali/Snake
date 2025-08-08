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
    Model background[GRID_SIZE][GRID_SIZE];

    Model food;
    ivec2 food_pos;

    Snake snake;
    ivec2 direction, last_pos;
    float time_since_last_tick;

    int score;
    bool finished;
} Game;

void game_create(Game* game);

void game_draw(Game* game, Shader shader);

void game_update(Game* game, float dt);

void game_set_snake_movement(Game* game, ivec2 dir);
void game_move_snake(Game* game, ivec2 new_pos);

void game_place_food(Game* game);

void game_destroy(Game* game);

