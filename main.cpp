#include "knight_board.h"


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

