#include<iostream>
using namespace std;


template < typename value >
class board_base{
public:
   board_base( size_t n, size_t m, value T)
      :nRow(n), nCol(m), board(std::vector<std::vector<value>>(n,std::vector<value>(m, T))) {
         size_t n = nn;
         size_t m = mm;
         if( n < m ){
            n = mm;
            m = nn;
         }
         if ( ((m%2)==1 && (n%2)==1) || (m==1) || (m==2) || (m==4) || (m == 3 && (n == 4 || n == 6 || n == 8 )))
            std::cout << " Knight's Tour is not possible"
      }

   ~board_base(){}

   board_base(const board_base& old){
      nRow = old.nRow;
      nCol = old.nCol;
      board = old.board;
   }

   board_base& operator= (board_base old){
      std::swap(nRow, old.nRow);
      std::swap(nCol, old.nCol);
      std::swap(board, old.nRow);
      return *this;
   }


   bool isMovePossible(const chess_moves& move) {
      int i = move.x;
      int j = move.y;
      if ((i >= 0 && i < nRow) && (j >= 0 && j < nCol) && (board[i][j] == 0))
         return true;
      return false;
}


private:
   size_t nRow;
   size_t nCol;
   std::vector < std::vector<value> > board;
};


