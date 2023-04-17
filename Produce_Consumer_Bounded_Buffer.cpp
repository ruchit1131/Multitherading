#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<string>

std::mutex m;
std::condition_variable empty, full;
int buffer[10];
int count = 0;
int MAX = 10;

using namespace std::literals::chrono_literals;

void print()
{
	for (int i = 0; i < MAX; i++)
	{
		std::cout << buffer[i] << " ";
	}
	std::cout << std::endl;
}

void producer()
{
	while (buffer[0] != 0)
	{
		std::unique_lock<std::mutex> guard(m);
		full.wait(guard, []() { return !(count == MAX); });

		int a = rand() % 200;
		std::cout << "Produced: " << a << std::endl;
		buffer[count] = a;
		count++;
		print();
		guard.unlock();
		empty.notify_one();
		//std::this_thread::sleep_for(10ms);

	}

}

void consumer()
{
	while (buffer[0] != 0)
	{
		std::unique_lock<std::mutex> guard(m);
		empty.wait(guard, []() { return !(count == 0); });

		//Exit condition
		if (buffer[0] == 0 && count == 1)
			break;
		std::cout << "Consumed: " << buffer[count - 1] << std::endl;
		buffer[count - 1] = -1;
		count--;
		print();
		guard.unlock();
		full.notify_one();
		//std::this_thread::sleep_for(10ms);

	}
}

int main()
{
	std::memset(buffer, -1, sizeof(buffer));
	std::thread t1(producer);
	std::thread t2(consumer);
	t1.join();
	t2.join();
	print();
}
