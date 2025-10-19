__lua__
function _init()
  cls()
  -- draw pixels at some fixed positions to test pset
  pset(10, 10, 7)  -- white pixel at (10,10)
  pset(20, 20, 8)  -- light gray pixel at (20,20)
  pset(30, 30, 9)  -- dark gray pixel at (30,30)
  pset(40, 40, 10) -- red pixel at (40,40)
end

function _update()
  -- no updates needed for now
end

function _draw()
  -- no redraw needed, pixels are drawn in _init
end