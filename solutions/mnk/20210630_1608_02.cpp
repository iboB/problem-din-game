#include <iostream>
#include <vector>
#define N 524288
#define MOD 100019

using namespace std;

vector <long long> vKeys[MOD], vVals[MOD];

long long get(long long key){
	long long h = key % MOD;
	
	for (int i = 0; i < vKeys[h].size(); i++){
		if (vKeys[h][i] == key){
			return vVals[h][i];
		}
	}
	
	return -1;
}

void set(long long key, long long val){
	long long h = key % MOD;
	vKeys[h].push_back(key);
	vVals[h].push_back(val);
}

vector <long long> vParents, vNodes;
int iFront = 0;

long long sq;
long long p;

void place_next(long long a, long long b){
	vParents.push_back(sq);
	vNodes.push_back(N*a+b);
}

int main(){
	long long a, b, c, a_, b_;

	cin >> a >> b >> c;
	
	long long sum = a + b + c;
	
	vParents.push_back(-2);
	vNodes.push_back(N*a+b);
	
	while (true){
		p = vParents[iFront];
		sq = vNodes[iFront];
		
		iFront++;
		
		if (get(sq) != -1) continue;
		
		a = sq / N;
		b = sq % N;
		c = sum - a - b;

		if (a == 0 || b == 0 || c == 0){
			set(sq, p);
			break;
		}
		
		if (a >= b) place_next(a-b, 2*b);
		if (a >= c) place_next(a-c, b);
		if (b >= a) place_next(2*a, b-a);
		if (b >= c) place_next(a, b-c);
		if (c >= a) place_next(2*a, b);
		if (c >= b) place_next(a, 2*b);
		
		set(sq, p);
	}
	
	iFront = 0;
	
	while (sq != -2){
		vNodes[iFront] = sq;
		iFront++;
		sq = get(sq);
	}

	for (int i = iFront-1; i > 0; i--){
		a = vNodes[i] / N;
		b = vNodes[i] % N;
		
		a_ = vNodes[i-1] / N;
		b_ = vNodes[i-1] % N;

		cout << (a > a_ ? 'A':(b > b_ ? 'B':'C')) << "->" << (a < a_ ? 'A':(b < b_ ? 'B':'C')) << endl;
	}

	return 0;
}