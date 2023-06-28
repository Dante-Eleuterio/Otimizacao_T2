#include <bits/stdc++.h>
#include "graph.h"
#include <chrono>
using namespace std::chrono;
using namespace std;

vector<graph> heroes;
vector<int> A, B, optimalA, optimalB;
vector<int*> conflicts;

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

vector<int*> find_triangles(vector<int> agrupeted_heroes){
    std::vector<int*> triangles;

    // Percorre todos os pares de inteiros
    for (int i = 0; i < conflicts.size(); ++i) {
        for (size_t j = i + 1; j < conflicts.size(); ++j) {
            for (size_t k = j + 1; k < conflicts.size(); ++k) {
                // Verifica se os pares formam um triângulo
                if ((conflicts[i][0] == conflicts[j][0] && conflicts[j][0] == conflicts[k][0]) ||
                    (conflicts[i][1] == conflicts[j][1] && conflicts[j][1] == conflicts[k][1])) {
                    // Adiciona o triângulo ao vetor de triângulos
                    int triangle[3];
                    triangle[0] = i;
                    triangle[1] = j;
                    triangle[2] = k;
                    triangles.push_back(triangle);
                }
            }
        }
    }

    //Remove triangulos que contem herois já agrupados
    vector<int*>::iterator w;
    for(w = triangles.begin(); w != triangles.end(); ++w)
        for(int i = 0; i < agrupeted_heroes.size(); ++i)
            if((*w)[0] == agrupeted_heroes[i] || (*w)[1] == agrupeted_heroes[i] || (*w)[2] == agrupeted_heroes[i])
                w = triangles.erase(w);

    return triangles;
}

int findPairs(int vetor1[3], int vetor2[3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (vetor1[i] == vetor2[j] && vetor1[j] == vetor2[i]) {
                return 1;
            }
        }
    }
    return 0;
}

int betterGuedes(){
    return 1;
}

int Guedes(){
    int conflicts = countconflicts();
    vector<int> agrupeted_heroes; //Conjunto dos herois que ainda não foram anexados a um grupo
    
    vector<graph>::iterator i;
    for(i = heroes.begin(); i != heroes.end(); ++i)
        if((*i).group != 0) //Se o herói já foi anexado a um grupo
            agrupeted_heroes.push_back((*i).name);

    int cont = 0;
    vector<int*> triangulos = find_triangles(agrupeted_heroes);
    for(int j = 0; j < triangulos.size(); ++j){
        for(int k = j + 1; k < triangulos.size(); ++k){
            if(findPairs(triangulos[j], triangulos[j]) == 0)
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
        int par_conflito[2];
        par_conflito[0] = h1;
        par_conflito[1] = h2;
        conflicts.push_back(par_conflito);
    }
    for (int i = 0; i < Nfriends; i++){
        cin>> h1>>h2;
        heroes[h1-1].friends.push_back(&heroes[h2-1]);
        heroes[h2-1].friends.push_back(&heroes[h1-1]);
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
