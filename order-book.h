#pragma once

class OrderBook {
    private:
        std::map<float, std::deque<std::pair<int, long long int>>> buyOrders;
        std::map<float, std::deque<std::pair<int, long long int>>> sellOrders;
        std::map<long long int, float>orderPriceMap;
    public:
        std::pair<int,float> executeOrder(const std::string s);
        std::pair<int,float> executeBuyOrder(const float buyPrice, const int quantity, const long long int orderId);
        std::pair<int,float> executeSellOrder(const float sellPrice, const int quantity, const long long int orderId);
        void insertBuyOrder(const float buyPrice, const int quantity, const long long int orderId);
        void insertSellOrder(const float sellPrice, const int quantity, const long long int orderId);
        void removeBuyOrder(const long long int orderId);
        void removeSellOrder(const long long int orderId);
        std::pair<int,float> modifyBuyOrder(const float buyPrice, const int quantity, const long long int orderId);
        std::pair<int,float> modifySellOrder(const float sellPrice, const int quantity, const long long int orderId);
        void getCurrentState();
};