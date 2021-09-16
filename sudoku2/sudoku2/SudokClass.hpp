#include "SudokuClass.h"

template <typename T>
int & Board<T>::get(int row, int col){
    return array[row*size + col];
}
template <typename T>
int & Board<T>::set(int row, int col, int value){
    return array[row*size + col] = value;
}


template <typename T>
location<T>::location(int row, int col, std::vector<T> options) : row(row), col(col), options(options){}

template <typename T>
SudokuGame<T>::SudokuGame(const int base) : base(base), board_size(base*base), min_hints(base*base*base){
    options.reserve(board_size);
    for (int i = 1; i <= board_size; ++i){
        options.push_back(i);
    }
    Board<T> map_board(board_size);
    Board<T> solution_board(board_size);
}
template <typename T>
int SudokuGame<T>::get_base() const{
    return base;
}
template <typename T>
int SudokuGame<T>::get_board_size() const{
    return board_size;
}
template <typename T>
int SudokuGame<T>::get_min_hints() const{
    return min_hints;
}

template <typename T>
std::vector<T> SudokuGame<T>::get_cell_options(int row, int col){
    std::vector<T> cur;
    for (int i = 0; i < board_size; i++){
        if (is_valid_place(row,col,i)){
            cur.push_back(i);
        }
    }
    return cur;
}

template <typename T>
void SudokuGame<T>::setup_locations(){
    for (int row = 0; row < board_size; row++){
        for (int col = 0; row < board_size; col++){
            std::vector<T> cur_options = get_cell_options(row,col);
            location<T> cur(row,col,cur_options,cur_options.size());
            locations.push(cur);
        }
    }
}
template <typename T>
void SudokuGame<T>::randomize_options(){
    std::random_shuffle(options.begin(),options.end());
    return;
    for (int i = 0; i < 100; i++){
        std::swap(options[rand() % board_size], options[rand() % board_size]);
    }
}

template <typename T>
bool SudokuGame<T>::is_in_column(Board<T>& map, int col, int value){
    for (int row = 0; row < board_size; row++){
        if (map.get(row,col) == value){
            return true;
        }
    }
    return false;
}
template <typename T>
bool SudokuGame<T>::is_in_row(Board<T> &map, int row, int value) {
    for (int col = 0; col < board_size; col++){
        if (map.get(row,col) == value){
            return true;
        }
    }
    return false;
}
template <typename T>
bool SudokuGame<T>::is_in_box(Board<T>& map,int row,int col, int value){
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

template <typename T>
bool SudokuGame<T>::is_valid_place(Board<T>& map,int row, int col, int value){
    return(!is_in_row(map,row,value) && !is_in_column(map,col,value) && !is_in_box(map,row,col,value));
}

template <typename T>
bool SudokuGame<T>::find_empty_location_smart(location<T> &location){
    if (!locations.empty()){
        location = locations.top();
        locations.pop();
    }
    //rememeber to put back in locations if the search does not go well
    return false;
}
template <typename T>
bool SudokuGame<T>::find_empty_place(Board<T>& map,int &row, int &col){ //get empty location and update row and column
   for (row = 0; row < board_size; row++){
      for (col = 0; col < board_size; col++){
         if (map.get(row,col) == 0) {
            return true;
         }
      }
   }
   return false;
}
template <typename T>
int SudokuGame<T>::GetGridLocation(int row, int col){
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
template <typename T>
void SudokuGame<T>::Copy(const SudokuGame &other){
    base = other.get_base();
    board_size = other.get_board_size();
    min_hints = other.get_min_hints();
    map_board = other.map_board;
    solution_board = other.solution_board;
    options = other.options;
}
template <typename T>
bool SudokuGame<T>::check_valid(Board<T> map){
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

template <typename T>
bool SudokuGame<T>::solve_smart(Board<T> &map){
    location<T> loc;
    if (!find_empty_location_smart(loc)){
       return true;
    }
    int row = loc.row;
    int col = loc.col;
    if (map.get(row,col)> 0){
        return solve_smart(map);
    }
    //for (int value = 1; value <= board_size; value++){
    for (auto & value : loc.options){
        if (is_valid_place(map, row, col, value)) {
            map.set(row,col,value);
            if (solve_smart(map)){
                return true;
            }
        }

    }
    map.set(row,col,0);
    locations.push(loc);
    return false;
}

template <typename T>
bool SudokuGame<T>::solve(Board<T> &map, int row, int col){
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
template <typename T>
bool SudokuGame<T>::solve(){
    return solve(map_board,0,0);
}
template <typename T>
void SudokuGame<T>::setup_map(){
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
template <typename T>
void SudokuGame<T>::print_map(Board<T> &map){
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
template <typename T>
void SudokuGame<T>::print_map(){
    return print_map(map_board);
}

