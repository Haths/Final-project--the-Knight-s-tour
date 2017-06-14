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

public:
   //default constructor
   board_base( size_t n, size_t m)
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

   //copy constructor disable
   board_base(const board_base& that) = delete;



   //destructor
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
   bool move(const int& x, const int& y){
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

   void Tour(const int& initx, const int& inity) {
      
      // Current points are same as initial points
      move(initx, inity);
   
      // Keep picking next points 
      while (!enough_move() || !istour(initx,inity))
          if (!update()){
              std::cout<<"\nBy examinating all possible walks, we claim that not such tour exist.";
              return;
          }

      printboard();
   
   }

      
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
   void Tour(const int& initx, const int& inity){
      srand(time(NULL));
      while (!findTour()){;}
   
      this->recal(initx,inity);
      printboard();
   }


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

   
private:
   //private member to store next_move, last_move
   int next_x,next_y;
};








// main
int main() {

   char input('a');
   std::cout<<"Enter: ordinary Depth first search[d]\t warnsdorff's heuristics[h]\n";
   while (!(std::cin >> input) || (input != 'd' && input!= 'h') ) {
      std::cout << "Bad value!\n";
      std::cout<<"\n"<<"Enter: ordinary Depth first search[d]\t warnsdorff's heuristics[h]\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   }

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


   if(input == 'd'){
      dfs_board game(nn,mm);
      game.Tour(x,y);
   }
   else{
      heuristic_board game(nn,mm);
      game.Tour(x,y);
   }
 
   return 0;
}

