#include "phys2d.h"

static void _calculate_velocity(Body2D *body) {
    body->velocity.y -= body->GRAVITY;
    body->velocity.y *= body->FRICTION;
    body->velocity.x *= body->FRICTION;
}

static void _calculate_position(Body2D *body) {
    body->position.y += body->velocity.y;
    body->position.x += body->velocity.x;
}

static void _bounce(Body2D *body)
{
    if (body->position.y < body->BOUNDS.Y_LOWER) {
        body->position.y = body->BOUNDS.Y_LOWER;
        body->velocity.y = -body->velocity.y * body->FRICTION;
    }
    else if (body->position.y > body->BOUNDS.Y_UPPER) {
        body->position.y = body->BOUNDS.Y_UPPER;
        body->velocity.y = -body->velocity.y * body->FRICTION;
    }

    if (body->position.x < body->BOUNDS.X_LOWER) {
        body->position.x = body->BOUNDS.X_LOWER;
        body->velocity.x = -body->velocity.x * body->FRICTION;
    }
    else if (body->position.x > body->BOUNDS.X_UPPER) {
        body->position.x = body->BOUNDS.X_UPPER;
        body->velocity.x = -body->velocity.x * body->FRICTION;
    }
}

void update(Body2D *body) {
    _calculate_velocity(body);
    _calculate_position(body);
    _bounce(body);
}