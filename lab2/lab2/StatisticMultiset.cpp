/************************************
 *		StatisticMultiset class		*
 ************************************/


#include "StatisticMultiset.h"
#include "SplayTree.h"

StatisticMultiset::StatisticMultiset()
{
	_isActual = false;
}

void StatisticMultiset::addNum(int num)
{
	_isActual = false;
	_data.insert(num);
	_statisticUpdate();
}

void StatisticMultiset::addNumLazy(int num)
{
	_data.insert(num);
	_isActual = false;
}

int StatisticMultiset::getMax() 
{
	if (!_isActual)
		_statisticUpdate();
	return _maxValue;
}

int StatisticMultiset::getMin()
{
	if (!_isActual)
		_statisticUpdate();
	return _minValue;
}

float StatisticMultiset::getAvg()
{
	if (!_isActual)
		_statisticUpdate();
	return _avgValue;
}


int StatisticMultiset::getCountUnder(float threshold)
{
	if (_belowCache.Search(threshold))
		return _belowCache.Find(threshold);
	else {
		int counter = 0;

		for (auto it = _data.begin(); it != _data.end(); it++) {
			if ((float)*it < threshold)
				counter++;
		}
		_belowCache.Insert(threshold, counter);
		return counter;
	}
}


int StatisticMultiset::getCountAbove(float threshold)
{
	if (_aboveCache.Search(threshold))
		return _aboveCache.Find(threshold);
	else {
		int counter = 0;

		for (auto it = _data.begin(); it != _data.end(); it++) {
			if ((float)*it > threshold)
				counter++;
		}
		_aboveCache.Insert(threshold, counter);
		return counter;
	}
}

void StatisticMultiset::_statisticUpdate()
{
	if (_data.size() == 0) {
		_isActual = false;
		return;
	}

	int min;
	int max;
	float avg;
	auto it = _data.begin();
	
	// Init
	min = *it;
	max = *it;
	avg = (float)*it;
	it++;
	
	for (; it != _data.end(); it++) {
		if (*it > max)
			max = *it;
		if (*it < min)
			min = *it;
		avg += *it;
	}

	avg /= _data.size();

	// Stats is ready
	_minValue = min;
	_maxValue = max;
	_avgValue = avg;

	// Cache reset
	_aboveCache.Clear();
	_belowCache.Clear();
	_isActual = true;
}
