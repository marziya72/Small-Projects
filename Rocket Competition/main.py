import sys
sys.stdout.reconfigure(encoding='utf-8')
# water_rocket_sim.py
# Top of file: user-editable presets (measure these first)
weight_empty_kg = 0.1       # **weight (empty rocket, kg)**
diameter_m = 0.075            # **diameter (m)**
height_m = 0.285             # **height (m)**
center_of_mass_m = 0.19      # **center of mass from base (m)**

# User interacts only with these two below
water_ml = 85              # **fuel (water) in ml**
psi_gauge = 18             # **air pressure (gauge) in psi**

"GOAL: 15 TO 20 METERS"
"CLOSEST TO 17.5M WINS!"
"BE MINDFUL OF MAX FUEL; FUEL ADDS WEIGHT!"
"MAX FUEL = 350ml"
"MAX PSI = 20"




# Simulation caps for display / target
MAX_RANGE_DISPLAY = 25.0     # meters (x axis cap)
MAX_HEIGHT_DISPLAY = 10.0    # meters (y axis cap)






# ---------- simulation (DO NOT EDIT unless you know what you are doing) ----------
# ---------- (trust me you do not know what you are doing so go back up) ----------


import math
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

g = 9.81
rho_water = 1000.0
rho_air = 1.225
gamma = 1.4
Cd = 0.35
atm = 101325.0
nozzle_diameter_m = 0.021
dt = 0.001

def simulate_water_rocket(m_empty_kg,
                          bottle_height_m,
                          bottle_diameter_m,
                          psi_gauge,
                          water_volume_ml,
                          launch_angle_deg,
                          center_of_mass_m,
                          nozzle_diameter_m=nozzle_diameter_m,
                          dt=dt):
    # geometry and conversion
    P0 = psi_gauge * 6894.75729 + atm
    bottle_radius = bottle_diameter_m / 2.0
    bottle_area = math.pi * bottle_radius**2
    A_nozzle = math.pi * (nozzle_diameter_m/2.0)**2
    bottle_volume = bottle_area * bottle_height_m
    water_volume_m3 = water_volume_ml / 1e6
    if water_volume_m3 <= 0:
        raise ValueError("water_volume_ml must be > 0")
    if water_volume_m3 >= bottle_volume:
        water_volume_m3 = bottle_volume*0.999
    V_air0 = bottle_volume - water_volume_m3
    P_air0 = P0
    m_water = rho_water * water_volume_m3
    const_PV = P_air0 * (V_air0**gamma)

    theta = math.radians(launch_angle_deg)
    x = 0.0; y = 0.01
    vx = 0.0; vy = 0.0

    times, xs, ys, vxs, vys, thrusts, masses, pressures = ([] for _ in range(8))
    t = 0.0
    launched = False
    max_steps = int(300 / dt)
    step = 0

    while step < max_steps:
        step += 1
        V_air = bottle_volume - (m_water / rho_water)
        if V_air <= 0:
            V_air = 1e-9
        P_air = const_PV / (V_air**gamma)
        if P_air < atm:
            P_air = atm

        if m_water > 1e-6 and P_air > atm:
            v_exit = math.sqrt(max(0.0, 2.0*(P_air - atm)/rho_water))
            mass_flow = rho_water * A_nozzle * v_exit
            dm_possible = mass_flow * dt
            if dm_possible > m_water:
                mass_flow = m_water / dt
                v_exit = mass_flow / (rho_water * A_nozzle) if (rho_water * A_nozzle) > 0 else 0.0
            thrust = mass_flow * v_exit + (P_air - atm) * A_nozzle
            dm = mass_flow * dt
            m_water -= dm
            if m_water < 0:
                m_water = 0.0
        else:
            thrust = 0.0
            mass_flow = 0.0
            v_exit = 0.0

        m_rocket = m_empty_kg + m_water
        v = math.hypot(vx, vy)
        A_cross = math.pi * bottle_radius**2
        drag = 0.5 * rho_air * v**2 * Cd * A_cross
        drag_x = -drag * (vx / v) if v > 1e-9 else 0.0
        drag_y = -drag * (vy / v) if v > 1e-9 else 0.0
        thrust_x = thrust * math.cos(theta)
        thrust_y = thrust * math.sin(theta)
        ax = (thrust_x + drag_x) / m_rocket
        ay = (thrust_y + drag_y) / m_rocket - g
        vx += ax * dt
        vy += ay * dt
        x += vx * dt
        y += vy * dt

        times.append(t); xs.append(x); ys.append(y)
        vxs.append(vx); vys.append(vy); thrusts.append(thrust)
        masses.append(m_rocket); pressures.append(P_air)

        t += dt
        if not launched and (thrust > 0 or v > 0.1):
            launched = True
        if launched and y <= 0.0 and t>0.01:
            break

    xs = np.array(xs); ys = np.array(ys); times = np.array(times)
    vxs = np.array(vxs); vys = np.array(vys)
    thrusts = np.array(thrusts); masses = np.array(masses)
    pressures = np.array(pressures)

    if len(xs)==0:
        flight_time = 0.0; range_m = 0.0; apogee = 0.0
    else:
        flight_time = times[-1]; range_m = xs[-1]; apogee = float(np.max(ys))

    return {
        "time": times, "x": xs, "y": ys,
        "vx": vxs, "vy": vys, "thrust": thrusts,
        "mass": masses, "pressure": pressures,
        "flight_time": flight_time, "range": range_m, "apogee": apogee
    }

def stability_check(bottle_height_m, center_of_mass_m):
    # simple heuristic:
    # approximate center of pressure (CP) for a blunt bottle body.
    # use CP ~ 0.6 * height from base (empirical for short bodies)
    cp = 0.6 * bottle_height_m
    cm = center_of_mass_m
    margin = cm - cp   # positive -> CM ahead of CP -> statically stable
    return {"CP_m": cp, "CM_m": cm, "stability_margin_m": margin, "stable": margin > 0.0}

# ---------- run ----------
if __name__ == "__main__":
    # target angle selection (user could change)
    launch_angle_deg = 45

    # run stability check
    stab = stability_check(height_m, center_of_mass_m)
    print(f"center_of_mass={stab['CM_m']:.3f} m  center_of_pressure≈{stab['CP_m']:.3f} m  margin={stab['stability_margin_m']:.3f} m")
    print("stability:", "STABLE" if stab["stable"] else "UNSTABLE (consider moving mass or adding fins)")

    # simulate
    res = simulate_water_rocket(weight_empty_kg, height_m, diameter_m, psi_gauge, water_ml, launch_angle_deg, center_of_mass_m)

    # print key results
    print(f"range = {res['range']:.2f} m  apogee = {res['apogee']:.2f} m  flight_time = {res['flight_time']:.2f} s")

    # plot with caps
    plt.figure(figsize=(8,4))
    plt.plot(res["x"], res["y"], linewidth=1.5)
    plt.xlabel("Range (m)"); plt.ylabel("Altitude (m)")
    plt.title(f"Trajectory (range={res['range']:.2f} m  apogee={res['apogee']:.2f} m)")
    # apply display caps
    plt.xlim(0, MAX_RANGE_DISPLAY)
    plt.ylim(0, MAX_HEIGHT_DISPLAY)
    # if exceeded, mark clipped point on edges
    if res["range"] > MAX_RANGE_DISPLAY:
        plt.plot(MAX_RANGE_DISPLAY, 0, 's', markersize=6)
        plt.text(MAX_RANGE_DISPLAY*0.98, 0.2, "range > cap", ha='right', va='bottom')
    if res["apogee"] > MAX_HEIGHT_DISPLAY:
        # find x at apogee and mark at capped height
        idx = int(np.argmax(res["y"]))
        x_ap = res["x"][idx] if len(res["x"])>0 else 0.0
        plt.plot(min(x_ap, MAX_RANGE_DISPLAY), MAX_HEIGHT_DISPLAY, 's', markersize=6)
        plt.text(min(x_ap, MAX_RANGE_DISPLAY)*0.98, MAX_HEIGHT_DISPLAY*0.98, "apogee > cap", ha='right', va='top')

    plt.grid(True)
    plt.show()

    # save trajectory CSV
    traj = pd.DataFrame({
        "time_s": res["time"], "x_m": res["x"], "y_m": res["y"],
        "vx_m_s": res["vx"], "vy_m_s": res["vy"],
        "thrust_N": res["thrust"], "mass_kg": res["mass"], "pressure_Pa": res["pressure"]
    })
    traj.to_csv("water_rocket_trajectory.csv", index=False)
    print("trajectory saved to water_rocket_trajectory.csv")

