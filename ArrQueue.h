#pragma once



template<typename T>
class ArrQueue
{
private:

	T* Arr;
	int front, back;
	int max;

public:
		
	ArrQueue(int max) :max(max)
	{
		Arr = new T[max+1];
		front = 0;
		back = 0;
	}

	bool Enqueue(const T item)
	{
		if (isFull())
			return false;

		else
		{
			back = (back + 1) % (max + 1);
			Arr[back] = item;
			return true;
		}
	}


	bool Dequeue(const T& item)
	{
		if (isEmpty())
		{
			return false;
		}
		else
		{
			front = (front + 1) % (max + 1);
			item = Arr[front];
			return true;
		}

	}

	bool isEmpty()
	{
		return(front == back);
	}

	bool isFull()
	{
		return(front == (back + 1) % (max + 1));

	}

};