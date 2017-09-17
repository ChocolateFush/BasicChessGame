#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "math.h"
#include <stdlib.h>
using namespace std;
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()



        /*

            ToDo:

            *   code in pawns turning into other pieces
            *   code in more obscure rules like enpasant and castling
            *

        */











void CreateAllPieces();
void DisplayBoard();
void WhitePlayerTurn();
void BlackPlayerTurn();
int ToNumber(string character);
int ToNumber2(string character);
bool BlockedPawn(int position_x, int position_y, int to_y, string name);
bool BlockedRook(int position_x, int position_y, int to_x, int to_y, string name);
bool BlockedBishop(int position_x, int position_y, int to_x, int to_y, string name);
bool BlockedKnight(int position_x, int position_y, int to_x, int to_y, string name);
bool TakingPawn(int to_x, int to_y, string name);
string ToLetter(int number);
bool InCheck(string colour);
void TakePiece(char colour);

bool taking_piece = false;
bool check_check = false;

string board[8][8];

bool Valid(int value){
    if(value >= 0 && (value <= 7)){
        return true;
    }
    return false;
}

class piece{

    public:
    string name;
    bool is_alive;
    int position_x;
    int position_y;

    piece(void){}

    piece(string defined_name, bool defined_is_alive, int defined_position_x, int defined_position_y){

        name = defined_name;
        is_alive = defined_is_alive;
        position_x = defined_position_x;
        position_y = defined_position_y;

    }

    bool IsValidMove(string move_to){

        int to_x = ToNumber(move_to.substr(0,1));
        int to_y = ToNumber2(move_to.substr(1,1));

        if(!Valid(position_x) || !Valid(to_x) || !Valid(position_y) || !Valid(to_y)){
            return false;
        }
        if((position_x == to_x) && (position_y == to_y)){
            return false;
        }

        if(name.find("P")!= std::string::npos){
            if(name.find("W") != std::string::npos){
                if((!BlockedPawn(position_x, position_y, to_y, name))&&(position_x == to_x) && (position_y == 1) && (to_y == (position_y + 2))){
                    return true;
                }else if((!BlockedPawn(position_x, position_y, to_y, name))&&(position_x == to_x)&&(to_y == (position_y + 1))){
                    return true;
                }else if(TakingPawn(to_x, to_y, name) && ((to_y - position_y) == 1) && (fabs(to_x - position_x) == 1)){
                    return true;
                }
                return false;
            }else if(name[0]=='B'){
                if((!BlockedPawn(position_x, position_y, to_y, name))&&(position_x == to_x) && (position_y == 6) && (to_y == (position_y - 2))){
                    return true;
                }else if((!BlockedPawn(position_x, position_y, to_y, name))&&(position_x == to_x)&&(to_y == (position_y - 1))){
                    return true;
                }else if(TakingPawn(to_x, to_y, name) && ((to_y - position_y) == 1) && (fabs(to_x - position_x) == 1)){
                    return true;
                }
                return false;
            }
        return false;
        }

        if(name.find("R")!= std::string::npos){
            if(!BlockedRook(position_x, position_y, to_x, to_y, name) && ((position_x == to_x)||(position_y == to_y))){
                return true;
            }
            return false;
        }

        if(name[1]==('B')){
            if(!BlockedBishop(position_x, position_y, to_x, to_y, name) && (fabs(position_x - to_x)==fabs(position_y - to_y))){
                return true;
            }
            return false;
        }

        if(name.find("N")!=std::string::npos){
            if(!BlockedKnight(position_x, position_y, to_x, to_y, name) && (((fabs(position_x - to_x) == 2) && (fabs(position_y - to_y) == 1))||((fabs(position_x - to_x) == 1) && (fabs(position_y - to_y) == 2)))){
                return true;
            }
            return false;
        }

        if(name.find("Q")!=std::string::npos){
            if((!BlockedBishop(position_x, position_y, to_x, to_y, name) && (fabs(position_x - to_x)==fabs(position_y - to_y)))||(!BlockedRook(position_x, position_y, to_x, to_y, name) && ((position_x == to_x)||(position_y == to_y)))){
                return true;
            }
            return false;
        }

        if(name.find("K")!=std::string::npos){
            if((!BlockedBishop(position_x, position_y, to_x, to_y, name) && (fabs(position_x - to_x)==fabs(position_y - to_y) && (fabs(position_y - to_y) == 1)))||(!BlockedRook(position_x, position_y, to_x, to_y, name) && ((position_x == to_x)||(position_y == to_y))) && ((fabs(position_y - to_y)<= 1)&&(fabs(position_x - to_x)<= 1))){
                return true;
            }
            return false;
        }

        return false;
    }

};

vector<piece> all_pieces;

int lastTakenIndex;

// Currently checks all blocking ones.
// Need to add in the taking piece check.
bool Checkmate(string Colour){

    int current_position_x = 0;
    int current_position_y = 0;

    int starting_index = 0;

    if(Colour == "Black"){
        starting_index = 16;
    }


    for(int i = starting_index; i < starting_index+16; i++){

        current_position_x = all_pieces[i].position_x;
        current_position_y = all_pieces[i].position_y;

        if(all_pieces[i].name[1] == 'P'){

            if(Colour == "Black"){

            for(int j = 1; j <= 2; j++){
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x) + SSTR(current_position_y + 1 - j))){
                all_pieces[i].position_x = current_position_x;
                all_pieces[i].position_y = current_position_y - j;
                if(!InCheck(Colour)){

                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                    return false;
                }

                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                }
            }

            if(all_pieces[i].IsValidMove(ToLetter(current_position_x + 1) + SSTR(current_position_y + 1 - 1))){
                    all_pieces[i].position_x = current_position_x + 1;
                    all_pieces[i].position_y = current_position_y - 1;
                    if(board[all_pieces[i].position_x][all_pieces[i].position_x][0] == 'W'){
                        TakePiece('W');
                        check_check = true;
                    }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }

            if(all_pieces[i].IsValidMove(ToLetter(current_position_x - 1) + SSTR(current_position_y + 1 - 1))){

                    all_pieces[i].position_x = current_position_x - 1;
                    all_pieces[i].position_y = current_position_y - 1;
                    if(board[current_position_x - 1][current_position_y - 1][0] == 'W'){
                        TakePiece('W');
                        check_check = true;
                    }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                    return false;
                }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }
            }else{

            for(int j = 1; j <= 2; j++){
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x) + SSTR(current_position_y + 1 + j))){
                all_pieces[i].position_x = current_position_x;
                all_pieces[i].position_y = current_position_y + j;

            if(!InCheck(Colour)){
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                    return false;
                }

                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                }
            }

            if(all_pieces[i].IsValidMove(ToLetter(current_position_x + 1) + SSTR(current_position_y + 1 + 1))){
                    all_pieces[i].position_x = current_position_x + 1;
                    all_pieces[i].position_y = current_position_y + 1;

                if(board[current_position_x + 1][current_position_y + 1][0] == 'B'){
                        TakePiece('B');
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }

            if(all_pieces[i].IsValidMove(ToLetter(current_position_x - 1) + SSTR(current_position_y + 1 + 1))){

                    all_pieces[i].position_x = current_position_x - 1;
                    all_pieces[i].position_y = current_position_y + 1;


                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                    return false;
                }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }
            }

        }else if(all_pieces[i].name[1] == 'R'){

            for(int j = 1; j <= 7; j++){
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x + j) + SSTR(current_position_y + 1))){
                    all_pieces[i].position_x = current_position_x + j;
                    all_pieces[i].position_y = current_position_y;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                        return false;
                    }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                }
            }
            for(int j = 1; j <= 7; j++){
               if(all_pieces[i].IsValidMove(ToLetter(current_position_x - j) + SSTR(current_position_y + 1))){
                    all_pieces[i].position_x = current_position_x - j;
                    all_pieces[i].position_y = current_position_y;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                        return false;
                    }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
               }
            }
            for(int j = 1; j <= 7; j++){
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x) + SSTR(current_position_y + 1 + j))){
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y + j;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                        return false;
                    }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                }
            }
            for(int j = 1; j <= 7; j++){
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x) + SSTR(current_position_y + 1 - j))){
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y - j;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                        return false;
                    }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                }
            }

        }else if(all_pieces[i].name[1] == 'N'){

            if(all_pieces[i].IsValidMove(ToLetter(current_position_x+1) + SSTR(current_position_y + 1 +3))){
                    all_pieces[i].position_x = current_position_x + 1;
                    all_pieces[i].position_y = current_position_y + 3;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }

            if(all_pieces[i].IsValidMove(ToLetter(current_position_x-1) + SSTR(current_position_y + 1 +3))){
                    all_pieces[i].position_x = current_position_x-1;
                    all_pieces[i].position_y = current_position_y+3;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
        }
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x+3) + SSTR(current_position_y + 1 +1))){
                    all_pieces[i].position_x = current_position_x+3;
                    all_pieces[i].position_y = current_position_y+1;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
    }
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x-3) + SSTR(current_position_y + 1 +1))){
                    all_pieces[i].position_x = current_position_x-3;
                    all_pieces[i].position_y = current_position_y+1;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x+3) + SSTR(current_position_y + 1 - 1))){
                    all_pieces[i].position_x = current_position_x+3;
                    all_pieces[i].position_y = current_position_y-1;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x-3) + SSTR(current_position_y + 1 - 1))){
                    all_pieces[i].position_x = current_position_x-3;
                    all_pieces[i].position_y = current_position_y-1;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x+1) + SSTR(current_position_y + 1 - 3))){
                    all_pieces[i].position_x = current_position_x+1;
                    all_pieces[i].position_y = current_position_y-3;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x-1) + SSTR(current_position_y + 1 - 3))){
                    all_pieces[i].position_x = current_position_x-1;
                    all_pieces[i].position_y = current_position_y-3;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}

        }else if(all_pieces[i].name[1] == 'B'){

            for(int j = 1; j <= 7; j++){

                if(all_pieces[i].IsValidMove(ToLetter(current_position_x+j) + SSTR(current_position_y + 1 + j))){
                    all_pieces[i].position_x = current_position_x+j;
                    all_pieces[i].position_y = current_position_y+j;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x-j) + SSTR(current_position_y + 1 - j))){
                    all_pieces[i].position_x = current_position_x-j;
                    all_pieces[i].position_y = current_position_y-j;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x+j) + SSTR(current_position_y + 1 - j))){
                    all_pieces[i].position_x = current_position_x+j;
                    all_pieces[i].position_y = current_position_y-j;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x-j) + SSTR(current_position_y + 1 + j))){
                    all_pieces[i].position_x = current_position_x-j;
                    all_pieces[i].position_y = current_position_y+j;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}

            }

        }else if(all_pieces[i].name[1] == 'Q'){


            for(int j = 1; j <= 7; j++){
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x + j) + SSTR(current_position_y + 1))){
                    all_pieces[i].position_x = current_position_x + j;
                    all_pieces[i].position_y = current_position_y;
                    if(!InCheck(Colour)){
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                        return false;
                    }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                }
            }
            for(int j = 1; j <= 7; j++){
               if(all_pieces[i].IsValidMove(ToLetter(current_position_x - j) + SSTR(current_position_y + 1))){
                    all_pieces[i].position_x = current_position_x - j;
                    all_pieces[i].position_y = current_position_y;
                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                        return false;
                    }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
               }
            }
            for(int j = 1; j <= 7; j++){
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x) + SSTR(current_position_y + 1 + j))){
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y + j;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                        return false;
                    }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                }
            }
            for(int j = 1; j <= 7; j++){
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x) + SSTR(current_position_y + 1 - j))){
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y - j;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                        return false;
                    }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                }
            }

            for(int j = 1; j <= 7; j++){

                if(all_pieces[i].IsValidMove(ToLetter(current_position_x+j) + SSTR(current_position_y + 1 + j))){
                    all_pieces[i].position_x = current_position_x+j;
                    all_pieces[i].position_y = current_position_y+j;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x-j) + SSTR(current_position_y + 1 - j))){
                    all_pieces[i].position_x = current_position_x-j;
                    all_pieces[i].position_y = current_position_y-j;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x+j) + SSTR(current_position_y + 1 - j))){
                    all_pieces[i].position_x = current_position_x+j;
                    all_pieces[i].position_y = current_position_y-j;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}
                if(all_pieces[i].IsValidMove(ToLetter(current_position_x-j) + SSTR(current_position_y + 1 + j))){
                    all_pieces[i].position_x = current_position_x-j;
                    all_pieces[i].position_y = current_position_y+j;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
}

            }

        }else if(all_pieces[i].name[1] == 'K'){

            if(all_pieces[i].IsValidMove(ToLetter(current_position_x) + SSTR(current_position_y + 1 +1))){
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y+1;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x+1) + SSTR(current_position_y + 1 + 1))){
                    all_pieces[i].position_x = current_position_x+1;
                    all_pieces[i].position_y = current_position_y+1;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x+1) + SSTR(current_position_y + 1))){
                    all_pieces[i].position_x = current_position_x+1;
                    all_pieces[i].position_y = current_position_y;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x+1) + SSTR(current_position_y + 1 -1))){
                    all_pieces[i].position_x = current_position_x+1;
                    all_pieces[i].position_y = current_position_y-1;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x) + SSTR(current_position_y + 1 -1))){
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y-1;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x-1) + SSTR(current_position_y + 1 -1))){
                    all_pieces[i].position_x = current_position_x-1;
                    all_pieces[i].position_y = current_position_y-1;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x-1) + SSTR(current_position_y + 1 ))){
                    all_pieces[i].position_x = current_position_x-1;
                    all_pieces[i].position_y = current_position_y;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }
            if(all_pieces[i].IsValidMove(ToLetter(current_position_x-1) + SSTR(current_position_y + 1 + 1))){
                    all_pieces[i].position_x = current_position_x-1;
                    all_pieces[i].position_y = current_position_y+1;

                if(board[all_pieces[i].position_x][all_pieces[i].position_y][0] != all_pieces[i].name[0]){
                        TakePiece(board[all_pieces[i].position_x][all_pieces[i].position_y][0]);
                        check_check = true;
                }

            if(!InCheck(Colour)){
                    if(check_check){all_pieces[lastTakenIndex].is_alive = true; check_check = false;}
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
                return false;
            }
                    all_pieces[i].position_x = current_position_x;
                    all_pieces[i].position_y = current_position_y;
            }


        }
    }

    return true;

}

bool InCheck (string Colour){

    int starting_index = 16;
    int king_index = 14;

    if(Colour == "Black"){
        starting_index = 0;
        king_index = 30;
    }

    for(int j = starting_index; j < starting_index+16; j++){
        if(all_pieces[j].is_alive && all_pieces[j].IsValidMove(ToLetter(all_pieces[king_index].position_x)+SSTR(all_pieces[king_index].position_y+1))){
            all_pieces[king_index].is_alive = true;
            return true;
        }
    }

    all_pieces[king_index].is_alive = true;

    return false;
}

bool TakingPawn(int to_x, int to_y, string name){
    for(int i = 0; i < 32; i++){
        if(all_pieces[i].position_x == to_x && (all_pieces[i].position_y == to_y) && (all_pieces[i].name[0] != name[0])){
            return true;
        }
    }
    return false;
}

bool BlockedKnight(int position_x, int position_y, int to_x, int to_y, string name){

    for(int i = 0; i < 32; i++){
        if((all_pieces[i].position_x == to_x) && (all_pieces[i].position_y == to_y)){
                if(all_pieces[i].name[0] != name[0]){
                    return false;
                }
            return true;
        }
    }
    return false;

}

bool BlockedBishop(int position_x, int position_y, int to_x, int to_y, string name){

    int increment_x = -1;

    if(position_x < to_x){
        increment_x = 1;
    }

    int increment_y = -1;

    if(position_y < to_y){
        increment_y = 1;
    }

    int x_value = position_x;
    int y_value = position_y;

    for(int i = 1; i <= fabs(position_x - to_x); i++){

        x_value += increment_x;
        y_value += increment_y;

        for(int j = 0; j < 32; j++){
            if(all_pieces[j].position_x == x_value){
                if(all_pieces[j].position_y == y_value){
                        if((all_pieces[j].name[0] != name[0]) && (i == fabs(position_x - to_x))){
                            return false;
                        }
                    return true;
                }

            }
        }

    }
        return false;

}

bool BlockedRook(int position_x, int position_y, int to_x, int to_y, string name){

    if(position_x == to_x){
        int smallest = position_y + 1;
        int largest = to_y;

        if(position_y > to_y){
            smallest = to_y;
            largest = position_y - 1;
        }

        for(int z = smallest; z <= largest; z++){
            for(int i = 0; i < 32; i++){
                if(all_pieces[i].position_x == position_x){
                    if(all_pieces[i].position_y == z){
                            if((z == largest) && (all_pieces[i].name[0] != name[0])){
                                return false;
                            }
                        return true;
                    }
                }
            }
        }
        return false;

    }else{

        int smallest = position_x + 1;
        int largest = to_x;

        if(position_x > to_x){
            smallest = to_x;
            largest = position_x - 1;
        }

        for(int z = smallest; z <= largest; z++){
            for(int i = 0; i < 32; i++){
                if(all_pieces[i].position_x == z){
                    if(all_pieces[i].position_y == position_y){
                            if(z == largest && (all_pieces[i].name[0] != name[0])){
                                return false;
                            }
                        return true;
                    }
                }
            }
        }

        return false;

    }

}

bool BlockedPawn(int position_x, int position_y, int to_y, string name){

    int smallest = position_y + 1;
    int largest = to_y;

    if(position_y > to_y){
        smallest = to_y;
        largest = position_y - 1;
    }

    for(int z = smallest; z <= largest; z++){
        for(int i = 0; i < 32; i++){
            if(all_pieces[i].position_x == position_x){
                if(all_pieces[i].position_y == z){
                    return true;
                }
            }
        }
    }
    return false;
}

bool game_won = false;

int main(void){

    // Create all of the pieces for the game.
    CreateAllPieces();
    int game_type = 1;

    cout << "Which kind of game do you want to play?\n1 = Player vs Player\n2 = Player vs Bot\n3 = Bot vs Bot" << endl;
    cin >> game_type;

    if(game_type == 1){
        while(!game_won){

            DisplayBoard();

            WhitePlayerTurn();

            if(game_won){
                break;
            }

            DisplayBoard();

            BlackPlayerTurn();

        }
    }else if(game_type == 3){

        cout << "How do you want to watch the bots?\n\n1 = move by move\n2 = See winner\n3 = play until stopped\n";

    }

    cout << "\n\n\nGame over!" << endl;

}


void DisplayBoard(){

    system("CLS");

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            board[i][j] = "  ";
        }
    }

    for(int i = 0; i < 32; i++){
        if(all_pieces[i].is_alive){
            board[all_pieces[i].position_x][7-all_pieces[i].position_y] = all_pieces[i].name;
        }
    }

    for(int j = 0; j < 8; j++){
        cout << 8 - j << " ";
        for(int i = 0; i < 8; i++){

            cout << "[" << board[i][j] << "]";

        }
        cout << "\n";

    }

    cout << "   A   B   C   D   E   F   G   H\nDead pieces: ";

    for(int i = 0; i < 32; i++){
        if(all_pieces[i].is_alive == false){
            cout << all_pieces[i].name << " ";
        }
    }
    cout << "\n";

}

string ToLetter(int number){
    if(number == 0){
        return "A";

    }else if(number == 1){
        return "B";

    }else if(number == 2){
        return "C";

    }else if(number == 3){
        return "D";

    }else if(number == 4){
        return "E";

    }else if(number == 5){
        return "F";

    }else if(number == 6){
        return "G";

    }else if(number == 7){
        return "H";
    }else{
        return "Z";
    }
}

int ToNumber(string character){
    if(character == "A"){
        return 0;

    }else if(character == "B"){
        return 1;

    }else if(character == "C"){
        return 2;

    }else if(character == "D"){
        return 3;

    }else if(character == "E"){
        return 4;

    }else if(character == "F"){
        return 5;

    }else if(character == "G"){
        return 6;

    }else if(character == "H"){
        return 7;
    }else{
        return 30;
    }
}

int ToNumber2(string character){
    stringstream ss(character);
    int usable = 0;

    ss >> usable;

    return  usable - 1;
}


void TakePiece(char Colour){

    int inner_index = 16;
    int outer_index = 0;
    if(Colour == 'W'){
        inner_index = 0;
        outer_index = 16;
    }

    for(int i = outer_index; i < outer_index+16; i++){
        for(int j = inner_index; j < inner_index+16; j++){
            if((all_pieces[i].position_x == all_pieces[j].position_x)&&(all_pieces[i].position_y == all_pieces[j].position_y) && (all_pieces[i].is_alive == true)){
                lastTakenIndex = j;
                all_pieces[j].is_alive = false;
            }
        }
    }

}

void WhitePlayerTurn(){

    bool valid_piece = false;
    string move_string = "";
    int current_position_x = 0;
    int current_position_y = 0;
    bool puts_in_check = false;

    while(!valid_piece){

        if(InCheck("W")){
                cout << "Check";
            if(Checkmate("W")){
                    cout << "mate";
                    game_won = true;
                    return;
            }
            cout << "\n\n";
        }

        cout << "White player turn:\n\nInput a valid move in the format:<Piece><Current position><Desired position>\ne.g. WPA2A4\n\n";
        cin >> move_string;

        for(int i = 0; i < 16; i++){
            if(all_pieces[i].is_alive && (all_pieces[i].name == move_string.substr(0, 2)) && (all_pieces[i].position_x == ToNumber(move_string.substr(2, 1))) && (all_pieces[i].position_y == ToNumber2(move_string.substr(3, 1))) && (move_string.substr(0,1).find("W") != std::string::npos)){
                if(all_pieces[i].IsValidMove(move_string.substr(4,2))){

                    current_position_x = all_pieces[i].position_x;
                    current_position_y = all_pieces[i].position_y;
                    all_pieces[i].position_x = ToNumber(move_string.substr(4,1));
                    all_pieces[i].position_y = ToNumber2(move_string.substr(5,1));
                    TakePiece('B');

                    if(InCheck("W")){
                        all_pieces[lastTakenIndex].is_alive = true;
                        all_pieces[i].position_x = current_position_x;
                        all_pieces[i].position_y = current_position_y;
                        break;
                    }
                    return;
                }
            }
        }
        DisplayBoard();
    }
}

void BlackPlayerTurn(){

    bool valid_piece = false;
    string move_string = "";
    int current_position_x = 0;
    int current_position_y = 0;
    bool puts_in_check = false;

    while(!valid_piece){

        if(InCheck("Black")){
            cout << "\nCheck";
            if(Checkmate("Black")){
                cout << "mate";
                game_won = true;
                return;
            }
            cout << "\n\n";
        }

        cout << "Black player turn:\n\nInput a valid move in the format:<Piece><Current position><Desired position>\ne.g. BPD7D6\n\n";
        cin >> move_string ;

        for(int i = 0; i < 32; i++){
            if(all_pieces[i].is_alive && (all_pieces[i].name == move_string.substr(0, 2)) && (all_pieces[i].position_x == ToNumber(move_string.substr(2, 1))) && (all_pieces[i].position_y == ToNumber2(move_string.substr(3, 1))) && (move_string.substr(0,1).find("B") != std::string::npos)){
                if(all_pieces[i].IsValidMove(move_string.substr(4,2))){

                    current_position_x = all_pieces[i].position_x;
                    current_position_y = all_pieces[i].position_y;
                    all_pieces[i].position_x = ToNumber(move_string.substr(4,1));
                    all_pieces[i].position_y = ToNumber2(move_string.substr(5,1));
                    TakePiece('W');

                    if(InCheck("Black")){
                        all_pieces[lastTakenIndex].is_alive = true;
                        all_pieces[i].position_x = current_position_x;
                        all_pieces[i].position_y = current_position_y;
                        break;
                    }

                    return;
                }
            }
        }
        DisplayBoard();
    }
}

void CreateAllPieces(){

    for(int i = 0; i < 8; i++){
        all_pieces.push_back( piece("WP", true, i, 1)); // pawn
    }

    all_pieces.push_back(piece("WR", true, 0, 0)); // rook
    all_pieces.push_back(piece("WR", true, 7, 0)); // rook
    all_pieces.push_back(piece("WB", true, 2, 0)); // bishop
    all_pieces.push_back(piece("WB", true, 5, 0)); // bishop
    all_pieces.push_back( piece("WN", true, 1, 0)); // knight
    all_pieces.push_back( piece("WN", true, 6, 0)); // knight
    all_pieces.push_back( piece("WK", true, 4, 0)); // king
    all_pieces.push_back( piece("WQ", true, 3, 0)); // queen

    for(int i = 16; i < 24; i++){
        all_pieces.push_back( piece("BP", true, i - 16, 6)); // pawn
    }

    all_pieces.push_back( piece("BR", true, 0, 7)); // rook
    all_pieces.push_back( piece("BR", true, 7, 7)); // rook
    all_pieces.push_back( piece("BB", true, 2, 7)); // bishop
    all_pieces.push_back( piece("BB", true, 5, 7)); // bishop
    all_pieces.push_back( piece("BN", true, 1, 7)); // knight
    all_pieces.push_back( piece("BN", true, 6, 7)); // knight
    all_pieces.push_back( piece("BK", true, 4, 7)); // king
    all_pieces.push_back( piece("BQ", true, 3, 7)); // queen

}
