levels = 100
step_h = 0.15

for i in range(1, levels + 1):
    z = step_h * i
    r = 0.5 + 0.2 * i
    gray = max(0.1, 0.7 - 0.006 * i)

    print(f'''\
<geom name="stair_north{i}" type="box" pos="0 {r:.2f} {z:.2f}" size="0.8 0.1 0.1" rgba="{gray:.2f} {gray:.2f} {gray:.2f} 1"/>
<geom name="stair_south{i}" type="box" pos="0 {-r:.2f} {z:.2f}" size="0.8 0.1 0.1" rgba="{gray:.2f} {gray:.2f} {gray:.2f} 1"/>
<geom name="stair_east{i}"  type="box" pos="{r:.2f} 0 {z:.2f}" size="0.1 0.8 0.1" rgba="{gray:.2f} {gray:.2f} {gray:.2f} 1"/>
<geom name="stair_west{i}"  type="box" pos="{-r:.2f} 0 {z:.2f}" size="0.1 0.8 0.1" rgba="{gray:.2f} {gray:.2f} {gray:.2f} 1"/>
''')
