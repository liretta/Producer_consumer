#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <algorithm>
#include <atomic>

std::atomic <int> curOperationNumber = 0; //for checking function call order
const int MAX_CONTAINER_SIZE = 40;
const int MAX_THREADS_COUNT = 10;
const int EXCHANGE_OPERATION_COUNT = 100;

void run_demo_process();

void rem(std::mutex & mt, std::vector<int>& vc);

void add(std::mutex& mt, std::vector<int>& vc);

int main()
{
	
	run_demo_process();
	system("pause");
	return 0;
}

void run_demo_process()
{
	std::mutex mt;
	std::vector<int> vc(MAX_CONTAINER_SIZE); //container with resources
	

	//fill container
	for(auto i =0; i<MAX_CONTAINER_SIZE; ++i)
	{
		vc[i] = 0;
	}

	//create holders for consumer and producer threads
	std::vector <std::thread> v_consumer_thread, v_producer_thread;

	//create threads and run the process off data using
	for(auto i = 0; i<MAX_THREADS_COUNT; ++i)
	{
		v_consumer_thread.push_back(std::move(std::thread(add, std::ref(mt), std::ref(vc))));
		v_producer_thread.push_back(std::move(std::thread(rem, std::ref(mt), std::ref(vc))));
	}

	
	//release producer and consumer threads 
	for(auto &it: v_consumer_thread)
	{
		if(it.joinable())
		{
			it.join();
		}
	}

	for(auto &it: v_producer_thread)
	{
		if(it.joinable())
		{
			it.join();
		}
	}
	
	std::cout << "Start vector size = " << MAX_CONTAINER_SIZE << ", result vector size = " << vc.size() << std::endl;
	std::cout << "\nResult vector :\n";
	for (auto &it : vc)
	{
		std::cout << it << ' ';
	}
	std::cout << std::endl;
};

//always delete last element
void rem(std::mutex & mt, std::vector<int>& vc)
{
	for (int i = 0; i < EXCHANGE_OPERATION_COUNT; ++i)
	{
		std::lock_guard<std::mutex> lock(mt);
		if (vc.size() == 0)
		{
			//std::cout <<"skip\n";
			continue;
		}
		std::cout << "#" << curOperationNumber++ <<" Remove " << *(vc.end()-1) << std::endl;
		vc.erase(vc.end()-1);
	}
}

//add new element
void add(std::mutex& mt, std::vector<int>& vc)
{
	for (int i = 0; i < EXCHANGE_OPERATION_COUNT; ++i)
	{
		std::lock_guard<std::mutex> lock(mt);
		if (vc.size() <= MAX_CONTAINER_SIZE - 1)
		{
			std::cout << "#" <<curOperationNumber++ << " Add " << i << std::endl;
			vc.push_back(i);
		}
	}
}