#include <iostream>
#include <vector>
using namespace std;

class CuckooHash{
    static const int TABLA_SIZE = 5;
    static const int MAX_CICLOS = 10;

    vector<string> tabla1;
    vector<string> tabla2;

    vector<string> temporal;
    int hash1(const string& clave) {
        int hash = 0; 
        for (char c : clave) {
            // Rotación de 3 bits
            hash = (hash << 3) | (hash >> 29);
            hash = hash + c;
        }
        return hash % TABLA_SIZE;
    }
    int hash2(const string& clave) {
        int hash = 0;
        for (char c : clave) {
            hash = (hash * 7) + c;  // Multiplica por primo y suma carácter
        }
        return hash % TABLA_SIZE;
    }

    public:
        CuckooHash(){
            tabla1.resize(TABLA_SIZE, "");
            tabla2.resize(TABLA_SIZE, "");
        }

        bool insertar(string clave){
            if(buscar(clave)){
                cout<<"Clave: "<<clave<<" ya existe"<<endl;
                return true;
            }

            string claveActual = clave;
            int tablaActual = 0; // 0 tabla1, 1 tabla2
            int ciclos = 0;

            while(ciclos < MAX_CICLOS){
                if(tablaActual == 0){
                    //Insertar en tabla1
                    int pos = hash1(claveActual);
                    if(tabla1[pos] == ""){
                        tabla1[pos]=claveActual;
                        //cout<<"Clave -> "<<clave<<" insertado en la tabla 1["<<pos<<"]"<<endl;
                        return true;
                    }else{
                        swap(claveActual, tabla1[pos]);
                        tablaActual = 1;
                    }
                }else{
                    int pos = hash2(claveActual);
                    if(tabla2[pos] == ""){
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
            temporal.push_back(claveActual);
            cout<<"Número de ciclo excedido"<<clave<<"\n";

            return false;
        }
        void mostrar() {
            cout << "Table 1: ";
            for (int i = 0; i < TABLA_SIZE; i++) {
                cout << "[" << tabla1[i] << "] ";
            }
            cout << endl;
            
            cout << "Table 2: ";
            for (int i = 0; i < TABLA_SIZE; i++) {
                cout << "[" << tabla2[i] << "] ";
            }
            cout << endl;

            cout << "Temporal: ";
            if(!temporal.empty()) {
                for(const string& elem : temporal) {
                    cout << "[" << elem << "] ";
                }
            }
            cout << endl << endl;
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
                         "ross", "ross2", "ross3", "ross4", "ross5"};
    
    for(int i = 0; i <= 10; i++){
        bool exito = tablaHash.insertar(elementos[i]);
        tablaHash.mostrar();
        if(!exito){
            cout << "Error en la inserción " << elementos[i] << endl;
        }
    }

   

    return 0;
}