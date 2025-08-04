#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

#define G 6.67430e-11  // gravitational constant
#define SEGMENT_SIZE 50  // number of bodies per thread
#define MAX_NAME 32

typedef struct {
    char name[MAX_NAME];
    double mass;
    double x, y;
    double vx, vy;
} Body;

typedef struct {
    // TODO: Add any necessary arguments here
    //       Each thread takes SEGMENT_SIZE of bodies to simulate
    int thread_id;
} ThreadArgs;

Body *bodies;
Body *new_bodies;
char *input_file;
unsigned int n_bodies;
unsigned int n_timesteps;
double dt;
int n_threads;
int write_every;

// TODO: Add any additional global variables

// Utility: derive output filename from input filename
void make_output_filename(char *out, size_t size, const char *input, int thread_id) {
    const char *base = strrchr(input, '/'); 
    base = base ? base + 1 : input; // remove path

    char name[256];
    strncpy(name, base, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';

    char *dot = strrchr(name, '.');
    if (dot) *dot = '\0'; // remove extension

    snprintf(out, size, "%s_simulate_%d.csv", name, thread_id);
}

void *simulate(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;

    // Open CSV file for this thread
    char filename[256];
    make_output_filename(filename, sizeof(filename), input_file, args->thread_id);
    FILE *outfile = fopen(filename, "w");
    if (!outfile) {
        perror("fopen");
        return NULL;
    }

    // CSV header
    fprintf(outfile, "step,body,x,y,vx,vy\n");

    for (unsigned int step = 0; step < n_timesteps; step++) {
        // TODO: 1. Compute velocities for this segment

        // TODO: 2. Update positions for this segment

        // TODO: 3. Write CSV every write_every steps

        // TODO: 4. Wait for main to swap
    }

    fclose(outfile);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <input_file> <timesteps> <dt> <write_every>\n", argv[0]);
        return 1;
    }

    input_file = argv[1];
    n_timesteps = atoi(argv[2]);
    dt = atof(argv[3]);
    write_every = atoi(argv[4]);
    if (write_every <= 0) write_every = 1;

    FILE *f = fopen(input_file, "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    fscanf(f, "%d", &n_bodies);

    bodies = malloc(n_bodies * sizeof(Body));
    new_bodies = malloc(n_bodies * sizeof(Body));
    if (!bodies || !new_bodies) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        return 1;
    }

    for (unsigned int i = 0; i < n_bodies; i++) {
        fscanf(f, "%[^,],%lf,%lf,%lf,%lf,%lf",
               bodies[i].name,
               &bodies[i].mass,
               &bodies[i].x, &bodies[i].y,
               &bodies[i].vx, &bodies[i].vy);
    }

    memcpy(new_bodies, bodies, n_bodies * sizeof(Body));

    fclose(f);

    n_threads = (n_bodies + SEGMENT_SIZE - 1) / SEGMENT_SIZE;

    // TODO: Create threads with appropriate initialization

    // Main loop: swap arrays after each step
    for (unsigned int step = 0; step < n_timesteps; step++) {
        // TODO: Make sure threads have updated new_bodies

        // Swap arrays
        Body *tmp = bodies;
        bodies = new_bodies;
        new_bodies = tmp;

        // TODO: Allow threads to continue
    }

    // TODO: Join all threads

    // TODO: Clean up

    free(bodies);
    free(new_bodies);

    return 0;
}
