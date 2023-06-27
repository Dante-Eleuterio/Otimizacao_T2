#include <bits/stdc++.h>
#include "graph.h"

using namespace std;

vector<graph> heroes;
vector<int> A, B, optimalA, optimalB;

int nodes=0;
int optimal=INT_MAX;
int optimalcut =0; 
int viabilitycut=1;
int myfunction=0;

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

//int contaConflitosGrupo(vector<int> grupo){
//    vector<int>::iterator i;
//    vector<int>::iterator j;
//    int conflicts = 0;
//
//    for (i = grupo.begin(); i != grupo.end(); ++i){
//        for (j = grupo.begin(); j != grupo.end(); ++j){
//            for(int k = 0; k < heroes[(*i) -1].enemies.size(); ++k){
//                if(heroes[(*i) -1].enemies[k]->name == (*j))
//                    conflicts++;
//            }
//        }
//    } 
//    return conflicts/2;
//}

vector<int*> find_triangles(vector<graph> C_falta){
    vector<int*> triangulos;

    for(int i = 0; i < C_falta.size(); ++i){
        for(int j = 0; j < C_falta[i].enemies.size(); ++j){
            for(int k = 0; k < C_falta[i].enemies[j]->enemies.size(); ++j){
                int size = C_falta[i].enemies[j]->enemies[k]->enemies.size();
                for(int w = 0; w < size; ++w){
                    if(C_falta[i].enemies[j]->enemies[k]->enemies[w]->name == i +1){
                        int triangulo[3];
                        triangulo[0] = i + 1;
                        triangulo[1] = j + 1;
                        triangulo[2] = k + 1;
                        triangulos.push_back(triangulo);
                    }
                }
            }
        }
    }
    return triangulos;
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
    vector<graph> C_falta; //Conjunto dos herois que ainda não foram anexados a um grupo
    
    vector<graph>::iterator i;
    for(i = heroes.begin(); i != heroes.end(); ++i)
        if((*i).group == 0) //Se o herói ainda não foi anexado a um grupo
            C_falta.push_back(*i);

    int cont = 0;
    vector<int*> triangulos = find_triangles(C_falta);
    for(int j = 0; j < triangulos.size(); ++j){
        for(int k = j + 1; k < triangulos.size(); ++k){
            if(!findPairs(triangulos[j], triangulos[j]))
                cont++;
        }
    }
    return conflicts + cont;
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

    nodes++;

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
    }
    for (int i = 0; i < Nfriends; i++){
        cin>> h1>>h2;
        heroes[h1-1].friends.push_back(&heroes[h2-1]);
        heroes[h2-1].friends.push_back(&heroes[h1-1]);
    }

   
    bnb(0);
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
