#pragma once
#include <string>
#include <vector>
using namespace std;

class Plvs{
    private:
        string palavra;
        vector<int> pos;
    public:
        void setpalavra(string p){palavra=p;};
        string getpalavra()const{return palavra;};
        void adicionaAparicao(int posi){pos.push_back(posi);};
        int getPrimPos(){return pos[0];}
        int totalDeOcorrencias()const{return (pos.size());}
        const vector<int>& getPosicoes() const { return pos; }
};
