#include<iostream>
#include<deque>
#include<algorithm>

#define N 20


// valid move pattern
static int move_x[8] = {2,1,-1,-2,-2,-1,1,2};
static int move_y[8] = {1,2,2,1,-1,-2,-2,-1};


// pure virtual base class for chess board
// use array to represent the chess board
// cannot be instantiated
class board_base{

public:
   //default constructor
   board_base( size_t n, size_t m)
      :nRow(n), nCol(m), count(1), board(nullptr) {
         try{
            board = new int [n*m];
            std::fill_n(board, n*m, 0); 
         }
         catch(const std::bad_alloc& e){
            delete[] board;
            board = nullptr;
            throw e;
         }
      }
      //destructor

   board_base(const board_base& that) = delete;

   virtual ~board_base(){
      delete[] board;
      board = nullptr;
   }

   //function for resize the chessboard 
   //user change
   inline void resize(size_t n, size_t m){
      delete [] board;
      board = nullptr;
      try{
            board = new int [n*m];
            std::fill_n(board, n*m, 0); 
         }
         catch(const std::bad_alloc& e){
            delete[] board;
            board = nullptr;
            throw e;
         }
      count = 1;
      nCol = m;
      nRow = n;
   }

   //function for resetting value in each grid
   inline void reset(){
      delete [] board;
      board = nullptr;
      try{
            board = new int [nRow*nCol];
            std::fill_n(board, nRow*nCol, 0); 
         }
         catch(const std::bad_alloc& e){
            delete[] board;
            board = nullptr;
            throw e;
         }
      count = 1;
   }

   //function for checking if it has a walk of enough length
   inline bool enough_move(){return (count == nRow * nCol + 1);}

   // check if the next move (as per knight's constraints) is possible 
   inline bool isMovePossible(const int& i, const int& j) {
      if ((i >= 0 && i < nRow) && (j >= 0 && j < nCol) && (board[i*nCol+j] == 0))
         return true;

      return false;
   }

   //pure virtual function
   //need to implement in derived class
   //key step to update the board
   virtual bool move(const int& x, const int& y) = 0;

protected:
   int* board;
   size_t nRow,nCol,count;
};

// use to perform normal depth first search
// exhaust all the possible state
// exponential complexity
// super inefficient
class dfs_board : public board_base {
public:
   dfs_board(size_t nn=6, size_t mm=6)
      :board_base(nn,mm){}

   // check the validity of the move
   bool move(const int& x, const int& y){
      if(!isMovePossible(x,y))
         return false;
     
      Gx_set.push_back(x);
      Gy_set.push_back(y);

      Xx_set.push_back(x);
      Xy_set.push_back(y);

      board[x*nCol + y] = count;
      ++count;
      
         //generate possible next move
      int sx,sy;
      
      for (size_t i = 0; i < 8; ++i){
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

   bool update(){
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

   bool Tour(const int& initx, const int& inity) {
      
       // Current points are same as initial points
       move(initx, inity);
    
       // Keep picking next points 
       while (!enough_move() || !istour(initx,inity))
           if (!update()){
               std::cout<<"\nBy examinating all possible walks, we claim that not such tour exist.";
               return false;
           }
   
      printboard();
      return true;
   
   }


   inline bool istour(const int& initx, const int& inity){
      for (size_t i = 0; i < 8; ++i) 
         if (Xx_set.back() + move_x[i] == initx && Xy_set.back() + move_y[i] == inity) 
            return true;
         return false;
      }


   inline void printboard() {
      //size_t num(1);
      //std::for_each(X_set.begin(), X_set.end(), [&](chess_moves a){board[a.x][a.y] = num; ++num;});
      for (size_t i = 0; i < nRow; i++) {
         for (size_t j = 0; j < nCol; j++) {

            printf("%d\t",board[i*nCol+j]);
            }
         printf("\n");
         }
      }
      
private:
   std::deque<int> Xx_set,Xy_set;
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
   //destructor
   virtual ~heuristic_board(){}
   //implemented pure virtual function in children class
   //store the minimum heuristic move into next_move
   //compare all the possible moves
   //if no next_move exists (stuck into a corner), next_move is set to equal to last_move
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
   bool findTour() {
      
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
              reset();
              return false;
          }
      //tour is not closed?
      if(!istour(x,y)){
         reset();
         return false;
      }
   
      
      return true;
   
   }

   //wrapper function for find tour
   void Tour(const int& initx, const int& inity){
      srand(time(NULL));
      while (!findTour()){;}
      printboard(initx, inity);
   }

   // helper function to get the heuristic of an move
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

   //check if last_move is equal to next_move
   //(meaning that we are stuck at a corner) return false
   //otherwise, perform next_move
   inline bool update(){
      if(last_x == next_x )
         return false;
      else 
         return move(next_x, next_y);
   }

   //check if is a closed tour
   inline bool istour(const int& initx, const int& inity){
      for (size_t i = 0; i < 8; ++i) 
         if (last_x + move_x[i] == initx && last_y + move_y[i] == inity) 
            return true;

      return false;
   }

   // genric algorithm
   // lambda function used to calculate correct value
   // print out the steps in correct way
   inline void printboard(const int& initx, const int& inity) {
      
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

      for (size_t i = 0; i < nRow; i++) {
         for (size_t j = 0; j < nCol; j++) {
             printf("%d\t", board[i*nCol+j]);
         }
         printf("\n");
      }
   }

   
private:
   //private member to store next_move, last_move
   int next_x,next_y,last_x,last_y;
};








// main
int main() {
   
   std::cout<<"Enter initial (x,y) coordinates \n"<<"x: ";
   int x(-1),y(-1);
   while (!(std::cin >> x) || x < 0 ) {
         std::cout << "Bad value!\n";
         std::cout<<"\n"<<"x: ";
         std::cin.clear();
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
   std::cout<<"y: ";
   while (!(std::cin >> y) || y < 0 ) {
      std::cout << "Bad value!\n";
      std::cout<<"\n"<<"y: ";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   }
   --x;--y;
   std::cout<<"Enter Row Number \n"<<"N: ";
   int nn(N),mm;
   while (!(std::cin >> nn) || nn < 0 || nn < x) {
         std::cout << "Bad value!\n";
         std::cout<<"\n"<<"N: ";
         std::cin.clear();
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
   std::cout<<"Enter Column Number \n"<<"M: ";
   while (!(std::cin >> mm) || mm< 0 || mm < y) {
      std::cout << "Bad value!\n";
      std::cout<<"\n"<<"M: ";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
   //int nn(5),mm(5),x(0),y(0);
   heuristic_board heuristic(nn,mm);

   //dfs_board dfs(nn,mm);

   //dfs.Tour(x,y);
   
   heuristic.Tour(x,y);
 
   return 0;
}

