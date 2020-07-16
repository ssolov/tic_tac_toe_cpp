#include<bits/stdc++.h> 
#include <algorithm> 
#include <iostream> 

using namespace std; 

enum BoardChar {X = 'X', O = 'O', Empty = ' '};

BoardChar opposite(BoardChar bc) {
    switch (bc) {
    case X:
        return BoardChar::O;
    case O:
        return BoardChar::X;
    default:
        return BoardChar::Empty;
    }        
}

BoardChar from_char(char c) {
    switch (c) {
    case 'o':
        return BoardChar::O;
    case 'O':
        return BoardChar::O;
    case 'x':
        return BoardChar::X;
    case 'X':
        return BoardChar::X;
    default:
        return BoardChar::Empty;
    }
}

struct Move { 
	int row, col; 
}; 

ostream& operator<< (ostream &out, const Move &m) {
    out << (m.row + 1);
    switch (m.col) {
    case 0:
        out << 'A';
        break;
    case 1:
        out << 'B';
        break;
    case 2:
        out << 'C';
    }

    return out;
}

bool from_str(Move &m, string s) {
    if (s.length() != 2) {
        return false;
    }

    int col = -1;
    int row = -1;
    
    for (unsigned i = 0; i < 2; ++i) {
        switch (s.at(i)) {
        case '1':
            row = 0;
            break;
        case '2':
            row = 1;
            break; 
        case '3':
            row = 2;
            break; 
        case 'A':
            col = 0;
            break;
        case 'a':
            col = 0;
            break;                                          
        case 'B':
            col = 1;
            break; 
        case 'b':
            col = 1;
            break;
        case 'C':
            col = 2;
            break;
        case 'c':
            col = 2;
        }
    }
    
    if (row != -1 && col != -1) {
        m.row = row;
        m.col = col;

        return true;
    }

    return false;
}

class TicTacToe {
    BoardChar board[3][3] = {
        {BoardChar::Empty, BoardChar::Empty, BoardChar::Empty},
        {BoardChar::Empty, BoardChar::Empty, BoardChar::Empty},
        {BoardChar::Empty, BoardChar::Empty, BoardChar::Empty}
    };
    BoardChar player_char, machine_char;

    int minimax(BoardChar c);
    bool evaluate(BoardChar c);

    public:
    TicTacToe(BoardChar player) {
        player_char = player;
        machine_char = opposite(player);
    };

    // This function returns true if there are moves remaining on the board. 
    // It returns false if there are no moves left to play. 
    bool has_moves();
    // This function makes the player's move
    void player_move(const Move &m) {
        board[m.row][m.col] = player_char;
    };
    // This function makes the machin's move
    void machine_move(const Move &m) {
       board[m.row][m.col] = machine_char;
    };
    // This function returns true if player won
    bool player_evaluate() { 
        return evaluate(player_char);
    };
    // This function returns true if machine won
    bool machine_evaluate() { 
        return evaluate(machine_char);
    };
    // This function will return the best possible move for machine
    bool find_best_move(Move &m);

    friend ostream& operator<< (ostream&, const TicTacToe &);
};

bool TicTacToe::has_moves() { 
	for (int i = 0; i<3; i++) 
		for (int j = 0; j<3; j++) 
			if (board[i][j] == BoardChar::Empty) 
				return true;

	return false; 
} 

bool TicTacToe::find_best_move(Move &best_move) {
    int best_val = -10;
    bool found = false;
    // Traverse all cells, evaluate minimax function for all empty cells. 
    // And return the cell with optimal value. 
    for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 3; j++) { 
            // Check if cell is empty 
            if (board[i][j] == BoardChar::Empty) { 
                // Make the move 
                board[i][j] = machine_char;
                // compute evaluation function for this move. 
                int move_val = minimax(player_char); 
                // If the move_value is more than the best_val, then update best_val
                if (move_val > best_val) { 
                    best_move.row = i;
                    best_move.col= j; 
                    best_val = move_val; 
                    found = true;
                } 

                // undo the move 
                board[i][j] = BoardChar::Empty;
            } 
        } 
    } 
    
    return found;
}

bool TicTacToe::evaluate(BoardChar c) { 
	// Checking for Rows for X or O victory. 
	for (int row = 0; row < 3; row++) { 
		if (board[row][0] == c && board[row][1] == c && board[row][2] == c) { 
            return true;
        }  
	} 

	// Checking for Columns for X or O victory. 
	for (int col = 0; col < 3; col++) { 
        if (board[0][col] == c && board[1][col] == c && board[2][col] == c) { 
            return true; 
        }	
    } 

    // Checking for Diagonals for X or O victory. 
    if (board[0][0] == c && board[1][1] == c && board[2][2] == c) {
        return true;
    } 
  
    if (board[0][2] == c && board[1][1] == c && board[2][0] == c) {
        return true;
    } 
  
    // Else if none of them have won
    return  false;
} 

// This is the minimax function. It considers all the possible ways 
// the game can go and returns the value of the board 
int TicTacToe::minimax(BoardChar c) {  
    // If Machine has won the game return his/her evaluated score 
    if (machine_evaluate()) {
        return 1;
    }

    // If Player has won the game return his/her evaluated score 
    if (player_evaluate()) {
        return -1;
    }

    if (!has_moves()) {
        return 0;
    }

    int best = c == machine_char ? -10 : 10;

    for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 3; j++) { 
            // check if cell is empty 
            if (board[i][j] == BoardChar::Empty) { 
                // make the move 
                board[i][j] = c;

                // call minimax recursively
                int next_best = minimax(opposite(c));

                if (c == machine_char) {
                    // choose the maximum value 
                    best = std::max(best, next_best); 
                } else {
                    // choose the minimum value 
                    best = std::min(best, next_best); 
                }
  
                // undo the move 
                board[i][j] = BoardChar::Empty; 
            } 
        } 
    } 
    
    return best;
}

ostream& operator<< (ostream &out, const TicTacToe &ttt) {
    out << "  A B C\n \u250C\u2500\u252C\u2500\u252C\u2500\u2510\n" << 1 << "\u2502";

    for (int col = 0; col < 3; col++) {
        out << static_cast<char>(ttt.board[0][col]) << "\u2502";
    }
        
    out << "\n \u251C\u2500\u253C\u2500\u253C\u2500\u2524\n" << 2 << "\u2502";

    for (int col = 0; col < 3; col++) {
        out << static_cast<char>(ttt.board[1][col]) << "\u2502";
    }
        
    out << "\n \u251C\u2500\u253C\u2500\u253C\u2500\u2524\n" << 3 << "\u2502";

    for (int col = 0; col < 3; col++) {
        out << static_cast<char>(ttt.board[2][col]) << "\u2502";
    }
    
    out << "\n \u2514\u2500\u2534\u2500\u2534\u2500\u2518\n";
    return out;
}

BoardChar read_char(string &txt) {
    cout << txt << endl;
    
    char c;
    cin >> c;
    
    BoardChar bc = from_char(c);
    if (bc == BoardChar::Empty) {
        return read_char(txt);
    }

    return bc;
}

Move read_move(string &txt) {
    cout << txt << endl;
    
    string line;
    cin >> line;
    
    Move m = Move{-1, -1};
    if (!from_str(m, line)) {
        return read_move(txt);
    }

    return m;
}

int main() {
    string read_char_txt = "Please choose a symbol: X or O";
    string read_move_txt = "your move: ";

    BoardChar bc = read_char(read_char_txt);
    
    TicTacToe ttt(bc);
    cout << ttt << endl;

    while (ttt.has_moves()
            && !ttt.player_evaluate() 
            && !ttt.machine_evaluate()) {

        Move m = read_move(read_move_txt);
        ttt.player_move(m);

        Move best_move = Move{-1, -1};
        if (ttt.find_best_move(best_move)) {
            ttt.machine_move(best_move);
            cout << "machine moved: " << m << endl;
        }

        cout << ttt;
    }

    if (ttt.player_evaluate()) {
        cout << "Congratulations, you won!" << endl;
    } else if (ttt.machine_evaluate()) {
        cout << "Sorry, but you lost" << endl;
    } else {
        cout << "Draw" << endl;
    }

	return 0; 
} 