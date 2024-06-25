# physics 2d
import turtle
import time
import os

def draw_bounds(bounds):
    bound_drawer = turtle.Turtle()
    bound_drawer.hideturtle()
    bound_drawer.penup()
    bound_drawer.goto(bounds[0], bounds[1])
    bound_drawer.pendown()
    bound_drawer.goto(bounds[0], bounds[3])
    bound_drawer.goto(bounds[2], bounds[3])
    bound_drawer.goto(bounds[2], bounds[1])
    bound_drawer.goto(bounds[0], bounds[1])

def main():
    wn = turtle.Screen()
    wn.tracer(0)

    # draw the boundary
    with open("./sim/info.txt") as file:
        temp = file.readlines()
        bounds = [int(float(x)) for x in temp[0].strip().split(',')]
        num_bodies = int(temp[1])
        iter_amount = int(temp[2])

    draw_bounds(bounds)

    bodies = [turtle.Turtle() for i in range(num_bodies)]

    for j in range(iter_amount):
        wn.update()
        for i, body in enumerate(bodies):
            with open(f"./sim/{i}.txt") as file:
                lines = file.readlines()
                x = float(lines[j].split(",")[0])
                y = float(lines[j].split(",")[1])
                body.penup()
                body.goto(x, y)
        time.sleep(1/60)


if __name__ == "__main__":
    main()