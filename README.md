# Final-project--the-Knight-s-tour
A [knight's tour](https://en.wikipedia.org/wiki/Knight%27s_tour) is a sequence of moves of a knight on a chessboard such that the knight visits every square only once. 

![knight's tour](https://github.com/Haths/Final-project--the-Knight-s-tour/blob/master/illustration/Knights-Tour-Animation.gif)  
>The knight's tour problem is the mathematical problem of finding a knight's tour. Creating a program to find a knight's tour is a common problem given to computer science students.[1] Variations of the knight's tour problem involve chessboards of different sizes than the usual 8 × 8, as well as irregular (non-rectangular) boards.



### Implementation Details
Source code is inside three files: ** main.cpp, knight_board.cpp, knight_board.h**.

####  Algorithms:
I provide two way to solve the problem, one is implemented with ordinary depth first search algorithm, the other with [_Warnsdorff’s algorithm_](http://www.geeksforgeeks.org/warnsdorffs-algorithm-knights-tour-problem/).

* Both ways guanrantee to solve the Knight's tour problem; however, the second one runs much faster than the first.
For this reason, I add a timer to determine the elapse time for each calculation.

#### **_Abstract CLASS_** and **_RAII_**

* Both solver are implemented in classes, the class that uses ordinary depth first search algorithm has a name called **_dfs_board_**, the class that uses Warnsdorff’s algorithm has a name called **_heuristic_board_**.
Both are derived class from abstract class **_board_base_**, in which _void Tour_ is the pure virtual function. 

* Inside class **_board_base_**, a **dynamical** array is used to store Information about the chess board and the progress of the solver as well. 

* _Constructor, destructor_ and _member function_ inside this class are all implemented followed from rules of **RAII** idioms. Safe guarded and exceptions.

* Default copy constructor is explicitly disabled.

#### Generic Algorithm

* A template function **_testmove(const int& x, const int&y, PRED pred)_** is implemeted to allow to pass in differnent predicates to check if next moves satisfies some conditions.

* inside class **_board_base_**'s constructor, **_std::fill_n(board, n*m, 0)_** is used to fill the undefined dynamical array to avoid undefined behavior.

* Inside class **_board_base_**'s member function **_bool istour_**, lambda function is passed in function **_testmove(const int& x, const int&y, PRED pred)_** to use as a predicate.

* Inside class **_heuristic_board_**'s member function **_inline void recal_**, a lambad function is defined to pass in **_std::for_each_** to calculate the correct move order.


#### According to [_Wiki_](https://en.wikipedia.org/wiki/Knight%27s_tour), here is a theorem about existence of a Knight's Tour
>1. Schwenk proved that for any m × n board with m ≤ n, a closed knight's tour is always possible unless one or more of these three conditions are met:
  >>* m and n are both odd
  >>* m = 1, 2, or 4
  >>* m = 3 and n = 4, 6, or 8.
>2. Cull et al. and Conrad et al. proved that on any rectangular board whose smaller dimension is at least 5, there is a (possibly open) knight's tour.
* This theorem can limits our choice of board size.

### Performance
>"On an 8 × 8 board, there are exactly 26,534,728,821,064 directed closed tours (i.e. two tours along the same path that travel in opposite directions are counted separately, as are rotations and reflections). The number of undirected closed tours is half this number, since every tour can be traced in reverse!”

* Using ordinary depth first search to exhaust all the paths are not very practical, even though the algorithm is complete by assumption that the board size satisfies the theorem.

* For board size greater than 6, running DFS takes more than a few minutes. Not recommend for general use.

* On the order hand, **Warnsdorff’s algorithm** works pretty well, there is even a math proof to show that it runs in linear time to the size of the board.

* In practice, I got an answer of 100 * 100 size board in two minutes.

### Command Line Interface(CLI)
* **Depth first search Algorithm:**
![cli-exsearch](https://github.com/Haths/Final-project--the-Knight-s-tour/blob/master/illustration/DFS.png)

* **Warnsdorff’s algorithm**:
![cli-check](https://github.com/Haths/Final-project--the-Knight-s-tour/blob/master/illustration/heuristic.png)

**Board_size Checking**:
![cli-tour1](https://github.com/Haths/Final-project--the-Knight-s-tour/blob/master/illustration/safe_guard.png)



### Under Construction:
GUI  ...
