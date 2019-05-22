
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <chrono>
#include <string>

template <typename E>
void swap(E *tab, int size, int a, int b)
{
	E tmp = tab[a];
	tab[a] = tab[b];
	tab[b] = tmp;
}

template <typename E>
int partition(E *tab,int size)
{
	int i = 0;
	int j = size-1;
	int tmp = (int)(size / 2);
	E pivot = tab[tmp-1];
	do
	{
		while (tab[i] < pivot) i++;
		while (tab[j] > pivot) j--;
		if (i <= j)
		{
			swap(tab,size, i, j);
			i++;
			j--;
		}
	} while (i <= j);
	return i;
}

template <typename E>
void quickSort(E *tab, int size, int left, int right)
{
	int i = left;
	int j = right;
	E pivot = tab[(int)(left + right) / 2];
	do
	{
		while (tab[i] < pivot) i++;
		while (tab[j] > pivot) j--;
		if (i <= j) {
			swap(tab, size, i, j);
			i++;
			j--;
	 	}
	} while (i <= j);
	
	if (left < j) quickSort(tab, size, left, j);
	if (right > i) quickSort(tab, size, i, right);
}



template <typename E>
void heap(E *tab,int size, int n)  
{
	for (int i = (int)(n / 2); i > 0; i--)
	{
		if (2 * i <= n)  // sprawdza czy jest syn w podanym zakresie
			if (tab[2 * i - 1] > tab[i - 1])
				swap(tab, size, i - 1, 2 * i - 1);
		if (2 * i + 1 <= n)     // sprawdza czy jest syn w podanym zakresie
			if (tab[2 * i] > tab[i - 1])
				swap(tab, size, i - 1, 2 * i);
	}
	
}

template <typename E>
void heapSort(E *tab, int size)
{
	int i = size;
	do
	{
		heap(tab,size, i);
		
		swap(tab, size, 0, i-1);
		i--;
	} while (i > 0);
}



template <typename E>
void split(E *tab, int size, E *tab1, int size1, E *tab2, int size2)
{
	for (int i = 0; i < size1; i++)
		tab1[i] = tab[i];
	for (int i = 0; i < size2; i++)
		tab2[i] = tab[size1 + i];

	delete[] tab;
}

template <typename E>
E* merge(E *tab1, int size1, E *tab2, int size2)
{
	E* tab = new E[size1 + size2];
	int i = 0, j = 0, n = 0;
	while (i < size1 and j < size2)
	{
		if (tab1[i] <= tab2[j])
		{
			tab[n] = tab1[i];
			i++; 
		}
		else
		{
			tab[n] = tab2[j];
			j++;
		}
		n++;
	}

	while (i < size1) 
	{
		tab[n] = tab1[i];
		i++;
		n++; 
	}
	while (j < size2)
	{
		tab[n] = tab2[j];
		j++;
		n++;
	}

	delete[] tab1;
	delete[] tab2;

	return tab;
}

template <typename E>
E* mergeSort(E *tab, int size)
{
	if (size > 1)
	{
		int tmp = (int)(size / 2);
		E *tab1 = new E[tmp];
		E* tab2 = new E[size - tmp];
		split(tab, size, tab1, tmp, tab2, size - tmp);
		tab1 = mergeSort(tab1, tmp);
		tab2 = mergeSort(tab2, size - tmp);
		tab = merge(tab1, tmp, tab2, size - tmp);
	}

	return tab;
}

template <typename E>
void introSort(E *tab, int size, int M)
{
	int i;
	if (size == 0)
	{
		std::string ex = "Zerowy rozmiar tablicy";
		throw ex;
	}
	if (size == 1 or size == 0) return;
	if (M <= 0) heapSort(tab, size);
	else
	{
		i = partition(tab,size);
		introSort(tab, i, M - 1);
		introSort(tab + i, size - i, M - 1);
	}
}



template <typename E>
std::chrono::duration<double> ExeTime(const int n, E** tabs, int size, E* (*sort)(E*, int))
{
	auto start = std::chrono::system_clock::now();
	for (int i = 0; i < n; i++)
	{
		tabs[i] = sort(tabs[i], size);
		//if ((i % (n / 10)) == 0) std::cout << '>';
	}
	auto end = std::chrono::system_clock::now();
	return end - start;
}

template <typename E>
std::chrono::duration<double> ExeTime(const int n, E** tabs, int size, int M, void (*sort)(E*, int, int))
{
	auto start = std::chrono::system_clock::now();
	for (int i = 0; i < n; i++)
	{
		sort(tabs[i], size, M);
	//	if ((i % (n / 10)) == 0) std::cout << '>';
	}
	auto end = std::chrono::system_clock::now();
	return end - start;
}

template <typename E>
std::chrono::duration<double> ExeTime(const int n, E** tabs, int size, int left, int right, void(*sort)(E*, int, int, int))
{
	auto start = std::chrono::system_clock::now();
	for (int i = 0; i < n; i++)
	{
		sort(tabs[i], size, left, right);
		//if ((i % (n / 10)) == 0) std::cout << '>';
	}
	auto end = std::chrono::system_clock::now();
	return end - start;
}

template <typename E>
void sprawdzaczka(E** tabs, int size, int n)
{
	for (int j = 0; j < n; j++)
		for (int i = 0; i < size - 1; i++)
			if (tabs[j][i + 1] < tabs[j][i])
			{
				std::string w = "Nieposortowane";
				throw w;
			}
}

template <typename E>
void RandElements(E** tabs, int n, int size)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < size; j++)
		{
			tabs[i][j] = rand() % RAND_MAX;
		}
}

template <typename E>
void PartlySorted(E** tabs, int n, int size, double part)
{
	srand((unsigned)time(NULL));
	int p = (int)(part * size / 100);
	int x = rand() % RAND_MAX;

	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i <= p - 1; i++)
		{
			tabs[j][i] = x + i;
		}
		for (int i = p; i < size; i++)
		{
			tabs[j][i] = rand() % RAND_MAX;
		}
	}
}

template <typename E>
void InvSorted(E** tabs, int n, int size)
{
	int x;
	srand((unsigned)time(NULL));
	//std::cout << "Generownie tablicy elementow posortowanych w odrotnej kolejnosci" << std::endl;

	x = rand() % RAND_MAX;
	for (int j = 0; j < n; j++)
		for (int i = 0; i < size; i++)
		{
			tabs[j][i] = x + size - i;
		}
}

template <typename E>
void AllAlgorithm(E** tabs, int n, int size, void (* TypeTabIn)(E**, int, int))
{
	std::chrono::duration<double> T;

	TypeTabIn(tabs, n, size);
	T = ExeTime(n, tabs, size, 0, size - 1, quickSort);
	std::cout << "Czas wykonywania algorytmu QuickSort: " << T.count() << std::endl;
	try
	{
		sprawdzaczka(tabs, size, n);
	}
	catch (std::string w)
	{
		std::cout << "Wyjątek: " << w << std::endl;
	}

	TypeTabIn(tabs, n, size);
	T = ExeTime(n, tabs, size, mergeSort);
	std::cout << "Czas wykonywania algorytmu MergeSort: " << T.count() << std::endl;
	try
	{
		sprawdzaczka(tabs, size, n);
	}
	catch (std::string w)
	{
		std::cout << "Wyjątek: " << w << std::endl;
	}

	TypeTabIn(tabs, n, size);
	int M = (2 * (int)log2(size));
	T = ExeTime(n, tabs, size, M, introSort);
	std::cout << "Czas wykonywania algorytmu IntroSort: " << T.count() << std::endl;
	try
	{
		sprawdzaczka(tabs, size, n);
	}
	catch (std::string w)
	{
		std::cout << "Wyjątek: " << w << std::endl;
	}
	
}

template <typename E>
void AllAlgorithm(E** tabs, int n, int size, double part, void(*TypeTabIn)(E**, int, int, double))
{	
	std::chrono::duration<double> T;

	TypeTabIn(tabs, n, size,part);
	T = ExeTime(n, tabs, size, 0, size - 1, quickSort);
	std::cout << "Czas wykonywania algorytmu QuickSort: " << T.count() << std::endl;
	try
	{
		sprawdzaczka(tabs, size, n);
	}
	catch (std::string w)
	{
		std::cout << "Wyjątek: " << w << std::endl;
	}
	

	TypeTabIn(tabs, n, size,part);
	T = ExeTime(n, tabs, size, mergeSort);
	std::cout << "Czas wykonywania algorytmu MergeSort: " << T.count() << std::endl;
	try
	{
		sprawdzaczka(tabs, size, n);
	}
	catch (std::string w)
	{
		std::cout << "Wyjątek: " << w << std::endl;
	}
	
	TypeTabIn(tabs, n, size,part);
	int M = (2 * (int)log2(size));
	T = ExeTime(n, tabs, size, M, introSort);
	std::cout << "Czas wykonywania algorytmu IntroSort: " << T.count() << std::endl;
	try
	{
		sprawdzaczka(tabs, size, n);
	}
	catch (std::string w)
	{
		std::cout << "Wyjątek: " << w << std::endl;
	}
	
}


template <typename E>
void QSAllTabs(E** tabs, int n, int size, double *parts)
{
	std::chrono::duration<double> T;
	RandElements(tabs, n, size);
	T = ExeTime(n, tabs, size, 0, size - 1, quickSort);
	std::cout << T.count() << std::endl;
	try
	{
		sprawdzaczka(tabs, size, n);
	}
	catch (std::string w)
	{
		std::cout << "Wyjątek: " << w << std::endl;
	}

	for (int i = 0; i < 6; i++)
	{
		PartlySorted(tabs, n, size, parts[i]);
		T = ExeTime(n, tabs, size, 0, size - 1, quickSort);
		std::cout << T.count() << std::endl;
		try
		{
			sprawdzaczka(tabs, size, n);
		}
		catch (std::string w)
		{
			std::cout << "Wyjątek: " << w << std::endl;
		}
	}
	InvSorted(tabs, n, size);
	T = ExeTime(n, tabs, size, 0, size - 1, quickSort);
	std::cout << T.count() << std::endl;
	try
	{
		sprawdzaczka(tabs, size, n);
	}
	catch (std::string w)
	{
		std::cout << "Wyjątek: " << w << std::endl;
	}
}

template <typename E>
void MSAllTabs(E** tabs, int n, int size, double *parts)
{
	std::chrono::duration<double> T;
	RandElements(tabs, n, size);
	T = ExeTime(n, tabs, size, mergeSort);
	std::cout << T.count() << std::endl;
	try
	{
		sprawdzaczka(tabs, size, n);
	}
	catch (std::string w)
	{
		std::cout << "Wyjątek: " << w << std::endl;
	}

	for (int i = 0; i < 6; i++)
	{
		PartlySorted(tabs, n, size, parts[i]);
		T = ExeTime(n, tabs, size, mergeSort);
		std::cout << T.count() << std::endl;
		try
		{
			sprawdzaczka(tabs, size, n);
		}
		catch (std::string w)
		{
			std::cout << "Wyjątek: " << w << std::endl;
		}
	}

	InvSorted(tabs, n, size);
	T = ExeTime(n, tabs, size, mergeSort);
	std::cout << T.count() << std::endl;
	try
	{
		sprawdzaczka(tabs, size, n);
	}
	catch (std::string w)
	{
		std::cout << "Wyjątek: " << w << std::endl;
	}
}

template <typename E>
void ISAllTabs(E** tabs, int n, int size, double *parts)
{
	int M = (2 * (int)log2(size));
	std::chrono::duration<double> T;
	RandElements(tabs, n, size);
	T = ExeTime(n, tabs, size, M, introSort);
	std::cout << T.count() << std::endl;
	try
	{
		sprawdzaczka(tabs, size, n);
	}
	catch (std::string w)
	{
		std::cout << "Wyjątek: " << w << std::endl;
	}

	for (int i = 0; i < 6; i++)
	{
		PartlySorted(tabs, n, size, parts[i]);
		T = ExeTime(n, tabs, size, M, introSort);
		std::cout << T.count() << std::endl;
		try
		{
			sprawdzaczka(tabs, size, n);
		}
		catch (std::string w)
		{
			std::cout << "Wyjątek: " << w << std::endl;
		}
	}

	InvSorted(tabs, n, size);
	T = ExeTime(n, tabs, size, M, introSort);
	std::cout << T.count() << std::endl;
	try
	{
		sprawdzaczka(tabs, size, n);
	}
	catch (std::string w)
	{
		std::cout << "Wyjątek: " << w << std::endl;
	}
}

int main()
{
    typedef int Typ;
	const int n = 100;
	int size;
	double parts[6] = { 25,50,75,95,99,99.7 };
	Typ * tabs[n];
	
	

	std::cout << "Liczba elementow: ";
	std::cin >> size;

	for (int i = 0; i < n; i++)
		tabs[i] = new Typ[size];


	std::cout << "QuickSort " << std::endl;

	QSAllTabs(tabs, n, size, parts);
	std::cout << "MergeSort " << std::endl;
	MSAllTabs(tabs, n, size, parts);
	std::cout << "IntroSort " << std::endl;
	ISAllTabs(tabs, n, size, parts);


	for (int i = 0; i < size; i++) std::cout << tabs[0][i] << ' ';
		


	system("PAUSE");
	return 0;
}