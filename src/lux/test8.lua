print('SDL2: test binding features of a 3rd party library')

--for name, value in pairs(SDL) do print(name) end

title = char('Lua binding to SDL2')
window = SDL.CreateWindow(title
                         ,SDL.WINDOWPOS_UNDEFINED
                         ,SDL.WINDOWPOS_UNDEFINED
                         ,320
                         ,240
                         ,SDL.WINDOW_HIDDEN)

screen = SDL.GetWindowSurface(window)



SDL.DestroyWindow(window)

