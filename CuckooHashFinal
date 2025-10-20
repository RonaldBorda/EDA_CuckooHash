#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

class CuckooHash {
    int MAX_CICLOS;

    vector<string> tabla1;
    vector<string> tabla2;
    vector<string> temporal;
    
    int hash1(const string& clave, int tamano) {
        int hash = 0; 
        for (char c : clave) {
            hash *= 8;
            hash = hash + c;
        }
        return hash % tamano;
    }
    
    int hash2(const string& clave, int tamano) {
        int hash = 0;
        for (char c : clave) {
            hash = (hash * 7) + c;
        }
        return hash % tamano;
    }
    
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

    void actualizarMaxCiclos() { //Función añadida para actualizar el número de ciclos para las nuevas tablas agrandadas
        MAX_CICLOS *= 2;
        cout << "MAX_CICLOS actualizado a: " << MAX_CICLOS << endl;
    }

public:
    CuckooHash(){
        tabla1.resize(10, "");
        tabla2.resize(10, "");
        MAX_CICLOS = 10;
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

        cout<<"Numero de ciclos excedido \n";
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
        cout << endl;
            
        cout << "Table 2: ";
        for (int i = 0; i < tabla2.size(); i++) {
            cout << "[" << tabla2[i] << "] ";
        }
        cout << endl;
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
        int nuevoTamano = tabla1.size() * 2;
        tabla1.resize(nuevoTamano,"");
        tabla2.resize(nuevoTamano,"");
        cout<<"Tamanio duplicado:\ntabla 1: "<<tabla1.size()<<" campos "<<"\ntabla 2: " <<tabla2.size()<<" campos \n";

        actualizarMaxCiclos();
    }

    bool buscar(string& clave){
        int pos1 = hash1(clave);
        int pos2 = hash2(clave);
        return (tabla1[pos1] == clave) || (tabla2[pos2] == clave) ;
    }
};


string generarNombreAleatorio() {

    string prefijos[] = {"Al", "An", "Be", "Ca", "Da", "El", "Fe", "Go", "Ha", "Ja",
                        "Ka", "La", "Ma", "Na", "Pa", "Ra", "Sa", "Ta", "Va", "Za"};
    
    string medios[] = {"be", "ri", "lo", "ma", "ne", "ro", "si", "te", "vi", "xe",
                        "lu", "mi", "do", "ra", "le", "mo", "ni", "po", "li", "ke"};
    
    string sufijos[] = {"to", "no", "ta", "na", "ro", "la", "do", "sa", "go", "zo",
                        "ry", "ly", "ny", "th", "el", "io", "os", "es", "as", "us"};
    
    string nombre = prefijos[rand()%20]+ medios[rand()%20]+sufijos[rand()%20];
    
    
    return nombre;
}

int main(){
    srand(time(nullptr));
    CuckooHash tablaHash;
    vector<string> nombresGenerados;

    for(int i = 0; i < 100; i++){
        string nombre = generarNombreAleatorio();
        nombresGenerados.push_back(nombre);
        cout << "Insertando [" << (i+1) << "/100]: " << nombre <<endl;
        tablaHash.insertar(nombresGenerados[i]);
        
    }
    cout<<endl;
    tablaHash.mostrar();
    return 0;
}
