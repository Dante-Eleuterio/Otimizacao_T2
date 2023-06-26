#include <bits/stdc++.h>
#include "graph.h"
using namespace std;
vector<graph> heroes;
vector<int> A,B,optimalA,optimalB,C;
int nodes=0;
int optimal=INT_MAX;
int optimalcut =0; 
int viabilitycut=0;


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


int limitating(){
    return 0;
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


void bnb(int count){
    int foundA=0;
    int foundB=0;
    if(count==heroes.size()){
        for (int i = 0; i < heroes.size(); i++){
            for (int j = 0; j < count; j++)
            {
                /* code */
            }
            
            if(heroes[count].friends[i]->group==1){
                foundA=1;
            }

            if(heroes[count].friends[i]->group==2){
                foundB=1;
            }
            
            if(foundA && foundB){
                cout<<"teste\n";
                return;
            }
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
                cout<<"teste\n";
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
            }else{
                removeA(heroes[count].name);
                heroes[count].group=0;
                return;
            }
        }else if(foundB){
            B.push_back(heroes[count].name);
            heroes[count].group=2;
            if(limitating()){
                bnb(count+1);
            }else{
                removeB(heroes[count].name);
                heroes[count].group=0;
                return;
            }
        }else{
            A.push_back(heroes[count].name);
            heroes[count].group=1;
            if(limitating()){
                bnb(count+1);
            }else{
                removeA(heroes[count].name);
                heroes[count].group=2;
                B.push_back(heroes[count].name);
                if(limitating()){
                    bnb(count+1);
                }else{
                    removeB(heroes[count].name);
                    heroes[count].group=0;
                    return;
                }
            }
        }
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
        return;

    }
}


int main(int argc, char const *argv[]){
    
    int Nheroes,Nenemies,Nfriends,h1,h2;
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

    // for (int i = 0; i < Nheroes; i++){
    //     for (int j = 0;j< heroes[i].enemies.size(); j++){
    //         cout<<heroes[i].name<<"->"<<heroes[i].enemies[j]->name<<"\n";
    //     }
    // }
    // for (int i = 0; i < Nheroes; i++){
    //     for (int j = 0;j< heroes[i].friends.size(); j++){
    //         cout<<heroes[i].name<<"->"<<heroes[i].friends[j]->name<<"\n";
    //     }
    // }
    
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