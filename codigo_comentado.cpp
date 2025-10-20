#include <iostream>
#include <vector> //se usarán vectores para representar las tablas hash dinámicas
//las siguientes librerias son para generar nombres aleatorios en el main
#include <cstdlib>
#include <ctime>
using namespace std;

class CuckooHash {
    int MAX_CICLOS; //limite de intentos antes de reestructurar la tabla

    vector<string> tabla1; //primera tabla hash
    vector<string> tabla2; //segunda tabla hash

    // tabla temporal para almacenar elementos que no pudieron insertarse y
    // recolectar todos los elementos durante el rehash
    vector<string> temporal;
    
    //en esta primera funcion hash se toma la clave y el tamaño de la tabla
    //se calcula un indice basado en los caracteres de la clave
    //el 'hash % tamano' asegura que el indice esté dentro de los límites de la tabla
    int hash1(const string& clave, int tamano) {
        int hash = 0; //inicializa el acumulador en 0
        for (char c : clave) { //recorre cada letra de la palabra
            
            //algoritmo: multiplica el hash actual por 8 y suma el valor ASCII del carácter
            hash *= 8;
            //c++ convierte autoamticamente un char a su valor ASCII
            hash = hash + c;
        }
        return hash % tamano; //asegura que el hash caiga dentro del tamaño de la tabla
    }
    
    // en esta función se utiliza un enfoque similar pero con una constante diferente
    // para reducir la probabilidad de colisiones entre las dos tablas hash
    // aquí se multiplica por 7 en lugar de 8
    int hash2(const string& clave, int tamano) {
        int hash = 0; //inicializa el acumulador en 0
        for (char c : clave) { //recorre cada letra de la palabra
            //algoritmo: multiplica el hash actual por 7 y suma el valor ASCII del carácter
            hash = (hash * 7) + c;
        }
        return hash % tamano; //asegura que el hash caiga dentro del tamaño de la tabla
    }
    
    // funciones auxiliares
    //es una sobrecarga de hash1 para no tener que pasar el tamaño cada vez que la llamamos desde otras funciones de la clase
    int hash1(const string& clave) { //sobrecarga de hash1
        return hash1(clave, tabla1.size()); //llama al hash1 original con el tamaño de tabla1
    }
    
    //igual que la anterior funcion pero para hash2
    int hash2(const string& clave) {
        return hash2(clave, tabla2.size()); //llama al hash2 original con el tamaño de tabla2
    }

    //funcion principal del hash
    //cuando las tablas se redimensionan, debemos reubicar todos los elementos existentes
    //ya que sus posiciones (el resultado del 'hash % tamano') cambiarán
    void almacenarElementos(){ //guarda todos los elementos actuales en un vector temporal
        vector<string> elementos; //vector local para guardar todo
        for (const string& elem : tabla1) { //recorre tabla1 y guarda todos los elementos que no estén vacíos
            if (!elem.empty()) elementos.push_back(elem);
        }
        
        for (const string& elem : tabla2) {//recorre tabla2 y guarda todos los elementos que no estén vacíos
            if (!elem.empty()) elementos.push_back(elem);
        }
        
        //añade los elementos que estaban en el almacén temporal (la clave que falló la inserción).
        for (const string& elem : temporal) {
            elementos.push_back(elem); //elemento que no llego a guardarse en ninguna tabla
        }

        //borra el contenido de las tablas y del vector temporal
        //prepara las tablas (ya redimensionadas en la función insertar, ultima parte) para recibir de nuevo todos los elementos
        clearTablas(); //limpia ambas tablas
        temporal.clear();//limpia el vector temporal

        //reinserta todos los elementos en las tablas hash redimensionadas
        for (const string& elem : elementos) { //recorre todos los elementos guardados
            reHashing(elem);
        }
    }

    //función para reinsertar un elemento durante el rehashing
    //simplemente llama a la función insertar, se usa otra función para mayor claridad y diferenciar de un insertar normal y un rehash insert
    void reHashing(const string& clave) {
        insertar(clave);
    }

    //resetea ambas tablas llenándolas con cadenas vacías
    //necesario antes de un rehash para no tener elementos duplicados o en posiciones antiguas
    void clearTablas() { //pone "" en todas las casillas de ambas tablas
        fill(tabla1.begin(), tabla1.end(), "");
        fill(tabla2.begin(), tabla2.end(), "");
    }

    //duplica el limite de ciclos despues de un rehash
    //ya que una tabla mas grande necesita mas intentos de expulsión antes de considerar otro rehash
    void actualizarMaxCiclos() { //Función para actualizar el número de ciclos para las nuevas tablas agrandadas
        MAX_CICLOS *= 2;
        cout << "MAX_CICLOS actualizado a: " << MAX_CICLOS << endl;
    }

public:
    CuckooHash(){//constructor de la clase
        //se llama automáticamente al crear un objeto CuckooHash
        //Inicializa las tablas con un tamaño base (10) y las llena con strings vacíos `""`
        //(importante para que `.empty()` funcione
        tabla1.resize(10, "");
        tabla2.resize(10, "");
        MAX_CICLOS = 10;
    }

    //funcion de inserción principal
    //contiene la lógica de Cuckoo Hashing
    bool insertar(string clave){ //metodo principal de insercion
        string claveActual = clave; // la clave que intentamos insertar (puede cambiar)
        int tablaActual = 0; // Empezamos intentando en la tabla 0 (tabla1)
        int ciclos = 0; //contador de ciclos para evitar bucles infinitos

        //el bucle de inserción/expulsión
        while(ciclos < MAX_CICLOS){ //Se ejecuta mientras no superemos el límite de ciclos.
            if(tablaActual == 0){ //intentamos insertar en la tabla 1
                int pos = hash1(claveActual); // Calcular posición en tabla 1
                if(tabla1[pos].empty()){ // Si la posición está vacía, insertamos la clave
                    tabla1[pos]=claveActual; // insertar clave
                    //cout<<"Clave -> "<<clave<<" insertado en la tabla 1["<<pos<<"]"<<endl;
                    return true;    // Inserción exitosa
                }else{ // Colisión en Tabla 1
                    //La casilla está ocupada. Ocurre la "expulsión".
                    //`swap` intercambia `claveActual` con el elemento en la tabla. Ahora `claveActual`
                    //contiene al elemento "víctima" que estaba allí
                    swap(claveActual, tabla1[pos]); // Expulsar el elemento existente
                    tablaActual = 1; // Cambiar a la tabla 2 para el próximo intento
                }

            }else{ //intentamos insertar en la tabla 2
                int pos = hash2(claveActual); // Calcular posición en tabla 2
                if(tabla2[pos].empty()){ // Si la posición está vacía, insertamos la clave
                    tabla2[pos] = claveActual; // insertar clave
                    //cout<<"Clave -> "<<clave<<" insertado en la tabla 2["<<pos<<"]"<<endl;
                    return true; // Inserción exitosa
                }else{ // Colisión en Tabla 2
                    //se toma la "víctima" de la tabla 2, y se marca que el próximo intento será de vuelta en la tabla 1
                    swap(claveActual, tabla2[pos]); // Expulsar el elemento existente
                    tablaActual = 0; // Cambiar a la tabla 1 para el próximo intento
                }
            }
            ciclos++; // Incrementar el contador de expulsiones
        }

        // Si salimos del bucle 'while', significa que `ciclos` alcanzó `MAX_CICLOS`
        // se debe realizar un rehashing (redimensionar y reinsertar todos los elementos)
        cout<<"Numero de ciclos excedido \n"; // aviso de rehash
        temporal.push_back(claveActual); //guardar la clave que no se pudo insertar
        resizeTabla();//aumentar el tamaño de las tablas
        almacenarElementos();//rehash de todos los elementos
        return false; // Indicar que la inserción simple falló

    }

    //funcion para mostrar el contenido de las tablas y el vector temporal
    void mostrar() {//muestra el contenido de ambas tablas y del vector temporal
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
        
    //funcion para redimensionar ambas tablas
    //Se necesita cuando ocurre un ciclo, para hacer más espacio y resolver el ciclo en el siguiente rehash
    void resizeTabla(){
        int nuevoTamano = tabla1.size() * 2;
        tabla1.resize(nuevoTamano,""); //Redimensiona y rellena con ""
        tabla2.resize(nuevoTamano,"");
        cout<<"Tamanio duplicado:\ntabla 1: "<<tabla1.size()<<" campos "<<"\ntabla 2: " <<tabla2.size()<<" campos \n";

        actualizarMaxCiclos();// Actualiza el límite de intentos para el nuevo tamaño
    }
    
    // Busca una clave en la tabla, la búsqueda es siempre O(1).
    bool buscar(string& clave){
        int pos1 = hash1(clave);
        int pos2 = hash2(clave);
        return (tabla1[pos1] == clave) || (tabla2[pos2] == clave) ; //retorna true si la encuentra en alguna tabla
    }
};

// Genera un nombre de 3 sílabas combinando partes aleatorias
string generarNombreAleatorio() {

    string prefijos[] = {"Al", "An", "Be", "Ca", "Da", "El", "Fe", "Go", "Ha", "Ja",
                       "Ka", "La", "Ma", "Na", "Pa", "Ra", "Sa", "Ta", "Va", "Za"};
    
    string medios[] = {"be", "ri", "lo", "ma", "ne", "ro", "si", "te", "vi", "xe",
                       "lu", "mi", "do", "ra", "le", "mo", "ni", "po", "li", "ke"};
    
    string sufijos[] = {"to", "no", "ta", "na", "ro", "la", "do", "sa", "go", "zo",
                       "ry", "ly", "ny", "th", "el", "io", "os", "es", "as", "us"};
    
    // rand() % 20 selecciona un índice aleatorio entre 0 y 19.
    string nombre = prefijos[rand()%20]+ medios[rand()%20]+sufijos[rand()%20];
    
    
    return nombre;
}

int main(){
    // Inicializa la semilla para la generación de números aleatorios
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
