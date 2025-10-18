__lua__
function _update()
  -- nothing needed here for this test
end

function _draw()
  cls(0)
  for i=1,2000 do
    local x = rnd(128)
    local y = rnd(128)
    local c = rnd(16)
    pset(x, y, c)
  end
  flip()
end
__gfx__