#include <bits/stdc++.h>

void drawField(int *field, int fieldOrder) {
  int positionOnArray = 0; 
  std::string outChar = ". ";
  for(int i = 0; i < fieldOrder; i++){
    for(int j =0; j < fieldOrder; j++) {

      positionOnArray = j * fieldOrder + i;
      outChar = (field[positionOnArray] == 0) ? ". " : "@ ";
      std::cout <<  outChar;
    }
    std::cout << "\n";
  }
}

void putCellOnField(int *field, int fieldOrder, int coordX, int coordY) {
  int positionOnArray = coordY * fieldOrder + coordX;
  field[positionOnArray] = (field[positionOnArray] == 0) ? 1 : 0;
}

int main() {
  int fieldOrder = 10;
  int fieldSize = fieldOrder * fieldOrder;
  int* field = (int*) malloc(fieldSize * sizeof(int)); 
  
  int coordX;
  int coordY;

  while(true){
    drawField(field, fieldOrder);
    std::cout << "Type coordinates x f from 0 to 9 to insert cell" << std::endl;
    std::cout << "coord(x y): ";
    std::cin >> coordX;
    std::cin >> coordY;
    if(coordX >= 0 && coordX < 10 && coordY >= 0 && coordY < 10) {
      putCellOnField(field, fieldOrder, coordX, coordY);
    }  
    system("clear");
  }
  
  free(field);
  return 0;
}
