#include <iostream>
#include <queue>
#define SIZE 51
using namespace std;

// 1,1부터 시작
// 기절 산타 or 격자 밖 산타 움직임 x
// 두 칸 거린 (r1-r2)^2 + (c1-c2)^2

// 루돌프는 탈락하지 않은 가장 가까운 산타로 1칸 이동
// 가장 가까운 산타 2명 이상이면 r,c 큰 산타한테 이동
// 8방향 이동 가능

// 산타는 1번부터 순서대로 움직임
// 루돌프에게 가장 가까워지는 방향으로 1칸 이동
// 다른 산타, 게임 밖 움직일 수 X
// 움직일 수 있는 칸이 없으면 움직이지 않음
// 루돌프로부터 가까워질 수 없다면 움직이지 않음
// 상우하좌 순위로 맞춰 움직임

// 충돌
// 산타와 루돌프가 같은칸에 있으면 충돌
// 루돌프가 움직여서 충돌 -> 산타 C점 획득, 루돌프가 이동한 방향으로 C칸 밀려남
// 산타가 움직여서 충돌 -> D 점수 획득, 자신이 이동한 반대방향으로 D칸 밀려남
// 움직이는 동안은 고려하지 않고 바로 그 자리로 이동함
// 밀려난 위치가 게임판 밖 -> 게임 탈락
// 밀려난 위치 다른 산타 -> 상호작용

// 상호작용
// 루돌프와 충돌 후 산타 이동하며 발생함
// 산타가 미리 있으면 그 산타는 1칸 해당 방향으로 밀려남
// 그 옆에 산타가 있다면 연쇄적으로 1칸씩 밀려나는 것 반복
// 게임판 밖으로 밀려난 경우 탈락

// 기절
// 산타 = 루돌프와 충돌 후 기절
// 현재가 k턴이면 k+1턴까지 기절
// k+2 턴부터 정상

// 게임 종료
// P명의 산타 코두 탈락
// M턴 모두 움직임
// 아직 탈락하지 않은 산타 1점씩 추가 부여

struct Node
{
	int y, x;
	int point = 0;
	int stun_turn = 0;
	int range_out = 0;
}santa[31];

struct Node2
{
	int y, x;
};

int N, M, P, C, D, t = 1, cnt = 0;

int ydir[] = { -1,0,1,0,1,-1,1,-1,1 };
int xdir[] = { 0,1,0,-1,-1,1, 1,-1 };

Node2 roodolph;

int visited[SIZE][SIZE];
int MAP[SIZE][SIZE];

void input()
{
	cin >> N >> M >> P >> C >> D;
	cin >> roodolph.y >> roodolph.x;

	for (int i = 1; i <= P; i++)
	{
		int idx;
		cin >> idx;
		cin >> santa[idx].y >> santa[idx].x;
		MAP[santa[idx].y][santa[idx].x] = idx;
	}
};

int get_dist(int ry, int rx, int sy, int sx)
{
	return (ry - sy) * (ry - sy) + (rx - sx) * (rx - sx);
}

bool range_check(int y, int x)
{
	if (y<1 || x<1 || y>N || x>N) return false;
	return true;
}

void Interaction(int origin, int sy, int sx, pair<int, int> dir)
{

	// sy , sx : 산타가 밀려나서 안착할 장소
	int dy = dir.first;
	int dx = dir.second;
	int ny = sy + dy;
	int nx = sx + dx;

	// ny, nx부터 쭉 확인, 범위 초과하거나 0이 나올때 까지
	while (MAP[ny][nx] > 0 && range_check(ny, nx))
	{
		ny += dy; nx += dx;
	}

	// 마지막 위치는 범위를 초과했거나, 0인 지역
	// 범위를 초과한 지역의 이전 산타는 탈락시키고, 그 지역부터 다시 시작
	if (!range_check(ny, nx))
	{
		ny -= dy; nx -= dx;
		santa[MAP[ny][nx]].range_out = 1;
		MAP[ny][nx] = 0;
	}

	// 여기부터는 초기 확인 지역(sy+dy, sx+dx)까지 확인하며, 내 이전을 나로 옮긴다.
	// 초기 ny nx는 무조건 0이다
	// 범위를 초과해서 내 전을 0으로 만들었거나
	// 0으로 끝났기 때문
	while (!(ny == sy && nx == sx))
	{

		int prevY = ny - dy;
		int prevX = nx - dx;
		if (!range_check(prevY, prevX))break;

		int idx = MAP[prevY][prevX];

		// 내 이전에 아무도 없다 ->? 내 위치로 옮길 산타가 없음 
		if (idx == 0) {
			MAP[ny][nx] = 0;
			break;
		}

		MAP[ny][nx] = idx;
		santa[idx].y = ny;
		santa[idx].x = nx;

		ny = prevY;
		nx = prevX;
	}

	// 산타를 준비된 sy, sx에 안착시킨다.
	MAP[sy][sx] = origin;
	santa[origin].y = sy;
	santa[origin].x = sx;
}

void collision(int idx, int v, pair<int, int> dir)
{
	int ny = roodolph.y + (dir.first * v);
	int nx = roodolph.x + (dir.second * v);

	MAP[santa[idx].y][santa[idx].x] = 0;

	if (!range_check(ny, nx))
	{
		cnt++;
		santa[idx].range_out = 1;
		return;
	}

	if (MAP[ny][nx] != 0) Interaction(idx, ny, nx, dir);
	else
	{
		MAP[ny][nx] = idx;
		santa[idx].y = ny;
		santa[idx].x = nx;
	}

}

void move_roodolph()
{
	int MIN = 21e8, ty = -1, tx = -1;
	int ry = roodolph.y;
	int rx = roodolph.x;

	for (int i = 1; i <= P; i++)
	{
		Node now = santa[i];
		if (now.range_out == 1) continue;
		int dist = get_dist(now.y, now.x, roodolph.y, roodolph.x);
		if (dist > MIN)continue;
		if (dist < MIN || (now.y > ty) || (now.y >= ty) && (now.x > tx))
		{
			MIN = dist;
			ty = now.y;
			tx = now.x;
		}
	}

	if (ty == -1)return;

	pair<int, int> dir = { 0, 0 };
	if (ty < ry) { dir.first = -1; }
	else if (ty > ry) { dir.first = 1; }

	if (tx < rx) { dir.second = -1; }
	else if (tx > rx) { dir.second = 1; }

	if (dir.first == 0 && dir.second == 0) return;

	roodolph.y += dir.first;
	roodolph.x += dir.second;


	if (MAP[roodolph.y][roodolph.x] != 0)
	{
		santa[MAP[roodolph.y][roodolph.x]].point += C;
		santa[MAP[roodolph.y][roodolph.x]].stun_turn = t + 2;
		collision(MAP[roodolph.y][roodolph.x],C,dir);
	}

}

void move_santa()
{
	int ry = roodolph.y;
	int rx = roodolph.x;

	for (int i = 1; i <= P; i++)
	{
		Node now = santa[i];
		if (now.range_out) continue;
		if (now.stun_turn > t) continue;
		if (now.stun_turn == t) santa[i].stun_turn = 0;

		int dist = get_dist(now.y, now.x, ry, rx);
		int ty = -1;
		int tx = -1;
		int dir;

		for (int j = 0; j < 4; j++)
		{
			int ny = now.y + ydir[j];
			int nx = now.x + xdir[j];

			if (!range_check(ny, nx))continue;
			if (MAP[ny][nx]!=0) continue;

			int dist2 = get_dist(ny, nx, ry, rx);
			if (dist > dist2)
			{
				dist = dist2;
				ty = ny;
				tx = nx;
				dir = j;
			}

		}

		if (ty == -1) continue;
		else if (ty == ry && tx == rx)
		{
			santa[i].point += D;
			santa[i].stun_turn = t + 2;

			if (dir == 0)dir = 2;
			else if (dir == 1)dir = 3;
			else if (dir == 3)dir = 1;
			else if (dir == 2) dir = 0;

			collision(i,D,{ydir[dir],xdir[dir]});
		}
		else
		{
			MAP[now.y][now.x] = 0;
			MAP[ty][tx] = i;
			santa[i].y = ty;
			santa[i].x = tx;
		}
	}
};

void bonus_point()
{
	for (int i = 1; i <= P; i++)
	{
		if (!santa[i].range_out)santa[i].point++;
	}
}


int main()
{
	ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
	input();
	while (t <= M)
	{
		move_roodolph();
		move_santa();
		bonus_point();
		if (cnt == P)break;
		t++;
	}

	for (int i = 1; i <= P; i++)
	{
		cout << santa[i].point << " ";
	}
	return 0;
}