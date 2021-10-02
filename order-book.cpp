#include <bits/stdc++.h>
#include "order-book.h"
using namespace std;

long long int sToLL(string s) {
    long long int num = 0;
    int n = s.length();
    for(int i = 0; i < n; i++) {
        num *= 10;
        num += s[i]-'0';
    }
    return num;
}
int sToI(string s) {
    int num = 0;
    int n = s.length();
    for(int i = 0; i < n; i++) {
        num *= 10;
        num += s[i]-'0';
    }
    return num;
}

float sToF(string s) {
    float num = 0;
    int n = s.length(), j = 0;
    for(int i = 0; i < n; i++) {
        if(s[i] == '.') {
            j = i;
            continue;
        }
        num *= 10;
        num += s[i]-'0';
    }
    num = num/(float)pow(10,j);
    return num;
}
// returns the quantity of traded shares for this order!
pair<int,float> OrderBook::executeOrder(const string s) {
    char operationType = s[0], orderType;
    int quantity = 0, ptr = 2;
    long long int orderId = 0;
    float tradePrice = 0.00;
    string temp;

    // converting order from string type to int type
    while(s[ptr] != ',') {
        temp.push_back(s[ptr++]);
    }
    orderId = sToLL(temp);

    orderType = s[++ptr];

    //converting quantity from string type to int type
    temp = "";ptr+=2;
    while(s[ptr] != ',') {
        temp.push_back(s[ptr++]);
    }
    quantity = sToI(temp);

    // converting tradeprice from string type to floating type
    temp = "";ptr++;
    while((ptr) < (int)s.length()) {
        temp.push_back(s[ptr++]);
    }
    tradePrice = sToF(temp);
    if(operationType == 'A') {
        if(orderType == 'B') {
            return executeBuyOrder(tradePrice, quantity, orderId);
        } else {
            return executeSellOrder(tradePrice, quantity, orderId);
        }
    } else if(operationType == 'X') {
        if(orderType == 'B') {
            removeBuyOrder(orderId);
        } else removeSellOrder(orderId);
        return make_pair(0, 0.00);
    } else {
        if(orderType == 'B') {
            return modifyBuyOrder(tradePrice, quantity, orderId);
        } else return modifySellOrder(tradePrice, quantity, orderId);
    }
}

pair<int,float> OrderBook::executeBuyOrder(const float buyPrice, const int quantity, const long long int orderId) {
    // will be executed at the lowest selling price possible
    int executionCount{0};
    float avgPrice{0};
    while(executionCount < quantity && (this->sellOrders).size() > 0 && (this->sellOrders).begin()->first <= buyPrice) {
        int availableQuantity = (this->sellOrders).begin()->second[0].first;
        long long int availableOrderId = (this->sellOrders).begin()->second[0].second;
        float availablePrice = (this->sellOrders).begin()->first;
        int previouslyExecutedCount = executionCount;
        bool isRemoved = false, isModified = false;
        if(availableQuantity > quantity - executionCount) {
            (this->sellOrders).begin()->second[0].first -= (quantity - executionCount);
            availableQuantity -= (quantity - executionCount);
            avgPrice += availablePrice * (quantity - executionCount);
            executionCount = quantity;
            isModified = true;
        } else {
            executionCount += availableQuantity;
            avgPrice += availableQuantity * availablePrice;
            (this->sellOrders).begin()->second.pop_front();
            if(availableQuantity != 0) {
                isRemoved = true;
            }
            if((this->sellOrders).begin()->second.size() == 0) {
                (this->sellOrders).erase((this->sellOrders).begin());
            }
        }
        if(executionCount - previouslyExecutedCount > 0) {
            cout<<"T,"<<executionCount - previouslyExecutedCount<<","<<availablePrice<<endl;
        }
        if(isRemoved) {
            cout<<"X,"<<availableOrderId<<",S,"<<availableQuantity<<","<<availablePrice<<endl;
        }
        if(isModified) {
            cout<<"M,"<<availableOrderId<<",S,"<<availableQuantity<<","<<availablePrice<<endl;
        }
    }
    if(executionCount != 0) {
        avgPrice = avgPrice/(float)executionCount;
    }
    if(executionCount != quantity) {
        insertBuyOrder(buyPrice, quantity - executionCount, orderId);
    } else cout<<"X,"<<orderId<<",B,"<<quantity<<","<<buyPrice<<endl;
    return make_pair(executionCount, avgPrice);
}

pair<int,float> OrderBook::executeSellOrder(const float sellPrice, const int quantity, const long long int orderId) {
    // will be executed at the highest buying price possible
    int executionCount{0};
    float avgPrice{0};
    while(executionCount < quantity && (this->buyOrders).size() > 0 && (this->buyOrders).rbegin()->first >= sellPrice) {
        int availableQuantity = (this->buyOrders).rbegin()->second[0].first;
        long long int availableOrderId = (this->buyOrders).rbegin()->second[0].second;
        float availablePrice = (this->buyOrders).rbegin()->first;
        int previouslyExecutedCount = executionCount;
        bool isRemoved = false, isModified = false;
        if(availableQuantity > quantity - executionCount) {
            (this->buyOrders).rbegin()->second[0].first -= (quantity - executionCount);
            availableQuantity -= (quantity - executionCount);
            avgPrice += availablePrice * (quantity - executionCount);
            executionCount = quantity;
        } else {
            executionCount += availableQuantity;
            avgPrice += availableQuantity * availablePrice;
            (this->buyOrders).rbegin()->second.pop_front();
            if(availableQuantity != 0) {
                isRemoved = true;
            }
            if((this->buyOrders).rbegin()->second.size() == 0) {
                this->buyOrders.erase(--(this->buyOrders.end()));
            }
        }
        if(executionCount - previouslyExecutedCount > 0) {
            cout<<"T,"<<executionCount - previouslyExecutedCount<<","<<availablePrice<<endl;
        }
        if(isRemoved) {
            cout<<"X,"<<availableOrderId<<",B,"<<availableQuantity<<","<<availablePrice<<endl;
        }
        if(isModified) {
            cout<<"M,"<<availableOrderId<<",B,"<<availableQuantity<<","<<availablePrice<<endl;
        }
    }
    if(executionCount != 0) {
        avgPrice = avgPrice/(float)executionCount;
    }
    if(executionCount != quantity) {
        insertSellOrder(sellPrice, quantity - executionCount, orderId);
    } else cout<<"X,"<<orderId<<",S,"<<quantity<<","<<sellPrice<<endl;
    return make_pair(executionCount, avgPrice);
}

void OrderBook::insertBuyOrder(const float buyPrice, const int quantity, const long long int orderId) {
    (this->buyOrders[buyPrice]).push_back(make_pair(quantity,orderId));
    this->orderPriceMap[orderId] = buyPrice;
    return ;
}

void OrderBook::insertSellOrder(const float sellPrice, const int quantity, const long long int orderId) {
    (this->sellOrders[sellPrice]).push_back(make_pair(quantity, orderId));
    this->orderPriceMap[orderId] = sellPrice;
    return ;
}

void OrderBook::removeBuyOrder(const long long int orderId) {
    float buyPrice = this->orderPriceMap[orderId];
    if(!buyPrice) {
        return ;
    }
    int size = (this->buyOrders[buyPrice]).size();
    for(int i = 0; i < size; i++) {
        if(this->buyOrders[buyPrice][i].second == orderId) {
            if(i == 0) {
                this->buyOrders[buyPrice].pop_front();
            } else {
                this->buyOrders[buyPrice][i] = make_pair(0,0);
            }
        }
    }
}

void OrderBook::removeSellOrder(const long long int orderId) {
    float sellPrice = this->orderPriceMap[orderId];
    if(!sellPrice) {
        return ;
    }
    int size = (this->sellOrders[sellPrice]).size();
    for(int i = 0; i < size; i++) {
        if(this->sellOrders[sellPrice][i].second == orderId) {
            if(i == 0) {
                this->sellOrders[sellPrice].pop_front();
            } else {
                this->sellOrders[sellPrice][i] = make_pair(0,0);
            }
        }
    }
}

pair<int,float> OrderBook::modifyBuyOrder(const float buyPrice, const int quantity, const long long int orderId) {
    removeBuyOrder(orderId);
    removeSellOrder(orderId); // earlier it could have been a sell order also, and now modified to buy order! That is why removing it from everywhere
    return executeBuyOrder(buyPrice, quantity, orderId);
}

pair<int,float> OrderBook::modifySellOrder(const float sellPrice, const int quantity, const long long int orderId) {
    removeSellOrder(orderId);
    removeSellOrder(orderId);
    return executeSellOrder(sellPrice, quantity, orderId);
}

void OrderBook::getCurrentState() {
    cout<<endl<<"Current State of Order Book is as follows:"<<endl;
    for(auto itr = (this->sellOrders).begin(); itr != (this->sellOrders).end(); itr++) {
        int size = (itr->second).size(), quantity{0};
        for(int i = 0; i < size; i++) {
            quantity += itr->second[i].first;
        }
        if(quantity != 0) {
            cout<<"S "<<quantity<<" "<<itr->first<<endl;
        }
    }
    cout<<endl;
    for(auto itr = (this->buyOrders).begin(); itr != (this->buyOrders).end(); itr++) {
        int size = (itr->second).size(), quantity{0};
        for(int i = 0; i < size; i++) {
            quantity += itr->second[i].first;
        }
        if(quantity != 0) {
            cout<<"B "<<quantity<<" "<<itr->first<<endl;
        }
    }
}