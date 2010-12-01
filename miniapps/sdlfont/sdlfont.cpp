/**
 * Copyright 2010 Scott MacDonald. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY SCOTT MACDONALD ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL <COPYRIGHT HOLDER> OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Scott MacDonald.
 */
#include <iostream>
#include <cassert>
#include <cmath>

#include <string>
#include <vector>
#include <fstream>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

std::vector<SDL_Surface*> generateFontSprites( 
        TTF_Font * font,
        SDL_Color& color,
        char first,
        char last
);
void deleteFontSprites( std::vector<SDL_Surface*>& );

void drawCharacter(
        SDL_Surface * surface,
        TTF_Font    * font,
        SDL_Color&    color,
        char          character,
        int           xpos,
        int           ypos 
);

int main( int, char** )
{
    //
    // Init
    //
    char startChar = ' ';
    char stopChar  = '~';
    int chars = stopChar - startChar + 1;

    std::cout << "Generating font sprite sheet..." << std::endl
              << "Start char: '" << startChar      << "', "
              << "Stop char: '"  << stopChar       << "', "
              << "total chars: " << chars          << std::endl
              << std::endl;

    if ( SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        std::cerr << "Failed to init SDL: " << SDL_GetError()
                  << std::endl;
        return 1;
    }

    //
    // Open a text file up that will hold font rendering information
    //
    std::ofstream fdatafile("font.data");

    if ( !fdatafile )
    {
        std::cerr << "Failed to open font.data" << std::endl;
        return 1;
    }

    //
    // Create a rendering surface
    //
    SDL_Surface * screen;

    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE );
    assert( screen != NULL );
   
    //
    // Create the font
    //
    TTF_Init();
    
    TTF_Font * font = TTF_OpenFont( "consola.ttf", 18 );
    assert( font != NULL );

    SDL_Color color = { 0, 0, 255, 0 };

    //
    // Spite out information about the font
    //
    fdatafile << "# font layout data for \"consola\""           << std::endl;
    fdatafile << "# "                                           << std::endl;
    fdatafile << "# name point_size characters start_ch end_ch" << std::endl;
    fdatafile << "# start_ch end_ch"                            << std::endl;
    fdatafile << "# descent ascent height lineskip"             << std::endl;
    fdatafile << "# id, char, pos_xtop, pos_ytop, xmin, xmax, " <<
                 "ymin, ymax, advance"  << std::endl;

    // First line
    fdatafile << "consola "
              << "12 " 
              << chars            << std::endl
              << (char) startChar << " "
              << (char) stopChar  << std::endl;

    fdatafile << TTF_FontDescent(font)  << " "
              << TTF_FontAscent(font)   << " "
              << TTF_FontHeight(font)   << " "
              << TTF_FontLineSkip(font) << " "
              << std::endl;

    //
    // Generate surfaces for each character
    //
/*    SDL_Surface *text = TTF_RenderText_Solid( font, "Hello World", color );

    SDL_Rect dest = { 0, 0, 0, 0 };
    SDL_BlitSurface( text, NULL, screen, &dest );

    SDL_FreeSurface( text );*/

    //
    // Generate list of sprite characters using SDL_TTF
    //
 /*   std::vector<SDL_Surface*> sprites = generateFontSprites(
            font,
            color,
            startChar,
            stopChar
    );*/

    //
    // Find the widest and the tallest characters
    //
    int maxWidth  = 0;
    int maxHeight = 0;

    for ( char c = startChar; c <= stopChar; ++c )
    {
        int min_x=0, max_x=0, min_y=0, max_y=0, advance=0;

        // Get information for this font
        TTF_GlyphMetrics(
                font,
                c,
                &min_x,
                &max_x,
                &min_y,
                &max_y,
                &advance
        );

        int width  = max_x - min_x;
        int height = max_y - min_y;

        if ( width  > maxWidth  ) { maxWidth  = width; }
        if ( height > maxHeight ) { maxHeight = height; }
    }

    assert( maxWidth  > 0 );
    assert( maxHeight > 0 );

    //
    // Calculate positioning data and whatnot
    //
    int cellWidth      = maxWidth + 1;      // maybe should round to nice num
    int cellHeight     = maxHeight + 1;     // same here
    int startX         = 0;
    //int startY         = 100;

    int charsPerRow    = ceilf( static_cast<float>( SCREEN_WIDTH - startX ) /
                                static_cast<float>( maxWidth ) );
    int charRowsNeeded = ceilf( static_cast<float>( chars ) / 
                                static_cast<float>( charsPerRow ) );
    int screenRows     = SCREEN_HEIGHT / maxHeight;

    assert( screenRows >= charRowsNeeded );

    std::cout << "Max char width: "  << maxWidth  << ", "
              << "max char height: " << maxHeight << std::endl;
    std::cout << "Cell width: " << cellWidth << ", "
              << "height: " << cellHeight << std::endl;
    std::cout << "Sprite sheet width: " << SCREEN_WIDTH << ", "
              << "height: " << SCREEN_HEIGHT << std::endl;
    std::cout << "Chars per row: " << charsPerRow << ", "
              << "rows needed: " << charRowsNeeded << std::endl;

    //
    // Iterate each character that needs to be generated, and
    // draw it in the appropriate place on the final sprite sheet.
    // Also write information about the character out to a data sheet
    //
    for ( int i = 0, c = startChar; c <= stopChar; ++i, ++c )
    {
        int min_x=0, max_x=0, min_y=0, max_y=0, advance=0;

        // Get information for this font
        TTF_GlyphMetrics(
                font,
                c,
                &min_x,
                &max_x,
                &min_y,
                &max_y,
                &advance
        );

        // Calculate sprite sheet position
        int row = i / charsPerRow;
        int col = i % charsPerRow;

        int x = col * cellWidth;
        int y = row * cellHeight;

        // Render it to the sprite sheet
        drawCharacter(
                screen,
                font,
                color,
                c,
                x,
                y
        );

        // Write information out
        fdatafile << (char) c << " "
                  << i        << " "
                  << x        << " "
                  << y        << " "
                  << min_x    << " "
                  << max_x    << " "
                  << min_y    << " "
                  << max_y    << " "
                  << advance  << std::endl;
    }

    //
    // Show the results
    //
    SDL_Flip( screen );
    SDL_Delay( 5000 );

    //
    // Write results to disk
    //

    //
    // Clean up
    //
 //   deleteFontSprites( sprites );
    fdatafile.close();

    TTF_CloseFont( font );
    SDL_Quit();
}

void drawCharacter(
        SDL_Surface * surface,
        TTF_Font    * font,
        SDL_Color&    color,
        char          character,
        int           xpos,
        int           ypos )
{
    // Set renderable string
    char str[] = " ";
    str[0] = character;

    SDL_Surface *text = TTF_RenderText_Solid( font, str, color );
    SDL_Rect dest     = { xpos, ypos, 0, 0 };

    SDL_BlitSurface( text, NULL, surface, &dest );

    SDL_FreeSurface( text );
}

std::vector<SDL_Surface*> generateFontSprites(
        TTF_Font * font,
        SDL_Color& color,
        char first,
        char last )
{
    std::vector<SDL_Surface*> sprites;
    char str[] = " ";

    for ( int c = first, i = 0; c <= last; ++c, ++i )
    {
        str[0] = c;     // Assign current character to rendering string

        // Generate surface and store it
        SDL_Surface * sprite = TTF_RenderText_Solid( font, str, color );
        sprites.push_back( sprite );
    }

    return sprites;
}


void deleteFontSprites( std::vector<SDL_Surface*>& sprites )
{
    for ( size_t i = 0; i < sprites.size(); ++i )
    {
        SDL_FreeSurface( sprites[i] );
        sprites[i] = 0;
    }
}
