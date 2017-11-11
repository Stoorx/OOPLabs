/****************************************
*		StatisticMultiset class	header	*
*****************************************/

#pragma once
#include <set>
#include "SplayTree.h"

class StatisticMultiset {
public:
	StatisticMultiset();
	// Добавляет число в набор статистику.
	void addNum(int num);
	// Добавляет число в набор и не пересчитывает статистику.
	void addNumLazy(int num);
	// Максимальное число в наборе.
	int getMax();
	// Минимальное число в наборе.
	int getMin();
	// Среднее арифметическое всего набора.
	float getAvg();
	// Кол-во чисел в наборе меньше заданного порога.
	int getCountUnder(float threshold);
	// Кол-во чисел в наборе больше заданного порога.
	int getCountAbove(float threshold);
private:
	std::multiset<int> _data;
	SplayTree<float, int> _aboveCache;
	SplayTree<float, int> _belowCache;
	bool _isActual;
	int _maxValue;
	int _minValue;
	float _avgValue;
	void _statisticUpdate();
};