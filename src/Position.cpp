#include "Position.h"

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
	: source{ source }, current_dir{ direction }
{
}

void Position::Iterator::operator++()
{
	switch (current_dir)
	{
	case Directions::UP:
		current_dir = Directions::LEFT;
		break;
	case Directions::LEFT:
		current_dir = Directions::DOWN;
		break;
	case Directions::DOWN:
		current_dir = Directions::RIGHT;
		break;
	case Directions::RIGHT:
		current_dir = Directions::NONE;
		break;
	case Directions::NONE:
		throw out_of_range("Overrun");
	default:
		throw out_of_range("Unknown direction");
	}
}

Position Position::Iterator::operator*()
{
	switch (current_dir)
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
	return (source != rhs.source) || (current_dir != rhs.current_dir);
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
	if (begin.first < end.first) return Directions::RIGHT;
	if (begin.first > end.first) return Directions::LEFT;
	if (begin.second < end.second) return Directions::DOWN;
	if (begin.second > end.second) return Directions::UP;

	return Directions::NONE;
}

