#include <iostream>
#include <vector>
using namespace std;

class CuckooHash{
    int MAX_CICLOS = 10;

    vector<string> tabla1;
    vector<string> tabla2;

    vector<string> temporal;
    
    int hash1(const string& clave, int tamano) {
        int hash = 0; 
        for (char c : clave) {
            // Multiplicación por 8
            hash *= 8;
            hash = hash + c;
        }
        return hash % tamano;
    }
    int hash2(const string& clave, int tamano) {
        int hash = 0;
        for (char c : clave) {
            hash = (hash * 7) + c;  // Multiplica por primo y suma carácter
        }
        return hash % tamano;
    }
    // funciones auxiliares
    int hash1(const string& clave) {
        return hash1(clave, tabla1.size());
    }
    
    int hash2(const string& clave) {
        return hash2(clave, tabla2.size());
    }

    void almacenarElementos(){
        vector<string> elementos;
        for (const string& elem : tabla1) {
            if (!elem.empty()) elementos.push_back(elem);
        }
        
        for (const string& elem : tabla2) {
            if (!elem.empty()) elementos.push_back(elem);
        }
        
        for (const string& elem : temporal) {
            elementos.push_back(elem);
        }

        clearTablas();
        temporal.clear();

        for (const string& elem : elementos) {
            reHashing(elem);
        }
    }

    void reHashing(const string& clave) {
        insertar(clave);
    }

    void clearTablas() {
        fill(tabla1.begin(), tabla1.end(), "");
        fill(tabla2.begin(), tabla2.end(), "");
    }

    public:
        CuckooHash(){
            tabla1.resize(10, "");
            tabla2.resize(10, "");
        }

        bool insertar(string clave){
            string claveActual = clave;
            int tablaActual = 0; 
            int ciclos = 0;

            while(ciclos < MAX_CICLOS){
                if(tablaActual == 0){
                    int pos = hash1(claveActual);
                    if(tabla1[pos].empty()){
                        tabla1[pos]=claveActual;
                        //cout<<"Clave -> "<<clave<<" insertado en la tabla 1["<<pos<<"]"<<endl;
                        return true;
                    }else{
                        swap(claveActual, tabla1[pos]);
                        tablaActual = 1;
                    }
                }else{
                    int pos = hash2(claveActual);
                    if(tabla2[pos].empty()){
                        tabla2[pos] = claveActual;
                        //cout<<"Clave -> "<<clave<<" insertado en la tabla 2["<<pos<<"]"<<endl;
                        return true;
                    }else{
                        swap(claveActual, tabla2[pos]);
                        tablaActual = 0;
                    }
                }
                ciclos++;
            }
            cout<<"Número de ciclo excedido \n";
            temporal.push_back(claveActual);
            resizeTabla();
            almacenarElementos();
            return false;
        }

        void mostrar() {
            cout << "Table 1: ";
            for (int i = 0; i < tabla1.size(); i++) {
                cout << "[" << tabla1[i] << "] ";
            }
            cout << endl;
            
            cout << "Table 2: ";
            for (int i = 0; i < tabla2.size(); i++) {
                cout << "[" << tabla2[i] << "] ";
            }
            cout << endl;

            cout << "Temporal: ";
            if(!temporal.empty()) {
                for(int i = 0; i < temporal.size(); i++) {
                    cout << "[" << temporal[i] << "] ";
                }
            }
            cout << endl << endl;
        }
        void resizeTabla(){
            int nuevoTamano = tabla1.size() + 5;
            tabla1.resize(nuevoTamano);
            tabla2.resize(nuevoTamano);
            cout<<"Tamaño aumentado:\ntabla 1: "<<tabla1.size()<<" campos "<<"\ntabla 2: " <<tabla2.size()<<" campos \n";
        }

        bool buscar(string& clave){
            int pos1 = hash1(clave);
            int pos2 = hash2(clave);
            return (tabla1[pos1] == clave) || (tabla2[pos2] == clave) ;
        }
};
int main(){
    CuckooHash tablaHash;
    string elementos[] = {"Jorge", "manu", "perez", "sara", "saul", 
                         "mary", "cary", "roxan", "hugo", "maylo"};
    
    for(int i = 0; i <= 10; i++){
        tablaHash.insertar(elementos[i]);
        tablaHash.mostrar();
    }
    return 0;
}
