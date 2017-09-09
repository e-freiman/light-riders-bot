#ifndef POSITION_H
#define POSITION_H

#include <utility>
#include <iterator>
#include "GameEnums.h"

class Position final : public std::pair<int, int>
{
public:
	//This iterator does not fully follow an iterator contract, because in our case this is redundant
	class Iterator : public std::iterator<std::forward_iterator_tag, Position>
	{
		const Position& source;
		Directions current_dir;
	public:
		Iterator(const Position& source, Directions direction);
		void operator++();
		Position operator*();
		bool operator!=(const Iterator& rhs) const;
	};

public:
	Position(int first, int second);
	Position();

	Iterator begin() const;
	Iterator end() const;
};

Directions ToDirection(const Position& begin, const Position& end);

#endif