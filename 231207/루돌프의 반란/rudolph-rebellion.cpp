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
	int stun_turn=0;
	int range_out=0;
}santa[31];

struct Node2
{
	int y, x;
};

int N, M, P, C, D,t=1,cnt=0;

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
		cin>> santa[idx].y >> santa[idx].x;
		MAP[santa[idx].y][santa[idx].x] = idx;
	}
};

int get_dist(int ry,int rx,int sy,int sx)
{
	return (ry - sy) * (ry - sy) + (rx - sx) * (rx - sx);
}

void collision_rooldolph_to_santa(int roodolph_y,int roodolph_x,pair<int,int> dir)
{
	
	int idx = MAP[roodolph_y][roodolph_x];
	int ny = santa[idx].y + (dir.first)*C;
	int nx = santa[idx].x + (dir.second)*C;
	MAP[santa[idx].y][santa[idx].x] = 0;
	while (1)
	{

		if (ny <1 || nx<1 || ny>N || nx>N)
		{
			MAP[santa[idx].y][santa[idx].x] = 0;
			santa[idx].range_out = 1;
			cnt++;
			break;
		}

		int next_santa_idx = MAP[ny][nx];
		santa[idx].y = ny;
		santa[idx].x = nx;
		MAP[ny][nx] = idx;

		if (next_santa_idx == 0) break;

		idx = next_santa_idx;
		ny = santa[next_santa_idx].y + dir.first;
		nx = santa[next_santa_idx].x + dir.second;
	}
	
}

void collision_santa_to_roodolph(int idx, int sy, int sx, int dir)
{
	if (dir % 2 == 0)dir = 2 - dir;
	else if (dir == 1) dir = 3;
	else if (dir == 3)dir = 1;
	int ny = sy + (ydir[dir]*D);
	int nx = sx + (xdir[dir]*D);
	MAP[santa[idx].y][santa[idx].x] = 0;

	while (1)
	{
		if (ny <1 || nx<1 || ny>N || nx>N)
		{
			MAP[santa[idx].y][santa[idx].x] = 0;
			santa[idx].range_out = 1;
			cnt++;
			break;
		}

		int next_santa_idx = MAP[ny][nx];

        santa[idx].y = ny;
		santa[idx].x = nx;
		MAP[ny][nx] = idx;

		if (next_santa_idx == 0) break;
		
		idx = next_santa_idx;
		
		santa[next_santa_idx].y = ny = santa[next_santa_idx].y + ydir[dir];
		santa[next_santa_idx].x = nx = santa[next_santa_idx].x + xdir[dir];
	}
}

void move_roodolph()
{
	int MIN = 21e8, ty=-1, tx=-1;
	for (int i = 1; i <= P; i++)
	{
		Node now = santa[i];
		if (now.range_out) continue;
		int dist = get_dist(now.y,now.x,roodolph.y,roodolph.x);

		if (dist > MIN)continue;
		if (dist < MIN || (now.y > ty) || (now.y == ty) && (now.x > tx))
		{
			MIN = dist;
			ty = now.y;
			tx = now.x;
		}
	}

	pair<int, int> dir = { 0, 0};
	if (ty < roodolph.y) { roodolph.y--; dir.first = -1;}
	else if (ty > roodolph.y) { roodolph.y++; dir.first = 1;}

	if (tx < roodolph.x) { roodolph.x--; dir.second = -1;}
	else if (tx > roodolph.x) { roodolph.x++; dir.second = 1; }

	// 산타 있음 -> 충돌
	if (roodolph.y == ty && roodolph.x == tx)
	{
		int ry = roodolph.y;
		int rx = roodolph.x;
		santa[MAP[ry][rx]].point += C;
		santa[MAP[ry][rx]].stun_turn = t + 2;
		collision_rooldolph_to_santa(ry,rx, dir);
	}
}

void move_santa()
{
	int ry = roodolph.y;
	int rx = roodolph.x;
	for (int i = 1; i <= P; i++)
	{
		Node now = santa[i];
		if (now.range_out) { continue; }
		if (now.stun_turn > t) { continue; }
		if (now.stun_turn == t) santa[i].stun_turn = 0;
		int dist = get_dist(now.y, now.x, ry,rx);
		int ty = -1;
		int tx = -1;
		int dir;

		for (int j = 0; j < 4; j++)
		{
			int ny = now.y + ydir[j];
			int nx = now.x + xdir[j];
			
			if (MAP[ny][nx]) { continue; }
			if (ny<1 || nx<1 || ny>N || nx>N)continue;
			
			int dist2 = get_dist(ny, nx, ry, rx);
			if (dist > dist2)
			{
				dist = dist2;
				ty = ny;
				tx = nx;
				dir = j;
			}

		}

		// 움직이지 않는다.
		if (ty == -1)
		{
			continue;
		}
		// 루돌프와 부딪혔다
		else if (ty == ry && tx == rx)
		{
			santa[i].point += D;
			santa[i].stun_turn = t + 2;
			collision_santa_to_roodolph(i, ty, tx, dir);
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
		if (santa[i].range_out==0)santa[i].point++;
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
		if (cnt == P)break;
		bonus_point();
		t++;
	}

	for (int i = 1; i <= P; i++)
	{
		cout << santa[i].point << " ";
	}
	return 0;
}