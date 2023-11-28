#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int q, n, m;
int firstBoxidx[10], lastBoxidx[10];
int beltCondition[10];
struct box { int weight, before, next, beltnum; };
unordered_map<int, struct box> um;

void input() {
    int t;
    cin >> t >> n >> m;
    vector<int> boxidx;
    vector<int> boxweight;


    for (int i = 0; i < n; i++)
    {
        int idx; cin >> idx; boxidx.push_back(idx);
    }
    for (int i = 0; i < n; i++)
    {
        int w; cin >> w; boxweight.push_back(w);
    }

    for (int i = 0; i < m; i++) {
        int start = i * n / m;
        firstBoxidx[i] = boxidx[start];
        um[boxidx[start]] = { boxweight[start],-1,-1,i };
        lastBoxidx[i] = boxidx[start];
        for (int j = 1; j < n / m; j++) {
            int idx = i * n / m + j;
            lastBoxidx[i] = boxidx[idx];
            um[boxidx[idx - 1]].next = boxidx[idx];
            um[boxidx[idx]] = { boxweight[idx],boxidx[idx - 1],-1,i };
        }
    }
}

void unloading(int MaxWeight) {
    // m 개의 벨트의 맨 앞을 내린다
    // 만약 내리고 나서 벨트가 비었다면
    // firstBox / Lastbox 변경 해주어야 한다
    int sum = 0;
    for (int i = 0; i < m; i++) {
        if (beltCondition[i]) continue; // 고장
        if (firstBoxidx[i] == -1)continue; // 벨트 비었음
        box now = um[firstBoxidx[i]];

        if (now.weight <= MaxWeight) {
            // 뺄 수 있다.
            sum += now.weight;
            um[firstBoxidx[i]].weight = 0;
            firstBoxidx[i] = um[firstBoxidx[i]].next;
            if (um[firstBoxidx[i]].next == -1) lastBoxidx[i] = -1;
            else um[now.next].before = -1;
        }
        else {
            // 마지막 박스라면 아무것도 안함
            // 마지막 박스가 아닐 때만 한다
            if (now.next != -1)
            {
                // 내 뒤를 맨 앞으로
                um[now.next].before = -1;
                // 맨뒤를 맨 앞의 앞으로
                um[lastBoxidx[i]].next = firstBoxidx[i];
                // 나의 앞을 마지막 박스로 나의 뒤를 -1로
                um[firstBoxidx[i]].before = lastBoxidx[i];
                um[firstBoxidx[i]].next = -1;

                lastBoxidx[i] = firstBoxidx[i];
                firstBoxidx[i] = now.next;
                
            }
        }

    }
    cout << sum << '\n';
}
    
void removeBox(int Boxidx) {
    if(um.find(Boxidx)== um.end()) { cout << -1 << '\n'; return; }
    if (um[Boxidx].weight == 0) { cout << -1 << '\n'; return;}
    box now = um[Boxidx];
    um[Boxidx].weight = 0;
    // 혼자일 경우
    if (now.before == -1 && now.next == -1) {
        firstBoxidx[now.beltnum] = -1;
        lastBoxidx[now.beltnum] = -1;
    }
    // 맨 앞일 경우
    else if (now.before == -1) {
        firstBoxidx[now.beltnum] = now.next;
        um[now.next].before = -1;
    }
    // 맨 뒤일 경우
    else if (now.next == -1) {
        lastBoxidx[now.beltnum] = now.before;
        um[now.before].next = -1;
    }
    // 가운데 있을 경우
    else
    {
        um[now.before].next = now.next;
        um[now.next].before = now.before;
    }
    cout << Boxidx << '\n';
}

void checkBox(int Boxidx) {
    if (um.find(Boxidx) == um.end()) { cout << -1 << '\n'; return; }
    if (um[Boxidx].weight == 0) { cout << -1 << '\n'; return; }

    box now = um[Boxidx];
    cout << now.beltnum +1 << '\n';
    // 맨뒤
    if (now.next == -1) {
        // 나혼자
        if (now.before == -1) return;
        // 진짜 맨뒤
        else
        {
            um[firstBoxidx[now.beltnum]].before = Boxidx;
            um[lastBoxidx[now.beltnum]].next = -1;

            um[Boxidx].before = -1;
            um[Boxidx].next = firstBoxidx[now.beltnum];
            um[now.before].next = -1;
            
            firstBoxidx[now.beltnum] = Boxidx;
            lastBoxidx[now.beltnum] = now.before;

        }
    }
    else if (now.before == -1) return;
    else {
        um[firstBoxidx[now.beltnum]].before = lastBoxidx[now.beltnum];
        um[lastBoxidx[now.beltnum]].next = firstBoxidx[now.beltnum];
        um[now.before].next = -1;
        um[Boxidx].before = -1;

        firstBoxidx[now.beltnum] = Boxidx;
        lastBoxidx[now.beltnum] = now.before;

    }


}

void errorBelt(int b_num) {
    b_num--;
    if (beltCondition[b_num]==1) { cout << -1 << '\n';return;}
    cout << b_num + 1 << '\n';
    int nb_num = b_num;
    beltCondition[b_num] = 1;

    // 벨트 찾기
    while (1)
    {
        nb_num++;
        if (nb_num == m) nb_num = 0;
        if (beltCondition[nb_num]==0)break;
    }

    int start = firstBoxidx[b_num];
    while (1) {
        um[start].beltnum = nb_num;
        if (um[start].next == -1)break;
        start = um[start].next;
    }

    um[lastBoxidx[nb_num]].next = firstBoxidx[b_num];
    um[firstBoxidx[b_num]].before = lastBoxidx[nb_num];
    lastBoxidx[nb_num] = lastBoxidx[b_num];


}

int main() {
    cin >> q;
    int cmd;
    int value;
    for (int i = 0; i < q; i++) {
        if (i == 0)input();
        else {
            cin >> cmd >> value;
            if (cmd == 200)  unloading(value);
            else if (cmd == 300) removeBox(value);
            else if (cmd == 400) checkBox(value);
            else if (cmd == 500) errorBelt(value);

        };
       /* cout << "\n===" << i << " 번째 결과 확인하기 ===\n";
        cout << "가장 맨 앞 벡터\n";
        for (int j = 0; j < m; j++)
            cout << firstBoxidx[j] << " ";
        cout << "\n가장 맨 뒤 벡터\n";
        for (int j = 0; j < m; j++)
            cout << lastBoxidx[j] << " ";
        cout << '\n';
        cout << "\n===" << i << "번째 순서 확인하기 ===\n";
        for (int j = 0; j < m; j++) {
            int start = firstBoxidx[j];
            if(beltCondition[j])cout <<" B R O K E N \n";
            else {
                while (1) {

                    if (start == -1) {
                        cout << "END\n";
                        break;
                    }
                    cout << start << " -> ";
                    start = um[start].next;
                }
            }*/
       /* }*/
    };

}