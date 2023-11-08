#include <iostream>
#define MAX_NUM 100001
using namespace std;

int n,arr[MAX_NUM];

void merge(int low, int mid, int high){
    int k=low, i=low, j=mid+1;
    int temp[MAX_NUM];

    while(i<=mid && j<=high){
        if(arr[i] < arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }

    while(i<=mid) temp[k++] = arr[i++];
    while(j<=high) temp[k++] = arr[j++];

    for(int c=low;c<=high;c++) arr[c] = temp[c];

}

void merge_sort(int low,int high)
{
    if(low < high){
        int mid = (low+high)/2;
        merge_sort(low,mid);
        merge_sort(mid+1,high);
        merge(low,mid,high);
    }
    
}
int main() {
    cin>>n;
    for(int i=1;i<=n;i++)cin>>arr[i];
    merge_sort(1,n);
    for(int i=1;i<=n;i++)cout<<arr[i]<<" ";
    return 0;
}