#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>

std::mutex m;
std::condition_variable cond;

using namespace std::literals::chrono_literals;

void odd()
{
	for (int i = 1; i <= 100; i += 2)
	{
		std::cout << "From odd: " << i << std::endl;
		cond.notify_one();
		std::unique_lock<std::mutex> guard(m);
		cond.wait(guard);
	}
}

void even()
{
	for (int i = 0; i <= 100; i += 2)
	{
		std::cout << "From even: " << i << std::endl;
		cond.notify_one();
		std::unique_lock<std::mutex> guard(m);
		cond.wait(guard);
	}
}

int main()
{
	std::thread t2(even);
	std::this_thread::sleep_for(100ms);
	std::thread t1(odd);

	t1.join();
	t2.join();
}
