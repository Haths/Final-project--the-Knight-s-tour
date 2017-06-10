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

static  chess_moves move_KT[8] = { {2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1} };


class board_base{
public:
      //default constructor
   board_base( size_t n, size_t m)
      :nRow(n), nCol(m), count(1), board(std::vector<std::vector<size_t>>(n,std::vector<size_t>(m, 0))) {
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
      if(G_set.size()>0){
         chess_moves a = G_set.top();
         G_set.pop();
         while( a.x == X_set.top().x && a.y == X_set.top().y){
            X_set.pop();

            a = G_set.top();
            G_set.pop();
         }

         
         return a;
      }
      return {-1,-1};
   } 

   bool last_move(){
      return count==nRow*nCol;
   }

   bool empty_stack(){
      return G_set.empty();
   }

      // check if the next move (as per knight's constraints) is possible
   bool isMovePossible(const chess_moves& move) {
      int i = move.x;
      int j = move.y;
      if ((i >= 0 && i < nRow) && (j >= 0 && j < nCol) && (board[i][j] == 0))
         return true;
      return false;
   }


   virtual bool move(const chess_moves& move) = 0;

protected:
   std::vector<std::vector<size_t>> board;
   std::stack<chess_moves> X_set;
   std::stack<chess_moves> G_set;
   size_t nRow;
   size_t nCol;
   size_t count;
   
};


class record_board : public board_base {
public:
   record_board(size_t nn, size_t mm)
      :board_base(nn,mm){}

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
      :board_base(nn,mm){}
    
   

   virtual ~heuristic_board(){}

   size_t getDegree(const chess_moves& move){
      
      chess_moves next_move;
      size_t num(0);

      for (size_t i = 0; i < 8; ++i) {
      // get the next move
         next_move.x = move.x + move_KT[i].x;
         next_move.y = move.y + move_KT[i].y;
         if (isMovePossible(next_move)) {
            ++num;
         }
      }
    
       return num;
   }

   bool move(const chess_moves& move){
         // check the validity of the move
      if(!isMovePossible(move)){
         std::cout<<" move not possible";
         return false;
      }
     
      
      G_set.push(move);

      X_set.push(move);

      board[move.x][move.y]=count;
      ++count;
      
         //generate possible next move
      chess_moves next_move;
      std::list<chess_moves> temp;
      std::list<size_t> heuristic;
      for (size_t i = 0; i < 8; ++i) {
         // get the next move
         next_move.x = move.x + move_KT[i].x;
         next_move.y = move.y + move_KT[i].y;
   
         if (isMovePossible(next_move)) {
            temp.push_back(next_move);
            heuristic.push_back(getDegree(next_move));
         }
      }

      insert_move(temp, heuristic);

      return true;
   }

   

private:
      //generic algorithm to find the minimum heuristic move
      //store all possible moves in descending order of its degree
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
         G_set.push(*it1);
         temp.erase(it1);
         index.erase(it3);
         it1 = temp.begin();
         it2 = temp.begin();
         it3 = index.begin();
         it4 = index.begin();
      }
   }

};




template < typename chessboard >
bool findTour(const chess_moves& move, chessboard& board) {
   

   board.move(move);

   while (!board.empty_stack()) {
      if (board.last_move()) {
         board.printboard();
         break;
      }
      
      board.move(board.next_move());

   }
   
   return false;
}



// main
int main() {
   heuristic_board heuristic;
   
   
   findTour({0,4},heuristic);

   return 0;
}

