#include <iostream>
#include <queue>
#include <cstring>
using namespace std;
int n, m, k, c, res = 0;
int MAP[20][20]; // 나무 수치 , 제초제 등 갱신
int WALL[20][20]; // 벽이다
int TREE[20][20]; // 나무가 있다
int ydir[] = { -1,1,0,0 };
int xdir[] = { 0,0,-1,1 };

struct coord { int y, x; };
queue<coord> wd;

void input() {
    cin >> n >> m >> k >> c;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> MAP[i][j];
            if (MAP[i][j] > 0) TREE[i][j] = 1;
            if (MAP[i][j] == -1) WALL[i][j] = 1;
        }
    }
}

void PRINT() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << "\t" << MAP[i][j];
            }cout << '\n';
        }cout << '\n';
}

bool rangeCheck(int y, int x) {
    if (y < 0 || x < 0 || y >= n || x >= n)return false;
    return true;
}

void growth() {
    for(int r=0; r < n; r++){
        for (int c = 0; c < n; c++) {

            if (TREE[r][c] == 0)continue;

            for (int i = 0; i < 4; i++) {
                int ny = r + ydir[i];
                int nx = c + xdir[i];
                if (!rangeCheck(ny, nx)) 
                    continue;
                if (TREE[ny][nx]==1) 
                    MAP[r][c]++;
            }

        }
    }
}

void breed() {
    // 번식
    // 1. 현재 나무 정보 복사 -> newTREE
    // 2. 번식조건 
    //      2.1 범위 내
    //      2.2 벽이 아님
    //      2.3 제초제 없는 곳(MAP[ny][nx]>=0)
    //      2.4 원래 나무가 없던 곳 (newTREE[ny][nx]==0)
    // 3. 번식
    //     r,c 에서 2번 조건에 맞는 애들 개수 cnt
    //      조건에 맞다면 breedtrees queue에 담음
    //      TREE[new.y][new.x]를 1로 바꾸고 번식(MAP[new.y][new.x] += MAP[r][c] / cnt)

    int newTREE[20][20] = { 0, };
    memcpy(newTREE, TREE, sizeof(TREE));

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (newTREE[r][c] == 0) continue;

            queue<coord> breedtrees = queue<coord>(); 
            // newTREE[r][c] 는 1이라는뜻 -> 원래 있는 나무

            // 번식할 수 있는 애들 좌표
            for (int j = 0; j < 4; j++) {
                int ny = r + ydir[j];
                int nx = c + xdir[j];

                if (!rangeCheck(ny, nx)) continue; 
                if (newTREE[ny][nx] == 1)continue;
                if (WALL[ny][nx] == 1)continue;
                if (MAP[ny][nx] < 0)continue;

                breedtrees.push({ ny,nx });
            }

            int qsize = breedtrees.size();
            if (qsize > MAP[r][c])continue;

            while (!breedtrees.empty()) {
                coord newtree = breedtrees.front(); breedtrees.pop();
                TREE[newtree.y][newtree.x] = 1;
                MAP[newtree.y][newtree.x] += (MAP[r][c] / qsize);
            }
        }
    }

}

void pesticide() {
    
    // 전체 좌표에서 진행해본다.
    // 0,1,2,3 방향으로 1~k까지 진행하다가
    // 1. 범위 초과 -> 바로 종료
    // 2. 벽을 만난다 -> 바로 종료
    // 3. 땅을 만난다(MAP[ny][nx]==0 && TREE[ny][nx]==0) -> 해당 좌표 추가후 종료
    // 4. TREE가 있을 경우 MAX에 더한다
    // 5. temp에 추가한다

    int yndir[] = { -1,-1,1,1 };
    int xndir[] = { -1,1,-1,1 };

    int MAX = -21e8;
    queue<coord> qq = queue<coord>();

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (WALL[r][c] == 1)continue;
            if (TREE[r][c] == 0)continue;
            if (MAP[r][c] < 0)continue;
            queue<coord> temp = queue<coord>();

            int sum = MAP[r][c];
            for (int i = 0; i < 4; i++) {
                for (int j = 1; j <= k; j++) {

                    int ny = r + yndir[i] * j;
                    int nx = c + xndir[i] * j;
                    // 범위 증가시 나간다.
                    if (ny < 0 || nx < 0 || ny >= n || nx >= n)
                        break;
                    // 벽이면 멈춘다.
                    if (WALL[ny][nx] == 1) 
                        break;
                    // 나무 없다 -> 제초제 / 땅이다
                    if (TREE[ny][nx] == 0) 
                    { temp.push({ ny,nx }); break; }
                    sum += MAP[ny][nx];
                    temp.push({ ny,nx });
                }
            }

            if (sum > MAX) {
                MAX = sum;
                qq = temp;
                qq.push({ r,c });
            }
        }
    }

    if (MAX != -21e8)
    {
        while (!qq.empty())
        {
            coord now = qq.front(); qq.pop();
            wd.push({ now });
            MAP[now.y][now.x] = -c - 1;
            TREE[now.y][now.x] = 0;
        }
        res += MAX;
    }
        
}

void pestplus(int wds) {
    int visited[20][20] = { 0, };

    for (int i = 0; i < wds; i++) {
        coord now = wd.front(); wd.pop();
        if (visited[now.y][now.x])continue;
        MAP[now.y][now.x]++;
        if (MAP[now.y][now.x] == 0)continue;
        visited[now.y][now.x] = 1;
        
        wd.push({ now });
    }
}

void solution( ) {

    growth();
    breed();
    pesticide();
    int wdsize = wd.size();
    if (wdsize)
        pestplus(wdsize);

   
}

int main() {
    ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    input();
    for (int i = 0; i < m; i++) {
        solution();
    }
    cout << res;

}