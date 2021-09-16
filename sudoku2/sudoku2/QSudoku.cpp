#include "QSudoku.h"
#include <QMessageBox>
QSudoku::QSudoku(QWidget *parent) : QWidget(parent)
{
    base = 3;

    bool ok;
    base = QInputDialog::getInt(this, tr("What size of a grid do you want? (3 is default)"),
                                 tr("Base:"), 3, 2, 5, 1, &ok);
    if (!ok){
        qApp->quit();
    }
    board_size = base*base;
    game = SudokuGame(base);
    game.setup_map();

    scrollArea = new QScrollArea;
    mainLayout = new QGridLayout(scrollArea);
    gridLayout = new QGridLayout;
    std::vector<QGridLayout*> box_grid_layouts;



    std::vector<QGroupBox*> boxes;
    //create boxes
    for(int i = 0; i < board_size;i++){
        QGroupBox * box = new QGroupBox();
        QGridLayout *layout = new QGridLayout;
        box_grid_layouts.push_back(layout);
        boxes.push_back(box);
        int row = (i)/base;
        int col = (i)%base;
        box->setLayout(layout);
        gridLayout->addWidget(box,row,col);
    }
    QSize size_box(1,1);

    int box_offset = 0;
    for (int row = 0; row < board_size; row++){
        if ((row) % base == 0 && row != 0){
            box_offset++;
        }
        for (int col = 0; col < board_size; col++){
            QSudokuTextEdit * cur_cell = new QSudokuTextEdit();
            display_grid[row][col] = cur_cell;
            cur_cell->setMinimumSize(size_box);
            int value = game.map_board.get(row,col);
            if (value != 0){
                cur_cell->setIntValue(value);
                cur_cell->setHintCell();
            }
            else{
                cur_cell->setEntryCell();
            }
            //pick which groupbox to put the text box
            int box = int(col/base) + base*box_offset;
            if (box >= board_size){break;}
            int new_col = col%base;
            int new_row = row%base;
            box_grid_layouts[box]->addWidget(display_grid[row][col],new_row,new_col,1,1);
            //box_grid_layouts[box]->addWidget(display_grid[row][col]);
        }
    }
    buttonLayout = new QGridLayout;
    button_submit = new QPushButton("Submit");
    connect(button_submit, &QPushButton::clicked, this, &QSudoku::on_button_submit_clicked);
    button_solve = new QPushButton("Solve");
    connect(button_solve, &QPushButton::clicked, this, &QSudoku::on_button_solve_clicked);
    button_hint = new QPushButton("Hint");
    connect(button_hint, &QPushButton::clicked, this, &QSudoku::on_button_hint_clicked);
    button_visual_solve = new QPushButton("Visual Solve");
    connect(button_visual_solve, &QPushButton::clicked, this, &QSudoku::on_button_visual_solve_clicked);
    buttonLayout->addWidget(button_submit,0,0);
    buttonLayout->addWidget(button_solve,0,1);
    buttonLayout->addWidget(button_visual_solve,0,2);
    buttonLayout->addWidget(button_hint,0,3);
    mainLayout->addLayout(gridLayout,0,0);
    mainLayout->addLayout(buttonLayout,1,0);
    setWindowTitle("Sudoku");
    setLayout(mainLayout);
}
QSudoku::~QSudoku(){
    delete mainLayout;
    delete gridLayout;
    delete buttonLayout;
    delete button_submit;
    delete button_solve;
    delete button_visual_solve;
    delete button_hint;
    delete scrollArea;
    for (int row = 0; row < board_size; row++){
        for (int col =0; col < board_size; col++){
            delete display_grid[row][col];
        }
    }
    for (auto & box : boxes){
        delete box;
    }
    for (auto & elem : box_grid_layouts){
        delete elem;
    }
}

Board QSudoku::get_user_map(){
    Board user_map(board_size);
    for (int row = 0; row < board_size; row++){
       for (int col = 0; col < board_size; col++){
           QString value = display_grid[row][col]->toPlainText();
           user_map.set(row,col,value.toInt());
       }
    }
    return user_map;
}

void QSudoku::on_button_submit_clicked(){
    Board user_map = get_user_map();
    //game.print_map(user_map);
    QMessageBox msgBox;
    msgBox.setText("Sudoku Game Board has been checked.");
    msgBox.setWindowTitle("Submission");
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Close);
    if (game.check_valid(user_map)){
        msgBox.setInformativeText("You have solved this puzzle! Great Job!");
    }
    else{
        msgBox.setInformativeText("Incorrect Solution, Please try again!");
    }


    msgBox.exec();
}
bool QSudoku::visual_solve_smart(Board &map, int row, int col){
    location loc;
    if (!game.find_best_location(map,loc,row,col)){
        return true;
    }
    if (loc.num_options < 0){
        return false;
    }
    row = loc.row;
    col = loc.col;
    if (map.get(row,col) != 0){
        return visual_solve_smart(map, row, col + 1);
    }
    //options will change so make a copy of it:
    std::vector<int> loc_options = loc.options;
    for (auto & value : loc_options){
        map.set(row,col,value);
        display_grid[row][col]->setIntValue(value);
        qApp->processEvents();
        if (visual_solve_smart(map,row, col + 1)){
            return true;
        }

    }
    map.set(row,col,0);
    //loc = game.generate_location(map,row,col);
    //game.locations.push(loc);
    display_grid[row][col]->clear();
    qApp->processEvents();
    return false;
}

bool QSudoku::visual_solve(Board &map, int row, int col){
    if (row == board_size - 1 && col == board_size){
            return true;
    }
    if (col == board_size) {
        row++;
        col = 0;
    }
    if (map.get(row,col) > 0){
        return visual_solve(map, row, col + 1);
    }
    //for (int value = 1; value <= board_size; value++){
    for (auto & value : game.options){
        if (game.is_valid_place(map, row, col, value)) {
            map.set(row,col,value);
            display_grid[row][col]->setIntValue(value);
            qApp->processEvents();
            if (visual_solve(map,row, col + 1)){
                return true;
            }
        }

    }
    map.set(row,col,0);
    display_grid[row][col]->clear();
    qApp->processEvents();
    return false;
}

void QSudoku::on_button_visual_solve_clicked(){
    //game.setup_locations(game.map_board);
    visual_solve_smart(game.map_board,0,0);
}

void QSudoku::on_button_solve_clicked(){
    //game.solve();
    SetMapValues(game.solution_board);
}
void QSudoku::SetMapValues(Board &map){
    for (int row = 0; row < board_size; row++){
        for (int col = 0; col < board_size; col++){
            if (map.get(row,col) != 0){
                if (!display_grid[row][col]->isReadOnly()){
                    display_grid[row][col]->setText(QString::number(map.get(row,col)));
                }
            }
        }
    }
}




bool QSudoku::board_read_only(){
    for(int row = 0; row < board_size; row++){
        for (int col = 0; col < board_size; col++){
            if (!display_grid[row][col]->isReadOnly()){
                return false;
            }
        }
    }
    return true;
}
void QSudoku::on_button_hint_clicked(){
    //check if whole board is read only.
    if (board_read_only()){
        return;
    }
    while(true){
        int row = rand() % board_size;
        int col = rand() % board_size;
        if (!display_grid[row][col]->isReadOnly()){
            int value = game.solution_board.get(row,col);
            display_grid[row][col]->setIntValue(value);
            game.map_board.set(row,col,value);
            display_grid[row][col]->setHintCell();
            break;
        }
    }
}
