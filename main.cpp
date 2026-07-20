#include <iostream>
#include <fstream>  
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>
#include <math.h>

#include "listadepalavras.h"

using namespace std;


void limparESalvarArquivo(const string& nomeArquivoEntrada, const string& nomeArquivoSaida);
bool compararPorOrdemAlfabetica(const Plvs& a, const Plvs& b);
void gerarIndiceInvertido(const string& nomeArquivoLimpo, vector<Plvs>& lista);
void apresentarArquivoInvertido(const vector<Plvs>& lista);
int procurarPalavra(vector<Plvs>& lista,int& ocorrenciaAtual);
void exibirOcorrenciaEContexto(const Plvs& plv, int indiceOcorrencia);
void menuPrincipal(vector<Plvs>& lista);
void submenuProximaOcorrencia(const Plvs& palavraEncontrada, int& idOcorrenciaAtual);

vector<string> textoCompleto;



int main(){
    vector<Plvs> lista;
    

    limparESalvarArquivo("Historia.txt","limpo.txt");
    gerarIndiceInvertido("limpo.txt",lista);
    menuPrincipal(lista);

}
void menuPrincipal(vector<Plvs>& lista) {
    int opcao = 0;

    while (opcao != 3) {
        std::cout << "\n================ MENU PRINCIPAL ================\n";
        std::cout << "1. Imprimir Lista Invertida (Vocabulario)\n";
        std::cout << "2. Procurar Palavra\n";
        std::cout << "3. Sair do Sistema\n";
        std::cout << "================================================\n";
        std::cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            apresentarArquivoInvertido(lista);
        } 
        else if (opcao == 2) {
            int idOcorrenciaAtual = 0; 
            int idPalavraAtual = procurarPalavra(lista, idOcorrenciaAtual);

           
            if (idPalavraAtual != -1) {
               
                submenuProximaOcorrencia(lista[idPalavraAtual], idOcorrenciaAtual);
            }
        } 
        else if (opcao == 3) {
            std::cout << "\nSaindo do sistema. Ate logo!\n";
        } 
        else {
            std::cout << "\nOpcao invalida! Tente novamente.\n";
        }
    }
}

void submenuProximaOcorrencia(const Plvs& palavraEncontrada, int& idOcorrenciaAtual) {
    int subOpcao = 0;

    while (subOpcao != 2) {
        std::cout << "\n---> Deseja ver a proxima ocorrencia de '" << palavraEncontrada.getpalavra() << "'?\n";
        std::cout << "1. Sim (Procurar proxima ocorrencia)\n";
        std::cout << "2. Nao (Voltar ao Menu Principal)\n";
        std::cout << "Escolha uma opcao: ";
        cin >> subOpcao;

        if (subOpcao == 1) {
            idOcorrenciaAtual++; 
            

            if (idOcorrenciaAtual < palavraEncontrada.totalDeOcorrencias()) {
                std::cout << "\nAvancando para a proxima ocorrencia...\n";
                exibirOcorrenciaEContexto(palavraEncontrada, idOcorrenciaAtual);
            } else {
                std::cout << "\nA busca chegou ao fim! Nao ha mais ocorrencias para a palavra '" 
                     << palavraEncontrada.getpalavra() << "'.\n";
                subOpcao = 2; 
            }
        } 
        else if (subOpcao != 2) {
            std::cout << "\nOpcao invalida! Tente novamente.\n";
        }
    }
}

void apresentarArquivoInvertido(const vector<Plvs>& lista) {
    if (lista.empty()) {
        std::cout << "\nA lista invertida esta vazia. Certifique-se de que o arquivo foi indexado corretamente.\n";
        return;
    }

    std::cout << "\n=== LISTA INVERTIDA (VOCABULARIO ORDENADO) ===\n";
    

    for (size_t i = 0; i < lista.size(); i++) {
        std::cout << lista[i].getpalavra() << " -> [";
        

        const vector<int>& posicoes = lista[i].getPosicoes();
        

        for (size_t j = 0; j < posicoes.size(); j++) {
            std::cout << posicoes[j];
            if (j < posicoes.size() - 1) {
                std::cout << ", "; 
            }
        }
        std::cout << "]\n";
    }
    std::cout << "==============================================\n";
}


void limparESalvarArquivo(const string& nomeArquivoEntrada, const string& nomeArquivoSaida) {
    
    ifstream arquivoEntrada(nomeArquivoEntrada, ios::binary);
    if (!arquivoEntrada.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo de entrada '" << nomeArquivoEntrada << "'.\n";
        return; 
    }

    ofstream arquivoSaida(nomeArquivoSaida);
    if (!arquivoSaida.is_open()) {
        cerr << "Erro: Nao foi possivel criar o arquivo de sua '" << nomeArquivoSaida << "'.\n";
        arquivoEntrada.close();
        return; 
    }

    string palavra = "";
    char c1;
    bool primeiraPalavra = true;

    while (arquivoEntrada.get(c1)) {
        unsigned char uc1 = static_cast<unsigned char>(c1);

        // --- DECODIFICADOR UTF-8 INTEGRADO ---
        if (uc1 == 195 || uc1 == 194) {
            char c2;
            if (arquivoEntrada.get(c2)) { 
                unsigned char uc2 = static_cast<unsigned char>(c2);
                
                if (uc1 == 195) {
                    if (uc2 == 160 || uc2 == 161 || uc2 == 162 || uc2 == 163 || // à, á, â, ã
                        uc2 == 128 || uc2 == 129 || uc2 == 130 || uc2 == 131)   // À, Á, Â, Ã
                        palavra += 'a';
                    else if (uc2 == 168 || uc2 == 169 || uc2 == 170 ||          // è, é, ê
                             uc2 == 136 || uc2 == 137 || uc2 == 138)            // È, É, Ê
                        palavra += 'e';
                    else if (uc2 == 172 || uc2 == 173 ||                        // ì, í
                             uc2 == 140 || uc2 == 141)                          // Ì, Í
                        palavra += 'i';
                    else if (uc2 == 178 || uc2 == 179 || uc2 == 180 || uc2 == 181 || // ò, ó, ô, õ
                             uc2 == 146 || uc2 == 147 || uc2 == 148 || uc2 == 149)   // Ò, Ó, Ô, Õ
                        palavra += 'o';
                    else if (uc2 == 186 || uc2 == 187 ||                        // ù, ú
                             uc2 == 154 || uc2 == 155)                          // Ù, Ú
                        palavra += 'u';
                    else if (uc2 == 167 || uc2 == 135)                          // ç, Ç
                        palavra += 'c';
                }
            }
            continue; 
        }

       
        if (isalnum(uc1)) {
            palavra += tolower(uc1); 
        } 
        else {
            
            if (!palavra.empty()) {
                if (!primeiraPalavra) {
                    arquivoSaida << " ";
                }
                arquivoSaida << palavra;
                primeiraPalavra = false;
                palavra.clear(); 
            }
        }
    }

   
    if (!palavra.empty()) {
        if (!primeiraPalavra) {
            arquivoSaida << " ";
        }
        arquivoSaida << palavra;
    }

    arquivoEntrada.close();
    arquivoSaida.close();

    std::cout << "1. Arquivo limpo com sucesso! Todos os acentos foram removidos no mapeamento.\n";
}


void gerarIndiceInvertido(const string& nomeArquivoLimpo, vector<Plvs>& lista){
    ifstream arquivo(nomeArquivoLimpo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo limpo para indexacao.\n";
        return;
    }

    string palavraLida;
    int posicao = 0;
    while (arquivo >> palavraLida) {
        
        textoCompleto.push_back(palavraLida);

        bool encontrada = false;
        
        for (size_t i = 0; i < lista.size(); i++) {
            if (lista[i].getpalavra() == palavraLida) {
                lista[i].adicionaAparicao(posicao);
                encontrada = true;
                break;
            }
        }

        
        if (!encontrada) {
            Plvs novaPlv;
            novaPlv.setpalavra(palavraLida);
            novaPlv.adicionaAparicao(posicao);
            lista.push_back(novaPlv);
        }

        posicao++;
    }
    arquivo.close();

  sort(lista.begin(), lista.end(), compararPorOrdemAlfabetica);

    cout << "Indice Invertido gerado com sucesso!\n\n";


}

bool compararPorOrdemAlfabetica(const Plvs& a, const Plvs& b) {
    return a.getpalavra() < b.getpalavra();
}

void exibirOcorrenciaEContexto(const Plvs& plv, int indiceOcorrencia) {

    int posNoTexto = plv.getPosicoes()[indiceOcorrencia];
    
    cout << "Ocorrencia exibida: " << (indiceOcorrencia + 1) << "ª de " << plv.totalDeOcorrencias() << "\n";
    cout << "Posicao no texto limpo: " << (posNoTexto + 1) << "ª palavra.\n";
    
    cout << "Contexto: ... ";
    

    int idInicioContexto = max(0, posNoTexto - 3);
    int idFimContexto = min((int)textoCompleto.size() - 1, posNoTexto + 3);


    for (int i = idInicioContexto; i <= idFimContexto; i++) {
        if (i == posNoTexto) {

            string destaque = textoCompleto[i];
            for (char &c : destaque) c = toupper(c);
            cout << "[" << destaque << "] ";
        } else {
            cout << textoCompleto[i] << " ";
        }
    }
    cout << "...\n--------------------------\n";
}


int procurarPalavra(vector<Plvs>& lista, int& ocorrenciaAtual){
    string palavraProcurada;
    cout << "\nDigite uma palavra para buscar: ";
    cin >> palavraProcurada;

    for (size_t i = 0; i < palavraProcurada.length(); i++) {
        palavraProcurada[i] = tolower(static_cast<unsigned char>(palavraProcurada[i]));
    }

    int inicio = 0;
    int fim = lista.size() - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;


        if (lista[meio].getpalavra() == palavraProcurada) {
            cout << "\n--- PALAVRA ENCONTRADA ---" << "\n";
            cout << "Palavra: " << lista[meio].getpalavra() << "\n";
            cout << "Total de ocorrencias: " << lista[meio].totalDeOcorrencias() << "\n";
            
            
            exibirOcorrenciaEContexto(lista[meio], ocorrenciaAtual);

            return meio; 
        }
        if (lista[meio].getpalavra() < palavraProcurada) {
            inicio = meio + 1; 
        } 
        else {
            fim = meio - 1;   
        }
    }

    cout << "\nA palavra '" << palavraProcurada << "' nao foi encontrada no texto.\n\n";
    return -1; 
}



