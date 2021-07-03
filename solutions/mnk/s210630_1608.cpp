#include <iostream>
using namespace std;

void reduce(int& iLeft, int& iMid, int& iRight, char cLeft, char cMid, char cRight){
	int iDiv = iMid / iLeft;

	while (iDiv > 0){
		if (iDiv % 2){
			iMid -= iLeft;
			cout << cMid << "->" << cLeft << endl;
		}
		else{
			iRight -= iLeft;
			cout << cRight << "->" << cLeft << endl;
		}
		
		iDiv /= 2;
		iLeft *= 2;
	}
}

int main(){
	int a, b, c;
	
	cin >> a >> b >> c;

	while (true){
		if (a == 0 || b == 0 || c == 0){
			break;
		}
		
		if (a <= b){
			if (c <= a) reduce(c, a, b, 'C', 'A', 'B');
			else if (b <= c) reduce(a, b, c, 'A', 'B', 'C');
			else reduce(a, c, b, 'A', 'C', 'B');
		}
		else{
			if (c <= b) reduce(c, b, a, 'C', 'B', 'A');
			else if (a <= c) reduce(b, a, c, 'B', 'A', 'C');
			else reduce(b, c, a, 'B', 'C', 'A');
		}
	}

	return 0;
}
