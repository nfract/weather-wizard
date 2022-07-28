#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <list>

#define HASH_POWER_CONSTANT    31
#define HASH_ASCII_INDEX       32

template <typename V>
struct Bucket
{
	std::string key;
	V value;

	std::list<Bucket<V>*> neighbors;
};

template <typename V>
class HashMap
{
public:
	HashMap(size_t buckets = 10, size_t multiplier = 2)
		: entries(0), buckets(buckets), loadFactor(0.0f), multiplier(multiplier)
	{
		table.resize(buckets);
	}

	~HashMap()
	{
		for (auto* bucket : table)
			delete bucket;

		table.empty();
	}

	void Insert(const std::string& key, const V& value)
	{
		Bucket<V>* bucket = new Bucket<V>;
		bucket->key = key;
		bucket->value = value;

		entries++;
		SudoInsert(bucket);
	}

	bool Find(const std::string& key) const
	{
		size_t index = Reduce(HashFunction(key));
		if (table[index] == nullptr)
			return false;

		Bucket<V>* bucket = table[index];
		bool found = false;
		if (bucket->key != key)
		{
			// Iterate over the linked list of neighbors
			for (auto* neighbor : bucket->neighbors)
			{
				if (neighbor->key == key)
				{
					found = true;
					break;
				}
			}
		}
		else
		{
			found = true;
		}

		return found;
	}

	size_t HashFunction(const std::string& str) const
	{
		// Covered in Module 7 lecture on hash tables
		size_t hash = 0;
		for (size_t i = 0; i < str.size(); i++)
			hash += (str[i] - HASH_ASCII_INDEX) * pow(HASH_POWER_CONSTANT, str.size() - i);

		return hash;
	}

	size_t Reduce(size_t hash) const
	{
		return hash % buckets;
	}

	void PrintStats() const
	{
		std::cout << "Entries: " << entries << "\n";
		std::cout << "Buckets: " << buckets << "\n";
		std::cout << "Load Factor: " << loadFactor << "\n";
		std::cout << "Underlying array size: " << table.size() << "\n";
		std::cout << "Table: " "\n";

		for (int i = 0; i < table.size(); i++)
		{
			if (table[i] == nullptr)
			{
				std::cout << "\tIndex: " << i << " Empty\n";
			}
			else
			{
				std::cout << "\tIndex: " << i << " Key: " << table[i]->key << " Value: " << table[i]->value << "\n";
				std::cout << "\t\tNeighbors: " << table[i]->neighbors.size() << "\n";
			}
		}
	}

private:
	void SudoInsert(Bucket<V>* bucket)
	{
		loadFactor = static_cast<float>(entries) / buckets;
		if (loadFactor >= 0.75)
		{
			RebuildTable();
		}

		size_t index = Reduce(HashFunction(bucket->key));
		if (table[index] != nullptr)
		{
			table[index]->neighbors.emplace_front(bucket);
		}
		else
		{
			table[index] = bucket;
		}
	}

	void RebuildTable()
	{
		// Test
		//buckets *= multiplier;
		//loadFactor = static_cast<float>(entries) / buckets;
		//table.resize(buckets);
	}

private:
	size_t entries;
	size_t buckets;
	float  loadFactor;
	size_t multiplier;

	std::vector<Bucket<V>*> table;
};