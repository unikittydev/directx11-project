#pragma once

struct Entity final
{
	const int id;

	Entity(int id) : id(id)
	{

	}

	friend bool operator<(const Entity& lhs, const Entity& rhs)
	{
		return lhs.id < rhs.id;
	}

	friend bool operator>(const Entity& lhs, const Entity& rhs)
	{
		return lhs.id > rhs.id;
	}
	
	friend bool operator==(const Entity& lhs, const Entity& rhs)
	{
		return lhs.id == rhs.id;
	}

	friend bool operator<=(const Entity& lhs, const Entity& rhs)
	{
		return lhs.id <= rhs.id;
	}

	friend bool operator>=(const Entity& lhs, const Entity& rhs)
	{
		return lhs.id >= rhs.id;
	}
};
