#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <utility>
#include <thread> 
#include <chrono> 
#include <algorithm> 
using namespace std;
using namespace std::chrono;

const int REDAK = 20;
const int STUPAC = 40;
const int INF = INT_MAX;

struct Celija {
    int x, y;
    bool operator==(const Celija& drugi) const {
        return x == drugi.x && y == drugi.y;
    }
    bool operator<(const Celija& drugi) const {
        if (x == drugi.x) {
            return y < drugi.y;
        }
        return x < drugi.x;
    }
};

bool isValid(int x, int y, const vector<vector<int>>& ploca) {
    return (x >= 0) && (x < REDAK) && (y >= 0) && (y < STUPAC) && (ploca[x][y] != 1);
}

vector<Celija> getSusjed(Celija celija, const vector<vector<int>>& ploca) {
    vector<Celija> susjedi;
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; ++i) {
        int noviX = celija.x + dx[i];
        int noviY = celija.y + dy[i];
        if (isValid(noviX, noviY, ploca)) {
            susjedi.push_back({ noviX, noviY });
        }
    }
    return susjedi;
}

void printPloca(const vector<vector<int>>& ploca, Celija pocetak, Celija kraj, Celija trenutni) {

    system("cls");
    for (int i = 0; i < REDAK; ++i) {
        for (int j = 0; j < STUPAC; ++j) {
            if (i == pocetak.x && j == pocetak.y) {
                cout << 'A';
            }
            else if (i == kraj.x && j == kraj.y) {
                cout << 'B';
            }
            else if (i == trenutni.x && j == trenutni.y) {
                cout << '*';
            }
            else if (ploca[i][j] == 1) {
                cout << '#';
            }
            else {
                cout << '.';
            }
        }
        cout << endl;
    }
}

vector<Celija> dijkstra(const vector<vector<int>>& ploca, Celija pocetak, Celija kraj) {
    vector<vector<int>> udaljenost(REDAK, vector<int>(STUPAC, INF));
    vector<vector<Celija>> prethodni(REDAK, vector<Celija>(STUPAC, { -1, -1 }));
    priority_queue<pair<int, Celija>, vector<pair<int, Celija>>, greater<pair<int, Celija>>> pq;

    udaljenost[pocetak.x][pocetak.y] = 0;
    pq.push({ 0, pocetak });

    while (!pq.empty()) {
        Celija u = pq.top().second;
        pq.pop();

        if (u == kraj) break;

        for (const auto& susjed : getSusjed(u, ploca)) {
            int alt = udaljenost[u.x][u.y] + 1;
            if (alt < udaljenost[susjed.x][susjed.y]) {
                udaljenost[susjed.x][susjed.y] = alt;
                prethodni[susjed.x][susjed.y] = u;
                pq.push({ alt, susjed });
            }
        }
    }

    vector<Celija> putanja;
    for (Celija at = kraj; !(at == Celija{ -1, -1 }); at = prethodni[at.x][at.y]) {
        putanja.push_back(at);
    }
    reverse(putanja.begin(), putanja.end());

    return putanja;
}

void pomiciSeDoCilja(const vector<vector<int>>& ploca, Celija pocetak, Celija kraj) {
    vector<Celija> putanja = dijkstra(ploca, pocetak, kraj);
    for (const auto& celija : putanja) {
        printPloca(ploca, pocetak, kraj, celija);
        this_thread::sleep_for(milliseconds(100));
    }
}

int main() {
    vector<vector<int>> ploca(REDAK, vector<int>(STUPAC, 0));


    for (int i = 0; i < 15; ++i) {
        ploca[i][20] = 1;
    }

    int pocetakX, pocetakY, krajX, krajY;

    cout << "Unesite redak i stupac pocetne tocke A (npr. 1 1): ";
    cin >> pocetakX >> pocetakY;
    cout << "Unesite redak i stupac krajnje tocke B (npr. 1 40): ";
    cin >> krajX >> krajY;

    pocetakX -= 1; pocetakY -= 1; krajX -= 1; krajY -= 1;

    if (pocetakX < 0 || pocetakX >= REDAK || pocetakY < 0 || pocetakY >= STUPAC || krajX < 0 || krajX >= REDAK || krajY < 0 || krajY >= STUPAC) {
        cerr << "Pogresan unos. Redak mora biti izmedju 1 i 20, a stupac izmedju 1 i 40." << endl;
        return 1;
    }

    Celija pocetak = { pocetakX, pocetakY };
    Celija kraj = { krajX, krajY };

    pomiciSeDoCilja(ploca, pocetak, kraj);

    return 0;
}
