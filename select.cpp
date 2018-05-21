/*
 * Copyright (C) 2018  Robert Ikanov, robert.ikanov@gmail.com
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

#include "boardview.h"
#include <algorithm>
#include<iostream>

static bool operator == (const SDL_Point& a, const SDL_Point& b)
{
    return a.x == b.x && a.y == b.y;
}
static bool operator != (const SDL_Point& a, const SDL_Point& b)
{
    return a.x != b.x || a.y != b.y;
}
void BoardView::select()
{
    show();
    exit_request = false;
    undo_request = false;
    redo_request = false;
    do
    {
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT)
        {
            exit_request = true;
            return;
        }
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym) // ToDo
            {
                case SDLK_LEFT:
                    undo_request = true;
                    return;
                case SDLK_RIGHT:
                    redo_request = true;
                    return;
                case SDLK_ESCAPE:
                    exit_request = true;
                    return;
                default:
                    ;
            }
        }
        SDL_GetMouseState(&mouse.x,&mouse.y);
        if(convertToTilePosition(mouse.x,mouse.y) == false)
        {
            continue;
        }
        if(mouse.x != mouse_act.x || mouse.y != mouse_act.y)
        {
            mouse_act = mouse;
            moveTileCursorTo(mouse_act.x, mouse_act.y);
            show();
        }
    }while(event.type != SDL_MOUSEBUTTONDOWN);
    
    if(SDL_GetMouseState(&mouse.x,&mouse.y) & SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
        selectJump();
    }
    else
    {
        selectGroup();
    }
}

void BoardView::selectGroup()
{
    for(bool solid_line = true;event.type != SDL_MOUSEBUTTONUP;)
    {
        if(selected.size() > 4)
        {
            selected.clear();
            show();
        }
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
        {
            exit(0);
        }
        if(SDL_GetMouseState(&mouse.x, &mouse.y) & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            if(convertToTilePosition(mouse.x,mouse.y) == false)
            {
                selected.clear();
                continue;
            }
            if(mouse != mouse_act)
            {
                mouse_act = mouse;
                moveTileCursorTo(mouse_act.x, mouse_act.y);
            }
            if(std::find(selected.begin(),selected.end(),mouse_act) != selected.end())
            {
                continue;
            }
            if(handles[mouse_act.x][mouse_act.y] == 0)
            {
                if(solid_line && selected.size() > 0)
                {
                    solid_line = false;
                    selected.push_back(mouse_act);
                    show();
                }
                continue;
            } 
            selected.push_back(mouse_act);
            show();
        }
    };    
}

void BoardView::selectJump()
{
    if(convertToTilePosition(mouse.x,mouse.y))
    {
        selected.push_back(mouse);
        show();
        SDL_Delay(120);
        while(true)
        {
            SDL_PollEvent(&event);
            if(event.type == SDL_QUIT)
            {
                exit(0);
            }
            const unsigned int mstate = SDL_GetMouseState(&mouse.x, &mouse.y);
            if(mstate)
            {
                if(convertToTilePosition(mouse.x,mouse.y) == false)
                {
                    selected.clear();
                    break;
                }
                if(mouse != mouse_act)
                {
                    mouse_act = mouse;
                    moveTileCursorTo(mouse_act.x, mouse_act.y);
                }
                if(mstate & (SDL_BUTTON(SDL_BUTTON_LEFT) | SDL_BUTTON(SDL_BUTTON_RIGHT)))
                {
                    if(selected.back() == mouse_act)
                    {
                        selected.clear();
                        break;
                    }
                    if(mstate & SDL_BUTTON(SDL_BUTTON_LEFT))
                    {
                        selected.push_back(mouse_act);
                        break;
                    }
                    else
                    {
                        selected[0] = mouse_act;
                        show();
                        SDL_Delay(120);
                        continue;
                    }
                }
            }
        }; // end while
    }
}

