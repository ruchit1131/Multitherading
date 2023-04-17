#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>

/*

	#CONDITION VARIABLES PREDICATE
		while (!pred())
			cond.wait(guard);

		cond.wait(guard, [](){return pred();})



	#HOW JOIN() IS IMPLEMENTED INTERNALLY USING CONDITION VARIABLES
		condition_variable cond;
		mutex m;
		bool done = false;


		child( fn_ptr)
		{
			unique_lock<mutex> guard(m);
			fn_ptr();
			done = true;
			cond.signal();
		}


		join()
		{
			unique_lock<mutex> guard(m);
			while(!done)
				cond.wait(guard);
		}

		parent()
		{
		//spawn child()
		join();
		}

*/

bool is_even = true;
std::mutex m;
std::condition_variable cond;

void odd()
{
	for (int i = 1; i <= 100; i += 2)
	{
		std::unique_lock<std::mutex> guard(m);
		cond.wait(guard, []() {return !is_even; });

		//while (is_even)
		//	cond.wait(guard);

		std::cout << "From odd: " << i<< std::endl;
		is_even = !is_even;
		guard.unlock();
		cond.notify_one();
	}
}

void even()
{
	for (int i = 0; i <= 100; i += 2)
	{
		std::unique_lock<std::mutex> guard(m);
		cond.wait(guard, []() {return is_even; });

		//while (!is_even)
		//	cond.wait(guard);

		std::cout <<"From even: "<< i<<std::endl;
		is_even = !is_even;
		guard.unlock();
		cond.notify_one();
	}
}

int main()
{
	std::thread t1(odd);
	std::thread t2(even);


	t1.join();
	t2.join();
}
