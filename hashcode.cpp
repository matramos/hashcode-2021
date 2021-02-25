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

//Los eventos son del tipo:
//	carId!=-1	"tal auto es agregado a la cola de tal semaforo"
//	carId=-1	"un auto de tal semaforo puede pasar"
struct Event {
	int time, carId, edgeId;
	Event(int time, int carId, int edgeId) : time(time), carId(carId), edgeId(edgeId){}
	bool operator<(const Event& ev) const
    {
        if(time != ev.time) return time < ev.time;
        if(carId == -1 || ev.carId == -1) return carId != -1;
        return carId < ev.carId;
    }
};

vector<vector<Edge*>> descartarSemaforos(int nEdges, vector<vector<Edge*>> &adyList, vector<vector<int>> &pathOfCar) {
    vector<bool> used(nEdges, false);
    vector<vector<Edge*>> ret(adyList.size());
    forn(i, sz(pathOfCar)){
        forn(j, sz(pathOfCar[i])){
            used[pathOfCar[i][j]]=true;
        }
    }

    forn(i, sz(adyList)){
        forn(j, sz(adyList[i])){
            if(used[adyList[i][j]->id]){
                ret[i].pb(adyList[i][j]);
            }
        }
    }

    return ret;
}

//para cada semaforo, una lista de los tiempo en que llegan autos, considerando que un auto demora "tiempoDemora" segundos en cruzar cada semaforo de su camino
vector<vector<int>> tiemposLlegadaPorSemaforo(vector<vector<Edge*>> adyList, vector<vector<int>> pathOfCar, int tiempoDemora,vector<Edge> &vEdges ) {
    vector<vector<int>> resultado(sz(vEdges));
    forn(i, sz(pathOfCar)){
        int acum=0;
        int id = pathOfCar[i][0];
        Edge edge = vEdges[id];
        resultado[id].pb(acum);    
        forr(j,1,sz(pathOfCar[i])){
            id = pathOfCar[i][j];    
            edge = vEdges[id];    
            acum+=tiempoDemora;
            acum+= edge.len;    
            resultado[id].pb(acum);
        }
    }
    return resultado;

}

//para cada nodo, una lista de sus semaforos que queremos prender {id_semaforo, tiempo en verde}, en el orden que los queremos prender
vector<vector<ii>> definirSchedule(vector<vector<int>> &tiemposLlegada, vector<vector<Edge*>> &adyList, int totalTime, int pointsPerCar, int nNodes, int nEdges) {
	vector<vector<ii>> ret(nEdges);
	//cout << nNodes << ' ' << sz(adyList) << ' ' << sz(tiemposLlegada) << endl;
	forn(i,nNodes){
		int cantTotalAutosNodo=0;
		vector<ii> primerAuto;
		forall(it, adyList[i]){
			if((*it)->to == i){ //si el edge llega a mi
				//cout << "edge llega a " << i << " desde " << (*it)->from << '\n';
				int cantAutosNodo = sz(tiemposLlegada[(*it)->id]); // sumos los que esperan en esta interseccion
				cantTotalAutosNodo+=cantAutosNodo; // cantidad de autos en todas las intersecciones 
				if(cantAutosNodo)primerAuto.pb({tiemposLlegada[(*it)->id][0], (*it)->id});
				else primerAuto.pb({-1, (*it)->id});
			}
		}
		//cout << i << ' ' << cantTotalAutosNodo << '\n';
		if(cantTotalAutosNodo != 0){
			int tiempoPromedio = totalTime/cantTotalAutosNodo; // no lo use al finaL, puede servir
			if(tiempoPromedio == 0) tiempoPromedio++;
			sort(primerAuto.begin(), primerAuto.end());
			// en vez de hacer el sort, puede ser un random????
			
			int particiones = 1; // para achicar linealmente los tiempos que le damos a c/uno, variar
			forall(it, primerAuto){
				if(it->fst == -1) continue;
				ret[i].pb({it->snd, max(sz(tiemposLlegada[it->snd])/particiones, 1)});
			}
		}
	}
	return ret;
}


// simular
ll solve(vector<vector<ii>> &schedule, vector<Edge> &vEdges, vector<vector<int>> &pathOfCar, int totalTime, int pointsPerCar) {
	int nNodes = sz(schedule);
	priority_queue<Event> pq;
	vector<int> cycleLenOfNode(nNodes, 0);
	vector<vector<int>> acumOfNode(nNodes);
	forn(i, nNodes) {
		acumOfNode[i] = vector<int>(sz(schedule[i])+1, 0);
		forn(j, sz(schedule[i])){
			 cycleLenOfNode[i] += schedule[i][j].snd;
			 acumOfNode[i][j+1] = cycleLenOfNode[i];
			 if(j == 0) {
				 pq.push({0, -1, schedule[i][j].fst});
			 }
		 }
	}
	int nEdges = sz(vEdges);
	vector<queue<int>> qOfEdge(nEdges);
	int nCars = sz(pathOfCar);
	vector<int> posOfCar(nCars, 0);
	forn(i,nCars) {
		pq.push({0,i,pathOfCar[i][0]});
	}
	ll ret = 0;
	int curt = 0;
	while(!pq.empty()) {
		Event ev = pq.top();
		pq.pop();
		assert(ev.time >= curt);
		curt = ev.time;
		
		if(curt > totalTime) break;
		
		//"un auto de tal semaforo puede pasar"
		if(ev.carId == -1) {
			int eid = ev.edgeId;
			if(!qOfEdge[eid].empty()) {
				int cid = qOfEdge[eid].front();
				qOfEdge[eid].pop();
				posOfCar[cid]++;
				assert(posOfCar[cid] < sz(pathOfCar[cid]));
				Edge e = vEdges[pathOfCar[cid][posOfCar[cid]]];
				pq.push({curt + e.len, cid, e.to});
			}
			//quien es el semaforo en verde en esta intersec en el siguiente segundo?
			Edge e = vEdges[eid];
			int auxt = (curt+1)%cycleLenOfNode[e.to];
			int auxpos = ub(acumOfNode[e.to].begin(), acumOfNode[e.to].end(), auxt) - acumOfNode[e.to].begin();
			assert(auxpos > 0);
			auxpos--;
			pq.push(Event(curt+1, -1, schedule[e.to][auxpos].fst));
		}
		//"tal auto es agregado a la cola de tal semaforo, o termina"
		else {
			int eid = ev.edgeId;
			int cid = ev.carId;
			
			//ESTE AUTO TERMINO!! :D
			if(posOfCar[cid] == sz(pathOfCar[cid])-1) {
				ret += pointsPerCar;
				ret += totalTime - curt;
				//cout << "se agrega el auto " << cid << " a la lista del semaforo " << eid << '\n';
			}
			else {
				qOfEdge[eid].push(cid);
				//cout << "se agrega el auto " << cid << " a la lista del semaforo " << eid << '\n';
			}
		}
	}
	return ret;
}

int main()
{
	//freopen("a.txt", "r", stdin);
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	int totalTime, nNodes, nEdges, nCars, pointsPerCar;
	cin >> totalTime>> nNodes>> nEdges>> nCars>> pointsPerCar;
	
	map<string, int> edgeNameToId;
	vector<Edge> vEdges(nEdges), vEdges2(nEdges);
	vector<vector<Edge*>> adyList(nNodes), adyList2(nNodes);
	forn(i, nEdges)
	{
		int a,b,len;
		string name;
		cin >> a >> b >> name >> len;
		vEdges[i] = {a, b, len, i, name};
		edgeNameToId[name] = i;
		adyList[a].pb(&vEdges[i]);
		
		vEdges2[i] = {a, b, len, i, name};
		adyList2[b].pb(&vEdges[i]);
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
	
	adyList = descartarSemaforos(nEdges, adyList, pathOfCar);
	
	
	ll ans = 0;
	vector<vector<ii>> vans;
	
	while(1) { // definir el valor distintos tiempos de espera a probar
		int tiempoDemora = 0;
		vector<vector<int>> tiemposLlegada = tiemposLlegadaPorSemaforo(adyList, pathOfCar, tiempoDemora, vEdges);
		vector<vector<ii>> schedule = definirSchedule(tiemposLlegada, adyList2, totalTime, pointsPerCar, nNodes, nEdges);
		
		/*forn(i, sz(schedule)) if(!schedule[i].empty()) {
			//cout << i << '\n';
			//cout << sz(schedule[i]) << '\n';
			forall(it, schedule[i]) cout << vEdges[it->fst].name << ' ' << it->snd << '\n';
		}*/
		
		ll aux = solve(schedule, vEdges, pathOfCar, totalTime, pointsPerCar);
		if(vans.empty() || aux > ans) {
			ans = aux;
			vans = schedule;
		}
		break;
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
