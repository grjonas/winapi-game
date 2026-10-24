#include "chess.hpp"

namespace chess
{
    bool operator==(const Vector2D& a, const Vector2D& b)
    {
        return a.x == b.x && a.y == b.y;
    }

    bool operator!=(const Vector2D& a, const Vector2D& b)
    {
        return a.x != b.x || a.y != b.y;
    }

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

    Piece::Piece (Vector2D pos, PieceKind piece_kind, PlayerKind player_kind)
    {
        set_pos(pos);
        set_piece_kind(piece_kind);
        set_player_kind(player_kind);
    }

    inline Vector2D Piece::get_pos ()
    {
        return this->pos;
    }

    inline PieceKind& Piece::get_piece_kind ()
    {
        return this->piece_kind;
    }

    inline PlayerKind& Piece::get_player_kind ()
    {
        return this->player_kind;
    }

    inline void Piece::set_pos (Vector2D pos)
    {
        this->pos = pos;
    }

    inline void Piece::set_piece_kind (PieceKind  piece_kind )
    {
        state.pawn.has_moved = false;
        this->piece_kind = piece_kind;
    }

    inline void Piece::set_player_kind (PlayerKind player_kind)
    {
        this->player_kind = player_kind;
    }

    inline bool Piece::get_pawn_has_moved ()
    {
        assert(this->piece_kind == PieceKind::PAWN);
        return this->state.pawn.has_moved;
    }

#define add_piece_white(piece_x, piece_y, piece_type)\
        (add_piece(Piece {Vector2D {piece_x, piece_y}, piece_type, PlayerKind::WHITE}))
#define add_piece_black(piece_x, piece_y, piece_type)\
        (add_piece(Piece {Vector2D {piece_x, piece_y}, piece_type, PlayerKind::BLACK}))
    Board::Board()
    {
        // White
        add_piece_white(0, 0, PieceKind::ROOK  );
        add_piece_white(0, 1, PieceKind::KNIGHT);
        add_piece_white(0, 2, PieceKind::BISHOP);
        add_piece_white(0, 3, PieceKind::KING  );
        add_piece_white(0, 4, PieceKind::QUEEN );
        add_piece_white(0, 5, PieceKind::BISHOP);
        add_piece_white(0, 6, PieceKind::KNIGHT);
        add_piece_white(0, 7, PieceKind::ROOK  );

        add_piece_white(1, 0, PieceKind::PAWN  );
        add_piece_white(1, 1, PieceKind::PAWN  );
        add_piece_white(1, 2, PieceKind::PAWN  );
        add_piece_white(1, 3, PieceKind::PAWN  );
        add_piece_white(1, 4, PieceKind::PAWN  );
        add_piece_white(1, 5, PieceKind::PAWN  );
        add_piece_white(1, 6, PieceKind::PAWN  );
        add_piece_white(1, 7, PieceKind::PAWN  );

        // Black
        add_piece_white(6, 0, PieceKind::PAWN  );
        add_piece_white(6, 1, PieceKind::PAWN  );
        add_piece_white(6, 2, PieceKind::PAWN  );
        add_piece_white(6, 3, PieceKind::PAWN  );
        add_piece_white(6, 4, PieceKind::PAWN  );
        add_piece_white(6, 5, PieceKind::PAWN  );
        add_piece_white(6, 6, PieceKind::PAWN  );
        add_piece_white(6, 7, PieceKind::PAWN  );

        add_piece_white(7, 0, PieceKind::ROOK  );
        add_piece_white(7, 1, PieceKind::KNIGHT);
        add_piece_white(7, 2, PieceKind::BISHOP);
        add_piece_white(7, 3, PieceKind::KING  );
        add_piece_white(7, 4, PieceKind::QUEEN );
        add_piece_white(7, 5, PieceKind::BISHOP);
        add_piece_white(7, 6, PieceKind::KNIGHT);
        add_piece_white(7, 7, PieceKind::ROOK  );
    }
#undef add_piece_white
#undef add_piece_black

    bool Board::move_piece_pawn (Index piece_index, Vector2D old_pos, Vector2D new_pos)
    {
        Piece piece = get_piece_from_index(piece_index);

        assert(piece.get_piece_kind() == PieceKind::PAWN);

        Vector2D diff_vec = new_pos - old_pos;
        if (diff_vec.x != 0
                || !(piece.get_pawn_has_moved() ? diff_vec.y == 1 : diff_vec.y == 2))
            return false;
        
        auto opt_piece = get_piece_index_from_pos(new_pos);
        if (opt_piece.has_value())
        {
            Index other_piece_index = opt_piece.value();
            Piece other_piece = get_piece_from_index(other_piece_index);

            // TODO: This is a bit bad, I just copied the 'range check'
            // from other code, fix later.
            Vector2D change { sign(diff_vec.x), sign(diff_vec.y) };

            for (Vector2D curr_pos = old_pos; curr_pos != old_pos; curr_pos = curr_pos + change)
                if (get_piece_index_from_pos(curr_pos) != std::nullopt)
                    return false;

            if (other_piece.get_player_kind() == piece.get_player_kind())
                return false;
            else
                remove_piece_from_index(other_piece_index);
        }
        piece.set_pos(new_pos);
        set_piece_from_index(piece_index, piece);

        return true;
    }

    bool Board::move_piece_rook   (Index piece_index, Vector2D old_pos, Vector2D new_pos)
    {
        Piece piece = get_piece_from_index(piece_index);

        assert(piece.get_piece_kind() == PieceKind::PAWN);

        Vector2D diff_vec = new_pos - old_pos;
        if ((diff_vec.x != 0 && diff_vec.y != 0)
                || (diff_vec.x == 0 && diff_vec.y == 0))
            return false;
        
        auto opt_piece = get_piece_index_from_pos(new_pos);
        if (opt_piece.has_value())
        {
            Index other_piece_index = opt_piece.value();
            Piece other_piece = get_piece_from_index(other_piece_index);

            Vector2D change { sign(diff_vec.x), sign(diff_vec.y) };

            for (Vector2D curr_pos = old_pos; curr_pos != old_pos; curr_pos = curr_pos + change)
                if (get_piece_index_from_pos(curr_pos) != std::nullopt)
                    return false;

            if (other_piece.get_player_kind() == piece.get_player_kind())
                return false;
            else
                remove_piece_from_index(other_piece_index);
        }

        piece.set_pos(new_pos);
        set_piece_from_index(piece_index, piece);

        return true;
    }

    bool Board::move_piece_bishop (Index piece_index, Vector2D old_pos, Vector2D new_pos)
    {
        Piece piece = get_piece_from_index(piece_index);

        assert(piece.get_piece_kind() == PieceKind::PAWN);

        Vector2D diff_vec = new_pos - old_pos;
        if ((abs(diff_vec.x) - abs(diff_vec.y) != 0)
                || (diff_vec.x == 0 && diff_vec.y == 0))
            return false;
        
        auto opt_piece = get_piece_index_from_pos(new_pos);
        if (opt_piece.has_value())
        {
            Index other_piece_index = opt_piece.value();
            Piece other_piece = get_piece_from_index(other_piece_index);

            Vector2D change { sign(diff_vec.x), sign(diff_vec.y) };

            for (Vector2D curr_pos = old_pos; curr_pos != old_pos; curr_pos = curr_pos + change)
                if (get_piece_index_from_pos(curr_pos) != std::nullopt)
                    return false;

            if (other_piece.get_player_kind() == piece.get_player_kind())
                return false;
            else
                remove_piece_from_index(other_piece_index);
        }

        piece.set_pos(new_pos);
        set_piece_from_index(piece_index, piece);

        return true;
    }

    bool Board::move_piece_knight (Index piece_index, Vector2D old_pos, Vector2D new_pos)
    {
        Piece piece = get_piece_from_index(piece_index);

        assert(piece.get_piece_kind() == PieceKind::PAWN);

        Vector2D diff_vec = new_pos - old_pos;
        if (diff_vec.x != 0
                || !(piece.get_pawn_has_moved() ? diff_vec.y == 1 : diff_vec.y == 2))
            return false;
        
        auto opt_piece = get_piece_index_from_pos(new_pos);
        if (opt_piece.has_value())
        {
            Index other_piece_index = opt_piece.value();
            Piece other_piece = get_piece_from_index(other_piece_index);

            if (other_piece.get_player_kind() == piece.get_player_kind())
                return false;
            else
                remove_piece_from_index(other_piece_index);
        }
        piece.set_pos(new_pos);
        set_piece_from_index(piece_index, piece);

        return true;
    }

    bool Board::move_piece_queen  (Index piece_index, Vector2D old_pos, Vector2D new_pos)
    {
        Piece piece = get_piece_from_index(piece_index);

        assert(piece.get_piece_kind() == PieceKind::PAWN);

        Vector2D diff_vec = new_pos - old_pos;
        if (
               !(abs(diff_vec.x) == 2 ?
                   abs(diff_vec.y) == 1 :
                   (abs(diff_vec.y) == 2 && abs(diff_vec.x) == 1))
           )
            return false;
        
        auto opt_piece = get_piece_index_from_pos(new_pos);
        if (opt_piece.has_value())
        {
            Index other_piece_index = opt_piece.value();
            Piece other_piece = get_piece_from_index(other_piece_index);

            if (other_piece.get_player_kind() == piece.get_player_kind())
                return false;
            else
                remove_piece_from_index(other_piece_index);
        }

        piece.set_pos(new_pos);
        set_piece_from_index(piece_index, piece);

        return true;
    }

    bool Board::move_piece_king   (Index piece_index, Vector2D old_pos, Vector2D new_pos)
    {
        Piece piece = get_piece_from_index(piece_index);

        assert(piece.get_piece_kind() == PieceKind::PAWN);

        Vector2D diff_vec = new_pos - old_pos;
        if (
               (abs(diff_vec.x) > 1)
               ||
               (abs(diff_vec.y) > 1)
               ||
               (
                   (diff_vec.x != 0 && diff_vec.y != 0)
                   &&
                   (abs(diff_vec.x) - abs(diff_vec.y) != 0)
               )
               ||
               (diff_vec.x == 0 && diff_vec.y == 0)
           )
            return false;
        
        auto opt_piece = get_piece_index_from_pos(new_pos);
        if (opt_piece.has_value())
        {
            Index other_piece_index = opt_piece.value();
            Piece other_piece = get_piece_from_index(other_piece_index);

            if (other_piece.get_player_kind() == piece.get_player_kind())
                return false;
            else
                remove_piece_from_index(other_piece_index);
        }
        piece.set_pos(new_pos);
        set_piece_from_index(piece_index, piece);

        return true;
    }

    bool Board::move_piece (Vector2D old_pos, Vector2D new_pos)
    {
        Index piece_index;

        auto opt_piece = get_piece_index_from_pos(old_pos);
        if (!opt_piece.has_value())
        {
            return false;
        }
        piece_index = opt_piece.value();
        Piece piece = get_piece_from_index(piece_index);

        if (new_pos.x < 0
                || this->board_size.x <= new_pos.x
                || new_pos.y < 0
                || this->board_size.y <= new_pos.y)
            return false;

        if (piece.get_player_kind() != this->curr_player)
            return false;

        bool result = false;
        switch (piece.get_piece_kind())
        {
            case PAWN  : result = move_piece_pawn   (piece_index, old_pos, new_pos); break;
            case ROOK  : result = move_piece_rook   (piece_index, old_pos, new_pos); break;
            case BISHOP: result = move_piece_bishop (piece_index, old_pos, new_pos); break;
            case KNIGHT: result = move_piece_knight (piece_index, old_pos, new_pos); break;
            case QUEEN : result = move_piece_queen  (piece_index, old_pos, new_pos); break;
            case KING  : result = move_piece_king   (piece_index, old_pos, new_pos); break;
            default:
                UNREACHABLE;
        }

        this->curr_player =
            (this->curr_player == PlayerKind::BLACK) ?
                PlayerKind::WHITE :
                PlayerKind::BLACK;

        return result;
    }

    bool Board::remove_piece (Vector2D pos)
    {
        auto opt_piece_index = get_piece_index_from_pos (pos);
        if (opt_piece_index == std::nullopt)
        {
            return false;
        }

        remove_piece_from_index (opt_piece_index.value());
        return true;
    }

    Piece Board::get_piece_from_index (Index piece_index)
    {
        return this->pieces[piece_index];
    }

    void Board::set_piece_from_index (Index piece_index, Piece piece)
    {
        this->pieces[piece_index] = piece;
    }

    std::optional<Index> Board::get_piece_index_from_pos (Vector2D pos)
    {
        // Ehh, good enough for this few elements.
        for (Index i = 0; i < this->pieces.size(); ++i)
        {
            if (this->pieces[i].get_pos() == pos)
            {
                return i;
            }
        }

        return std::nullopt;
    }

    bool Board::add_piece (Piece piece)
    {
        auto opt_piece_index = get_piece_index_from_pos(piece.get_pos());
        if (opt_piece_index != std::nullopt)
        {
            return false;
        }

        this->pieces.push_back(piece);
        return true;
    }

    void Board::remove_piece_from_index (Index piece_index)
    {
        this->pieces.erase(this->pieces.begin() + piece_index);
    }
}
