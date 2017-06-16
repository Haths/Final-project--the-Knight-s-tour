#include "knight_board.h"
#include "time.h"


// main
int main() {

   struct timespec start, finish;
   double elapsed;

   char input('a');
   int x(-1),y(-1);
   int nn(N),mm;

   while( input!='q'){

      std::cout<<"Enter: ordinary Depth first search[d]\t warnsdorff's heuristics[h]\n";
      while (!(std::cin >> input) || (input != 'd' && input!= 'h' ) ) {
         std::cout << "Bad value!\n";
         std::cout<<"\n"<<"Enter: ordinary Depth first search[d]\t warnsdorff's heuristics[h]\n";
         std::cin.clear();
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }

      std::cout<<"Enter initial (x,y) coordinates \n"<<"x: ";
      
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
      
      std::cout<<"Enter Row Number \n"<<"N: ";
      
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
      --x;--y;
      int n = nn;
      int m = mm;
      if( n < m ){
         n = mm;
         m = nn;
      }
      if ( ((m%2)==1 && (n%2)==1) || (m==1) || (m==2) || (m==4) || (m == 3 && (n == 4 || n == 6 || n == 8 ))){
         std::cout <<" size of the board is "<<nn<<"*"<<mm<< " Knight's Tour is impossible\n";
         continue;
      }
   
      clock_gettime(CLOCK_MONOTONIC, &start);
      if(input == 'd'){
         dfs_board game(nn,mm);
         game.Tour(x,y);
         game.printboard();
      }
      else{
         heuristic_board game(nn,mm);
         game.Tour(x,y);
         game.printboard();
      }

      clock_gettime(CLOCK_MONOTONIC, &finish);
      elapsed = (finish.tv_sec - start.tv_sec);
      elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

      std::cout<<"The board is "<<nn<<"*"<<mm<<".\nIt takes "<<elapsed<<" seconds to solve this problem.\n\n";

      std::cout<<"Quit[q]? Enter any other key to continue\n";
      std::cin >> input;
   }

 
   return 0;
}

