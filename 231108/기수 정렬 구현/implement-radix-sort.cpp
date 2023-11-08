#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n,arr[100000],MAX=-21e8;
    cin>>n;

    for(int i=0;i<n;i++){
        cin>>arr[i];
        if(arr[i]>MAX)MAX=arr[i];
    }

    for(int i=0;i<6;i++){
        vector<int> temp[10];
        int d = 1;
        for(int j=0;j<n;j++){
            int num = (arr[j]/d)%10;
            temp[num].push_back(arr[j]);
        }
        d*=10;
        int idx=0;
        for(int j=0;j<10;j++){
            int cursize = temp[j].size();
            for(int p=0;p<cursize;p++){
                arr[idx++]=temp[j][p];
            }
        }

    }

    for(int i=0;i<n;i++){
        cout<<arr[i]<<" ";
    }

    return 0;
}