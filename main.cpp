#include<iostream>
#include<stack>
#include<vector>
#include<list>
#define N 8

using namespace std;

typedef struct chess_moves {
   // 'x' and 'y' coordinates on chess board
   int x,y;
}chess_moves;

static  chess_moves move_KT[8] = { {2,1},{1,2},{-1,2},{-2,1},
                              {-2,-1},{-1,-2},{1,-2},{2,-1} };


class board_base{
public:
      //default constructor
   board_base( size_t n, size_t m, size_t T)
      :nRow(n), nCol(m), count(0), board(std::vector<std::vector<size_t>>(n,std::vector<size_t>(m, T))) {
     /*    size_t n = nn;
         size_t m = mm;
         if( n < m ){
            n = mm;
            m = nn;
         }
         if ( ((m%2)==1 && (n%2)==1) || (m==1) || (m==2) || (m==4) || (m == 3 && (n == 4 || n == 6 || n == 8 )))
            std::cout << " Knight's Tour is not possible"
      }*/
      //destructor
      }
   virtual ~board_base(){}
      //copy constructor
   void printboard() {
   for (size_t i = 0; i < nRow; i++) {
      for (size_t j = 0; j < nCol; j++) {
          std::cout<<board[i][j]<<"\t";
      }
      std::cout<<endl;
      }
   }

   virtual bool move(const chess_moves& move) = 0;

protected:
   std::vector<std::vector<size_t>> board;

   

   size_t nRow;
   size_t nCol;
   size_t count;
   
};


class record_board : public board_base {
public:
   record_board(size_t nn, size_t mm, size_t T=0)
      :board_base(nn,mm,T){}

   bool move(const chess_moves& move){
      
      board[move.x][move.y] = count;
      ++count;
      return true;
   }

   

private:
   

};


class heuristic_board : public board_base {

public:

   heuristic_board(size_t nn=N, size_t mm=N)
      :board_base(nn,mm,8){

        for ( size_t i = 0; i < nn; ++i){
         for ( size_t j = 0; j < mm; ++j){
            if(i == 0 || i == nn-1) {
               if (j == 0 || j == mm-1)
                     board[i][j] = 2;
               else if (j == 1 || j== mm-2)
                     board[i][j] = 3;
               else   
                     board[i][j] = 4;
               }
            else if(i == 1 || i == nn-2) {
               if (j == 0 || j == mm-1)
                     board[i][j] = 3;
               else if (j == 1 || j==mm-2)       
                     board[i][j] = 4;
               else
                     board[i][j] = 6;

               }
            else{
               if (j == 0 || j==mm-1)
                     board[i][j] = 4;
               else if ( j == 1 || j == mm-2)   
                     board[i][j] = 6;
               
               }

            }
         }
        }
    
   
      

   virtual ~heuristic_board(){}

   bool move(const chess_moves& move){

      if(!isMovePossible(move)){
         std::cout<<" move not possible";
         return false;
      }
     
      board[move.x][move.y] = 0;
      ++count;

      chess_moves next_move;
      std::list<chess_moves> temp;
      std::list<size_t> heuristic;
      for (size_t i = 0; i < 8; ++i) {
      // get the next move
         next_move.x = move.x + move_KT[i].x;
         next_move.y = move.y + move_KT[i].y;
   
         if (isMovePossible(next_move)) {
            temp.push_back(next_move);
            board[next_move.x][next_move.y] = board[next_move.x][next_move.y] + 1;
            heuristic.push_back(board[next_move.x][next_move.y]);
         }
      }
      std::list<chess_moves>::iterator it1 = temp.begin();
      std::list<chess_moves>::iterator it2 = temp.begin();
      std::list<size_t>::iterator it3 = heuristic.begin();
      std::list<size_t>::iterator it4 = heuristic.begin();
      while(!temp.empty()){
         auto fn = [&](size_t& T)->void{
            if(T > *it3){
               it1 = it2;
               it3 = it4;
            }
            ++it2;
            ++it4;
         };
         std::for_each(heuristic.begin(),heuristic.end(),fn);
         depth.push(*it1);
         temp.erase(it1);
         heuristic.erase(it3);
         it1 = temp.begin();
         it2 = temp.begin();
         it3 = heuristic.begin();
         it4 = heuristic.begin();
      }

      return true;
   }

   void back_track( const chess_moves& move){


      chess_moves next_move;
      size_t num(0);

      for (size_t i = 0; i < 8; ++i) {
      // get the next move
         next_move.x = move.x + move_KT[i].x;
         next_move.y = move.y + move_KT[i].y;
         
         if (isMovePossible(next_move)) {
            
            board[next_move.x][next_move.y] = board[next_move.x][next_move.y] + 1;
            ++num;
         }
      }
      board[move.x][move.y] = num;
      --count;

   }
      // check if the next move (as per knight's constraints) is possible
   bool isMovePossible(const chess_moves& move) {
      int i = move.x;
      int j = move.y;
      if ((i >= 0 && i < nRow) && (j >= 0 && j < nCol) && (board[i][j] != 0 && count < nCol*nRow))
         return true;
      return false;
   }

private:
   std::stack<chess_moves> depth;

};


// check if the next move (as per knight's constraints) is possible
bool isMovePossible(chess_moves next_move, int tour[N][N]) {
   int i = next_move.x;
   int j = next_move.y;
   if ((i >= 0 && i < N) && (j >= 0 && j < N) && (tour[i][j] == 0))
      return true;
   return false;
}


// recursive function to find a knight tour
bool findTour(int tour[N][N], chess_moves move_KT[],
               chess_moves curr_move, int move_count) {
   int i;
   chess_moves next_move;
   if (move_count == N*N-1) {
      // Knight tour is completed i.e all cells on the
      // chess board has been visited by knight once 
      return true;
   }

   // try out the possible moves starting from the current coordinate
   for (i = 0; i < N; i++) {
      // get the next move
      next_move.x = curr_move.x + move_KT[i].x;
      next_move.y = curr_move.y + move_KT[i].y;

      if (isMovePossible(next_move, tour)) {
         // if the move is possible
         // increment the move count and store it in tour matrix
         tour[next_move.x][next_move.y] = move_count+1;
         if (findTour(tour, move_KT, next_move, move_count+1) == true) {
            return true;
         }
         else {
            // this move was invalid, try out other possiblities 
            tour[next_move.x][next_move.y] = 0;
         }
      }
   }
   return false;
}

// wrapper function
void knightTour() {
   int tour[N][N];
   int i,j;

   // initialize tour matrix
   for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
         tour[i][j] = 0;
      }
   }

   // all possible moves that knight can take
   chess_moves move_KT[8] = { {2,1},{1,2},{-1,2},{-2,1},
                              {-2,-1},{-1,-2},{1,-2},{2,-1} };

   // knight tour starts from coordinate (0,0)
   chess_moves curr_move = {0,0};

   // find a possible knight tour using a recursive function
   // starting from current move 
   if(findTour(tour, move_KT, curr_move, 0) == false) {
      cout<<"\nKnight tour does not exist";
   }
   else {
      cout<<"\nTour exist ...\n";
      printTour(tour);
   }
}

// main
int main() {
   heuristic_board heuristic;
   heuristic.printboard();
   return 0;
}

