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
#include <memory>
#include <bitset>

class aBoard{
public:
    //std::unique_ptr<int []> array;
    int * array;
    size_t size;
    size_t board_size;

    aBoard(const size_t board_size) : size(board_size*board_size),board_size(board_size) {
        array = new int[size] {};
    }
    ~aBoard(){
        delete [] array;
    }
    bool is_out_of_range(const int row, const int col) const{
        size_t loc = row*board_size + col;
        if (loc >= size){
            std::cout << "TRIED TO ACCESS DATA PASSED THE ARRAY" << std::endl;
            return true;
        }
        return false;
    }
    int get(const int row, const int col) const{
        return array[(row*size) + col];
        //return *(array + row*board_size + col);
    }
    void set(const int row, const int col, const int value){
        array[(row*size) + col] = value;
        //*(array + row*board_size + col) = value;
    }
    void print(){
        for (size_t i = 0; i < size; i++){
            std::cout << array[i] << ",";
            if ((i+1)%board_size == 0){
                std::cout << std::endl;
            }
        }
    }
    int & operator[](const int index){
        return array[index];
    }
};

class Board{
public:
    std::vector<int> array;
    size_t size,board_size;

    Board(const size_t board_size) : size(board_size*board_size),board_size(board_size) {
        array.reserve(size);
        //std::fill(array.begin(),array.end(),0);
        for (size_t i = 0; i < size; i++){
            array.push_back(0);
        }
    }
    int get(const int row, const int col) const{
        return array[(row*board_size) + col];
    }
    void set(const int row, const int col, const int value){
        array[(row*board_size) + col] = value;
    }
    void print(){
        for (size_t i = 0; i < size; i++){
            std::cout << array[i] << ",";
            if ((i+1)%board_size == 0){
                std::cout << std::endl;
            }
        }
    }
};


struct location{
    int row, col,num_options;
    std::vector<int> options;
    location(int row, int col, int num_options, std::vector<int> new_options) : row(row), col(col), num_options(num_options){
        for (auto elem : new_options){
            options.push_back(elem);
        }
    }
    location() : row(-1), col(-1), num_options(-1){}
    void set(int new_row, int new_col, int new_num_options, std::vector<int> new_options){
        row = new_row;
        col = new_col;
        num_options = new_num_options;
        options = new_options;
    }
    bool operator<(const location &rhs) const{
        return (num_options > rhs.num_options);
    }
    bool operator>(const location &rhs) const{
        return (num_options < rhs.num_options);
    }
    void add_option(int value){
        options.push_back(value);
        num_options++;
    }
    void remove_option(int value){
        auto it = std::find(options.begin(),options.end(),value);
        if (it != options.end()){
            options.erase(it);
            num_options--;
        }
    }
};


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



    void randomize_options(){
        std::random_shuffle(options.begin(),options.end());
        return;
        for (int i = 0; i < 100; i++){
            std::swap(options[rand() % board_size], options[rand() % board_size]);
        }
    }
    bool is_in_column(Board& map, int col, int value){
        for (int row = 0; row < board_size; row++){
            if (map.get(row,col) == value){
                return true;
            }
        }
        return false;
    }
    bool is_in_row(Board &map, int row, int value){
        for (int col = 0; col < board_size; col++){
            if (map.get(row,col) == value){
                return true;
            }
        }
        return false;
    }
    bool is_in_box(Board& map,int row,int col, int value){
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
    Board map_board, solution_board;
    std::vector<int> options;
    std::priority_queue<location> locations;
    SudokuGame(const int base = 3): base(base), board_size(base*base), min_hints(base*base*base),map_board(board_size),solution_board(board_size){
        for (int i = 1; i <= board_size; ++i){
            options.push_back(i);
        }
    }
    bool is_valid_place(Board& map,int row, int col, int value){
        return(!is_in_row(map,row,value) && !is_in_column(map,col,value) && !is_in_box(map,row,col,value));
    }
    std::vector<int> get_cell_options(Board &map, int row, int col){
        std::vector<int> cur;
        for (int i = 1; i <= board_size; i++){
            if (is_valid_place(map,row,col,i)){
                cur.push_back(i);
            }
        }
        return cur;
    }
    location generate_location(Board & map, int row, int col){
        auto cur_options = get_cell_options(map,row,col);
        location cur(row,col,cur_options.size(),cur_options);
        return cur;
    }
    void setup_locations(Board & map){
        while (!locations.empty()){
            locations.pop();
        }
        for (int row = 0; row < board_size; row++){
            for (int col = 0; col < board_size; col++){
                if (map.get(row,col) == 0){
                    locations.push(generate_location(map,row,col));
                }
            }
        }
    }
    std::priority_queue<location> get_related_locations(Board &map, int row, int col){
        if (locations.empty()){
            setup_locations(map);
            return locations;
        }
        std::priority_queue<location> new_locations;
        std::priority_queue<location> copy_locations = locations;
        int min_row = row - row % base;
        int max_row = min_row + base;
        int min_col = col - col % base;
        int max_col = min_col + base;
        while (!copy_locations.empty()){
            auto cur = copy_locations.top();
            //add everything from the same column and row:
            //add everything from the same box.
            if ((cur.col == col || cur.row == row) ||
                ((cur.col >= min_col && cur.col < max_col) &&
                (cur.row >= min_row && cur.row < max_row))){
                    new_locations.push(cur);
            }
            copy_locations.pop();
        }
        return new_locations;
    }
    void remove_value_related_locations(Board & map, int row, int col,int value){
        std::priority_queue<location> new_locations;
        int min_row = row - row % base;
        int max_row = min_row + base;
        int min_col = col - col % base;
        int max_col = min_col + base;
        while (!locations.empty()){
            auto cur = locations.top();
            //reset everything from the same column and row:
            //reset everything from the same box.
            if ((cur.col == col || cur.row == row) ||
                ((cur.col >= min_col && cur.col < max_col) &&
                (cur.row >= min_row && cur.row < max_row))){
                    cur.remove_option(value);
            }
            cur = generate_location(map,row,col);
            //only add locations if they don't already have a value.
            new_locations.push(cur);
            locations.pop();
        }
        locations = new_locations;
    }
    void add_value_related_locations(int row, int col,int value){
        std::priority_queue<location> new_locations;
        int min_row = row - row % base;
        int max_row = min_row + base;
        int min_col = col - col % base;
        int max_col = min_col + base;
        while (!locations.empty()){
            auto cur = locations.top();
            //reset everything from the same column and row:
            //reset everything from the same box.
            if ((cur.col == col || cur.row == row) ||
                ((cur.col >= min_col && cur.col < max_col) &&
                (cur.row >= min_row && cur.row < max_row))){
                    cur.add_option(value);
            }
            //only add locations if they don't already have a value.
            new_locations.push(cur);
            locations.pop();
        }
        locations = new_locations;
    }
    void setup_inline_locations(Board & map,int row, int col){
        std::priority_queue<location> new_locations;
        int min_row = row - row % base;
        int max_row = min_row + base;
        int min_col = col - col % base;
        int max_col = min_col + base;
        while (!locations.empty()){
            auto cur = locations.top();
            //reset everything from the same column and row:
            //reset everything from the same box.
            if ((cur.col == col || cur.row == row) ||
                ((cur.col >= min_col && cur.col < max_col) &&
                (cur.row >= min_row && cur.row < max_row))){
                    cur = generate_location(map,row,col);
            }
            //only add locations if they don't already have a value.
            if (map.get(cur.row,cur.col) == 0){
                new_locations.push(cur);
            }
            locations.pop();
        }
        locations = new_locations;
    }
    bool find_empty_location_smart(location &location){
        if (!locations.empty()){
            location = locations.top();
            locations.pop();
            return true;
        }
        //rememeber to put back in locations if the search does not go well
        return false;
    }
    bool find_best_location(Board & map,location &location, int row, int col){
        setup_locations(map);
        //setup_inline_locations(map,row,col);
        if (!locations.empty()){
            location = locations.top();
            locations.pop();
            return true;
        }
        //rememeber to put back in locations if the search does not go well
        return false;
    }

    bool find_empty_place(Board& map,int &row, int &col){ //get empty location and update row and column
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
    }
    void Copy(const SudokuGame &other){
        base = other.get_base();
        board_size = other.get_board_size();
        min_hints = other.get_min_hints();
        map_board = other.map_board;
        solution_board = other.solution_board;
        options = other.options;
    }
    bool check_valid(Board map){
        for (int row = 0; row < board_size; row++){
            for (int col = 0; col < board_size; col++){
                int value = map.get(row,col);
                if (value < 1 || value > board_size){
                    return false;
                }
                map.set(row,col,0);
                if (!is_valid_place(map,row,col,value)){
                    return false;
                }
                map.set(row,col,value);
            }
        }
        return true;
    }
    bool solve_smart(Board &map,int row, int col){
        location loc;
        if (!find_best_location(map,loc,row,col)){
            return true;
        }
        if (loc.num_options <= 0){
            return false;
        }
        row = loc.row;
        col = loc.col;
        //options will change so make a copy of it:
        std::vector<int> loc_options = loc.options;
        for (auto & value : loc_options){
            map.set(row,col,value);
            //update related locations
            //remove_value_related_locations(map,row,col,value);
            if (solve_smart(map,row, col + 1)){
                return true;
            }

        }
        map.set(row,col,0);
        //add value to related locations:
        //setup_locations(map);
        //re-add the location because we removed it from the queue
        //loc = generate_location(map,row,col);
        //locations.push(loc);
        return false;
    }
    bool solve(Board &map, int row, int col){
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
        if (map.get(row,col) != 0){
            return solve(map, row, col + 1);
        }
        for (int value = 1; value <= board_size; value++){
        //for (auto & value : options){
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
        //return solve(map_board,0,0);
        setup_locations(map_board);
        return solve_smart(map_board,0,0);
    }
    void setup_map(){
        srand(time(NULL));
        //setup first row
        randomize_options();
        for (int col = 0; col < board_size; ++col){
            map_board.set(0,col,options[col]);
        }
        randomize_options();
        for (int row = 1; row < board_size; ++row){
            for (auto & elem : options){
                if (is_valid_place(map_board,row,0,elem)){
                    map_board.set(row,0,elem);
                }
            }
        }
        //now solve
        //shuffle again for more goodness
        randomize_options();

        solve();
        solution_board.array = map_board.array;
        solution_board.size = map_board.size;
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
    void print_map(Board &map){
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
