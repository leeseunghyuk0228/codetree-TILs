#include <iostream>
#define MAX_NUM 100001
using namespace std;

int n;
int arr[MAX_NUM];

int find_pivot(int low, int high){
    if(high-low<2) return high;
    else{
        int mid = (high+low)/2;
        if((arr[low]>=arr[mid] && arr[low]<=arr[high])||(arr[low]>=arr[high] && arr[low]<=arr[mid])) return low;
        else if ((arr[mid]>=arr[low] && arr[mid]<=arr[high])||(arr[mid]>=arr[high] && arr[mid]<=arr[low])) return mid;
        else return high;
    }
}

void quick_sort(int low, int high){
    if(low < high){
        int pivot_idx = find_pivot(low,high);
        int temp = arr[high];
        arr[high] = arr[pivot_idx];
        arr[pivot_idx] = temp;

        int left = low-1;
        for(int right=low;right<high;right++){
            if(arr[right] < arr[high]){
                left++;
                int temp = arr[right];
                arr[right] = arr[left];
                arr[left] = temp;
            }
        }

        temp = arr[left+1];
        arr[left+1] = arr[high];
        arr[high] = temp;

        quick_sort(low, left);
        quick_sort(left+2, high);

    }
}
int main() {
    
    cin>>n;

    for(int i=1;i<=n;i++)cin>>arr[i];

    quick_sort(1,n);

    for(int i=1;i<=n;i++)cout<<arr[i]<<" ";

    return 0;
}