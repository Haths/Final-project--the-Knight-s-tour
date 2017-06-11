#include<iostream>
#include<stack>
#include<deque>
#include<vector>
#include<list>
#define N 20
#define M 20

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
      :nRow(n), nCol(m), count(1), board(std::vector<int>(n*m, 0)) {}
      //destructor
   virtual ~board_base(){}

      
   virtual void resizeboard(size_t n, size_t m){
      board=std::vector<int>(n*m, 0);
      count = 0;
      nCol = m;
      nRow = n;
   }
   
   void printboard(const chess_moves& init) {
      int offset = board[init.x * nCol + init.y] - 1;
      auto fn = [&](int& value){
         if(value > offset)
            value = value - offset;
         else
            value = value + nRow * nCol - offset;
      };
      std::for_each(board.begin(), board.end(), fn);
      for (size_t i = 0; i < nRow; i++) {
         for (size_t j = 0; j < nCol; j++) {
             std::cout<<board[i*nCol+j]<<"\t";
         }
         std::cout<<endl;
         }
   }
   bool enough_move(){return (count == nRow * nCol + 1);}

   

   // check if the next move (as per knight's constraints) is possible 
   bool isMovePossible(const chess_moves& move) {
      int i = move.x;
      int j = move.y;
      if ((i >= 0 && i < nRow) && (j >= 0 && j < nCol) && (board[i*nCol+j] == 0))
         return true;
      return false;
   }


   virtual bool move(const chess_moves& move) = 0;

protected:
   std::vector<int> board;
   size_t nRow;
   size_t nCol;
   size_t count;
   
};


class search_board : public board_base {
public:
   search_board(size_t nn=N, size_t mm=N)
      :board_base(nn,mm){}

// check the validity of the move
   bool move(const chess_moves& move){
      if(!isMovePossible(move)){
         std::cout<<" move not possible";
         return false;
      }
     
      
      G_set.push_back(move);

      X_set.push_back(move);

      board[move.x*nCol +move.y]=count;
      ++count;
      
         //generate possible next move
      chess_moves next_move;
      
      for (size_t i = 0; i < 8; ++i) {
         // get the next move
         next_move.x = move.x + move_KT[i].x;
         next_move.y = move.y + move_KT[i].y;
   
         if (isMovePossible(next_move)) {
            G_set.push_back(next_move);
            
         }
      }
      return true;
  } 

  chess_moves next_move(){
      if(G_set.size()>0){
         chess_moves a = G_set.back();
         G_set.pop_back();
         while( a.x == X_set.back().x && a.y == X_set.back().y){
            X_set.pop_back();
            board[a.x*nCol+a.y] = 0;
            --count;
            a = G_set.back();
            G_set.pop_back();
         }

         
         return a;
      }
      return {-1,-1};// G_set is empty
   } 

   bool istour(const chess_moves& init){
      chess_moves next_move;
      for (size_t i = 0; i < 8; ++i) {
      // get the next move
         next_move.x = X_set.back().x + move_KT[i].x;
         next_move.y = X_set.back().y + move_KT[i].y;
         if (next_move.x == init.x && next_move.y == init.y) {
            return true;
         }
      }
      return false;
   }

   

   bool empty_stack(){
      return G_set.empty();
   }

      

private:
   std::deque<chess_moves> X_set;
   std::deque<chess_moves> G_set;
   

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
         return false;
      }
     
      last_move = move;
      board[move.x * nCol + move.y]=count;
      ++count;
      
         //generate possible next move
      chess_moves next_move;
      chess_moves temp = last_move;
      size_t min_deg =10;
      for (size_t i = 0; i < 8; ++i) {
         // get the next move
         next_move.x = move.x + move_KT[i].x;
         next_move.y = move.y + move_KT[i].y;
   
         if (isMovePossible(next_move) && min_deg > getDegree(next_move)) {
            temp=next_move;
            min_deg = getDegree(next_move);
         }
      }
   
     next_step = temp;
     return true;
      
   }


   bool move(){
      if(last_move.x == next_step.x )
         return false;
      else 
         return move(next_step);
   }

   bool istour(const chess_moves& init){
      for (size_t i = 0; i < 8; ++i) 
         if (last_move.x + move_KT[i].x == init.x && last_move.y + move_KT[i].y == init.y) 
            return true;

      return false;
   }

private:
      //generic algorithm to find the minimum heuristic move
      //store all possible moves in descending order of its degree
      // insert possible move in depth

   chess_moves next_step;
   chess_moves last_move;
};




template < typename chessboard >
bool findTour(const chess_moves& init, chessboard board) {

    // Randome initial position
   int sx = rand()%N;
   int sy = rand()%N;
   
    // Current points are same as initial points
    board.move({sx,sy});

 
    // Keep picking next points using
    // Warnsdorff's heuristic
    while (!board.enough_move())
        if (!board.move())
            return false;

   if(!board.istour({sx,sy}))
      return false;

   board.printboard(init);
   return true;

}



// main
int main() {
/*
   std::cout<<"Enter initial (x,y) coordinates \n"<<"x: ";
   int x(-1),y(-1);
   while (!(cin >> x) || x < 0 || x > N) {
         cout << "Bad value!\n";
         std::cout<<"\n"<<"x: ";
         cin.clear();
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
      }
   std::cout<<"\ny: ";
   while (!(cin >> y) || y < 0 || y > M) {
      cout << "Bad value!\n";
      std::cout<<"\n"<<"y: ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }

   std::cout<<"Enter Row Number \n"<<"N: ";
   int nn(N),mm(M);
   while (!(cin >> nn) || nn < 0 ) {
         cout << "Bad value!\n";
         std::cout<<"\n"<<"N: ";
         cin.clear();
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
      }
   std::cout<<"Enter Column Number \n"<<"M: ";
   while (!(cin >> mm) || mm< 0 ) {
      cout << "Bad value!\n";
      std::cout<<"\n"<<"M: ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }

   size_t n = nn;
   size_t m = mm;
   if( n < m ){
      n = mm;
      m = nn;
   }
   if ( ((m%2)==1 && (n%2)==1) || (m==1) || (m==2) || (m==4) || (m == 3 && (n == 4 || n == 6 || n == 8 ))){
      std::cout << " Knight's Tour is impossible";
      throw;
   }
*/
   heuristic_board heuristic;
   srand(time(NULL));
 
    // While we don't get a solution
    while (!findTour({0,0},heuristic))
    {;}
 
    return 0;
}

