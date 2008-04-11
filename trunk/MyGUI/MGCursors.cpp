//---------------------------------------------------------------------------//
// File: MGCursors.h
//
//---------------------------------------------------------------------------//

#include <MGPch.h>
#include "MGBase.h"
#include "MGCursors.h"
#include "SDL.h"
#include "SDL_Ttf.h"

static SDL_Cursor *s_Cursors[MAX_CURSORS];

static const char *s_CurArrow[] =
{
  /* width height num_colors chars_per_pixel */
  "    32    32        3            1",
  /* colors */
  "X c #000000",
  ". c #ffffff",
  "  c None",
  /* pixels */
  "X                               ",
  "XX                              ",
  "X.X                             ",
  "X..X                            ",
  "X...X                           ",
  "X....X                          ",
  "X.....X                         ",
  "X......X                        ",
  "X.......X                       ",
  "X........X                      ",
  "X.....XXXXX                     ",
  "X..X..X                         ",
  "X.X X..X                        ",
  "XX  X..X                        ",
  "X    X..X                       ",
  "     X..X                       ",
  "      X..X                      ",
  "      X..X                      ",
  "       XX                       ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "0,0"
};

static const char *s_CurDrag[] =
{
  /* width height num_colors chars_per_pixel */
  "    32    32        3            1",
  /* colors */
  "X c #000000",
  ". c #ffffff",
  "  c None",
  /* pixels */
  "X                               ",
  "XX                              ",
  "X.X                             ",
  "X..X                            ",
  "X...X                           ",
  "X....X                          ",
  "X.....X                         ",
  "X......X                        ",
  "X.......X                       ",
  "X........X                      ",
  "X.....XXXXX                     ",
  "X..X..X                         ",
  "X.X X..X                        ",
  "XX  X..X                        ",
  "X    X..X                       ",
  "   XXX..XXXXX                   ",
  "   X..X..X..XX                  ",
  "   X..X..X..X.X                 ",
  "   X...XX...X..X                ",
  "   X........XXXXX               ",
  "   X............X               ",
  "   X............X               ",
  "   X............X               ",
  "   X............X               ",
  "   X............X               ",
  "   X............X               ",
  "   X............X               ",
  "   X............X               ",
  "   X............X               ",
  "   X............X               ",
  "   X............X               ",
  "   XXXXXXXXXXXXXX               ",
  "0,0"
};

static const char *s_CurDragNo[] =
{
  /* width height num_colors chars_per_pixel */
  "    32    32        3            1",
  /* colors */
  "X c #000000",
  ". c #ffffff",
  "  c None",
  /* pixels */
  "X                               ",
  "XX                              ",
  "X.X                             ",
  "X..X                            ",
  "X...X                           ",
  "X....X                          ",
  "X.....X                         ",
  "X......X                        ",
  "X.......X                       ",
  "X........X                      ",
  "X.....XXXXX                     ",
  "X..X..X                         ",
  "X.X X..X                        ",
  "XX  X..X .....                  ",
  "X    X..XXXXXX..                ",
  "     X..XXXXXXXX.               ",
  "     .X..X....XXX.              ",
  "    .XX..X    .XXX.             ",
  "   .XX.XX    .XXXXX.            ",
  "   .XX.     .XXX.XX.            ",
  "  .XX.     .XXX. .XX.           ",
  "  .XX.    .XXX.  .XX.           ",
  "  .XX.   .XXX.   .XX.           ",
  "  .XX.  .XXX.    .XX.           ",
  "  .XX. .XXX.     .XX.           ",
  "   .XX.XXX.     .XX.            ",
  "   .XXXXX.      .XX.            ",
  "    .XXX.     ..XX.             ",
  "     .XXX.....XXX.              ",
  "      .XXXXXXXXX.               ",
  "        .XXXXX.                 ",
  "         .....                  ",
  "0,0"
};

//---------------------------------------------------------------------------//
// Load Cursor
//
//---------------------------------------------------------------------------//
static SDL_Cursor *CUR_Load(const char *image[])
{
  int i, row, col;
  Uint8 data[4*32];
  Uint8 mask[4*32];
  int hot_x, hot_y;

  i = -1;
  for ( row=0; row<32; ++row ) {
    for ( col=0; col<32; ++col ) {
      if ( col % 8 ) {
        data[i] <<= 1;
        mask[i] <<= 1;
      } else {
        ++i;
        data[i] = mask[i] = 0;
      }
      switch (image[4+row][col]) {
        case 'X':
          data[i] |= 0x01;
          mask[i] |= 0x01;
          break;
        case '.':
          mask[i] |= 0x01;
          break;
        case ' ':
          break;
      }
    }
  }
  sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
  return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
void CUR_Init()
{
  s_Cursors[CUR_ARROW]  = CUR_Load(s_CurArrow);
  s_Cursors[CUR_DRAG]   = CUR_Load(s_CurDrag);
  s_Cursors[CUR_DRAGNO] = CUR_Load(s_CurDragNo);
}


//---------------------------------------------------------------------------//
// Quit
//
//---------------------------------------------------------------------------//
void CUR_Quit()
{
  for (int i = 0; i < MAX_CURSORS; i++)
    SDL_FreeCursor(s_Cursors[i]);
}


//---------------------------------------------------------------------------//
// GetCursor
//
//---------------------------------------------------------------------------//
SDL_Cursor  *CUR_GetCursor(int Cursor)
{
  return s_Cursors[Cursor];
}
