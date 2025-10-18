#GFXFunctions:
camera([x, y]) -- set camera position
circ(x, y, r, [col]) -- draw circle
circfill(x, y, r, [col]) -- draw filled circle
clip([x, y, w, h]) -- set screen clipping region
#cls([col]) -- clear screen; col = clear color
color(col) -- set default color
cursor(x, y) -- set cursor and CR/LF margin position
fget(n, [f]) -- get values of sprite flags
fillp(mask) -- set fill pattern for circ, circfill, rect, rectfill, pset, and line
#flip() -- flip screen back buffer (30fps)
fset(n, [f], v) -- set values of sprite flags
line(x0, y0, x1, y1, [col]) -- draw line
oval(x0, y0, x1, y1, [col]) -- draws an ellipse inside of a bounding rectangle
ovalfill(x0, y0, x1, y1, [col]) -- draws a colored ellipse
pal(c0, c1, [p]) -- swaps col 0 to col 1; p = 0 = draw palette; p = 1 = screen palette
palt(col, t) -- set transparency for colour to t (bool)
pget(x, y) -- get pixel colour
print(str, [x, y, [col]]) -- print string
#pset(x, y, [col]) -- set pixel colour
rect(x0, y0, x1, y1, [col]) -- draw rectangle
rectfill(x0, y0, x1, y1, [col]) -- draw filled rectangle
sget(x, y) -- get spritesheet pixel colour
spr(n, x, y, [w, h], [flip_x], [flip_y]) -- draw sprite
sset(x, y, [col]) -- set spritesheet pixel colour
sspr(sx, sy, sw, sh, dx, dy, [dw, dh], [flip_x], [flip_y]) -- draw texture from spritesheet

 -- Draws a textured line between two points, sampling the map for data
tline(x0, y0, x1, y1, mx, my, [mdx], [mdy])