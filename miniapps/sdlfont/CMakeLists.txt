find_package(SDL REQUIRED)
find_package(SDL_ttf REQUIRED)

add_program_with(sdlfont "SDL;SDL_ttf" "" "" sdlfont.cpp)
