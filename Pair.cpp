#include "Pair.h"

template <typename A, typename B>
Pair<A, B>::Pair(const A& k, const B& v) : key{ k }, value{ v } {}

template <typename A, typename B>
const A& Pair<A, B>::getKey() const
{
	return this->key;
}

template <typename A, typename B>
const B& Pair<A, B>::getValue() const
{
	return this->value;
}