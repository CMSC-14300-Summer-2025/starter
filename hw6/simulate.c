#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define G 6.67430e-11
#define MAX_NAME 32

typedef struct {
    char name[MAX_NAME];
    double mass;
    double x, y;
    double vx, vy;
} Body;

// Utility: derive output filename from input filename
void make_output_filename(char *out, size_t size, const char *input) {
    const char *base = strrchr(input, '/'); 
    base = base ? base + 1 : input; // remove path

    char name[256];
    strncpy(name, base, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';

    char *dot = strrchr(name, '.');
    if (dot) *dot = '\0'; // remove extension

    snprintf(out, size, "%s_simulate.csv", name);
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <input_file> <timesteps> <dt> <write_every>\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];
    unsigned int n_timesteps = atoi(argv[2]);
    double dt = atof(argv[3]);
    int write_every = atoi(argv[4]);
    if (write_every <= 0) write_every = 1;

    char output_file[256];
    make_output_filename(output_file, sizeof(output_file), input_file);

    FILE *f = fopen(input_file, "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    unsigned int n_bodies;
    if (fscanf(f, "%d", &n_bodies) != 1) {
        fprintf(stderr, "Failed to read header n_bodies\n");
        fclose(f);
        return 1;
    }

    Body *bodies = malloc(n_bodies * sizeof(Body));
    Body *new_bodies = malloc(n_bodies * sizeof(Body));
    if (!bodies || !new_bodies) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        return 1;
    }

    for (unsigned int i = 0; i < n_bodies; i++) {
        if (fscanf(f, " %31[^,],%lf,%lf,%lf,%lf,%lf",
                   bodies[i].name,
                   &bodies[i].mass,
                   &bodies[i].x, &bodies[i].y,
                   &bodies[i].vx, &bodies[i].vy) != 6) {
            fprintf(stderr, "Failed to read body %d\n", i);
            free(bodies);
            free(new_bodies);
            fclose(f);
            return 1;
        }
    }
    fclose(f);

    memcpy(new_bodies, bodies, n_bodies * sizeof(Body));

    f = fopen(output_file, "w");
    if (!f) {
        perror("fopen output");
        free(bodies);
        free(new_bodies);
        return 1;
    }
    fprintf(f, "step,body,x,y,vx,vy\n");

    for (unsigned int step = 0; step < n_timesteps; step++) {
        // Calculate acceleration & new velocity for each body
        for (unsigned int i = 0; i < n_bodies; i++) {
            double fx = 0, fy = 0;
            for (unsigned int j = 0; j < n_bodies; j++) {
                if (i == j) continue;
                double dx = bodies[j].x - bodies[i].x;
                double dy = bodies[j].y - bodies[i].y;
                double dist_sq = dx * dx + dy * dy + 1e-10;
                double dist = sqrt(dist_sq);
                double force = G * bodies[i].mass * bodies[j].mass / dist_sq;
                fx += force * dx / dist;
                fy += force * dy / dist;
            }
            double ax = fx / bodies[i].mass;
            double ay = fy / bodies[i].mass;

            new_bodies[i].vx = bodies[i].vx + ax * dt;
            new_bodies[i].vy = bodies[i].vy + ay * dt;
        }

        // Update positions
        for (unsigned int i = 0; i < n_bodies; i++) {
            new_bodies[i].x = bodies[i].x + new_bodies[i].vx * dt;
            new_bodies[i].y = bodies[i].y + new_bodies[i].vy * dt;
        }

        // Write output every write_every steps
        if (step % write_every == 0) {
            for (unsigned int i = 0; i < n_bodies; i++) {
                fprintf(f, "%d,%d,%.9lf,%.9lf,%.9lf,%.9lf\n",
                        step / write_every, i,
                        new_bodies[i].x, new_bodies[i].y,
                        new_bodies[i].vx, new_bodies[i].vy);
            }
            fflush(f);
        }

        // Swap pointers
        Body *tmp = bodies;
        bodies = new_bodies;
        new_bodies = tmp;
    }

    fclose(f);
    free(bodies);
    free(new_bodies);

    return 0;
}
