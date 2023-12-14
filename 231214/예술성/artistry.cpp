#include <iostream>
#include <vector>
#include <queue>
#include <string.h>
using namespace std;
int n, mid, MAP[29][29], visited[29][29],TEAM[29][29];
int res = 0,t_num;

int ydir[] = { -1,0,1,0 };
int xdir[] = { 0,1,0,-1 };

vector<vector<pair<int, int>>> Groups;
vector<int> Gnum;

void input()
{
	cin >> n;
	mid = n / 2;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> MAP[i][j];
}

void BFS(int sy, int sx,int t_num) {
	queue<pair<int, int>> q;
	vector<pair<int, int>> tempv;
	
	TEAM[sy][sx] = t_num;
	visited[sy][sx] = 1;
	
	
	int NOW = MAP[sy][sx];
	tempv.push_back({ sy,sx });
	Gnum.push_back(t_num);
	q.push({ sy,sx });
	

	while (!q.empty()) {
		int nowy = q.front().first;
		int nowx = q.front().second;
		q.pop();

		for (int i = 0; i < 4; i++) {
			int ny = nowy + ydir[i];
			int nx = nowx + xdir[i];
			if (ny < 0 || nx < 0 || ny >= n || nx >= n) continue;

			if (visited[ny][nx])continue;
			if (MAP[ny][nx] != NOW)continue;

			visited[ny][nx] = 1;
			TEAM[ny][nx] = t_num;
			q.push({ ny,nx });
			tempv.push_back({ ny,nx });
		}

	}
	Groups.push_back(tempv);
}

void Divgroup()
{
	memset(visited, 0, sizeof(visited));
	memset(TEAM, 0, sizeof(TEAM));
	Groups.clear();
	Gnum.clear();

	t_num = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (visited[i][j])continue;
			BFS(i, j,t_num++);
		}
	}
}

void Beauty()
{
	int GroupsCnt = Groups.size();

	for (int i = 0; i < GroupsCnt-1; i++) 
	{ 
		int groupsize = Groups[i].size();
		int gnum1 = MAP[Groups[i][0].first][Groups[i][0].second];

		for (int j = i+1; j < GroupsCnt; j++) 
		{
			int othergroupnum = Gnum[j];
			int gnum2 = MAP[Groups[j][0].first][Groups[j][0].second];
			int groupsize2 = Groups[j].size();
			int cnt = 0;

			for (int idx = 0; idx < groupsize; idx++) {
				int y = Groups[i][idx].first;
				int x = Groups[i][idx].second;

				for (int d = 0; d < 4; d++)
				{
					int ny = y + ydir[d];
					int nx = x + xdir[d];
					
					if (ny < 0 || nx < 0 || ny >= n || nx >= n)continue;
					if (TEAM[ny][nx] == othergroupnum)cnt++;
				}

			}
			res +=gnum1* gnum2 * (groupsize + groupsize2) * cnt;

		}
	}
}

void Unclockwise()
{
	int arr[14] = { 0, };
	// 세로 방향 먼저 arr 저장
	int ny = mid, nx = mid;
	for (int i = 1; i <= mid; i++)
		arr[i - 1] = MAP[ny + ydir[0] * i][nx];

	for (int d = 0; d < 3; d++)
		for (int i = 1; i <= mid; i++)
			MAP[ny + ydir[d] * i][nx + xdir[d] * i] = MAP[ny + ydir[d + 1] * i][nx + xdir[d + 1] * i];

	for (int i = 1; i <= mid; i++)
		MAP[ny][nx+xdir[3]*i] = arr[i-1];
}

void Colckwise(int sy, int sx)
{
	int temp_arr[29][29] = { 0, };

	for (int i = 0; i < mid; i++) {
		for (int j = 0; j < mid; j++) {
			temp_arr[j+sy][mid - 1 - i+sx] = MAP[i+sy][j+sx];
		}
	}

	for (int i = 0; i < mid; i++) {
		for (int j = 0; j < mid; j++) {
			MAP[i + sy][j + sx] = temp_arr[i + sy][j + sx];
		}
	}

}

void solution() {

	// 그룹 나누기
	Divgroup();

	// 조화로움 계산
	Beauty();
	// 가운데 반시계 회전
	Unclockwise();
	
	// 나머지 시계 회전
	Colckwise(0,0);
	Colckwise(0,mid+1);
	Colckwise(mid+1,0);
	Colckwise(mid+1,mid+1);

}

int main() {
    ios_base::sync_with_stdio(false);cin.tie(0);
	input();
	for (int i = 0; i < 4; i++) solution();
	cout << res << '\n';
}