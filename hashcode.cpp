#include <bits/stdc++.h>
#define forr(i,a,b) for(int i=(a);i<(b);i++)
#define forn(i,n) forr(i,0,n)
#define dforn(i,n) for(int i=n-1;i>=0;i--)
#define forall(it,v) for(auto it=v.begin();it!=v.end();it++)
#define sz(c) ((int)c.size())
#define rsz resize
#define pb push_back
#define mp make_pair
#define lb lower_bound
#define ub upper_bound
#define fst first
#define snd second

using namespace std;

typedef long long ll;
typedef pair<int,int> ii;

struct Edge {
	int from, to, len, id;
	string name;
};


vector<vector<Edge*>> descartarSemaforos(vector<vector<Edge*>> &adyList, vector<vector<int>> &pathOfCar) {
	
}

//para cada semaforo, una lista de los tiempo en que llegan autos, considerando que un auto demora "tiempoDemora" segundos en cruzar cada semaforo de su camino
vector<vector<int>> tiemposLlegadaPorSemaforo(adyList, pathOfCar, tiempoDemora) {
	
}

//para cada nodo, una lista de sus semaforos que queremos prender {id_semaforo, tiempo en verde}, en el orden que los queremos prender
vector<vector<ii>> definirSchedule(vector<vector<int>> &tiemposLlegada, vector<vector<Edge*>> &adyList, int totalTime, int pointsPerCar) {
	
}

// simular
ll solve(schedule, adyList, pathOfCar) {
	
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	int totalTime, nNodes, nNedges, nCars, pointsPerCar;
	cin >> totalTime>> nNodes>> nNedges>> nCars>> pointsPerCar;
	
	map<string, int> edgeNameToId;
	vector<Edge> vEdges(nEdges);
	vector<vector<Edge*>> adyList(nNodes);
	forn(i, nEdges)
	{
		int a,b,len;
		string name;
		cin >> a >> b >> name >> len;
		vEdges[i] = {a, b, len, i, name};
		edgeNameToId[name] = i;
	}
	
	vector<vector<int>> pathOfCar(nCars);
	forn(i, nCars) {
		int k;
		cin >> k;
		pathOfCar[i].rsz(k);
		forn(j,k) {
			string name;
			cin >> name;
			pathOfCar[i][j] = edgeNameToId[name];
		}
	}
	
	adyList = descartarSemaforos(adyList, pathOfCar);
	
	ll ans = 0;
	vector<vector<ii>> vans;
	
	for( ... ) { // definir el valor distintos tiempos de espera a probar
		vector<vector<int>> tiemposLlegada = tiemposLlegadaPorSemaforo(adyList, pathOfCar, tiempoDemora);
		vector<vector<ii>> schedule = definirSchedule(tiemposLlegada, adyList, totalTime, pointsPerCar);
		ll aux = solve(schedule, adyList, pathOfCar);
		if(aux > ans) {
			ans = aux;
			vans = schedule;
		}
	}
	
	int nonEmptyNodes = 0;
	forn(i, sz(vans)) if(!vans[i].empty()) nonEmptyNodes++;
	cout << nonEmptyNodes << '\n';
	forn(i, sz(vans)) if(!vans[i].empty()) {
		cout << i << '\n';
		cout << sz(vans[i]) << '\n';
		forall(it, vans[i]) cout << vEdges[it->fst].name << ' ' << it->snd << '\n';
	}
	
	return 0;
}
