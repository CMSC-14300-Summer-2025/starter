import sys

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd

# -------------------------------
# CONFIGURATION
# -------------------------------
INPUT_FILE = sys.argv[1]
VELOCITY_SCALE = 5e3
OUTPUT_MP4 = "animation.gif"  # set to "animation.mp4" to save video

# -------------------------------
# READ INPUT FILE (get body names)
# -------------------------------
with open(INPUT_FILE) as f:
    header = f.readline().strip().split()
    n_bodies = int(header[0])  # first line: n_bodies n_timesteps dt
    body_names = [line.split(",")[0] for line in f]

print(f"Loaded {n_bodies}")

# -------------------------------
# READ SIMULATION CSV
# -------------------------------
data = pd.concat(pd.read_csv(f, header=0, index_col=[0, 1]) for f in sys.argv[2:])
steps = data.index.get_level_values("step").unique()
n_steps = len(steps)

# -------------------------------
# SET UP PLOT
# -------------------------------
fig, ax = plt.subplots(figsize=(8, 8))
ax.set_aspect("equal", "box")

# Determine axis limits dynamically
all_x = data["x"].values
all_y = data["y"].values
margin = 0.1 * max(all_x.max() - all_x.min(), all_y.max() - all_y.min())
ax.set_xlim(all_x.min() - margin, all_x.max() + margin)
ax.set_ylim(all_y.min() - margin, all_y.max() + margin)

# Plot elements
scatter, = ax.plot(data.loc[steps[0], 'x'], data.loc[steps[0], 'y'], "o")

# Arrows for velocities
quiver = ax.quiver([0] * len(body_names), [0] * len(body_names), [0] * len(body_names), [0] * len(body_names), angles="xy", scale_units="xy", scale=1, color="gray")

# -------------------------------
# ANIMATION FUNCTION
# -------------------------------
def init():
    return []

def update(frame):
    frame_data = data.loc[steps[frame]]
    scatter.set_data(frame_data['x'], frame_data['y'])
    quiver.set_offsets(frame_data[['x', 'y']])
    quiver.set_UVC(frame_data['vx'] * VELOCITY_SCALE, frame_data['vy'] * VELOCITY_SCALE)

    ax.set_title(f"Step {steps[frame]}")
    return [scatter, quiver]

ani = animation.FuncAnimation(
    fig, update, frames=n_steps, init_func=init, blit=True, interval=50
)

if OUTPUT_MP4:
    ani.save(OUTPUT_MP4, dpi=300)
else:
    plt.show()
