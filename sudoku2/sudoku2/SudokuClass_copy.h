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
#include <cstdlib>
#include <queue>

template <typename T>
class vBoard{
private:
    std::vector<std::vector<T>> arr;
    size_t size = 9;
public:
    vBoard(const size_t size = 9) : size(size){
        arr.reserve(size);
        for(size_t i = 0; i < size; ++i) {
            std::vector<T> tmp(size);
            arr.push_back(tmp);
        }
    }
    std::vector<T>& operator[](int index){
        return arr[index];
    }
};

template <typename T>
class Board{
private:
    T * array;
    size_t size;
public:
    Board(size_t size = 9) : size(size) {
        array = new T[size*size];
    }
    ~Board(){
        delete array;
    }
    int & get(int row, int col){
        return array[row*size + col];
    }
    int & set(int row, int col, int value){
        return array[row*size + col] = value;
    }
    T &operator[](int index){
        return array[index];
    }
};
template <typename T>
struct location{
    int row, col;
    std::vector<T> options;
    location(int row, int col, std::vector<T> options) : row(row), col(col), options(options){}
};



class SudokuGame{
private:
    int base;
    int board_size;
    int min_hints;
    std::priority_queue<int> locations;
    int get_base() const{
        return base;
    }
    int get_board_size() const{
        return board_size;
    }
    int get_min_hints() const{
        return min_hints;
    }
    std::vector<int> get_cell_options(int row, int col){
        std::vector<int> cur;
        for (int i = 0; i < board_size; i++){

        }
    }
    void setup_locations(){
        for (int row = 0; row < board_size; row++){
            for (int col = 0; row < board_size; col++){

            }
        }
    }
    void randomize_options(){
        std::random_shuffle(options.begin(),options.end());
        return;
        for (int i = 0; i < 100; i++){
            std::swap(options[rand() % board_size], options[rand() % board_size]);
        }
    }
    bool is_in_column(Board<int>& map, int col, int value){
        for (int row = 0; row < board_size; row++){
            if (map.get(row,col) == value){
                return true;
            }
        }
        return false;
    }
    bool is_in_row(Board<int> &map, int row, int value) {
        for (int col = 0; col < board_size; col++){
            if (map.get(row,col) == value){
                return true;
            }
        }
        return false;
    }
    bool is_in_box(Board<int>& map,int row,int col, int value){
        int startRow = row - row % base;
        int startCol = col - col % base;
        for (int i = 0; i < base; i++){
            for (int j = 0; j < base; j++){
                if (map.get(i + startRow,j + startCol) == value){
                    return true;
                }
            }
        }
        return false;
    }


public:
    Board<int> map_board, solution_board;
    std::vector<int> options;
    SudokuGame(const int base = 3) : base(base), board_size(base*base), min_hints(base*base*base){
        options.reserve(board_size);
        for (int i = 1; i <= board_size; ++i){
            options.push_back(i);
        }
        Board<int> map_board(board_size);
        Board<int> solution_board(board_size);
    }
    bool is_valid_place(Board<int>& map,int row, int col, int value){
        return(!is_in_row(map,row,value) && !is_in_column(map,col,value) && !is_in_box(map,row,col,value));
    }

    template <typename T>
    bool find_empty_place_smart(Board<T> &map, int &row, int &col){

    }
    bool find_empty_place(Board<int>& map,int &row, int &col){ //get empty location and update row and column
       for (row = 0; row < board_size; row++){
          for (col = 0; col < board_size; col++){
             if (map.get(row,col) == 0) {
                return true;
             }
          }
       }
       return false;
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
    void Copy(const SudokuGame &other){
        base = other.get_base();
        board_size = other.get_board_size();
        min_hints = other.get_min_hints();
        map_board = other.map_board;
        solution_board = other.solution_board;
        options = other.options;
    }
    bool check_valid(Board<int> map){
        for (int row = 0; row < board_size; row++){
            for (int col = 0; col < board_size; col++){
                int value = map.get(row,col);
                if (value < 1 || value > board_size){
                    return false;
                }
                map.get(row,col) = 0;
                if (!is_valid_place(map,row,col,value)){
                    return false;
                }
                map.get(row,col) = value;
            }
        }
        return true;
    }
    bool solve(Board<int> &map, int row, int col){
        /*if (!find_empty_place(map,row, col)){
           return true;
        }*/
        if (row == board_size - 1 && col == board_size){
                return true;
        }
        if (col == board_size) {
            row++;
            col = 0;
        }
        if (map.get(row,col)> 0){
            return solve(map, row, col + 1);
        }
        //for (int value = 1; value <= board_size; value++){
        for (auto & value : options){
            if (is_valid_place(map, row, col, value)) {
                map.set(row,col,value);
                if (solve(map,row, col + 1)){
                    return true;
                }
            }

        }
        map.set(row,col,0);
        return false;
    }
    bool solve(){
        return solve(map_board,0,0);
    }
    void setup_map(){
        srand(time(NULL));
        //setup first row
        randomize_options();
        for (int col = 0; col < board_size; ++col){
            map_board.set(0,col,options[col]);
        }
        //now solve
        //shuffle again for more goodness
        randomize_options();
        solve();
        solution_board = map_board;
        //remove options
        for (int i = min_hints; i < (board_size*board_size); ++i){
            int row = rand() % board_size;
            int col = rand() % board_size;
            if (map_board.get(row,col) != 0){
                map_board.set(row,col,0);
                continue;
            }
            else{
                --i;
            }
        }
    }
    void print_map(Board<int> &map){
        for (int i = 0; i < board_size; ++i){
            for (int j = 0; j < board_size; ++j){
                std::cout << map.get(i,j);
                if (((j+1) % base) == 0){std::cout << "|";}
            }
            std::cout << std::endl;
            if (((i+1) % base) == 0){std::cout << "------------"<< std::endl;}
        }
        std::cout << std::endl;

    }
    void print_map(){
        return print_map(map_board);
    }

};



#endif // SUDOKUCLASS_H
