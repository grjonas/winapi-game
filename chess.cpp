#include "chess.hpp"

using namespace chess;

Vector2D operator+(const Vector2D& a, const Vector2D& b)
{
    return (Vector2D) { .x = a.x + b.x, .y = a.y + b.y };
}

Vector2D operator-(const Vector2D& a)
{
    return (Vector2D) { .x = -a.x, .y = -a.y };
}

Vector2D operator-(const Vector2D& a, const Vector2D& b)
{
    return a + (-b);
}

Piece::Piece (PieceKind piece_kind, PlayerKind player_kind)
{
    this->piece_kind  = piece_kind ;
    this->player_kind = player_kind;

    switch (piece_kind)
    {
        case PAWN  : state.pawn_has_moved = false; break;
        default:
    }
}

inline Piece::PieceKind get_piece_kind ()
{
    return this->piece_kind;
}

inline Piece::PieceKind get_player_kind ()
{
    return this->player_kind;
}

inline void Piece::set_piece_kind (PieceKind  piece_kind )
{
    this->piece_kind = piece_kind;
}

inline void Piece::set_player_kind (PlayerKind player_kind)
{
    this->player_kind = player_kind;
}

inline bool Piece::is_move_valid (Point2D point_a, Point2D point_b);
{
    switch (this->piece_kind)
    {
        case PAWN  :
            if (state.pawn_has_moved)
            {
            }
            else
            {
            }
            break;
        case ROOK  :
            break;
        case BISHOP:
            break;
        case KNIGHT:
            break;
        case QUEEN :
            break;
        case KING  :
            break;
        default:
            UNREACHABLE;
    }
}
