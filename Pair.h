#pragma once
template<typename A, typename B>
class Pair
{
private:
	A key;
	B value;
public:
	Pair() = default;
	Pair(const A&, const B&);
	const A& getKey() const;
	const B& getValue() const;
};
