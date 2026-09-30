import numpy as np
import matplotlib.pyplot as plt

# -----------------------------
# USER INPUT
# -----------------------------

target = float(input("Enter target value: "))
initial_value = float(input("Enter starting value: "))

Kp = float(input("Enter Kp: "))
Ki = float(input("Enter Ki: "))
Kd = float(input("Enter Kd: "))

# -----------------------------
# SIMULATION SETTINGS
# -----------------------------

dt = 0.01
simulation_time = 10

time = np.arange(0, simulation_time, dt)

# Physical system properties
mass = 1.0
damping = 2.0

# Maximum control force
max_control = 100

# -----------------------------
# INITIAL CONDITIONS
# -----------------------------

position = initial_value
velocity = 0

integral = 0
previous_error = target - position

positions = []
errors = []
control_values = []

# -----------------------------
# PID SIMULATION
# -----------------------------

for t in time:

    # Error
    error = target - position

    # P term
    P = error

    # I term
    integral += error * dt
    I = integral

    # D term
    derivative = (error - previous_error) / dt
    D = derivative

    # PID controller
    control = Kp * P + Ki * I + Kd * D

    # Limit control output
    control = np.clip(control, -max_control, max_control)

    # Physics:
    # Force = mass * acceleration
    #
    # acceleration is affected by:
    #   PID control
    #   damping
    acceleration = (control - damping * velocity) / mass

    # Update velocity
    velocity += acceleration * dt

    # Update position
    position += velocity * dt

    # Store values
    positions.append(position)
    errors.append(error)
    control_values.append(control)

    previous_error = error

# -----------------------------
# GRAPH
# -----------------------------

plt.figure(figsize=(10, 6))

plt.plot(time, [target] * len(time),
         label="Target")

plt.plot(time, positions,
         label="Actual response")

plt.xlabel("Time (seconds)")
plt.ylabel("Value")

plt.title("PID Controller Simulation")

plt.legend()
plt.grid(True)

plt.show()
