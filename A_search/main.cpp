#include "main.h"

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};


vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
      if (n == 0) {
        row.push_back(State::kEmpty);
      } else {
        row.push_back(State::kObstacle);
      }
    }
    return row;
}


vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

// TODO: Write function to compare the f-value of two nodes here
bool Compare(vector<int> node1, vector<int> node2)
{
    if ((node1[2] + node1[3]) > (node2[2]+ node2[3]))
        return true;
    else    
        return false;
}

/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}

// // TODO: Write the Heuristic function here.
int Heuristic(int x1, int y1, int x2, int y2)
{
    return (abs(x2-x1)+ abs(y2-y1));
}
// TODO: Write CheckValidCell here. Check that the 
// cell is on the grid and not an obstacle (i.e. equals kEmpty)
bool CheckValidCell(int x, int y, vector<vector<State>>grid)
{
  
  if (grid.empty())
  {
    cout << " CheckValidCell(): Error, received empty Grid";
    return false;
  }
  // check range
  if ( x < 0 || x >= grid.size() || y < 0 || y >= grid[0].size())
  {
    return false;
  } 

  // Check that cell is not closed and not an obstacle grid status
  if (grid[x][y] == State::kEmpty)
  {
    return true;
  }

  return false;
    
}


// TODO: Write the AddToOpen function here.
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &open_nodes, vector<vector<State>> &grid)
{
    open_nodes.push_back(vector<int>{x, y, g, h});

    // close the cell in the grid.
    if (x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size())
        grid[x][y] = State::kClosed;
    else
        cout << "AddToOpen: Error, Illegal point["<< x << "," << y << "] Not on Grid" << std::endl;
}

/** 
 * Expand current nodes's neighbors and add them to the open list.
 */
void ExpandNeighbors(vector<int> current, int goal[2], vector<vector<int>> &open, vector<vector<State>> &grid) {

  // TODO: Get current node's data.
  int x_curr = current[0];
  int y_curr = current[1];
  int g_curr = current[2];

  // TODO: Loop through current node's potential neighbors.
  for (auto del: delta)
  {
    if (CheckValidCell(x_curr + del[0], y_curr + del[1], grid))
    {
      int h_val = Heuristic(x_curr + del[0], y_curr + del[1], goal[0], goal[1]);
      AddToOpen(x_curr + del[0], y_curr + del[1], g_curr + 1, h_val, open, grid);
    }
      
  }
    
}



// TODO: Write the Search function stub here.
std::vector<vector<State>> Search(vector<vector<State>> grid, int start[2], int goal[2] )
{
    // Create the vector of open nodes.
    vector<vector<int>> open {};

    // calc the H for the initial node
    vector<int> node{start[0], start[1], 0, Heuristic(start[0], start[1], goal[0], goal[1])};

    // Add the the node to the open list
    AddToOpen(node[0], node[1], node[2], node[3], open, grid);

    grid[start[0]][start[1]] = State::kStart;



    // TODO: while open vector is non empty {
    while (!open.empty())
    {

        // TODO: Sort the open list using `CellSort`, and get the current node.
        CellSort(&open);
        
        // TODO: Get the x and y values from the current node,
        auto current = open.back();
        open.pop_back();

        // and set grid[x][y] to kPath.
        grid[current[0]][current[1]] = State::kPath;

        // TODO: Check if you've reached the goal. If so, return grid.
        if(current[0] == goal[0] && current[1] == goal[1])
        {
            grid[goal[0]][goal[1]] = State::kFinish;
            return grid;

        }
            

        // If we're not done, expand search to current node's neighbors. This step will be completed in a later quiz.
        ExpandNeighbors(current, goal, open, grid);


    } // TODO: End while loop
    

        
    // Failed to find a path, return an empty grid;
    cout << "Path not found" << std::endl;
    vector<vector<State>> solution;
    return solution;

}


string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰️    ";
    case State::kPath: return "🚗   ";
    case State::kStart: return"🚦   ";
    case State::kFinish: return "🏁   ";
    default: return "0    "; 
  }
}


void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}

#include "tests.cpp"

int main() {
  // TODO: Declare "init" and "goal" arrays with values {0, 0} and {4, 5} respectively.
    auto board = ReadBoardFile("1.board");
    int init[] = {0,0};
    int goal[] = {4,5}; 
    // TODO: Call Search with "board", "init", and "goal". Store the results in the variable "solution".
    auto solution = Search(board, init, goal);
    // TODO: Change the following line to pass "solution" to PrintBoard.
    PrintBoard(solution);
   // Tests
    // TestHeuristic();
    // TestAddToOpen();
    // TestCompare();
    // TestSearch();
    // TestCheckValidCell();
    // TestExpandNeighbors();

}