#include <iostream>
#include <stack>

using namespace std;
// LxL 체스판
// 빈칸, 함정, 벽, 체스판 밖 = 벽
// 연쇄 이동, 끝이 벽이라면 이동 X
struct soldier
{
	int y, x;
	int h,w;
	int health;
	int is_alive = 1;
}sol[31];

int L, N , Q;
int MAP_info[41][41], MAP[41][41],ans;
int ydir[] = {-1,0,1,0};
int xdir[] = { 0,1,0,-1 };

void input()
{
	cin >> L >> N >> Q;;
	for (int i = 1; i <= L; i++)
		for (int j = 1; j <= L; j++)
			cin >> MAP_info[i][j];

	for (int i = 1; i <= N; i++)
	{
		int y, x, h, w, k;
		cin >> y >> x >> h >> w >> k;
		sol[i] = { y,x,h-1,w-1,k };

		for (int r = y; r < y + h; r++)
			for (int c = x; c < x + w; c++)
				MAP[r][c] = i;
	}
}

bool range_check(int y, int x)
{
	if (y<1 || x<1 || y>L || x>L) return false;
	return true;
}

void delete_map(int id)
{
	soldier now = sol[id];
	for (int y = now.y; y <= now.y + now.h; y++)
		for (int x = now.x; x <= now.x + now.w; x++)
			MAP[y][x] = 0;
}
// 현재 아이디를 한칸 옮긴다.
bool move_and_check_damage(int temp_id, int dir)
{
	soldier now = sol[temp_id];
	for (int y = now.y; y <= now.y + now.h; y++)
		for (int x = now.x; x <= now.x + now.w; x++)
			if (MAP_info[y + ydir[dir]][x + xdir[dir]] == 2) return false;

	delete_map(temp_id);
	

	sol[temp_id].y += ydir[dir];
	sol[temp_id].x += xdir[dir];

	for (int y = now.y; y <= now.y + now.h; y++) {
		for (int x = now.x; x <= now.x + now.w; x++) {
			MAP[y + ydir[dir]][x + xdir[dir]] = temp_id;
			if (MAP_info[y][x] == 1)
			{
				ans++;
				sol[temp_id].health--;
			}
		}
	}
	
	if (sol[temp_id].health == 0) delete_map(temp_id);
	return true;
}

void move(int id, int dir)
{
	int DAT[31] = { 0, };
	int sy, sx, ly, lx;
	DAT[id] = 1;
	sy = ly = sol[id].y;
	sx = lx = sol[id].x;
	
	if (!sol[id].is_alive)return;

	// 0:상, 1:우, 2:하, 3:좌
	if (dir % 2 == 0)
	{
		lx = lx + sol[id].w;
		if (dir == 2) sy += sol[id].h;
		stack<int> s;
		s.push(id);
		for (int nowx = sx; nowx <= lx; nowx++)
		{
			while (1)
			{
				int nowy = sy + ydir[dir];
				// 다음이 0이면 이동 가능, 멈춘다.
				if (MAP[nowy][nowx] == 0)break;
				// 벽이다(범위)
				if (!range_check(nowy, nowx))return;
				// 벽이다(실제벽)
				if (MAP_info[nowy][nowx] == 2)return;
				// 이미 저장한 녀석
				if (DAT[MAP[nowy][nowx]])continue;
				DAT[MAP[nowy][nowx]] = 1;
				s.push(MAP[nowy][nowx]);
				sy = nowy;
				
			}
		}

		while (!s.empty())
		{
			int temp_id = s.top();
			if (!move_and_check_damage(temp_id, dir))break;
			s.pop();
		}
	}
	else
	{
		// 우, 좌
		// y값 고정, x 값 변경
		ly = ly + sol[id].h;
		if (dir == 3) sx = sol[id].h;
		stack<int> s;
		s.push(id);

		for (int nowy = sy; nowy <= ly; nowy++)
		{
			int nowx = sx + xdir[dir];
			if (MAP[nowy][nowx] == 0)break;
			if (!range_check(nowy, nowx))return;
			if (MAP_info[nowy][nowx] == 2)return;
			if (DAT[MAP[nowy][nowx]])continue;
			DAT[MAP[nowy][nowx]] = 1;
			s.push(MAP[nowy][nowx]);
		}

		while (!s.empty())
		{
			int temp_id = s.top();
			if (!move_and_check_damage(temp_id, dir))break;
			s.pop();
		}
	}

	

}

int main()
{
	ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
	input();
	for (int i = 1; i <= Q; i++)
	{
		int id, dir;
		cin >> id >> dir;
		move(id, dir);
	}
	cout << ans;

}