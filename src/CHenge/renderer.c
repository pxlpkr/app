#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// #include <time.h>

#include "renderer.h"

CH_Renderer* CH_Renderer_Create(int width, int height) {
    CH_Renderer* renderer = malloc(sizeof(CH_Renderer));

    renderer->camera = CH_Camera_Create(width, height);

    renderer->width = width;
    renderer->height = height;

    renderer->geometry = CH_Array_Create();

    return renderer;
}

void CH_Renderer_Destroy(CH_Renderer* renderer) {
    CH_Camera_Destroy(renderer->camera);
    for (int i = 0; i < renderer->geometry->len; i++) {
        CH_Geometry_Destroy(renderer->geometry->at[i]);
    }
    CH_Array_Destroy(renderer->geometry);

    free(renderer);
}

void render_pixel(CH_Renderer* renderer, u_int32_t buffer[], int x, int y, u_int32_t color) {
    if (x < 0 || x >= renderer->width || y < 0 || y >= renderer->height)
        return;
    buffer[y * renderer->width + x] = color;
}

int get_in_triangle(int x, int y, CH_Point verts[]) {
    CH_Point v0, v1;

    for (int i = 0; i < 3; i++) {
        v0 = verts[i];
        if (i + 1 == 3)
            v1 = verts[0];
        else
            v1 = verts[i + 1];

        if ((v1.x - v0.x)*(y - v0.y) - (v1.y - v0.y)*(x - v0.x) < 0)
            return 0;
    }

    return 1;
}

double get_avg_dist(CH_Vector locations[]) {
    double dist = locations[0].z;
    for (int i = 1; i < 3; i++) {
        CH_Vector vector = locations[i];
        dist += vector.z;
    }
    return dist / 3;
}

double get_z(CH_Renderer* renderer, CH_Point p[], CH_Vector v[], int x, int y) {
    CH_Point b = {
        ((p[1].y-p[2].y)*(x-p[2].x)+(p[2].x-p[1].x)*(y-p[2].y))/((p[1].y-p[2].y)*(p[0].x-p[2].x)+(p[2].x-p[1].x)*(p[0].y-p[2].y)),
        ((p[2].y-p[0].y)*(x-p[2].x)+(p[0].x-p[2].x)*(y-p[2].y))/((p[1].y-p[2].y)*(p[0].x-p[2].x)+(p[2].x-p[1].x)*(p[0].y-p[2].y))
    };

    double r = v[0].z*b.x+v[1].z*b.y+v[2].z*(1-b.x-b.y);

    return r;
}

void render_geometry(CH_Renderer* renderer, CH_Geometry* geometry, u_int32_t buffer[], double zbuffer[]) {

    CH_Vector locations3d[] = {
        CH_Camera_GetRelativeCoordinates(renderer->camera, geometry->vertices[0]),
        CH_Camera_GetRelativeCoordinates(renderer->camera, geometry->vertices[1]),
        CH_Camera_GetRelativeCoordinates(renderer->camera, geometry->vertices[2])
    };
    
    if (locations3d[0].z < 0 && locations3d[1].z < 0 && locations3d[2].z < 0)
        return;

    int min_x = renderer->width;
    int max_x = 0;
    int min_y = renderer->height;
    int max_y = 0;

    CH_Point locations2d[] = {
        CH_Camera_Transform(renderer->camera, &locations3d[0]),
        CH_Camera_Transform(renderer->camera, &locations3d[1]),
        CH_Camera_Transform(renderer->camera, &locations3d[2])
    };

    for (int i = 0; i < 3; i++) {
        CH_Point p = locations2d[i];
        if (p.x > max_x)
            max_x = p.x;
        if (p.x < min_x)
            min_x = p.x;
        if (p.y > max_y)
            max_y = p.y;
        if (p.y < min_y)
            min_y = p.y;
    }

    if (min_x < 0)
        min_x = 0;
    if (max_x >= renderer->width)
        max_x = renderer->width - 1;
    if (min_y < 0)
        min_y = 0;
    if (max_y >= renderer->height)
        max_y = renderer->height - 1;

    double dist = get_avg_dist(locations3d);

    for (int x = min_x; x < max_x; x++) {
        for (int y = min_y; y < max_y; y++) {
            // double dist = get_z(renderer, locations2d, locations3d, x, y);
            if (dist < zbuffer[y * renderer->width + x] && get_in_triangle(x, y, locations2d)) {
                zbuffer[y * renderer->width + x] = dist;
                render_pixel(renderer, buffer, x, y, geometry->color);
            }
        }
    }
}

void CH_Renderer_Render(CH_Renderer* renderer, u_int32_t buffer[]) {
    double zbuffer[renderer->width * renderer->height];

    for (int i = 0; i < renderer->width * renderer->height; i++)
        zbuffer[i] = renderer->camera->render_distance;

    for (int i = 0; i < renderer->geometry->len; i++) {
        CH_Geometry* geometry = (CH_Geometry*) renderer->geometry->at[i];
        render_geometry(renderer, geometry, buffer, zbuffer);
    }
}