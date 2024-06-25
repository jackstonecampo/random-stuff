#ifndef PHYS2D_H
#define PHYS2D_H

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    const float X_LOWER, X_UPPER;
    const float Y_LOWER, Y_UPPER;
} Area;

typedef struct {
    Vec2 position, velocity;
    const Area BOUNDS;
    const float GRAVITY, FRICTION;
} Body2D;

// Body2D functions

static void _calculate_velocity(Body2D *body);
static void _calculate_position(Body2D *body);
static void _bounce(Body2D *body);
void update(Body2D *body);

#endif