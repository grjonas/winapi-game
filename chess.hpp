#pragma once

#include "deps.hpp"

namespace chess
{
    using Index = std::size_t;

    using Vector2D = struct
    {
        std::int32_t x;
        std::int32_t y;
    };

    bool operator!=(const Vector2D& a, const Vector2D& b);
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
             Piece (Vector2D pos, PieceKind piece_kind, PlayerKind player_kind);
            ~Piece () = default;

            inline Vector2D    get_pos ();
            inline PieceKind&  get_piece_kind ();
            inline PlayerKind& get_player_kind ();
            inline void set_pos (Vector2D pos);
            inline void set_piece_kind (PieceKind  piece_kind );
            inline void set_player_kind (PlayerKind player_kind);

            // Piece specific
            inline bool get_pawn_has_moved();

        private:
            Vector2D   pos;
            PieceKind  piece_kind;
            PlayerKind player_kind;
            union
            {
                struct
                {
                    bool has_moved;
                }
                pawn;
            }
            state;
    };

    class Board
    {
        public:
             Board ();
            ~Board () = default;
            bool move_piece (Vector2D old_pos, Vector2D new_pos);
            bool add_piece (Piece piece);
            bool remove_piece (Vector2D pos);

        private:
            // bool board_active = true;
            PlayerKind curr_player = PlayerKind::WHITE;
            std::vector<Piece> pieces {};
            Vector2D board_size { 8, 8 };

            bool move_piece_pawn   (Index piece_index, Vector2D old_pos, Vector2D new_pos);
            bool move_piece_rook   (Index piece_index, Vector2D old_pos, Vector2D new_pos);
            bool move_piece_bishop (Index piece_index, Vector2D old_pos, Vector2D new_pos);
            bool move_piece_knight (Index piece_index, Vector2D old_pos, Vector2D new_pos);
            bool move_piece_queen  (Index piece_index, Vector2D old_pos, Vector2D new_pos);
            bool move_piece_king   (Index piece_index, Vector2D old_pos, Vector2D new_pos);

            Piece get_piece_from_index (Index piece_index);
            void  set_piece_from_index (Index piece_index, Piece piece);
            std::optional<Index> get_piece_index_from_pos (Vector2D pos);
            void set_piece_from_pos (Vector2D pos, Piece piece);
            void  remove_piece_from_index (Index piece_index);
    };
}
