#include <bits/stdc++.h>
#include "order-book.h"
using namespace std;

int main() {
    freopen("input.txt", "r", stdin);  // read input from input.txt    
    freopen("output.txt", "w", stdout); // write output to output.txt
    OrderBook order;
    string s;
    while(getline(cin,s)) {
        pair<int,float> pi= order.executeOrder(s);
    }
    order.getCurrentState();
    return 0;
}