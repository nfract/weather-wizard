#pragma once

#include "../formats/PrecipitationNormal.h"

#include <map>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <iostream>

class MaxHeap 
{
    std::vector<PrecipitationNormal> big;
    int size = 0;
public:
    void MaxHeapify(int i) 
    {
        int max = i;
        int right = 2 * i + 2;
        int left = 2 * i + 1;
        if ((big[right].normalAverageForYear) > (big[max].normalAverageForYear) && right < size)
        {
            max = right;
        }

        if ((big[left].normalAverageForYear) > (big[max].normalAverageForYear) && left < size) 
        {
            max = left;
        }

        if (i != max) 
        {
            PrecipitationNormal lasting = big[i];
            big[i] = big[max];
            big[max] = lasting;
            MaxHeapify(max);
        }
    }

    PrecipitationNormal extractMax() 
    {
        PrecipitationNormal lasting = big[0];
        big[0] = big[--size];
        MaxHeapify(0);
        return lasting;
    }

    PrecipitationNormal Max()
    {
        return big[0];
    }

    void insertMax(PrecipitationNormal blue) 
    {
        big.push_back(blue);
        size++;

        for (int i = size / 2 - 1; i >= 0; i--) 
        {
            MaxHeapify(i);
        }
    }

    int Size()
    {
        return size;
    }

    bool Empty()
    {
        return size == 0;
    }
};

class MinHeap 
{
    std::vector<PrecipitationNormal> little;
    int size = 0;
public:
    void MinHeapify(int i) 
    {
        int min = i;
        int right = 2 * i + 2;
        int left = 2 * i + 1;

        if ((little[right].normalAverageForYear) < (little[min].normalAverageForYear) && right < size) 
        {
            min = right;
        }

        if (little[left].normalAverageForYear < little[min].normalAverageForYear && left < size) 
        {
            min = left;
        }

        if (i != min) 
        {
            PrecipitationNormal lasting = little[i];
            little[i] = little[min];
            little[min] = lasting;
            MinHeapify(0);
        }
    }

    PrecipitationNormal extractMin() 
    {
        PrecipitationNormal lasting = little[0];
        little[0] = little[--size];
        MinHeapify(0);
        return lasting;
    }

    PrecipitationNormal Min()
    {
        return little[0];
    }

    void insertMin(PrecipitationNormal blue) 
    {
        little.push_back(blue);
        size++;
        for (int i = size / 2 - 1; i >= 0; i--) 
        {
            MinHeapify(i);
        }
    }

    int Size() 
    {
        return size;
    }
    
    bool Empty()
    {
        return size == 0;
    }
};