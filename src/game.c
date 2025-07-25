#include "game.h"
#include "cglm/ivec2.h"
#include <stdio.h>

void game_create(Game* game, int width, int height) {
    srand(time(NULL));
    glm_ivec2_copy((ivec2){ -1, -1 }, game->last_pos);
    game->finished = false;
    game->width = width;
    game->height = height;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            game->grid[i][j] = BACKGROUND;

            game->background[i][j] = model_create(((i + j) % 2) ? color_background1 : color_background2);
        }
    }

    game->grid[8][7] = FOOD;
    game->food = model_create(color_food);
    glm_ivec2_copy((ivec2){ 8, 7 }, game->food_pos);

    ivec2 snake_pos = { 4, 4 };
    game->grid[snake_pos[0]][snake_pos[1]] = SNAKE;

    game->snake = (Snake){0};
    game->snake.head = model_create(color_head);
    glm_ivec2_copy(snake_pos, game->snake.positions[0]);
}

void game_draw(Game* game, Shader shader) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            switch (game->grid[i][j]) {
                case BACKGROUND:
                    model_draw(game->background[i][j], shader, (ivec2){i, j});
                    break;
                case FOOD:
                    model_draw(game->food, shader, game->food_pos);
                    break;
                default:
                    break;
            }
        }
    }

    model_draw(game->snake.head, shader, game->snake.positions[0]);
    for (size_t i = 0; i < game->snake.num_body_parts; i++) {
        model_draw(game->snake.body[i], shader, game->snake.positions[i+1]);
    }
}

void game_move_snake(Game* game, ivec2 new_pos) {
    Snake* snake = &(game->snake);
    ivec2 head_pos = { snake->positions[0][0], snake->positions[0][1] };

    ivec2 old_pos;
    glm_ivec2_copy(head_pos, old_pos);
    glm_ivec2_add(head_pos, new_pos, head_pos);

    if (glm_ivec2_eqv(head_pos, game->last_pos)) {
        return;
    }
    glm_ivec2_copy(old_pos, game->last_pos);

    if (head_pos[0] < 0) {
        head_pos[0] = GRID_SIZE - 1;
    } else if (head_pos[0] >= GRID_SIZE) {
        head_pos[0] = 0;
    }
    if (head_pos[1] < 0) {
        head_pos[1] = GRID_SIZE - 1;
    } else if (head_pos[1] >= GRID_SIZE) {
        head_pos[1] = 0;
    }

    if (game->grid[head_pos[0]][head_pos[1]] == SNAKE) {
        printf("Finished\n");
        game->finished = true;
        return;
    }

    game->grid[old_pos[0]][old_pos[1]] = BACKGROUND;
    game->grid[head_pos[0]][head_pos[1]] = SNAKE;
    glm_ivec2_copy(head_pos, snake->positions[0]);

    ivec2 tmp;
    for (size_t i = 0; i < snake->num_body_parts; i++) {
        glm_ivec2_copy(snake->positions[i+1], tmp);
        game->grid[tmp[0]][tmp[1]] = BACKGROUND;
        game->grid[old_pos[0]][old_pos[1]] = SNAKE;
        glm_ivec2_copy(old_pos, snake->positions[i+1]);
        glm_ivec2_copy(tmp, old_pos);
    }

    if (glm_ivec2_eqv(head_pos, game->food_pos)) {
        snake->num_body_parts++;
        assert(snake->num_body_parts < (GRID_SIZE * GRID_SIZE - 1));
        snake->body[snake->num_body_parts - 1] = model_create(color_body);
        glm_ivec2_copy(old_pos, snake->positions[snake->num_body_parts]);
        game->grid[old_pos[0]][old_pos[1]] = SNAKE;

        game_place_food(game);
    }
}

void game_place_food(Game* game) {
    int limit = (GRID_SIZE * GRID_SIZE) - (game->snake.num_body_parts + 1);
    int divisor = RAND_MAX / (limit + 1);
    int rand_index;
    do { 
        rand_index = rand() / divisor;
    } while (rand_index > limit);

    int tmp = rand_index;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (game->grid[i][j] == BACKGROUND) {
                tmp--;
                if (tmp == 0) {
                    glm_ivec2_copy((ivec2){ i, j }, game->food_pos);
                    game->grid[i][j] = FOOD;
                    printf("i=%d, pos(%d,%d)\n", rand_index, i+1, j+1);
                    return;
                }
            }
        }
    }
}

void game_destroy(Game* game) {
    model_destroy(game->snake.head);
    for (size_t i = 0; i < game->snake.num_body_parts; i++) {
        model_destroy(game->snake.body[i]);
    }

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            model_destroy(game->background[i][j]);
        }
    }
}
