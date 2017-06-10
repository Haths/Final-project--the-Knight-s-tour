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

   chess_moves next_move(){
      if(depth.size()>0)
         return depth.top();
      return {-1,-1};
   } 

   bool last_move(){
      return count==nRow*nCol-1;
   }

   bool empty_stack(){
      return depth.empty();
   }
   virtual bool move(const chess_moves& move) = 0;

protected:
   std::vector<std::vector<size_t>> board;
   std::stack<chess_moves> depth;
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
      :board_base(nn,mm,0),heuristic(std::vector<std::vector<size_t>>(n,std::vector<size_t>(m, 8))){

        for ( size_t i = 0; i < nn; ++i){
         for ( size_t j = 0; j < mm; ++j){
            if(i == 0 || i == nn-1) {
               if (j == 0 || j == mm-1)
                     heuristic[i][j] = 2;
               else if (j == 1 || j== mm-2)
                     heuristic[i][j] = 3;
               else   
                     heuristic[i][j] = 4;
               }
            else if(i == 1 || i == nn-2) {
               if (j == 0 || j == mm-1)
                     heuristic[i][j] = 3;
               else if (j == 1 || j==mm-2)       
                     heuristic[i][j] = 4;
               else
                     heuristic[i][j] = 6;

               }
            else{
               if (j == 0 || j==mm-1)
                     heuristic[i][j] = 4;
               else if ( j == 1 || j == mm-2)   
                     heuristic[i][j] = 6;
               
               }

            }
         }
        }
    
   

   virtual ~heuristic_board(){}

   bool move(const chess_moves& move){
         // check the validity of the move
      if(!isMovePossible(move)){
         std::cout<<" move not possible";
         return false;
      }
     
      explore_set.push_back(move);
      
      board[move.x][move.y]=count;
      ++count;
      
         //generate possible next move
      chess_moves next_move;
      std::list<chess_moves> temp;
      std::list<size_t> index;
      for (size_t i = 0; i < 8; ++i) {
         // get the next move
         next_move.x = move.x + move_KT[i].x;
         next_move.y = move.y + move_KT[i].y;
   
         if (isMovePossible(next_move)) {
            temp.push_back(next_move);
            heuristic[next_move.x][next_move.y] = heuristic[next_move.x][next_move.y] - 1;
            index.push_back(heuristic[next_move.x][next_move.y]);
         }
      }

      if( index.empty())

         
      insert_move(temp, index);

      return true;
   }

   void back_track( const chess_moves& move){


      chess_moves next_move;
      

      for (size_t i = 0; i < 8; ++i) {
      // get the next move
         next_move.x = move.x + move_KT[i].x;
         next_move.y = move.y + move_KT[i].y;
         
         if (isMovePossible(next_move)) {
            heuristic[next_move.x][next_move.y] = heuristic[next_move.x][next_move.y] + 1;
         }
      }
      
      --count;
      board[move.x][move.y]=0;

   }
      // check if the next move (as per knight's constraints) is possible
   bool isMovePossible(const chess_moves& move) {
      int i = move.x;
      int j = move.y;
      if ((i >= 0 && i < nRow) && (j >= 0 && j < nCol) && (board[i][j] == 0))
         return true;
      return false;
   }

private:
      // insert possible move in depth
   void insert_move(std::list<chess_moves> temp, std::list<size_t> index){
      std::list<chess_moves>::iterator it1 = temp.begin();
      std::list<chess_moves>::iterator it2 = temp.begin();
      std::list<size_t>::iterator it3 = index.begin();
      std::list<size_t>::iterator it4 = index.begin();

      while(!temp.empty()){
         auto fn = [&](size_t& T)->void{
            if(T > *it3){
               it1 = it2;
               it3 = it4;
            }
            ++it2;
            ++it4;
         };
         std::for_each(index.begin(),index.end(),fn);
         depth.push(*it1);
         temp.erase(it1);
         index.erase(it3);
         it1 = temp.begin();
         it2 = temp.begin();
         it3 = index.begin();
         it4 = index.begin();
      }
   }
   std::vector<std::vector<size_t>> heuristic;
   std::vector<chess_moves> explore_set;
};




template < typename chessboard >
bool findTour(chess_moves move, chessboard board) {
   int i;
   chess_moves next_move;
   if (board.last_move()) {
      return true;
   }

   board.move(move);

   while (!board.empty_stack()) {
      if (board.last_move()) {
         board.printboard()
         break;
      }
      
      board.move(board.next_move());

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

