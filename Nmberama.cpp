#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <fstream>
#include <cstring>

using namespace std;

struct Game {

    int row;
    int column;
    int score;
    char user[30];
    int** loc;
};

struct user {

    char user[30];
    char pass[30];
    int score;
};

//general
void clear();
int binary_search(user u[], int &len, char username[30]);
void sort_by_username(user u[], int &len);
void show(Game& g, int& x1, int& x2, int& y1, int& y2, int& tx, int& ty);

//check
bool check_it(int& x, int& y);
int check_all(Game &g);
bool accept(Game &g, int &x1, int &y1, int &x2, int& y2);
bool check_pass(user &u, user  users_array[], int &len);

//star
void sort_it_s(int line[], int &len);
void sort_it_n(int line[], int& len);
int boom(int* line, int* uline, int* dline, int &len);
int count(int* line, int* uline, int* dline, int &len);
int star(Game& g, int &x1, int& y1, int &x2, int& y2);

//user
void new_user(Game& g, user u[], int& len);
void load_score(Game& g, user u[], int len);
void sort_by_score(user u[], int& len);
int read_it(user u[]);
void write_it(user u[], int &len);
void save_it(user u[], int &len, Game &g);
void show_scorboard(user u[], int &len);
bool menu(Game& g, user u[], int &len);

//gameplay

void move(Game& g, int& x, int& y, int& chosen_x, int& chosen_y, char& in);
void start_it(Game& g);
void choose_two(Game& g, int& x1, int& y1, int& x2, int& y2, int& tx, int& ty);
void play_it(Game& g,user u[],int &len);

int main() {
    
    user registered_users[100],player;
    Game game;
    bool FINISH = false, play = false, done = false, accepted = false;
    char input;
    ifstream saves;
    ifstream users;
    int len;

    len=read_it(registered_users);

    //log in 

    cout << endl << "HI !";
    
    while (!accepted) {
        
        clear();
        cout << endl << endl << "    " << "1 . Login" << endl << "    " << "2 . Create account" << endl << endl;
        
        input = _getch();
        
        switch (input) {

        case '1': { //login

            while (!done) {

                clear();

                //get user name and password
                cout << "Enter username : " << endl;
                cin >> player.user;

                cout << "Enter password : " << endl;
                cin >> player.pass;
                //*****

                //check them
                clear();
                cin.ignore();
                done = check_pass(player, registered_users, len);

                if (done) {
                    strcpy_s(game.user, strlen(player.user) + 1, player.user);
                    accepted = true;
                }
                if (!done)
                    cout << "Wrong username or password !" << endl << endl;
                //*****
            }
            break;
        }

        case '2': {//new account

            clear();
            new_user(game, registered_users, len);
            clear();
            read_it(registered_users);
            accepted = true;
            break;
        }
        }
    }
    accepted = false;

    while (!FINISH) {
        play = false;
        done = false;
        load_score(game, registered_users, len);

        play=menu(game, registered_users, len);
        if (!play)
            break;

        while (play) {
            
            //chosing size
            while (!done) {
                clear();
                
                cout << "  please enter the size (5 is the minimum): " << endl << endl << "row : ";
                cin >> game.row;
                if (game.row < 5)
                    continue;
                cout << "column  : ";
                cin >> game.column;
                if (game.column < 5)
                    continue;
                break;
            }
                //*****

            start_it(game);
            play_it(game,registered_users,len);

            clear();

            cout << endl << "    " << "FINISHED ! GOOD JOB! " << endl << endl << endl << "  press any key to continue"<<endl;
            input = _getch();
            clear();
            
            show_scorboard(registered_users, len);
            cout << endl << endl << "    press any key to continue";
            input = _getch();
            clear();


            cout << "do you want to play again ? (Y/N)" << endl;
            while (true) {
         
                input = _getch();
                
                if (input == 'y') {
                    play = true;
                    break;
                }
                else if (input == 'n') {
                    play = false;
                    break;
                }
                else cout << " try again , (Y/N)" << endl;
            }
        }
    }
    
    return 0;
}

// GENERAL

void clear() {
    system("CLS");
}

void sort_by_score(user u[], int& len) {
    int j;
    user temp;
    for (int i = 1; i < len; i++) {
        j = i - 1;
        temp = u[i];

        while (u[j].score < temp.score) {

            u[j + 1] = u[j];
            j--;

            if (j < 0)
                break;
        }
        u[j + 1] = temp;
    }
}

void sort_by_username(user u[], int& len) {
    int j;
    user temp;
    for (int i = 1; i < len; i++) {
        j = i - 1;
        temp = u[i];

        while (strcmp(u[j].user, temp.user) > 0) {

            u[j + 1] = u[j];
            j--;

            if (j < 0)
                break;
        }
        u[j + 1] = temp;
    }
}

int binary_search(user u[], int& len, char username[30]) {
    int first = 0, last = len - 1, mid;

    while (first <= last) {

        mid = (first + last) / 2;

        if (strcmp(username, u[mid].user) == 0)
            return mid;
        if (strcmp(username, u[mid].user) > 0)
            first = mid + 1;
        else last = mid - 1;
    }
    return -1;
}

int check_all(Game &g) {
    int temp_x, temp_y;
    int counter = 0;

    for (int i = 0; i < g.row; i ++) {
        for (int j = 0; j < g.column; j++) {
            temp_x = i;
            temp_y = j;

            if (g.loc[i][j] == 0)
                continue;

            if (i != 0) {
                temp_x--;
                
                while (g.loc[temp_x][j] == 0) {
                    if (temp_x != 0)
                        temp_x--;
                    if (temp_x == 0)
                        break;
                }

                if (check_it(g.loc[i][j], g.loc[temp_x][j]))
                    counter++;
            }
            temp_x = i;
            if (i != g.row - 1) {
                temp_x++;
                
                while (g.loc[temp_x][j] == 0) {
                    if (temp_x != g.row - 1)
                        temp_x++;
                    if (temp_x == g.row - 1)
                        break;
                }
                if (check_it(g.loc[i][j], g.loc[temp_x][j]))
                    counter++;
            }
            if (j != 0) {
                temp_y--;
                
                while (g.loc[i][temp_y] == 0) {
                    if (temp_y != 0)
                        temp_y--;
                    if (temp_y == 0)
                        break;
                }
                if (check_it(g.loc[i][j], g.loc[i][temp_y]))
                    counter++;
            }
            temp_y = j;
            if (j != g.column - 1) {
                temp_y++;
                
                while (g.loc[i][temp_y] == 0) {
                    if (temp_y != g.column - 1)
                        temp_y++;
                    if (temp_y == g.column - 1)
                        break;
                }
                if (check_it(g.loc[i][j], g.loc[i][temp_y]))
                    counter++;
            }
        }
    }
    return counter;
}

void show(Game &g, int &x1, int &x2, int &y1, int& y2, int &tx, int &ty) {

    clear();
    for (int i = 0; i < g.row; i++) {
        for (int j = 0; j < g.column; j++) {

            if (((i == x1) && (j == y1)) || ((i == x2) && (j == y2)) || ((i == tx) && (j == ty))) {
                if (g.loc[i][j] == 0)
                    cout << "[" << "X" << "]";
                else if (g.loc[i][j] == 10)
                    cout << "[" << "*" << "]";
                else cout << "[" << g.loc[i][j] << "]";
            }
            else {

                if (g.loc[i][j] == 0)
                    cout << "X";
                else if (g.loc[i][j] == 10)
                    cout << '*';
                else cout << g.loc[i][j];
            }
            
            if (j != g.column - 1)
                cout << "   ";
        }
        if (i != g.row - 1)
            cout << endl<<endl;
    }
    cout << endl << endl << "    score : " << g.score;
}

// CHECK

bool check_it(int& x, int& y) {

    if (((x == y) || ((x + y) == 10)) && (x != 0))
        return true;

    return false;
}

bool accept(Game &g,int &x1, int &y1, int &x2, int &y2) {

    if ((x1 == x2) && (y1 != y2)) {
        if (y1 < y2) {
            for (int i = y1 + 1; i < y2; i++)
                if (g.loc[x1][i] != 0)
                    return false;
        }
        else {
            for (int i = y2 + 1; i < y1; i++)
                if (g.loc[x1][i] != 0)
                    return false;
        }
        return true;
    }
    if ((y1 == y2) && (x1 != x2)) {
        if (x1 < x2) {
            for (int i = x1 + 1; i < x2; i++)
                if (g.loc[i][y1] != 0)
                    return false;
        }
        else {
            for (int i = x2 + 1; i < x1; i++)
                if (g.loc[i][y1] != 0)
                    return false;
        }
        return true;
    }
    return false;
}

bool check_pass(user& u, user users_array[], int& len) {
    int index;
    sort_by_username(users_array, len);

    index = binary_search(users_array, len, u.user);
    if (index < 0) {
        cout << "username not found !" << '\a' << endl;
        return false;
    }

    if (strcmp(u.pass, users_array[index].pass) == 0)
        return true;
    return false;
}

// STAR

void sort_it_s(int line[], int& len) {
    int temp, j;

    for (int i = 1; i < len; i++) {
        j = i - 1;
        temp = line[i];

        while (temp < line[j]) {

            line[j + 1] = line[j];
            j--;
            if (j < 0) break;
        }

        line[j + 1] = temp;
    }
}

void sort_it_n(int line[], int& len) {
    int temp, j;

    for (int i = 1; i < len; i++) {
        j = i - 1;
        temp = line[i];

        while (temp > line[j]) {

            line[j + 1] = line[j];
            j--;
            if (j < 0) break;
        }

        line[j + 1] = temp;
    }
}

int count(int* line, int* uline, int* dline, int& len) {
    int counter=0;
    for (int i = 0; i < len - 1; i++) {
        if ((line[i] == line[i + 1]) || (line[i] + line[i + 1] == 10)) {
            counter++;
            i++;
        }
        else if ((line[i] == uline[i]) || (line[i] + uline[i] == 10)) {
            counter++;
            i++;
        }
        else if ((line[i] == dline[i]) || (line[i] + dline[i] == 10)) {
            counter++;
            i++;
        }
    }
    return counter;
}

int boom(int* line, int* uline, int* dline, int &len) {
    int scores=0;
    for (int i = 0; i < len - 1; i++) {
        
        if (line[i] == 0)
            continue;

        if ((line[i] == line[i + 1]) || (line[i] + line[i + 1] == 10)) {
            line[i] = 0;
            line[i + 1] = 0;
            scores++;
        }
        else if ((line[i] == uline[i]) || (line[i] + uline[i] == 10)) {
            line[i] = 0;
            uline[i] = 0;

            scores++;
        }
        else if ((line[i] == dline[i]) || (line[i] + dline[i] == 10)) {
            line[i] = 0;
            dline[i] = 0;
            scores++;
        }
    }
    for (int i = 0; i < len; i++)
        if (line[i] == 10)
            line [i] = 0;
    return scores;

}

int star(Game& g, int& x1, int& y1, int& x2, int& y2) {
    int* line, * upperline, * downline, len, s_pairs, n_pairs, score;
    bool vertical;

    if ((y2 > y1) || (y2 < y1)) { // same line
        line = new int[g.column];
        upperline = new int[g.column];
        downline = new int[g.column];

        for (int i = 0; i < g.column; i++) {
            line[i] = g.loc[x1][i];
            upperline[i] = g.loc[x1 - 1][i];
            downline[i] = g.loc[x1 + 1][i];
        }
        len = g.column;
        vertical = false;
    }
    else {                        // same column
        line = new int[g.row];
        upperline = new int[g.row];
        downline = new int[g.row];

        for (int i = 0; i < g.row; i++) {
            line[i] = g.loc[i][y1];
            upperline[i] = g.loc[i][y1 - 1];
            downline[i] = g.loc[i][y1 + 1];
        }
        len = g.row;
        vertical = true;
    }
    cout << len;
    sort_it_n(line, len);
    n_pairs = count(line, upperline, downline, len);

    sort_it_s(line, len);
    s_pairs = count(line, upperline, downline, len);

    if (n_pairs > s_pairs)
        sort_it_n(line, len);

    score = boom(line, upperline, downline, len);

    if (vertical)
        for (int i = 0; i < len; i++) {
            g.loc[i][y1] = line[i];
            g.loc[i][y1 - 1] = upperline[i];
            g.loc[i][y1 + 1] = downline[i];
        }
    else
        for (int i = 0; i < len; i++) {
            g.loc[x1][i] = line[i];
            g.loc[x1 - 1][i] = upperline[i];
            g.loc[x1 + 1][i] = downline[i];
        }

    delete[] line;
    delete[] upperline;
    delete[] downline;

    return score;
}

// USER

void new_user(Game &g,user u[],int &len) {
    char in_user[30], in_pass[30];
    bool match = false;
    int index;
    ofstream users("users.txt", ios::app); //appended mode
    
    if (!users.is_open())
        cout << "failed to open file !";
   
    while (true) {
        cout << "Enter username :" << endl;
        cin >> in_user;
        clear();
        index = binary_search(u, len, in_user);
        if (index < 0)
            break;
        cout << "this username is already used"<<endl;
    }
    users << in_user << " ";

    cout << "Enter password :" << endl;
    cin >> in_pass;
    users << in_pass << " " << 0;
    users << endl;
    strcpy_s(g.user, strlen(in_user)+1, in_user);

    users.close();
}

void load_score(Game& g, user u[],int len) {
    int index;
    sort_by_score(u, len);

    index = binary_search(u, len, g.user);
    g.score = u[index].score;
}

int read_it(user u[]) {
    ifstream savefile;
    savefile.open("users.txt");
    int i = 0;
    
    while (savefile >> u[i].user) {
        savefile >> u[i].pass;
        savefile >> u[i].score;
        i++;
    }
    return i;
}

void write_it(user u[],int &len) {
    ofstream savefile("users.txt");
    
    for (int i = 0; i < len; i++)
        savefile << u[i].user << " " << u[i].pass << " " << u[i].score<<endl;

    savefile.close();
}

void save_it(user u[],int &len,Game &g) {
    int index;

    sort_by_username(u, len);
    index = binary_search(u, len, g.user);

    if (index < 0)
        cout << "failed to save data !";
    else u[index].score = g.score;

    sort_by_score(u, len);

}

void show_scorboard(user u[],int &len) {
    char trash;
    for (int i = 0; i < len; i++)
        cout << i+1 << " . " << u[i].user << " : " << u[i].score << endl;
}

bool menu(Game &g, user u[],int &len) {
    char input;

    while (true) {

        cout << endl << endl << "    ";
        cout << "1 . Start Game " << endl << "    ";
        cout << "2 . Score Board " << endl << "    ";
        cout << "3 . Exit " << endl << endl;


        input = _getch();

        switch (input) {

        case '1': {
            return true;
            break;
        }
        case '2': {
            clear();
            show_scorboard(u, len);
            cout << endl << endl << "    press any key to continue";
            input = _getch();
            clear();
            break;
        }
        case '3': {
            clear();
            cout << "Goodbye :) " << endl;
            return false;
            break;
        }
        default:
            cout << '\a';
        }
    }
}

// GAMEPLAY

void move(Game& g, int& x, int& y, int& chosen_x, int& chosen_y, char& in) {

    int temp;
    switch (in) {

    case 75:

        temp = y;
        y--;

        if (y < 0) {
            cout << '\a';
            y = temp;
            break;
        }

        break;


    case 77:

        temp = y;
        y++;

        if (y >= g.row) {
            cout << '\a';
            y = temp;
            break;
        }

        break;

    case 72:

        temp = x;
        x--;

        if (x < 0) {
            cout << '\a';
            x = temp;
            break;
        }

        break;

    case 80:

        temp = x;
        x++;

        if (x >= g.column) {
            cout << '\a';
            x = temp;
            break;
        }

        break;

    case 32:
        chosen_x = x;
        chosen_y = y;

        break;
    }
}

void choose_two(Game& g, int& x1, int& y1, int& x2, int& y2, int& tx, int& ty) {
    char input;
    x1 = x2 = -1;
    tx = ty = y1 = y2 = 0;

    while (x1 < 0) {
        show(g, x1, x2, y1, y2, tx, ty);
        input = _getch();
        move(g, tx, ty, x1, y1, input);
    }
    while (x2 < 0) {
        show(g, x1, x2, y1, y2, tx, ty);
        input = _getch();
        move(g, tx, ty, x2, y2, input);
    }
    show(g, x1, x2, y1, y2, tx, ty);
}

void start_it(Game& g) {

    bool done = false;
    srand(time(0));

    while (!done) {

        g.loc = new int* [g.row];
        for (int i = 0; i < g.row; i++)
            g.loc[i] = new int[g.column];

        for (int i = 0; i < g.row; i++)
            for (int j = 0; j < g.column; j++)
                g.loc[i][j] = rand() % 9 + 1;

        g.loc[rand() % (g.row - 2) + 1][rand() % (g.column - 2) + 1] = 10;

        if (check_all(g) >= (g.row * g.column / 8))
            done = true;
    }
}

void play_it(Game& g, user u[], int& len) {
    int x1, x2, y1, y2, tx, ty;

    while (check_all(g)) {

        choose_two(g, x1, y1, x2, y2, tx, ty);

        if (accept(g, x1, y1, x2, y2)) {

            if (g.loc[x1][y1] == 10)
                g.score += star(g, x1, y1, x2, y2);
            else
                if (check_it(g.loc[x1][y1], g.loc[x2][y2])) {
                    g.loc[x1][y1] = g.loc[x2][y2] = 0;
                    g.score++;
                }
        }
        show(g, x1, x2, y1, y2, tx, ty);
        save_it(u, len, g);
        write_it(u, len);
    }
    g.score += 10;

}


