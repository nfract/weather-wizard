#pragma once

#include <cinttypes>
#include <iostream>
#include <optional>
#include <bitset>
#include <vector>
#include <list>

#define HASH_POWER_CONSTANT      31
#define HASH_ASCII_INDEX         32
#define BUCKET_STATUS_OPEN       0
#define BUCKET_STATUS_FILLED     1
#define BUCKET_STATUS_REHASH     1
#define BUCKET_STATUS_ACCESS_BIT 0
#define BUCKET_STATUS_FLAG_BIT   1

template <typename V>
struct Bucket
{
	std::string key;
	V value;
};

template <typename V>
class HashMap
{
public:
	HashMap(size_t buckets = 10, float multiplier = 1.15f)
		: entries(0), buckets(buckets), loadFactor(0.0f), multiplier(multiplier)
	{
		table.resize(buckets);
		statuses.resize(buckets);

		// Fill the entire array with empty bucket status
		for (size_t i = 0; i < statuses.size(); i++)
		{
			statuses[i].reset(BUCKET_STATUS_ACCESS_BIT); // Bucket open or closed
			statuses[i].reset(BUCKET_STATUS_FLAG_BIT);   // Bucket needs rehashing
		}
	}

	~HashMap()
	{
		statuses.empty();
		table.empty();
	}

	void Insert(const std::string& key, const V& value)
	{
		Bucket<V> bucket;
		bucket.key = key;
		bucket.value = value;

		entries++;
		SudoInsert(bucket);
	}

	intmax_t Find(const std::string& key) const
	{
		size_t index = Reduce(HashFunction(key));
		if (statuses[index].test(BUCKET_STATUS_ACCESS_BIT) == BUCKET_STATUS_OPEN)
			return -1;

		const Bucket<V>& bucket = table[index];
		bool found = false;

		while (index < table.size() - 1)
		{
			std::cout << table[index].key << ", " << key << "\n";

			if (table[index].key == key) return index;
			if (statuses[index].test(BUCKET_STATUS_ACCESS_BIT) == BUCKET_STATUS_OPEN) return -1;

			index = Reduce(++index);
		}

		// We did not find the key at all
		return -1;
	}

	V Search(const std::string& key) const
	{
		intmax_t index = Find(key); V defaultv;

		// Return the value or the default value
		return index != -1 ? table[index].value : defaultv;
	}

	V Search(intmax_t index) const
	{
		V defaultv;

		// Return the value or the default value
		return index != -1 ? table[index].value : defaultv;
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
		std::cout << "Space Ratio (Higher -> Less wasted space): " << (float) entries / buckets << "\n";
		std::cout << "Underlying array size: " << table.size() << "\n";

		for (const auto& bucket : table)
		{
			std::cout << bucket.key << "\n";
		}
	}

private:
	void SudoInsert(const Bucket<V>& bucket)
	{
		loadFactor = static_cast<float>(entries) / buckets;
		if (loadFactor >= 0.75)
		{
			RebuildTable();
		}

		size_t index = Reduce(HashFunction(bucket.key));
		if (statuses[index].test(BUCKET_STATUS_ACCESS_BIT) == BUCKET_STATUS_FILLED)
		{
			// Resolve the collision using linear probing
			index = ResolveCollision(index);
			table[index] = bucket;
			statuses[index].flip(BUCKET_STATUS_ACCESS_BIT);
		}
		else
		{
			statuses[index].flip(BUCKET_STATUS_ACCESS_BIT);
			table[index] = bucket;
		}
	}

	void RebuildTable()
	{
		buckets *= multiplier;
		loadFactor = static_cast<float>(entries) / buckets;
		table.resize(buckets);
		statuses.resize(buckets);
		
		// Mark all filled buckets with the rehashing bit
		for (auto& status : statuses)
		{
			if (status.test(BUCKET_STATUS_ACCESS_BIT) == BUCKET_STATUS_FILLED) status.flip(BUCKET_STATUS_FLAG_BIT);
			status.reset(BUCKET_STATUS_ACCESS_BIT); // Reopen all buckets
		}

		// Rehash and insert all waiting buckets
		for (size_t i = 0; i < statuses.size(); i++)
		{
			// Re-insert buckets
			if (statuses[i].test(BUCKET_STATUS_FLAG_BIT) == BUCKET_STATUS_REHASH)
				SudoInsert(table[i]);

			// Reset the "needs hashing" flag bit
			statuses[i].reset(BUCKET_STATUS_FLAG_BIT);
		}
	}

	size_t ResolveCollision(size_t index)
	{
		size_t newIndex = index;

		while (statuses[newIndex].test(BUCKET_STATUS_ACCESS_BIT) == BUCKET_STATUS_FILLED)
		{
			newIndex = Reduce(++newIndex);
		}

		return newIndex;
	}

private:
	size_t entries;
	size_t buckets;
	float  loadFactor;
	float  multiplier;

	std::vector<Bucket<V>> table;
	std::vector<std::bitset<2>> statuses;
};