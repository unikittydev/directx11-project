#pragma once
#include <chrono>

class Time
{
	friend class Game;

	using duration = std::chrono::duration<double>;
	using sys_clock = std::chrono::system_clock;
	using time_point = std::chrono::time_point<sys_clock, duration>;

private:
	static Time* s_instance;

	time_point init_time = std::chrono::time_point_cast<duration>(sys_clock::now());
	time_point t1 = std::chrono::time_point_cast<duration>(sys_clock::now());
	time_point t2 = std::chrono::time_point_cast<duration>(sys_clock::now());

	void UpdateTime()
	{
		t2 = std::chrono::time_point_cast<duration>(sys_clock::now());
	}

	void Finish()
	{
		t1 = t2;
	}
public:
	Time()
	{
		s_instance = this;
	}

	static double timeAsDouble()
	{
		const duration d = s_instance->t2 - s_instance->init_time;
		return d.count();
	}

	static float time()
	{
		return static_cast<float>(timeAsDouble());
	}

	static double deltaTimeAsDouble()
	{
		const duration d = s_instance->t2 - s_instance->t1;
		return d.count();
	}

	static float deltaTime()
	{
		return static_cast<float>(deltaTimeAsDouble());
	}
};