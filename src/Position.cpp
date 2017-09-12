#include "Position.h"
#include <assert.h>

using namespace std;

Position::Position(int first, int second)
	: pair<int, int>(first, second)
{
}

Position::Position()
	: pair<int, int>()
{

}

Position::Iterator::Iterator(const Position& source, Directions direction)
	: source{ source }, current_direction{ direction }
{
}

void Position::Iterator::operator++()
{
	switch (current_direction)
	{
	case Directions::UP:
		current_direction = Directions::LEFT;
		break;
	case Directions::LEFT:
		current_direction = Directions::DOWN;
		break;
	case Directions::DOWN:
		current_direction = Directions::RIGHT;
		break;
	case Directions::RIGHT:
		current_direction = Directions::NONE;
		break;
	case Directions::NONE:
		throw out_of_range("Overrun");
	default:
		throw out_of_range("Unknown direction");
	}
}

Position Position::Iterator::operator*() const
{
	switch (current_direction)
	{
	case Directions::UP:
		return Position(source.first, source.second - 1);
	case Directions::LEFT:
		return Position(source.first - 1, source.second);
	case Directions::DOWN:
		return Position(source.first, source.second + 1);
	case Directions::RIGHT:
		return Position(source.first + 1, source.second);
	case Directions::NONE:
		throw out_of_range("Attempt to access an element after the last");
	default:
		throw out_of_range("Unknown direction");
	}
}

bool Position::Iterator::operator!=(const Iterator& rhs) const
{
	return (source != rhs.source) || (current_direction != rhs.current_direction);
}

Position::Iterator Position::begin() const
{
	return Iterator(*this, Directions::UP);
}

Position::Iterator Position::end() const
{
	return Iterator(*this, Directions::NONE);
}

Directions ToDirection(const Position& begin, const Position& end)
{
	int distance = abs(begin.first - end.first) + abs(begin.second - end.second);

	assert(distance == 0 || distance == 1);

	if (begin.first < end.first) return Directions::RIGHT;
	if (begin.first > end.first) return Directions::LEFT;
	if (begin.second < end.second) return Directions::DOWN;
	if (begin.second > end.second) return Directions::UP;

	return Directions::NONE;
}

