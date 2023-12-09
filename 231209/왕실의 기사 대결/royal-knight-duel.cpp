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
	int dmg=0;
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

bool move_and_check_damage(int origin, int temp_id, int dir)
{
	soldier now = sol[temp_id];
	delete_map(temp_id);

	sol[temp_id].y += ydir[dir];
	sol[temp_id].x += xdir[dir];
	for (int y = now.y; y <= now.y + now.h; y++) {
		for (int x = now.x; x <= now.x + now.w; x++) {
			MAP[y + ydir[dir]][x + xdir[dir]] = temp_id;
			if (temp_id == origin)continue;
			if (MAP_info[y + ydir[dir]][x + xdir[dir]] == 1)
				sol[temp_id].dmg++;
		}
	}
	if (sol[temp_id].health == sol[temp_id].dmg)
	{
		sol[temp_id].is_alive = 0;
		delete_map(temp_id);
	}
	return true;
}

bool is_valid(int id,int dir)
{
	soldier now = sol[id];
	for (int y = now.y; y <= now.y + now.h; y++)
	{
		for (int x = now.x; x <= now.x + now.w; x++)
		{
			int ny = y + ydir[dir];
			int nx = x + xdir[dir];
			if (!range_check(ny, nx))return false;
			if (MAP_info[y + ydir[dir]][x + xdir[dir]] == 2) return false;
		}
	}
	return true;
}

void range_set(int arr[4], int id, int dir)
{
	//arr[4] = {sy,sx,ly,lx}
	soldier now = sol[id];
	if (dir %2==0)
	{
		if (dir == 2) arr[0] = arr[2] = sol[id].y + sol[id].h;
		else arr[0] = arr[2] = sol[id].y;
		arr[1] = sol[id].x;
		arr[3] = arr[1] + sol[id].w;
	}
	else
	{
		if (dir == 1) arr[1] = arr[3] = now.x + now.w;
		else arr[1] = arr[3] = now.x;
		arr[0] = sol[id].y;
		arr[2] = arr[0] + sol[id].h;
	}
}

void move(int id, int dir)
{
	int DAT[31] = { 0, };
	int r[4];
	DAT[id] = 1;
	range_set(r, id, dir);
	if (!sol[id].is_alive)return;

	if (dir % 2 == 0)
	{
		stack<int> s;
		s.push(id);
		for (int nowx = r[1]; nowx <= r[3]; nowx++)
		{
			while (1)
			{
				int nowy = r[0] + ydir[dir];
				// 다음이 0이면 이동 가능, 멈춘다.
				if (MAP[nowy][nowx] == 0)break;
				// 벽이다(범위)
				if (!range_check(nowy, nowx))return;
				// 벽이다(실제벽)
				if (MAP_info[nowy][nowx] == 2)return;
				// 이미 저장한 녀석
				if (DAT[MAP[nowy][nowx]])continue;
				if (!is_valid(MAP[nowy][nowx],dir))return;
				DAT[MAP[nowy][nowx]] = 1;
				s.push(MAP[nowy][nowx]);
				r[0] = nowy;
				
			}
		}

		while (!s.empty())
		{
			int temp_id = s.top();
			if (!move_and_check_damage(id,temp_id, dir))break;
			s.pop();
		}
	}
	else
	{
		stack<int> s;
		s.push(id);

		for (int nowy = r[0]; nowy <= r[2]; nowy++)
		{
			while (1)
			{
				int nowx = r[1] + xdir[dir];
				if (MAP[nowy][nowx] == 0)break;
				if (!range_check(nowy, nowx))return;
				if (MAP_info[nowy][nowx] == 2)return;
				if (DAT[MAP[nowy][nowx]])continue;
				if (!is_valid(MAP[nowy][nowx], dir))return;
				DAT[MAP[nowy][nowx]] = 1;
				s.push(MAP[nowy][nowx]);
				r[1] = nowx;
			}
		}

		while (!s.empty())
		{
			int temp_id = s.top();
			if (!move_and_check_damage(id,temp_id, dir))break;
			s.pop();
		}
	}
}

void PRINT()
{
	for (int i = 1; i <= L; i++)
	{
		for (int j = 1; j <= L; j++)
		{
			cout << MAP[i][j];
		}cout << '\n';
	}cout << '\n';
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

	for (int i = 1; i <= N; i++) if (sol[i].is_alive) ans += sol[i].dmg;
	cout << ans;

}