#include <bits/stdc++.h>
#include <thread>

#ifdef _WIN32
#define WINDOWS_SYSTEM
#define CLEAR "cls"
#define SLEEP_FUNC Sleep
#include <windows.h>
#else
#include <unistd.h>
#define CLEAR "clear"
#define SLEEP_FUNC usleep
#endif
#define SHOW_COL_N_ROW_NUMBERS false

using namespace std;

// Structure of a coordinate
typedef struct coordinate{
    int i;
    int j;
} Coordinate;

typedef struct field{
    int *matrix;
    int nRows;
    int nColumns;
} Field;

Field createField(int nRows, int nColumns){
    Field field;
    field.nRows = nRows;
    field.nColumns = nColumns;
    field.matrix = (int *) malloc((nRows*nColumns) * sizeof(int));

    for(int i=0; i<nRows*nColumns; i++){
        field.matrix[i] = 0;
    }

    return field;
}

void showField(Field field, int currGen){
    system(CLEAR);
    printf(" %d   ", currGen);
    int counter = 0;
    for(int i=0; i<field.nColumns; i++){
        if(!SHOW_COL_N_ROW_NUMBERS)
          break;
        if(counter == 0)
            cout << "| ";
        else
            cout << counter << " ";
        counter ++;
        if(counter > 9)
            counter = 0;
    }
    cout << "\n\n";
    for(int i=0; i<field.nRows; i++){
        if(SHOW_COL_N_ROW_NUMBERS) {
          if(i<9) {
            cout << "0";
          }
          cout << i+1 << " - ";
        }
        for(int j=0; j<field.nColumns; j++){
            int posOnArray = j * field.nRows + i;
            if(field.matrix[posOnArray] == 0)
                cout << "  ";
            else
                cout << "◼ ";
        }
        cout << "\n";
    }
}

void destroyField(Field field){
    free(field.matrix);
}

Coordinate createCoordinate(int i, int j){
    Coordinate coord;
    coord.i = i;
    coord.j = j;
    return coord;
}

void createRepeater(Coordinate coord, Field *field){
    // border correction
    if(coord.i == 0)
        coord.i++;
    if(coord.i > field->nRows-2)
        coord.i = field->nRows-2;
    
    // bounds validation
    if(coord.i > field->nRows - 1 || coord.i < 0 || coord.j < 0 || coord.j > field->nColumns -1)
        return;

    // Creating repeater on field matrix
    // repeater top
    int posOnArray = coord.j * field->nRows + (coord.i-1);
    field->matrix[posOnArray] = 1;

    // repeater mid
    posOnArray = coord.j * field->nRows + coord.i;
    field->matrix[posOnArray] = 1;
    
    // repeater bottom
    posOnArray = coord.j * field->nRows + (coord.i+1);
    field->matrix[posOnArray] = 1;
}


void createGlider(Coordinate coord, Field *field){
    int pos = coord.j * field->nRows + coord.i;
    field->matrix[pos] = 1;

    pos = (coord.j+1) * field->nRows + (coord.i+1);
    field->matrix[pos] = 1;
    
    pos = (coord.j+1) * field->nRows + (coord.i+2);
    field->matrix[pos] = 1;

    pos = (coord.j) * field->nRows + (coord.i+2);
    field->matrix[pos] = 1;
    
    pos = (coord.j-1) * field->nRows + (coord.i+2);
    field->matrix[pos] = 1;
}

void createT(Coordinate coord, Field *field){
    int pos = coord.j * field->nRows + coord.i;
    field->matrix[pos] = 1;

    pos = coord.j * field->nRows + (coord.i+1);
    field->matrix[pos] = 1;

    pos = (coord.j-1) * field->nRows + (coord.i+1);
    field->matrix[pos] = 1;

    pos = (coord.j+1) * field->nRows + (coord.i+1);
    field->matrix[pos] = 1;
}

void createBlockNxN(int x, int y,int n, Field *field){
    int pos = 0;
    for(int i=x; i<x+n; i++){
        for(int j=y; j<y+n; j++){
            pos = j * field->nRows + i;
            field->matrix[pos] = 1;
        }
    }
}
int countNeighbors(int i, int j, Field *field){
    int neighbors = 0;
    // Cardeals neighbors
    //  Norte
    if(i > 0){
        int pos = j * field->nRows + (i-1);
        if(field->matrix[pos] == 1)
            neighbors++;
    }

    //  Sul
    if(i<field->nRows-1){
        int pos = j * field->nRows + (i+1);
        if(field->matrix[pos] == 1)
            neighbors++;
    }
    //  Leste
    if(i<field->nColumns-1){
        int pos = (j+1) * field->nRows + i;
        if(field->matrix[pos] == 1)
            neighbors++;
    }
    //  Oeste
    if(i>0){
        int pos = (j-1) * field->nRows + i;
        if(field->matrix[pos] == 1)
            neighbors++;
    }

    // Collateral neighbors
    //  Nordeste
    if(i>0 && j<(field->nColumns-1)){
        int pos = (j+1) * field->nRows + (i-1);
        if(field->matrix[pos] == 1)
            neighbors++;
    }
    //  Noroeste
    if(i>0 && j>0){
        int pos = (j-1) * field->nRows + (i-1);
        if(field->matrix[pos] == 1)
            neighbors++;
    }
    //  Sudeste
    if(i<(field->nRows-1) && j<(field->nColumns-1)){
        int pos = (j+1) * field->nRows + (i+1);
        if(field->matrix[pos] == 1)
            neighbors++;
    }
    //  Sudoeste
    if(i<(field->nRows-1) && j>0){
        int pos = (j-1) * field->nRows + (i+1);
        if(field->matrix[pos] == 1)
            neighbors++;
    }
    return neighbors;
}


void updateLife(Field *field){
    Field mirrorField = createField(field->nRows, field->nColumns);

    for(int i=0; i<field->nRows; i++){
        for(int j=0; j<field->nColumns; j++){
            int neighborsAmount = countNeighbors(i, j, field);
            int posOnArray = j * field->nRows + i;

            if(field->matrix[posOnArray] == 1)
                if(neighborsAmount == 2 || neighborsAmount == 3)
                    mirrorField.matrix[posOnArray] = 1;

            if(field->matrix[posOnArray] == 0 && neighborsAmount == 3)
                mirrorField.matrix[posOnArray] = 1;
            
        }
    }

    free(field->matrix);
    field->matrix = mirrorField.matrix;
}

int main(){
    // Field field = createField(90, 168);
    Field field = createField(140, 231);
    Coordinate coord = createCoordinate(40,40);
    // createRepeater(coord, &field);

    coord = createCoordinate(50,90);
    // createGlider(coord, &field);

    // coord = createCoordinate(20,40);
    // createGlider(coord, &field);      

    // coord = createCoordinate(30,20);
    // createGlider(coord, &field);

    // coord = createCoordinate(50,60);
    // createT(coord, &field);

    // coord = createCoordinate(50,65);
    // createT(coord, &field);

    // coord = createCoordinate(50,75);
    // createT(coord, &field);

    // coord = createCoordinate(40,85);
    // createT(coord, &field);

    createBlockNxN(30, 60, 39, &field);
    createBlockNxN(30, 120, 39, &field);
    int currentGeneration = 0;
    do{
        showField(field, currentGeneration);
        updateLife(&field);
        currentGeneration++;
        SLEEP_FUNC(100000);
    } while(true);
    destroyField(field);

    return 0;
}
