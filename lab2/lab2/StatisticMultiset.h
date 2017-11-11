/****************************************
*		StatisticMultiset class	header	*
*****************************************/

#pragma once
#include <set>
#include "SplayTree.h"

class StatisticMultiset {
public:
	StatisticMultiset();
	// ��������� ����� � ����� ����������.
	void addNum(int num);
	// ��������� ����� � ����� � �� ������������� ����������.
	void addNumLazy(int num);
	// ������������ ����� � ������.
	int getMax();
	// ����������� ����� � ������.
	int getMin();
	// ������� �������������� ����� ������.
	float getAvg();
	// ���-�� ����� � ������ ������ ��������� ������.
	int getCountUnder(float threshold);
	// ���-�� ����� � ������ ������ ��������� ������.
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