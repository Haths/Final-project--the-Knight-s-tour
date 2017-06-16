#include "knight_board.h"

//default constructor
board_base::board_base( int n, int m)
      :nRow(n), nCol(m), count(1), board(nullptr) {
         try{
            board = new int [n*m];
            //generic
            std::fill_n(board, n*m, 0); 
         }
         catch(const std::exception& e){
            delete[] board;
            throw e;
         }
      }

   //destructor
board_base::~board_base(){
      delete[] board;
      board = nullptr;
   }
   
void board_base::resize(int n, int m){
      delete [] board;
      board = nullptr;
      try{
            board = new int [n*m];
            std::fill_n(board, n*m, 0); 
         }
         catch(const std::exception& e){
            delete[] board;
            throw e;
         }
      count = 1;
      nCol = m;
      nRow = n;
   }

   // check the validity of the move
bool dfs_board::move(const int& x, const int& y){
      if(!isMovePossible(x,y))
         return false;

      last_x = x;
      last_y = y;
     
      Gx_set.push_back(x);
      Gy_set.push_back(y);

      Xx_set.push_back(x);
      Xy_set.push_back(y);

      board[x*nCol + y] = count;
      ++count;

         //generate possible next move
      int sx,sy;
      
      for (int i = 0; i < 8; ++i){
         // get the next move
         sx = x + move_x[i];
         sy = y + move_y[i];
   
         if (isMovePossible(sx,sy)){
            Gx_set.push_back(sx);
            Gy_set.push_back(sy);
         }
      }
      return true;
      }

bool dfs_board::update(){
      if(Gx_set.empty())
         return false;

      int x = Gx_set.back(), y = Gy_set.back();
      Gx_set.pop_back();Gy_set.pop_back();
      while( x == Xx_set.back() && y == Xy_set.back()){
         Xx_set.pop_back();Xy_set.pop_back();
         board[x*nCol+y] = 0;
         --count;
         x = Gx_set.back(); y = Gy_set.back();
         Gx_set.pop_back(); Gy_set.pop_back();
         if(Gx_set.empty())
            return false;
         }

      move(x,y);

      return true;
      } 

void dfs_board::Tour(const int& initx, const int& inity) {
      
      // Current points are same as initial points
      move(initx, inity);
   
      // Keep picking next points 
      while (!enough_move() || !istour(initx,inity))
          if (!update()){
              std::cout<<"\nBy examinating all possible walks, we claim that not such tour exist.";
              return;
          }
   
   }

   //implemented pure virtual function in children class
   //store the minimum heuristic move into next_move
   //compare all the possible moves
   //if no next_move exists (stuck into a corner), next_move is set to equal to last_move
bool heuristic_board::move(const int& x, const int& y){
      // check the validity of the move
      if(!isMovePossible(x,y))
         return false;
     
      last_x = x;
      last_y = y;

      board[x * nCol + y]=count;
      ++count;

      //generate possible next move
      int sx,sy;
      
      int min_deg = 10;
      int temp;
      for (int i = 0; i < 8; ++i) {
         // get the next move
         sx = last_x + move_x[i];
         sy = last_y + move_y[i];
         temp = getDegree(sx,sy);
         if (isMovePossible(sx,sy) && min_deg > temp) {
            next_x = sx;
            next_y = sy;
            min_deg = temp;
         }
      }

      return true;
   }
   //try function to find a closed tour
   //using warsdorff heuristic
   //return true if such tour is found
   //otherwise clear the board and return false
bool heuristic_board::findTour() {
      
      // Randome initial position
      int x = rand()%nRow;
      int y = rand()%nCol;
      
      // Current points are same as initial points
      move(x,y);
   
      // Keep picking next points using
      // Warnsdorff's heuristic
      while (!enough_move())
         //tour stuck into a corner?
         if (!update()){
              return false;
          }
      //tour is not closed?
      if(!istour(x,y)){
         this->reset();
         return false;
      }
   
      
      return true;
   
   }

   //wrapper function for find tour
void heuristic_board::Tour(const int& initx, const int& inity){
      srand(time(NULL));
      while (!findTour()){;}
   
      this->recal(initx,inity);
   }



