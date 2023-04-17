#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>

bool chance = true;
std::mutex m;
std::condition_variable cond;

int value = -1;

void producer()
{
	while(value != 10)
	{
		std::unique_lock<std::mutex> guard(m);
		if (!chance)
			cond.wait(guard);
		value = rand() % 20;
		std::cout << "From producer: " << value << std::endl;
		chance = !chance;
		guard.unlock();
		cond.notify_one();
	}
}

void consumer()
{
	while (value != 10)
	{
		std::unique_lock<std::mutex> guard(m);
		if (chance)
			cond.wait(guard);
		//using the value
		std::cout << "From consumer: " << value << std::endl;
		chance = !chance;
		value = -1;
		guard.unlock();
		cond.notify_one();
	}
}

int main()
{

	std::thread t1(consumer);
	std::thread t2(producer);

	t1.join();
	t2.join();
}
