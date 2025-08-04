import numpy as np
import csv
import sys

import scipy

def generate_bodies(n_bodies, mass_min, mass_max, alpha, space_size, output_file):
    """
    Generate random celestial bodies with a power-law mass distribution.

    Parameters:
        n_bodies (int): Number of bodies to generate.
        mass_min (float): Minimum mass.
        mass_max (float): Maximum mass.
        alpha (float): Power-law exponent (e.g. 2.35 like Salpeter IMF).
        space_size (float): Max coordinate range for x, y positions.
        velocity_max (float): Max velocity magnitude.
        output_file (str): CSV output file name.
    """

    # Generate masses using inverse transform sampling
    r = np.random.random(n_bodies)
    masses = scipy.stats.powerlaw.rvs(alpha, loc=mass_min, scale=mass_max-mass_min, size=n_bodies)

    # Random positions in 2D space
    xs = np.random.uniform(-space_size, space_size, n_bodies)
    ys = np.random.uniform(-space_size, space_size, n_bodies)

    # Random velocities in 2D space
    vxs = np.zeros(n_bodies)
    vys = np.zeros(n_bodies)
    # vys = np.random.uniform(-velocity_max, velocity_max, n_bodies)

    # Write to CSV
    with open(output_file, "w", newline="") as f:
        print(n_bodies, file=f)
        writer = csv.writer(f)
        # writer.writerow(["name", "mass", "x", "y", "vx", "vy"])
        for i in range(n_bodies):
            writer.writerow([f"Body_{i+1}", masses[i], xs[i], ys[i], vxs[i], vys[i]])

    print(f"Generated {n_bodies} bodies and wrote to {output_file}")


if __name__ == "__main__":
    if len(sys.argv) != 7:
        print("Usage: python generate_bodies.py <n_bodies> <mass_min> <mass_max> <alpha> <space_size> <output_file>")
        sys.exit(1)

    n_bodies = int(sys.argv[1])
    mass_min = float(sys.argv[2])
    mass_max = float(sys.argv[3])
    alpha = float(sys.argv[4])
    space_size = float(sys.argv[5])
    # velocity_max = float(sys.argv[6])
    output_file = sys.argv[6]

    generate_bodies(n_bodies, mass_min, mass_max, alpha, space_size, output_file)
