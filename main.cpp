#include <iostream>
#include <queue>
#include <stack>
#include <set>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

const int N = 4;

struct State {
    int board[N][N];
    int g, h, f;
    string path;

    bool operator > (const State& other) const {
        return f > other.f;
    }
};

int goal[N][N] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9,10,11,12},
    {13,14,15,0}
};

string boardToString(int board[N][N]) {
    string s = "";
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            s += to_string(board[i][j]) + ",";
    return s;
}

void copyBoard(int boardA[N][N], int boardB[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            boardB[i][j] = boardA[i][j];
}

void printBoard(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        cout << "|";
        for (int j = 0; j < N; j++) {
            cout << " " << board[i][j];
            if (board[i][j] < 10) {
                cout << "  |";
            } else {
                cout << " |";
            }
        }
        cout << endl;
    }
}

bool isGoal(int board[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] != goal[i][j])
                return false;
    return true;
}

bool isReverse(char last, char now) {
    return (last=='U'&&now=='D') || (last=='D'&&now=='U') ||
           (last=='L'&&now=='R') || (last=='R'&&now=='L');
}

bool isSolvable(int board[N][N]) {
    int array[16];
    int k = 0;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            array[k++] = board[i][j];

    int inversions = 0;
    for (int i = 0; i < 15; i++) {
        for (int j = i + 1; j < 16; j++) {
            if (array[i] && array[j] && array[i] > array[j])
                inversions++;
        }
    }

    int blankRow;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == 0)
                blankRow = i;

    int row = N - blankRow;

    if (row % 2 == 0)
        return inversions % 2 == 1;
    else
        return inversions % 2 == 0;
}

void inputBoard(int board[N][N]) {
    cout << "Enter your board:" << endl;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> board[i][j];
}

void randomize(int board[N][N]) {
    copyBoard(goal, board);
    
    int directionX[4] = {-1,1,0,0};
    int directionY[4] = {0,0,-1,1};
    
    int x = 3, y = 3;
    
    for (int i = 0; i < 100; i++) {
        int direction = rand() % 4;
        
        int nextX = x + directionX[direction];
        int nextY = y + directionY[direction];
        
        if (nextX >= 0 && nextX < N && nextY >= 0 && nextY < N) {
            int temp = board[x][y];
            board[x][y] = board[nextX][nextY];
            board[nextX][nextY] = temp;
            
            x = nextX;
            y = nextY;
        }
    }
}

int manhattan(int board[N][N]) {
    int dist = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int val = board[i][j];
            if (val != 0) {
                int X = (val - 1) / N;
                int Y = (val - 1) % N;
                dist += abs(i - X) + abs(j - Y);
            }
        }
    }
    return dist;
}

void AStar(int board[N][N]) {
    priority_queue<State, deque<State>, greater<State> > openSet;
    set<string> closedSet;

    State start;
    copyBoard(board, start.board);
    start.g = 0;
    start.h = manhattan(board);
    start.f = start.g + start.h;
    start.path = "";

    openSet.push(start);

    int dx[4] = {-1,1,0,0};
    int dy[4] = {0,0,-1,1};
    char moves[4] = {'U','D','L','R'};

    while (!openSet.empty()) {
        State current = openSet.top();
        openSet.pop();

        string key = boardToString(current.board);
        if (closedSet.count(key)) continue;
        closedSet.insert(key);

        if (isGoal(current.board)) {
            cout << "Solved puzzle using A* search:\n";
            cout << "Moves: " << current.path << endl;
            cout << "Steps: " << current.path.size() << endl;
            return;
        }

        int x, y;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (current.board[i][j] == 0) {
                    x = i;
                    y = j;
                }

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                State next;
                copyBoard(current.board, next.board);
                swap(next.board[x][y], next.board[nx][ny]);
                next.g = current.g + 1;
                next.h = manhattan(next.board);
                next.f = next.g + next.h;
                next.path = current.path + moves[i];
                openSet.push(next);
            }
        }
    }

    cout << "No solution found\n";
}

bool DFS(int board[N][N], int maxDepth) {
    stack<State> st;
    set<string> visited;

    State start;
    copyBoard(board, start.board);
    start.path = "";

    st.push(start);

    int dx[4] = {-1,1,0,0};
    int dy[4] = {0,0,-1,1};
    char moveChar[4] = {'U','D','L','R'};

    while (!st.empty()) {
        State current = st.top();
        st.pop();

        if (current.path.size() > maxDepth) continue;

        string key = boardToString(current.board);
        if (visited.count(key)) continue;
        visited.insert(key);

        if (isGoal(current.board)) {
            cout << "Solved puzzle using Depth-First Search:\n";
            cout << "Moves: " << current.path << endl;
            cout << "Steps: " << current.path.size() << endl;
            return true;
        }

        int x, y;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (current.board[i][j] == 0) {
                    x = i;
                    y = j;
                }

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                State next;
                copyBoard(current.board, next.board);
                swap(next.board[x][y], next.board[nx][ny]);
                next.path = current.path + moveChar[i];
                st.push(next);
            }
        }
    }

    cout << "No solution found at depth " << maxDepth << endl;
    return false;
}

void Greedy(int board[N][N]) {
    priority_queue<State, deque<State>, greater<State> > openSet;
    set<string> closedSet;

    State start;
    copyBoard(board, start.board);
    start.g = 0;
    start.h = manhattan(board);
    start.f = start.h;
    start.path = "";

    openSet.push(start);

    int dx[4] = { -1,1,0,0 };
    int dy[4] = { 0,0,-1,1 };
    char moves[4] = { 'U','D','L','R' };

    while (!openSet.empty()) {
        State current = openSet.top();
        openSet.pop();

        string key = boardToString(current.board);
        if (closedSet.count(key)) continue;
        closedSet.insert(key);

        if (isGoal(current.board)) {
            cout << "Solved puzzle using Greedy Search:\n";
            cout << "Moves: " << current.path << endl;
            cout << "Steps: " << current.path.size() << endl;
            return;
        }

        int x, y;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (current.board[i][j] == 0) {
                    x = i;
                    y = j;
                }

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                State next;
                copyBoard(current.board, next.board);
                swap(next.board[x][y], next.board[nx][ny]);
                next.g = current.g + 1;
                next.h = manhattan(next.board);
                next.f = next.h;
                next.path = current.path + moves[i];
                openSet.push(next);
            }
        }
    }

    cout << "No solution found\n";
}


int main() {
    int board[N][N];
    int choice, algorithm;

    srand(time(0));

    while (true) {
        cout << "15-Puzzle Problem Menu:" << endl;
        cout << "1. Create a puzzle" << endl;
        cout << "2. Randomize a puzzle" << endl;
        cout << "3. Exit" << endl << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            inputBoard(board);
            cout << endl << "Created board:" << endl;
            printBoard(board);
            break;
        case 2:
            randomize(board);
            cout << endl << "Created board:" << endl;
            printBoard(board);
            break;
        case 3:
            cout << "Exiting Program." << endl;
            return 0;
        default:
            cout << "Invalid choice, enter a valid option." << endl;
            continue;
        }

        while (true) {
            cout << endl << "Choose an algorithm:" << endl;
            cout << "1. A* Search" << endl;
            cout << "2. Depth-First Search" << endl;
            cout << "3. Greedy Search" << endl;
            cout << "4. Back to Main Menu" << endl << endl;
            cin >> algorithm;

            switch (algorithm) {
            case 1:
                if (!isSolvable(board)) {
                    cout << "This puzzle is not solvable." << endl;
                    break;
                }
                else {
                    AStar(board);
                }
                break;
            case 2:
                if (!isSolvable(board)) {
                    cout << "This puzzle is not solvable." << endl;
                    break;
                }
                for (int depth = 1; depth <= 40; depth++) {
                    if (DFS(board, depth)) {
                        break;
                    }
                }
                break;
            case 3:
                if (!isSolvable(board)) {
                    cout << "This puzzle is not solvable." << endl;
                    break;
                }
                else {
                    Greedy(board);
                }
                break;
            case 4:
                cout << "Returning to Main Menu." << endl;
                break;
            default:
                cout << "Invalid choice, enter a valid option." << endl;
                continue;
            }

            if (algorithm == 4) break;
        }
    }
    return 0;
}
