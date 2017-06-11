#include<iostream>
#include<stack>
#include<deque>
#include<vector>
#include<list>
#define N 40
#define M 40

using namespace std;

static int move_x[8] = {2,1,-1,-2,-2,-1,1,2};
static int move_y[8] = {1,2,2,1,-1,-2,-2,-1};


class board_base{

public:
      //default constructor
   board_base( size_t n, size_t m)
      :nRow(n), nCol(m), count(1), board(std::vector<int>(n*m, 0)) {}
      //destructor
   virtual ~board_base(){}

      
   virtual void resize(size_t n, size_t m){
      board=std::vector<int>(n*m, 0);
      count = 1;
      nCol = m;
      nRow = n;
   }

   void reset(){
      board=std::vector<int>(nRow*nCol, 0);
      count = 1;
   }



   
   bool enough_move(){return (count == nRow * nCol + 1);}

   

   // check if the next move (as per knight's constraints) is possible 
   bool isMovePossible(const int& i, const int& j) {
      if ((i >= 0 && i < nRow) && (j >= 0 && j < nCol) && (board[i*nCol+j] == 0))
         return true;

      return false;
   }


   virtual bool move(const int& x, const int& y) = 0;

protected:
   std::vector<int> board;
   size_t nRow;
   size_t nCol;
   size_t count;
   
};

/*
class search_board : public board_base {
public:
   search_board(size_t nn=N, size_t mm=M)
      :board_base(nn,mm){}

// check the validity of the move
   bool move(const int& i, const int& j){
      if(!isMovePossible(i,j))
         return false;
     
      
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
   

};*/



class heuristic_board : public board_base {

public:

   heuristic_board(size_t nn=N, size_t mm=M)
      :board_base(nn,mm){}
    
   

   virtual ~heuristic_board(){}

   size_t getDegree(const int& x, const int& y){
      
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

   bool move(const int& x, const int& y){
         // check the validity of the move
      if(!isMovePossible(x,y))
         return false;
     
      last_x = x;
      last_y = y;

      board[x * nCol + y]=count;
      ++count;

      //generate possible next move
      int sx,sy;
      
      size_t min_deg = 10;
      size_t temp;
      for (size_t i = 0; i < 8; ++i) {
         // get the next move
         sx = last_x + move_x[i];
         sy = last_y + move_y[i];
         temp = getDegree(sx,sy);
         if (isMovePossible(sx,sy) && min_deg > getDegree(sx,sy)) {
            next_x = sx;
            next_y = sy;
            min_deg = temp;
         }
      }
     return true;
      
   }


   bool move(){
      if(last_x == next_x )
         return false;
      else 
         return move(next_x, next_y);
   }

   bool istour(const int& initx, const int& inity){
      for (size_t i = 0; i < 8; ++i) 
         if (last_x + move_x[i] == initx && last_y + move_y[i] == inity) 
            return true;

      return false;
   }

   void printboard(const int& initx, const int& inity) {
      int offset = board[initx * nCol + inity] - 1;
      // genric algorithm
      //lambda function used to calculate correct value
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

   bool findTour(const int& initx, const int& inity) {
      

       // Randome initial position
      int x = rand()%nRow;
      int y = rand()%nCol;
      
       // Current points are same as initial points
       move(x,y);
   
    
       // Keep picking next points using
       // Warnsdorff's heuristic
       while (!enough_move())
           if (!move()){
               reset();
               return false;
           }
   
      if(!istour(x,y)){
         reset();
         return false;
      }
   
      printboard(initx, inity);
      return true;
   
   }
   
   void Tour(const int& initx, const int& inity){
      srand(time(NULL));
      while (!findTour(initx, inity))
         {;}
   }

private:
      //generic algorithm to find the minimum heuristic move
      //store all possible moves in descending order of its degree
      // insert possible move in depth

   int next_x,next_y;
   int last_x,last_y;
};








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

   heuristic.Tour(0,0);
 
    return 0;
}

