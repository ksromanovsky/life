#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
using namespace std;

class point
{
private:
    bool alive;
public:
    int count_neghbours;
    point () {
        this->alive = false;
        this->count_neghbours = 0;
    }

    ~point (){}

    bool get_alive () {
        return alive;
    }

    void set_alive(bool a) {
        this->alive = a;
    }
};

class canvas
{
private:
    int rows, cols;
    point **work_area, **temp_area;
public:
    canvas(int rows, int cols) : rows(rows), cols(cols) {
        this->work_area = new point *[rows];
        this->temp_area = new point *[rows];
        for (int i = 0; i < rows; i++) {
                *(this->work_area + i) = new point [cols];
                *(this->temp_area + i) = new point [cols];
        }
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                if (0 + rand() % (100 - 0 + 1) > 50) {
                    (*(this->work_area + i) + j)->set_alive(true);
                } else {
                    (*(this->work_area + i) + j)->set_alive(false);
                }
            }
        }
    }

    ~canvas(){
        for (int i = 0; i < this->rows; i++) {
                delete [] *(this->work_area + i);
                delete [] *(this->temp_area + i);
        }
        delete [] this->work_area;
        delete [] this->temp_area;
    }

    int find_neghbors(int x, int y) {
        (*(this->work_area + x) + y)->count_neghbours = 0;
        int x1, x2, y1, y2;
        if ( x - 1 < 0 ) x1 = x; else x1 = x - 1;
        if ( x + 1 > this->rows - 1 ) x2 = x; else x2 = x + 1;
        if ( y - 1 < 0 ) y1 = y; else y1 = y - 1;
        if ( y + 1 > this->cols - 1 ) y2 = y; else y2 = y + 1;

        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) {
                if (i == x && j == y) {
                    continue;
                }
                else if ((*(this->work_area + i) + j)->get_alive())
                {
                    (*(this->work_area + x) + y)->count_neghbours++;
                }
            }
        }
        return (*(this->work_area + x) + y)->count_neghbours;
    }

    void show () {
        printf("\033c");
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                if ((*(this->work_area + i) + j)->get_alive()) {
                    // printf("\033[42m ");
                    cout << "$";
                } else {
                    // printf("\033[0m ");
                    cout << "-";
                }
            }
            cout << endl;
        }
    }

    void next_epoch () {
        int n;
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                n = this->find_neghbors(i, j);
                if( !(*(this->work_area + i) + j)->get_alive() ) {
                    if ( n == 3 ) {
                        (*(this->temp_area + i) + j)->set_alive(true);
                    } else {
                        (*(this->temp_area + i) + j)->set_alive((*(this->work_area + i) + j)->get_alive());
                    }
                } else {
                    if ( n > 3 || n < 2) {
                        (*(this->temp_area + i) + j)->set_alive(false);
                    } else {
                        (*(this->temp_area + i) + j)->set_alive((*(this->work_area + i) + j)->get_alive());
                    }
                }
            }
        }
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                *(*(this->work_area + i) + j) = *(*(this->temp_area + i) + j);
            }
        }
    }
};

int main () {
    srand(time(NULL));
    canvas c(10, 10);
    for (int i = 0; i < 100; i++) {
        c.show();
        c.next_epoch();
        sleep(1);
    };
    return 0;
}