#include <iostream>
#include <algorithm>
#include <cstring>
#include <ctime>
using namespace std;

int main(){
    int n;
    while(true){
        cout << "Input the number of numbers to process: ";
        cin >> n;
        if(n>1 && n<31) break;
    }
    int num[n];

    cout << "Input the numbers to be processed:\n";
    for(int i=0; i<n; i++){
        cin >> num[i];
        if(num[i]<=0 || num[i]>100000){
            cout << "Input dismissed";
            i = 0;
            continue;
        }
    }

    clock_t start = clock();
    bool prime[100001];
    memset(prime, true, sizeof(prime));
    prime[1] = false;
    for(int i=2; i<100001; i++)
        for(int j=2; i*j<100001; j++)
            if(prime[i]) prime[i*j] = false;
    sort(num, num+n);
    for(int i=0; i<n-1; i++){
        cout << "Number of prime numbers between " << num[i] << " " << num[i+1]<<": ";
        int cnt = 0;
        for(int j=num[i]; j<=num[i+1]; j++)
            if(prime[j]) cnt++;
        cout << cnt << "\n";
    }
    clock_t end = clock();
    cout << "Total execution time using c++ is " << (double)(end-start)/CLOCKS_PER_SEC << " seconds!\n";
}