#ifndef KNIGHT_BOARD_H
#define KNIGHT_BOARD_H

#include<iostream>
#include<deque>
#include<algorithm>

#define N 20


// valid move pattern
static int move_x[8] = {2,1,-1,-2,-2,-1,1,2};
static int move_y[8] = {1,2,2,1,-1,-2,-2,-1};

//checking if next moves satisfies some conditions
//temp function allow different predicate
template<typename PRED>
bool _testmove(const int& x, const int&y, PRED pred){
   int sx,sy;

   for (size_t i = 0; i < 8; ++i) {
      sx = x + move_x[i];
      sy = y + move_y[i];

      if (pred(sx, sy))
         return true;
   }
   return false;
}
// pure virtual base class for chess board
// use array to represent the chess board
// cannot be instanstiated
class board_base{


   //default constructor
   board_base( size_t n, size_t m);

   //copy constructor disable
   board_base(const board_base& that) = delete;



   //destructor
   virtual ~board_base();


   // print out the steps in correct way
   inline void printboard() {
      for (size_t i = 0; i < nRow; i++) {
         for (size_t j = 0; j < nCol; j++) {
             printf("%d\t", board[i*nCol+j]);
         }
         printf("\n");
      }
   }

   //pure virtual function
   //need to implement in derived class
   //key find the tour
   virtual void Tour(const int& x, const int& y) = 0;

protected:
   //dynamic array store the move
   int* board;
   // board size, move count, previoius move
   size_t nRow,nCol,count,last_x,last_y;

      //function for resize the chessboard 
   //user change
   inline void resize(size_t n, size_t m){
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



   //function for checking if it has a walk of enough length
   inline bool enough_move(){return (count == nRow * nCol + 1);}

   // check if the next move (as per knight's constraints) is possible 
   inline bool isMovePossible(const int& i, const int& j) {
      if ((i >= 0 && i < nRow) && (j >= 0 && j < nCol) && (board[i*nCol+j] == 0))
         return true;

      return false;
   }

   //check if is a closed tour
   inline bool istour(const int& initx, const int& inity){

      auto fn=[&](const int& x, const int& y)->bool{
         return (x  == initx && y == inity) ;   
      };

      return _testmove(last_x, last_y, fn);

   }



};

// use lambda function to recalculate the move order
// use to perform normal depth first search
// exhaust all the possible state
// exponential complexity
// super inefficient
class dfs_board : public board_base {
public:
   dfs_board(size_t nn=6, size_t mm=6)
      :board_base(nn,mm){}

   // check the validity of the move
   bool move(const int& x, const int& y);

   bool update();

   void Tour(const int& initx, const int& inity);

      
private:
   //explore set
   std::deque<int> Xx_set,Xy_set;
   //generated set
   std::deque<int> Gx_set,Gy_set;
};


// implemented using warnsdorff's heuristics.
// ramdom select initial starting point to speed up search
// re-calculate order of solution at print time
// theory claim that it is run in linear time
// in practice, go super fast, handle huge chess board
class heuristic_board : public board_base {

public:
   //constructor
   heuristic_board(size_t nn=N, size_t mm=N)
      :board_base(nn,mm){}

   //implemented pure virtual function in children class
   //store the minimum heuristic move into next_move
   //compare all the possible moves
   //if no next_move exists (stuck into a corner), next_move is set to equal to last_move
   bool move(const int& x, const int& y);

   //try function to find a closed tour
   //using warsdorff heuristic
   //return true if such tour is found
   //otherwise clear the board and return false
   bool findTour();

   //wrapper function for find tour
   void Tour(const int& initx, const int& inity);


   
   
private:
   //private member to store next_move, last_move
   int next_x,next_y;

   // genric algorithm
   //lambda function used to calculate correct value
   //recalculate the move order
   inline void recal(const int& initx, const int& inity){
      int offset = board[initx * nCol + inity] - 1;
      // genric algorithm
      //lambda function used to calculate correct value
      auto fn = [&](int& value){
         if(value > offset)
            value = value - offset;
         else
            value = value + nRow * nCol - offset;
      };

      std::for_each(board, board + nCol*nRow, fn);
   }

   //  heuristic of an move
   inline size_t getDegree(const int& x, const int& y){
      
      int sx,sy;
      size_t num(0);

      for (size_t i = 0; i < 8; ++i) {
      // get the next move
         sx = x + move_x[i];
         sy = y + move_y[i];
         if (isMovePossible(sx,sy)) {
            ++num;
         }
      }

       return num;
   }

   //function for resetting value in each grid
   inline void reset(){
      delete [] board;
      board = nullptr;
      try{
            board = new int [nRow*nCol];
            std::fill_n(board, nRow*nCol, 0); 
         }
         catch(const std::exception& e){
            delete[] board;
            throw e;
         }
      count = 1;
   }

   //check if last_move is equal to next_move
   //(meaning that we are stuck at a corner) return false
   //otherwise, perform next_move
   inline bool update(){
         if(last_x == next_x ){
            this->reset();
            return false;
         }
         else 
            return move(next_x, next_y);
      }

};




#endif // KNIGHT_TOUR_H
