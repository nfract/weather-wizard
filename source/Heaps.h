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
    public:
    void MaxHeapify(struct PrecipitationNormal arr[], int size, int i) {
        int max = i;
        int right = 2 * i + 2;
        int left = 2 * i + 1;
        if ((arr[right].normalAverageForYear / 100) > (arr[max].normalAverageForYear / 100) && right < size) {
            max = right;
        }
        if ((arr[left].normalAverageForYear / 100) > (arr[max].normalAverageForYear / 100) && left < size ) {
            max = left;
        }
        if (i != max) {
            PrecipitationNormal lasting = arr[i];
            arr[i] = arr[max];
            arr[max] = lasting;
            MaxHeapify(arr, size, max);
        }
    }
    PrecipitationNormal extractMax(struct PrecipitationNormal arr[], int size) {
        PrecipitationNormal lasting = arr[0];
        arr[0] = arr[--size];
        MaxHeapify(arr, size, 0);
        return lasting; 
    }
    void BuildHeapMax(struct PrecipitationNormal arr[], int size) {
        for (int i  = size / 2 - 1; i >= 0; i--) {
            MaxHeapify(arr, size, i);
        }
    }
    vector<PrecipitationNormal> kthlargestMax(struct PrecipitationNormal arr[], int k, int size) {
        vector<PrecipitationNormal> vec;
        for(int i = 0; i < k; i++) {
            vec.push_back(extractMax(arr, size));
        }
        return vec;
    }
};

class MinHeap {
    public:
    void MinHeapify(struct PrecipitationNormal arr[], int size, int i){
        int min = i;
        int right = 2 * i + 2;
        int left = 2 * i + 1;
        if ((arr[right].normalAverageForYear / 100) < (arr[min].normalAverageForYear / 100) && right < size) {
            min = right;
        }
        if (arr[left].normalAverageForYear < arr[min].normalAverageForYear && left < size) {
            min = left;
        }
        if (i != min) {
            PrecipitationNormal lasting = arr[i];
            arr[i] = arr[min];
            arr[min] = lasting;

            MinHeapify(arr, size, 0);
        }
    }
    PrecipitationNormal extractMin(struct PrecipitationNormal arr[], int size) {
        PrecipitationNormal lasting = arr[0];
        arr[0] = arr[--size];
        MinHeapify(arr, size, 0);
        return lasting;
    }
    void BuildHeapMin(struct PrecipitationNormal arr[], int size) {
        for (int i  = size / 2 - 1; i >= 0; i--) {
            MinHeapify(arr, size, i);
        }
    }
    vector<PrecipitationNormal> kthlargestMin(struct PrecipitationNormal arr[], int k, int size) {
        vector<PrecipitationNormal> vec;
        for(int i = 0; i < k; i++) {
            vec.push_back(extractMin(arr, size));
        }
        return vec;
    }
};