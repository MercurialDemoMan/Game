/*
This file is part of Game.

Game is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Game is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Game.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "GameLogic.hpp"

/**
 * entry point
 */
int main(int argc, const char* argv[])
{
    GameLogic* game = new GameLogic(argc, argv, "Lost Kingdom", 1280, 720, 60);
    
    game->run();
    
    delete game;

    return 0;
}