/************************************
 *		Lab2						*
 ************************************/


#include <iostream>
#include "StatisticMultiset.h"
#include <string>

int main() {
	StatisticMultiset a;
	
	while (true) {
		std::cout << "Choose item:\n1. Insert\t2. GetMax\t3. GetMin\t4. GetAvg\n5. GetAboveCount\t6. GetBelowCount \t7. ShowElements\t8. Exit\n>>";
		static int command;
		std::cin >> command;
		switch (command)
		{
		case 1:
			int val;
			std::cout << "Insert number>> ";
			std::cin >> val;
			a.addNum(val);
			break;
		case 2:
			std::cout << a.getMax() << std::endl;
			break;
		case 3:
			std::cout << a.getMin() << std::endl;
			break;
		case 4:
			std::cout << a.getAvg() << std::endl;
			break;
		case 5:
		{
			float threshold;
			std::cout << "Threshold>> ";
			std::cin >> threshold;
			std::cout << a.getCountAbove(threshold) << std::endl;
			break; 
		}
		case 6:
		{
			float threshold;
			std::cout << "Threshold>> ";
			std::cin >> threshold;
			std::cout << a.getCountUnder(threshold) << std::endl;
			break;
		}
		case 7:
			std::cout << "Stumb!\n";
			break;
		case 8:
			return 0;
		default:
			std::cout << "Incorrect command!\n";
			break;
		}
		std::cout << "DONE!\n\n";
	}
}