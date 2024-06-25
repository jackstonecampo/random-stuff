#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>
#include "phys2d.h"

int main(void) {

    // file stuff

    const char *sim_directory = "./sim/";

    char command[256];
    snprintf(command, sizeof(command), "rm -rf %s", sim_directory);

    int status = system(command);
    if (status == 1) {
        fprintf(stderr, "failed to delete sim directory\n");
        return EXIT_FAILURE;
    }

    if (mkdir(sim_directory, 0777) == 1) {
        fprintf(stderr, "failed to create sim directory\n");
    }

    // lua config

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    const char *config_file = "./config.lua";
    if (luaL_loadfile(L, config_file) || lua_pcall(L, 0, 0, 0)) {
        fprintf(stderr, "error loading lua config file\n");
        lua_close(L);
        return EXIT_FAILURE;
    }

    lua_getglobal(L, "num_bodies");
    const int NUM_BODIES = lua_tointeger(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "num_frames");
    const int NUM_FRAMES = lua_tointeger(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "gravity");
    const float GRAVITY = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "friction");
    const float FRICTION = lua_tonumber(L, -1);
    lua_pop(L, 1);

    float tmp_bounds[4];
    lua_getglobal(L, "bounds");

    lua_getfield(L, -1, "x_lower");
    tmp_bounds[0] = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "x_upper");
    tmp_bounds[1] = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "y_lower");
    tmp_bounds[2] = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "y_upper");
    tmp_bounds[3] = lua_tonumber(L, -1);
    lua_pop(L, 1);

    Area bounds = {tmp_bounds[0], tmp_bounds[1], tmp_bounds[2], tmp_bounds[3]};

    FILE *fp;
    char filename[20];

    fp = fopen("./sim/info.txt", "a");
    fprintf(fp, "%f,%f,%f,%f", bounds.X_LOWER, bounds.Y_LOWER, bounds.X_UPPER, bounds.Y_UPPER);
    fprintf(fp, "\n%d\n%d", NUM_BODIES, NUM_FRAMES);

    // physics

    srand(time(NULL));

    for (int i = 0; i < NUM_BODIES; ++i) {
        sprintf(filename, "./sim/%d.txt", i);

        // new body with random position within bounds

        float random_x_pos = bounds.X_LOWER + (bounds.X_UPPER - bounds.X_LOWER) * rand() / (RAND_MAX + 1.0f);
        float random_y_pos = bounds.Y_LOWER + (bounds.Y_UPPER - bounds.Y_LOWER) * rand() / (RAND_MAX + 1.0f);

        float random_x_vel = 0.0 + 80.0 * rand() / (RAND_MAX + 1.0f);
        float random_y_vel = 0.0 + 80.0 * rand() / (RAND_MAX + 1.0f);

        Body2D body = {
            .position = {random_x_pos, random_y_pos},
            .velocity = {random_x_vel, random_x_vel},
            .BOUNDS = bounds,
            .GRAVITY = GRAVITY,
            .FRICTION = FRICTION,
        };

        // write sim file

        fp = fopen(filename, "a");
        if (fp == NULL) {
            perror("error opening file");
            return EXIT_FAILURE;
        }

        for (int count = 0; count < NUM_FRAMES; ++count) {
            fprintf(fp, "%f,%f\n", body.position.x, body.position.y);
            update(&body);
        }

        fclose(fp);
    }

    lua_close(L);
    return EXIT_SUCCESS;
}