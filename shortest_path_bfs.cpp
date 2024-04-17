#include <iostream>
#include<vector>
#include <queue>
#include <tuple>
#include <climits>
using namespace std;

//array of exits in the building
vector<int> exits={3,4,5,6};

/* 3D Matrix Model
0->empty node; 1->obstacle; 2->staircase to allow movement between different levels; 3->exit */
vector<vector<vector<int>>> matrix = {
    // level 1
    {{0, 1, 0, 0, exits[0]},
     {2, 1, 0, 1, exits[1]},
     {0, 0, 0, 1, 0},
     {exits[2], 1, 0, 0, 0},
     {exits[3], 1, 0, 1, 2}},

    // level 2
    {{0, 0, 0, 1, exits[0]},
     {2, 0, 0, 1, exits[1]},
     {0, 0, 0, 1, 0},
     {exits[2], 0, 0, 0, 0},
     {0, 0, 0, 1, 2}},

    // level 3
    {{0, 0, 0, 1, exits[0]},
     {2, 0, 0, 1, 0},
     {0, 0, 0, 1, 0},
     {0, 0, 0, 1, 0},
     {0, 0, 0, 1, 2}},
};

// width and height of each level
int width = matrix[0][0].size();
int height = matrix[0].size();

// number of levels aka floors
int depth = matrix.size();

void bfs(vector<vector<vector<int>>>& matrix, vector<vector<vector<int>>>& dist, queue<tuple<int, int, int>> q) {

    int dx[6] = {0, 0, 0, 0, 1, -1};
    int dy[6] = {0, 0, 1, -1, 0, 0};
    int dz[6] = {1, -1, 0, 0, 0, 0};

    while (!q.empty()) {
    
        tuple<int, int, int> cell = q.front();
        q.pop();

        int x = get<0>(cell);
        int y = get<1>(cell);
        int z = get<2>(cell);

        for (int i = 0; i < 6; i++) {
            if(dz[i]==0 || matrix[z][y][x]==2){
            int nx = x + dx[i];
            int ny = y + dy[i];
            int nz = z + dz[i];

            if (nx >= 0 && nx < width && ny >= 0 && ny < height && nz >= 0 && nz < depth &&
                matrix[nz][ny][nx] != 1 && dist[nz][ny][nx] == -1) {
                dist[nz][ny][nx] = dist[z][y][x] + 1;
                q.push(make_tuple(nx, ny, nz));
               }
            }
        }
    }
}

vector<tuple<int, int, int>> shortest_path(vector<vector<vector<int>>>& dist, int sx, int sy, int sz) {
    int dx[6] = {0, 0, 0, 0, 1, -1};
    int dy[6] = {0, 0, 1, -1, 0, 0};
    int dz[6] = {1, -1, 0, 0, 0, 0};

    queue<tuple<int, int, int, int>> q;

    q.push(make_tuple(sx, sy, sz, 0));

    vector<tuple<int, int, int>> path;

    while (!q.empty()) {
        tuple<int, int, int, int> cell = q.front();
        q.pop();

        int x = get<0>(cell);
        int y = get<1>(cell);
        int z = get<2>(cell);
        int d = get<3>(cell);

        if (matrix[z][y][x]>2) {
            cout<<"Nearest exit: EXIT0"<<matrix[z][y][x]-3<<endl;
            return path;
        }

        for (int i = 0; i < 6; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            int nz = z + dz[i];

            if (nx >= 0 && nx < width && ny >= 0 && ny < height && nz >= 0 && nz < depth &&
                dist[nz][ny][nx] != -1 && dist[nz][ny][nx] == dist[z][y][x] - 1) {
                q.push(make_tuple(nx, ny, nz, d + 1));
                path.push_back(make_tuple(nx, ny, nz));
                if (matrix[nz][ny][nx]>2) {
                    cout<<"Nearest exit: EXIT0"<<matrix[nz][ny][nx]-3<<endl;
                    return path;
                }
            }
        }
    }

    return path;
}

int main() {
    vector<vector<vector<int>>> dist(depth, vector<vector<int>>(height, vector<int>(width,-1)));
    queue<tuple<int, int, int>> q;

    //BFS traversal from each exit
    for (int i = 0; i < depth; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < width; k++) 
                if (matrix[i][j][k] > 2) {
                    q.push(make_tuple(k, j, i));
                    dist[i][j][k] = 0;
                }
            }
        }
    
    bfs(matrix, dist,q);
    
    // Simulate the shortest path from a cell to its nearest exit
    int sx = 4;
    int sy = 4;
    int sz = 0;
    auto t= shortest_path(dist, sx,sy, sz);
    cout << "Shortest path from (" << sx << ", " << sy << ", " << sz << ") to the nearest exit: ";
    for (auto p : t) {
        cout << "(" << get<0>(p) << ", " << get<1>(p) << ", " << get<2>(p) << ") ";
    }
    cout<< endl;

    /*
      TERMINAL OUTPUT
      Nearest exit: EXIT01
      Shortest path from (4, 4, 0) to the nearest exit: (4, 3, 0) (4, 2, 0) (4, 1, 0)
    */

    return 0;
}