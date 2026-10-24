#include <iostream>
#include "chess.hpp"

int main()
{
    chess::Board board;
    bool is_game_active = true;

    while (is_game_active)
    {
        Vector2D move = get_move();
    }

    std::cout << "Hello, World!\n" << std::endl;
    return 0;
}
