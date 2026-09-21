#pragma once

#include "deps.hpp"

namespace chess
{
    using Point2D = struct
    {
        std::int32_t x;
        std::int32_t y;
    };

    using Vector2D = struct
    {
        std::int32_t x;
        std::int32_t y;

    };

    Vector2D operator+(const Vector2D& a, const Vector2D& b);
    Vector2D operator-(const Vector2D& a);
    Vector2D operator-(const Vector2D& a, const Vector2D& b);

    using PieceKind = enum : std::uint8_t
    {
        PAWN  ,
        ROOK  ,
        BISHOP,
        KNIGHT,
        QUEEN ,
        KING  ,
    };

    using PlayerKind = enum : std::uint8_t
    {
        WHITE,
        BLACK,
    };

    class Piece
    {
        public:
             Piece (PieceKind piece_kind, PlayerKind player_kind);
            ~Piece () = default;

            inline PieceKind get_piece_kind  ();
            inline PieceKind get_player_kind ();
            inline void set_piece_kind  (PieceKind  piece_kind );
            inline void set_player_kind (PlayerKind player_kind);

            inline bool move_piece    (Point2D point_a, Point2D point_b);
            inline bool is_move_valid (Point2D point_a, Point2D point_b);

        private:
            PieceKind  piece_kind;
            PlayerKind player_kind;
            union
            {
                bool pawn_has_moved;
            }
            state;
    };

    class Board
    {
        public:
             Board ();
            ~Board () = default;
            bool is_move_valid (Point2D point_a, Point2D point_b);

        private:
            // bool board_active = true;
            PlayerKind curr_player = PlayerKind::WHITE;
            std::vector<std::pair<Point2D, Piece>> pieces;
    };
}
