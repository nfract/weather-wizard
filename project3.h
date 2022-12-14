#include <iostream>
#include <map>
#include <string>
#include <array>
#include <vector>
using namespace std;
struct PrecipitationNormal {
    string stationCode;
    float normalAverageForYear;
    float normalAverageByMonth[12] = {};
}

class MaxHeap {
    int cap;
    public:
    void MaxHeap(int cap);
    void MaxHeapify(struct PrecipitationNormal arr[], int size, int i);
    float extractMax(struct PrecipitationNormal arr[], int size);
};

class MinHeap {
    int cap;
    public:
    void MinHeap(int cap);
    void MinHeapify(struct PrecipitationNormal arr[], int size, int i);
    float extractMin(struct PrecipitationNormal arr[], int size);
};


void MaxHeap::MaxHeapify(struct PrecipitationNormal arr[], int size, int i) {
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

PrecipitationNormal MaxHeap::extractMax(struct PrecipitationNormal arr[], int size)
{
    PrecipitationNormal lasting = arr[0];
	arr[0] = arr[--size];
	MaxHeapify(arr, size, 0);
	return lasting; 
}

void MinHeap::MinHeapify(struct PrecipitationNormal arr[], int size. int i){
    int min = i;
    int right = 2 * i + 2;
    int left = 2 * i + 1;
    if ((arr[right].normalAverageForYear / 100) < (arr[min].normalAverageForYear / 100) && right < size) {
        min = right;
    }
    if (arr[left] < arr[min] && left < size) {
        min = left;
    }
    if (i != min) {
        PrecipitationNormal lasting = arr[i];
        arr[i] = arr[min];
        arr[min] = lasting;

        MinHeapify(arr, size, 0);
    }
}



PrecipitationNormal MinHeap::extractMin(struct PrecipitationNormal arr[], int size) {
    PrecipitationNormal lasting = arr[0];
    arr[0] = arr[--size];
    MinHeapify(arr, size, 0);
    return lasting;
}

void BuildHeapMax(struct PrecipitationNormal arr[], int size) {
    for (int i  = n / 2 - 1; i >= 0; i++) {
        MaxHeapify(arr, size, i);
    }

}

void BuildHeapMin(struct PrecipitationNormal arr[], int size) {
    for (int i  = n / 2 - 1; i >= 0; i++) {
        MinHeapify(arr, size, i);
    }

}

int main() {
    vector<PrecipitationNormal> maxP;
    vector<PrecipitationNormal> minP;
    struct PrecipitationNormal ArrMax[maxP.size()];
    struct PrecipitationNormal ArrMin[minP.size()];
    BuildHeapMax(ArrMax, ArrMax.size());
    BuildHeapMin(ArrMin, ArrMin.size());
    MinHeap min(ArrMin.size());
    MaxHeap max(ArrMax.size());
    return 0;
}