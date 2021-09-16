#ifndef SUDOKUCLASS_H
#define SUDOKUCLASS_H
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <time.h>



class SudokuGame{
private:
    int base;
    int board_size;
    int min_hints;
    int get_base() const{
        return base;
    }
    int get_board_size() const{
        return board_size;
    }
    int get_min_hints() const{
        return min_hints;
    }
    int GetGridLocation(int row, int col){
        return int(row/base) + int(col/base);
        int cur_row = (row) /3;
        int cur_col = (col) / 3;
        if (cur_row == 0){
            if (cur_col == 0){return 0;}
            else if (cur_col == 1){return 1;}
            else if (cur_col == 2){return 2;}
        }
        else if (cur_row == 1){
            if (cur_col == 0){return 3;}
            else if (cur_col == 1){return 4;}
            else if (cur_col == 2){return 5;}
        }
        else if (cur_row == 2){
            if (cur_col == 0){return 6;}
            else if (cur_col == 1){return 7;}
            else if (cur_col == 2){return 8;}
        }
        std::cout << "ERROR UNDEFINED GRID LOCATION" << std::endl;
        //return int((row+1)/3) + int((col+1)/3);
    }
    bool is_in_column(std::map<int,std::map<int,int>>& map, int col, int value){
        for (int row = 0; row < board_size; row++){
            if (map[row][col] == value){
                return true;
            }
        }
        return false;
    }
    bool is_in_row(std::map<int,std::map<int,int>>& map, int row, int value) {
        for (int col = 0; col < board_size; col++){
            if (map[row][col] == value){
                return true;
            }
        }
        return false;
    }
    bool is_in_box(std::map<int,std::map<int,int>>& map,int row,int col, int value){
        int startRow = row - row % base;
        int startCol = col - col % base;
        for (int i = 0; i < base; i++){
            for (int j = 0; j < base; j++){
                if (map[i + startRow][j + startCol] == value){
                    return true;
                }
            }
        }
        return false;
    }
    bool is_valid_place(std::map<int,std::map<int,int>>& map,int row, int col, int value){
        return(!is_in_row(map,row,value) && !is_in_column(map,col,value) && !is_in_box(map,row,col,value));
    }
    bool find_empty_place(std::map<int,std::map<int,int>>& map,int &row, int &col){ //get empty location and update row and column
       for (row = 0; row < board_size; row++){
          for (col = 0; col < board_size; col++){
             if (map[row][col] == 0) {
                return true;
             }
          }
       }
       return false;
    }

public:
    std::map<int,std::map<int,int>> map_board, solution_board;
    std::vector<int> options;
    SudokuGame(const int base = 3) : base(base), board_size(base*base), min_hints(base*base*2){
        options.reserve(board_size);
        for (int i = 1; i <= board_size; ++i){
            options.push_back(i);
        }
        for (int row = 0; row < board_size; ++row){
            for (int col = 0; col < board_size; ++col){
                map_board[row][col] = 0;
            }
        }
    }
    void Copy(const SudokuGame &other){
        base = other.get_base();
        board_size = other.get_board_size();
        min_hints = other.get_min_hints();
        map_board = other.map_board;
        solution_board = other.solution_board;
        options = other.options;
    }
    bool solve(std::map<int,std::map<int,int>> &map){
        int row, col;
        if (!find_empty_place(map,row, col)){
           return true;
        }
        auto cur_options = options;
        std::random_shuffle(cur_options.begin(),cur_options.end());
        for (auto value : cur_options){
        //for (int value = 1; value <= board_size; value++){
            if (is_valid_place(map, row, col, value)) {
                map[row][col] = value;
                if (solve(map)){
                    return true;
                }
            }

        }
        map[row][col] = 0;
        return false;
    }
    bool solve(){
        return solve(map_board);
    }
    void setup_map(){
        srand(time(NULL));
        //setup first row
        std::random_shuffle(options.begin(),options.end());
        for (int col = 0; col < board_size; ++col){
            map_board[0][col] = options[col];
        }
        //now solve
        solve(map_board);
        solution_board = map_board;
        //remove options
        for (int i = min_hints; i < (board_size*board_size); ++i){
            int row = rand() % board_size;
            int col = rand() % board_size;
            if (map_board[row][col] != 0){
                map_board[row][col] = 0;
                continue;
            }
            else{
                --i;
            }
        }
    }
    void print_map(){
        for (int i = 0; i < board_size; ++i){
            for (int j = 0; j < board_size; ++j){
                std::cout << map_board[i][j];
                if (((j+1) % base) == 0){std::cout << "|";}
            }
            std::cout << std::endl;
            if (((i+1) % base) == 0){std::cout << "------------"<< std::endl;}
        }
        std::cout << std::endl;

    }
};



#endif // SUDOKUCLASS_H
