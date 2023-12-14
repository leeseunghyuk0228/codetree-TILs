#include <iostream>
#include <deque>
#include <unordered_map>

using namespace std;
int n, m, k;
int MAP[20][20], point=0;
int ydir[] = { -1,1,0,0 };
int xdir[] = { 0,0,-1,1 };
deque<pair<int, int>> team[5];
unordered_map <int, int > um;

// 팀 나누기
void Divteam(int sy, int sx, int team_num) {
    deque<pair<int, int>> q;
    q.push_back({ sy,sx });
    
    int visited[20][20] = { 0, };
    visited[sy][sx] = 1;
    
    while (!q.empty()) {
        int nowy = q.front().first;
        int nowx = q.front().second; q.pop_front();
        int now = MAP[nowy][nowx];
        // 팀등록은 무조건
        um[nowy * n + nowx] = team_num;
        // 팀큐에 넣는건 123만
        if (now < 4) team[team_num].push_back({ nowy,nowx });

        for (int i = 0; i < 4; i++) {
            int ny = nowy + ydir[i];
            int nx = nowx + xdir[i];
            if (ny < 0 || nx < 0 || ny >= n || nx >= n) continue;
            if (visited[ny][nx])continue;
            if (!MAP[ny][nx])continue;
            if (now == 1 && MAP[ny][nx] == 3) continue;
            visited[ny][nx] = 1;
            q.push_back({ ny,nx });
        }
    }
}

// 입력
void input() {
    deque<pair<int, int>> dq;

    cin >> n >> m >> k;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            cin >> MAP[i][j];
            if (MAP[i][j] == 1)dq.push_back({ i, j });
        }

    for (int i = 0; i < m; i++) 
        Divteam(dq[i].first, dq[i].second, i);
}

// 한 칸 이동
void MoveHead() {
    for (int i = 0; i < m; i++) {

        int teamsize = team[i].size();
        int hy, hx;

        // 3이랑 4인곳만 찾기
        for (int j = 0; j < 4; j++) {
            int ny = team[i][0].first + ydir[j];
            int nx = team[i][0].second + xdir[j];
            if (ny < 0 || nx < 0 || ny >= n || nx >= n) continue;
            if (MAP[ny][nx] == 4 || MAP[ny][nx]==3) {
                hy = ny;
                hx = nx;
                break;
            }
        }
        
        for (int j = 0; j < teamsize; j++) {
            int nowy = team[i][j].first;
            int nowx = team[i][j].second;
            // 자기자리 4 세팅
            MAP[nowy][nowx] = 4;
            team[i][j].first = hy;
            team[i][j].second = hx;

            if (j == 0) MAP[hy][hx] = 1;
            else if (j == teamsize - 1) MAP[hy][hx] = 3;
            else MAP[hy][hx] = 2;

            hy = nowy;
            hx = nowx;
        }

        MAP[team[i][0].first][team[i][0].second] = 1;


    }

}

// 헤드 변경
void ChangeHead(int y,int x, int team_num) {
    int teamsize = team[team_num].size();

    for (int i = 0; i < teamsize; i++) {
        if (team[team_num][i].first == y && team[team_num][i].second == x) {
            point += (i + 1) * (i + 1);
            break;
        }
    }
    deque<pair<int, int>> temp;

    for (int i = teamsize-1; i >0; i--) {
        temp.push_back(team[team_num][i]);
        team[team_num].pop_back();
    }

    int tempsize = temp.size();
    for (int i = tempsize - 1; i >= 0; i--) team[team_num].push_front(temp[i]);

    MAP[team[team_num][0].first][team[team_num][0].second] = 1;
    MAP[team[team_num][teamsize-1].first][team[team_num][teamsize-1].second] = 3;

}

// 공 던진다
void Throw(int round) {
    
    int type = (round / n) % 4;
    int index = (round) % n;

    if (type == 2 || type == 3) index = n - 1 - index;

    // 왼 -> 오 : 0열 ~ n열
    if (type == 0) {
        for (int i = 0; i < n; i++) {
            if (MAP[index][i] > 0 && MAP[index][i] < 4) { ChangeHead(index,i,um[index * n + i]); return;}
        }
    }
    // 하 -> 상 : n행 ~ 0행
    else if (type == 1) {
        for(int i=n-1;i>=0;i--)
            if(MAP[i][index] > 0 && MAP[i][index]<4) {ChangeHead(i,index,um[i * n + index]); return; }
    }
    // Index 반대임
    // 우 -> 좌 : n열 ~ 0 열
    else if (type == 2) {
        for (int i = n - 1; i >= 0; i--)
            if (MAP[index][i] > 0 && MAP[index][i] < 4) {ChangeHead(index,i,um[index * n + i]); return; }
    }
    // 상 -> 하 : 0행 ~ n행  (3n+1 ~ 4n) 17
    else if (type == 3) {
        for (int i = 0; i < n; i++)
            if (MAP[i][index] > 0 && MAP[i][index] < 4) {
                ChangeHead(i, index, um[i * n + index]); return;
            }
    }

}

void solution(int round) {
    
    MoveHead();
   
    Throw(round);
   
}

int main() {
    ios_base::sync_with_stdio(false);cin.tie(0);
    input();
    for (int i = 0; i < k; i++) solution(i);
    cout << point;


}