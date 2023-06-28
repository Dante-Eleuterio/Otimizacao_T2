#include <bits/stdc++.h>
#include "graph.h"
#include <chrono>
using namespace std::chrono;
using namespace std;

vector<graph> heroes;
vector<int> A, B, optimalA, optimalB;
vector<int*> conflictsV;
vector<int*> triangles;

int nodes=0;
int optimal=INT_MAX;
int optimalcut =1; 
int viabilitycut=1;
int myfunction=1;

int countconflicts(){
    int conflicts=0;
    for (int i = 0; i < heroes.size(); i++){
        if(heroes[i].group!=0){
            for (int j = 0; j < heroes[i].enemies.size(); j++){
                if(heroes[i].group==heroes[i].enemies[j]->group){
                    conflicts++;
                }
            }
        }
    }
    return conflicts/2;    
}


void FAT(){
    unordered_set<int> distinctNumbers;
    int aux=0;
    for (int i = 0; i < conflictsV.size(); ++i) {
        for (size_t j = i + 1; j < conflictsV.size(); ++j) {
            for (size_t k = j + 1; k < conflictsV.size(); ++k) {
                // Verifica se os pares formam um triângulo
                if(((conflictsV[i][0]==conflictsV[j][0]) || (conflictsV[i][0]==conflictsV[j][1]) || (conflictsV[i][0]==conflictsV[k][0]) || (conflictsV[i][0]==conflictsV[k][1]))&&
                    ((conflictsV[i][1]==conflictsV[j][0]) || (conflictsV[i][1]==conflictsV[j][1]) || (conflictsV[i][1]==conflictsV[k][0]) || (conflictsV[i][1]==conflictsV[k][1]))&&
                    ((conflictsV[j][0]==conflictsV[k][0]) || (conflictsV[j][0]==conflictsV[k][1]) || (conflictsV[j][1]==conflictsV[k][0]) || (conflictsV[j][1]==conflictsV[k][1]))){
                    // Adiciona o triângulo ao vetor de triângulos
                    distinctNumbers.insert(conflictsV[i][0]);                    
                    distinctNumbers.insert(conflictsV[i][1]);                    
                    distinctNumbers.insert(conflictsV[j][0]);                    
                    distinctNumbers.insert(conflictsV[j][1]);                    
                    distinctNumbers.insert(conflictsV[k][0]);                    
                    distinctNumbers.insert(conflictsV[k][1]);
                    aux=0;
                    int* triangle = new int[3];
                    for (int num : distinctNumbers) {
                        triangle[aux]=num;
                        aux++;
                    }
                    triangles.push_back(triangle);
                    distinctNumbers.clear();
                }
            }
        }
    }
    
}

vector<int*> find_triangles(vector<int> agrupeted_heroes){
    vector<int*> aux;
    vector<int*>::iterator it = triangles.begin();
    for (int i = 0; i < triangles.size(); i++){
        int *hero = new int[3];
        hero[0]=triangles[i][0];
        hero[1]=triangles[i][1];
        hero[2]=triangles[i][2];
        aux.push_back(hero);
    }
    it = aux.begin();
    while (it != aux.end()) {
        bool shouldRemove = false;
        for (int i = 0; i < agrupeted_heroes.size(); ++i) {
            if (find(agrupeted_heroes.begin(), agrupeted_heroes.end(), (*it)[0]) != agrupeted_heroes.end() ||
                find(agrupeted_heroes.begin(), agrupeted_heroes.end(), (*it)[1]) != agrupeted_heroes.end() ||
                find(agrupeted_heroes.begin(), agrupeted_heroes.end(), (*it)[2]) != agrupeted_heroes.end()){
                shouldRemove = true;
                break;
            }
        }
        if (shouldRemove) {
            it = aux.erase(it);
        } else {
            ++it;
        }
    }
    return aux;
}

vector<int*> find_pentagons(vector<int> agrupeted_heroes) {
    vector<int*> pentagons;
    // Iterate over all combinations of five integers
    for (int i = 0; i < conflictsV.size(); ++i) {
        for (size_t j = i + 1; j < conflictsV.size(); ++j) {
            for (size_t k = j + 1; k < conflictsV.size(); ++k) {
                for (size_t l = k + 1; l < conflictsV.size(); ++l) {
                    for (size_t m = l + 1; m < conflictsV.size(); ++m) {
                        // Check if the five integers form a pentagon
                        if ((conflictsV[i][0] == conflictsV[j][0] && conflictsV[j][0] == conflictsV[k][0] &&
                             conflictsV[k][0] == conflictsV[l][0] && conflictsV[l][0] == conflictsV[m][0]) ||
                            (conflictsV[i][1] == conflictsV[j][1] && conflictsV[j][1] == conflictsV[k][1] &&
                             conflictsV[k][1] == conflictsV[l][1] && conflictsV[l][1] == conflictsV[m][1])) {
                            // Add the pentagon to the vector of pentagons
                            int pentagon[15];
                            pentagon[0] = i;
                            pentagon[1] = j;
                            pentagon[2] = k;
                            pentagon[3] = l;
                            pentagon[4] = m;
                            pentagons.push_back(pentagon);
                        }
                    }
                }
            }
        }
    }
    int aux=0;
    vector<int*>::iterator it = pentagons.begin();
    while (it != pentagons.end()) {
        bool shouldRemove = false;
        for (int i = 0; i < agrupeted_heroes.size(); ++i) {
            if (find(agrupeted_heroes.begin(), agrupeted_heroes.end(), (*it)[0]) != agrupeted_heroes.end() ||
                find(agrupeted_heroes.begin(), agrupeted_heroes.end(), (*it)[1]) != agrupeted_heroes.end() ||
                find(agrupeted_heroes.begin(), agrupeted_heroes.end(), (*it)[2]) != agrupeted_heroes.end() ||
                find(agrupeted_heroes.begin(), agrupeted_heroes.end(), (*it)[3]) != agrupeted_heroes.end() ||
                find(agrupeted_heroes.begin(), agrupeted_heroes.end(), (*it)[4]) != agrupeted_heroes.end()) {
                shouldRemove = true;
                break;
            }
        }
        if (shouldRemove) {
            it = pentagons.erase(it);
        } else {
            ++it;
        }
    }

    return pentagons;
}


int findPairsTri(int vetor1[3], int vetor2[3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (vetor1[i] == vetor2[j] && vetor1[j] == vetor2[i]) {
                return 1;
            }
        }
    }
    return 0;
}

int findPairsPenta(int vetor1[5], int vetor2[5]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (vetor1[i] == vetor2[j] && vetor1[j] == vetor2[i]) {
                return 1;
            }
        }
    }
    return 0;
}


int betterGuedes() {
    int conflicts = countconflicts();
    vector<int> agrupeted_heroes; // Conjunto dos heróis que foram anexados a um grupo

    vector<graph>::iterator i;
    for (i = heroes.begin(); i != heroes.end(); ++i)
        if ((*i).group != 0) // Se o herói já foi anexado a um grupo
            agrupeted_heroes.push_back((*i).name);

    int cont = 0;
    vector<int*> triangles = find_triangles(agrupeted_heroes);
    for (int j = 0; j < triangles.size(); ++j) {
        for (int k = j + 1; k < triangles.size(); ++k) {
            if (findPairsTri(triangles[j], triangles[k]) == 0)
                cont++;
        }
    }
    vector<int*> pentagons = find_pentagons(agrupeted_heroes);
    for (int j = 0; j < pentagons.size(); ++j) {
        for (int k = j + 1; k < pentagons.size(); ++k) {
            if (findPairsPenta(pentagons[j], pentagons[k]) == 0)
                cont++;
        }
    }

    int B = conflicts + cont;
    if (B <= optimal)
        return 1;
    return 0;
}


int Guedes(){
    int conflicts = countconflicts();
    vector<int> agrupeted_heroes; //Conjunto dos herois que foram anexados a um grupo
    
    vector<graph>::iterator i;
    for(i = heroes.begin(); i != heroes.end(); ++i)
        if((*i).group != 0) //Se o herói já foi anexado a um grupo
            agrupeted_heroes.push_back((*i).name);

    int cont = 0;
    vector<int*> triangulos = find_triangles(agrupeted_heroes);
    for(int j = 0; j < triangulos.size(); ++j){
        for(int k = j + 1; k < triangulos.size(); ++k){
            if(findPairsTri(triangulos[j], triangulos[k]) == 0)
                cont++;
        }
    }

    int B_dada = conflicts + cont;
    if (B_dada <= optimal)
        return 1;
    return 0;
}

int limitating(){
    if (myfunction){
        return betterGuedes();
    }else
        return Guedes();
}

void removeA(int tgt){
    for (int i = 0; i < A.size(); i++){
        if(A[i]==tgt){
            A.erase(A.begin()+i);
            return;
        }
    }
    
}

void removeB(int tgt){
    for (int i = 0; i < B.size(); i++){
        if(B[i]==tgt){
            B.erase(B.begin()+i);
            return;
        }
    }
}
int checksolution(){
    int foundA=0;
    int foundB=0;
    for (int i = 0; i < heroes.size(); i++){
        for (int j = 0; j < heroes[i].friends.size(); j++){
            if(heroes[i].friends[j]->group==1){
                foundA=1;
            }
            if(heroes[i].friends[j]->group==2){
                foundB=1;
            }
            if(foundA && foundB){
                return 0;
            }
        }
        foundA=0;
        foundB=0;
    }
    return 1;

}

void bnb(int count){
    nodes++;
    int foundA=0;
    int foundB=0;

    if(count==heroes.size()){
        if(!viabilitycut && !checksolution()){
            return;
        }
        if(countconflicts()<optimal){
            optimal=countconflicts();
            optimalA.clear();
            optimalB.clear();
            for (int i = 0; i < A.size(); i++){
                optimalA.push_back(A[i]);
            }
            for (int i = 0; i < B.size(); i++){
                optimalB.push_back(B[i]);
            }
        }
        return;
    }


    if(viabilitycut){
        for (int i = 0; i < heroes[count].friends.size(); i++){
            if(heroes[count].friends[i]->group==1){
                foundA=1;
            }

            if(heroes[count].friends[i]->group==2){
                foundB=1;
            }
            
            if(foundA && foundB){
                return;
            }
        }
    }    

    if(optimalcut){
        if(foundA){
            A.push_back(heroes[count].name);
            heroes[count].group=1;
            if(limitating()){
                bnb(count+1);
            }
            removeA(heroes[count].name);
            heroes[count].group=0;
            return;
        }else if(foundB){
            B.push_back(heroes[count].name);
            heroes[count].group=2;
            if(limitating()){
                bnb(count+1);
            }
            removeB(heroes[count].name);
            heroes[count].group=0;
            return;
        }else{
            A.push_back(heroes[count].name);
            heroes[count].group=1;
            if(limitating()){
                bnb(count+1);
            }
            removeA(heroes[count].name);
            heroes[count].group=2;
            B.push_back(heroes[count].name);
            if(limitating()){
                bnb(count+1);
            }
            removeB(heroes[count].name);
            heroes[count].group=0;
            return;
        }
    }else{
        if(foundA){
            A.push_back(heroes[count].name);
            heroes[count].group=1;
            bnb(count+1);
            removeA(heroes[count].name);
            heroes[count].group=0;
        }else if(foundB){
            heroes[count].group=2;
            B.push_back(heroes[count].name);
            bnb(count+1);
            removeB(heroes[count].name);
            heroes[count].group=0;
        }else{
            A.push_back(heroes[count].name);
            heroes[count].group=1;
            bnb(count+1);
            removeA(heroes[count].name);
            heroes[count].group=2;
            B.push_back(heroes[count].name);
            bnb(count+1);
            removeB(heroes[count].name);
            heroes[count].group=0;
        }
        return;
    }
}

int main(int argc, char const *argv[]){
    
    int Nheroes,Nenemies,Nfriends,h1,h2;
    for (int i = 0; i <argc; i++){
        if(!strncmp(argv[i],"-a",3))
            myfunction=0;
        if(!strncmp(argv[i],"-f",3))
            viabilitycut=0;
        if(!strncmp(argv[i],"-o",3))
            optimalcut=0;
    }
    cin >> Nheroes >> Nenemies >> Nfriends;
    for (int i = 0; i < Nheroes; i++){
        graph aux;
        aux.name=i+1;
        aux.group=0;
        heroes.push_back(aux);
    }
    
    for (int i = 0; i < Nenemies; i++){
        cin>> h1>>h2;
        heroes[h1-1].enemies.push_back(&heroes[h2-1]);
        heroes[h2-1].enemies.push_back(&heroes[h1-1]);
        int *par_conflito = new int[2];
        par_conflito[0] = h1;
        par_conflito[1] = h2;
        conflictsV.push_back(par_conflito);
    }
   
    for (int i = 0; i < Nfriends; i++){
        cin>> h1>>h2;
        heroes[h1-1].friends.push_back(&heroes[h2-1]);
        heroes[h2-1].friends.push_back(&heroes[h1-1]);
    }

    if(optimalcut){
        FAT();
    }
    

    auto start = high_resolution_clock::now();
    bnb(0);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Time= " <<duration.count()<< endl;
    cout<<"Nodes= "<<nodes<<"\n";
    cout<<"Optimal= "<<optimal<<"\n";
    cout<<"Group A:"<<"\n";
    for(int i=0;i<optimalA.size();i++){
        cout<<optimalA[i]<<"\n";
    }
    cout<<"Group B:"<<"\n";
    for(int i=0;i<optimalB.size();i++){
        cout<<optimalB[i]<<"\n";
    }




    return 0;
}
