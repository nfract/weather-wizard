#include <iostream>
#include <map>
#include <string>
#include <array>
#include <vector>
#include <queue>
using namespace std;
struct PrecipitationNormal {
    string stationCode;
    float normalAverageForYear;
    float normalAverageByMonth[12] = {};
};

class MaxHeap {
    vector <PrecipitationNormal> big;
    int size = 0;
    public:
    void MaxHeapify(int i) {
        int max = i;
        int right = 2 * i + 2;
        int left = 2 * i + 1;
        if ((big[right].normalAverageForYear / 100) > (big[max].normalAverageForYear / 100) && right < size) {
            max = right;
        }
        if ((big[left].normalAverageForYear / 100) > (big[max].normalAverageForYear / 100) && left < size ) {
            max = left;
        }
        if (i != max) {
            PrecipitationNormal lasting = big[i];
            big[i] = big[max];
            big[max] = lasting;
            MaxHeapify(max);
        }
    }
    PrecipitationNormal extractMax() {
        PrecipitationNormal lasting = big[0];
        big[0] = big[--size];
        MaxHeapify(0);
        return lasting; 
    }
    void insertMax(PrecipitationNormal blue) {
        big.push_back(blue);
        size++;
        for (int i  = size / 2 - 1; i >= 0; i--) {
            MaxHeapify(i);
        }
    }
    PrecipitationNormal kthSmallest(int k) {
        for(PrecipitationNormal yellow : big) {
            if(size == k && yellow.normalAverageForYear > big[0].normalAverageForYear) {
                continue;
            }
            insertMax(yellow);
            if(size > k) {
                extractMax();
            }
        }
        print big;
    }
};

class MinHeap {
    vector<PrecipitationNormal> little;
    int size = 0;
    public:
    void MinHeapify(int i){
        int min = i;
        int right = 2 * i + 2;
        int left = 2 * i + 1;
        if ((little[right].normalAverageForYear / 100) < (little[min].normalAverageForYear / 100) && right < size) {
            min = right;
        }
        if (little[left].normalAverageForYear < little[min].normalAverageForYear && left < size) {
            min = left;
        }
        if (i != min) {
            PrecipitationNormal lasting = little[i];
            little[i] = little[min];
            little[min] = lasting;
            MinHeapify(0);
        }
    }
    PrecipitationNormal extractMin() {
        PrecipitationNormal lasting = little[0];
        little[0] = little[--size];
        MinHeapify(0);
        return lasting;
    }
    void insertMin(PrecipitationNormal blue) {
        little.push_back(blue);
        size++;
        for(int i = size / 2 - 1; i >= 0; i--) {
           MinHeapify(i);
        }
    }
    PrecipitationNormal kthlargest(int k) {
        for(PrecipitationNormal red : little) {
            if(size == k && red.normalAverageForYear < little[0].normalAverageForYear) {
                continue;
            }
            insertMin(red);
            if (size > k) {
                extractMin();
            }
        }
        print little;
    }
};