/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018  <copyright holder> <email>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef BOARDVIEW_H
#define BOARDVIEW_H
#include<vector>
#include"../Engine/enums.h"
#include"../View2D/sdl_dep.h"
class BoardView
{
protected:
    int handles[20][20] = {};
    void createWindow();
    
    SDL_Event event;
    SDL_Point mouse;
    SDL_Point mouse_act;
    
    
    void selectGroup();
    void selectJump();
    
    virtual void showTileCursor() =0;
    virtual void showSelected() =0;
    virtual void redrawBoard() =0;
    virtual void moveTileCursorTo(const int& row, const int& col) =0;
    virtual bool convertToTilePosition(int& x, int& y) const =0;
public:
    std::vector<SDL_Point> marked;
    std::vector<SDL_Point> selected;
    bool undo_request;
  
    virtual int createPieces(const Ally & A, const int& col, const int& row, const char*) =0;
    virtual int moveSelection(const int& revive = 0) =0;
    virtual void moveCharacterTo(const int& handle, const int& col, const int& row) =0;
    virtual void show() =0;
    const int& getHandle(const int& col, const int& row) const
    {
        return handles[col][row];
    }
    void /*std::vector<int> &&*/ select();
};

#endif // BOARDVIEW_H
