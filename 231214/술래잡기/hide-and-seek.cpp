#include <iostream>
#include <vector>
#include <string.h>
using namespace std;

int n, m, tree, k, res = 0;
int MAP[99][99][2],Tmove[99][99], CwCCw = 1, totalmove = 0, nowmove = 0, moveamount = 1;

int ydir[] = { 0,0,1,-1 };
int xdir[] = { -1,1,0,0 };
int Changedir[] = { 1,0,3,2 };

int tydir[] = { -1,0,1,0 };
int txdir[] = { 0,1,0,-1 };

struct person { int y, x, dir; };
vector<person> Runner;
person Tracer;
    

void input() {
    cin >> n >> m >> tree >> k;
    int y, x, d;

    for (int i = 0; i < m; i++) {
        cin >> y >> x >> d;
        MAP[y-1][x-1][1] = 1;
        Runner.push_back({ y-1,x-1,d });
    }

    for (int i = 0; i < tree; i++) {
        cin >> y >> x;
        MAP[y-1][x-1][0] = 1;
    }

    Tracer = { n / 2,n / 2,0 };
    Tmove[n / 2][n / 2] = 1;
    MAP[Tracer.y][Tracer.x][1] = 2;
}

int GetDistance(int ry, int rx) {
    return abs(Tracer.y - ry) + abs(Tracer.x - rx);
}

void RunnerMove(int idx) {
    person now = Runner[idx];
    if (now.dir == -1)return;
    if (GetDistance(now.y, now.x) > 3) return;

    int ny = now.y + ydir[now.dir];
    int nx = now.x + xdir[now.dir];

    if (ny < 0 || nx < 0 || ny >= n || nx >= n) {
        Runner[idx].dir = Changedir[now.dir];
        now.dir = Runner[idx].dir;
        ny = now.y + ydir[now.dir];
        nx = now.x + xdir[now.dir];
    }
    if (Tracer.y == ny && Tracer.x == nx) return;

    MAP[now.y][now.x][1] = 0;
    MAP[ny][nx][1] = 1;

    Runner[idx].y = ny; Runner[idx].x = nx;
}

void TracerMove()
{
    MAP[Tracer.y][Tracer.x][1] = 0;
    Tracer.y += tydir[Tracer.dir];
    Tracer.x += txdir[Tracer.dir];
    
    int nowy = Tracer.y;
    int nowx = Tracer.x;
    MAP[Tracer.y][Tracer.x][1] = 2;
    if ( (nowy == 0 && nowx == 0) || (nowy == n / 2 && nowx == n / 2)) {
        memset(Tmove, 0, sizeof(Tmove));
        Tmove[nowy][nowx] = 1;
        if (CwCCw) {
            Tracer.dir =2;
            CwCCw = 0;
        }
        else {
            CwCCw = 1;
            Tracer.dir = 0;
        }
    }
    else
    {
        int flag = 0;
        Tmove[nowy][nowx] = 1;
        if (CwCCw) {
            if (Tracer.dir == 0) {
                if (!Tmove[nowy][nowx + 1]) flag = 1;
            }
            else if (Tracer.dir == 1) {
                if (!Tmove[nowy + 1][nowx]) flag = 1;
            }
            else if (Tracer.dir == 2) {
                if (!Tmove[nowy][nowx - 1]) flag = 1;
            }
            else if (Tracer.dir == 3) {
                if (!Tmove[nowy - 1][nowx]) flag = 1;
            }
            if (flag && CwCCw) {
                Tracer.dir++;
                if (Tracer.dir > 3)Tracer.dir = 0;
            }
        }
        else
        {
            int ny = nowy + tydir[Tracer.dir];
            int nx = nowx + txdir[Tracer.dir];
            if (ny < 0 || nx < 0 || ny >= n || nx >= n){
                Tracer.dir--;
                if (Tracer.dir < 0)Tracer.dir = 3;
            }
            if (Tmove[ny][nx]) {
                Tracer.dir--;
                if (Tracer.dir < 0)Tracer.dir = 3;
            }

        }
    }
    
}

void CanCatch(int t)
{   
    int catchCnt = 0;
    for (int i = 0; i < 3; i++)
    {
        int ny = Tracer.y + tydir[Tracer.dir] * i;
        int nx = Tracer.x + txdir[Tracer.dir] * i;

    if (ny < 0 || nx < 0 || ny >= n || nx >= n) break;
    if (MAP[ny][nx][0] == 1)continue;
    for (int j = 0; j < m; j++) {
        person catched = Runner[j];
            if (catched.dir == -1)continue;
            if (catched.y == ny && catched.x == nx) {
                Runner[j].dir = -1;
                catchCnt++;
            }
        }
    }
    res += t * catchCnt;

}



void solution(int t) {

   

    for (int i = 0; i < m; i++) RunnerMove(i);
    TracerMove();
    CanCatch(t);

    
}


int main()
{
    sync_with_stdio(false);cin.tie(0);
    input();
    for (int i = 1; i <=k; i++) solution(i);
    cout << res;
}