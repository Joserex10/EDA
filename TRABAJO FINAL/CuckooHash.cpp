#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <unordered_map>
#include <random>
#include <vector>
#include <algorithm> // Para std::shuffle
#include <limits>
#include <regex>
#include <cstdint>
#include <cctype> // Para isdigit
#include <cmath>  // Para std::ceil
#include <functional>
#include <stdexcept>
#include <utility>

using namespace std;

const int RECORD_COUNT = 33000000;  // Catidad de registros a generar
const int DNI_LENGTH = 8;           // Longitud del DNI
const int NAME_LENGTH = 20;         // Longitud del nombre
const int SURNAME_LENGTH = 20;      // Longitud del apellido
const int AGE_LENGTH = 3;           // Longitud de la edad
const int GENDER_LENGTH = 1;        // Longitud del genero (M/F)
const int BIRTHDATE_LENGTH = 10;    // Longitud de la fecha de nacimiento (dd-mm-yyyy)
const int ADDRESS_LENGTH = 30;      // Longitud de la direccion
const int NATIONALITY_LENGTH = 15;  // Longitud de la nacionalidad
const int BIRTHPLACE_LENGTH = 30;   // Longitud del lugar de nacimiento
const int PHONE_LENGTH = 15;        // Longitud del telefono de referencia
const int EMAIL_LENGTH = 30;        // Longitud del correo electronico
const int CIVIL_STATUS_LENGTH = 10; // Longitud del estado civil
const char DATA_FILE[] = "data.txt";
const char HASH_INDEX_FILE[] = "hash_index.dat";

size_t hashFunction1(uint32_t key)
{
    return std::hash<uint32_t>{}(key);
}

size_t hashFunction2(uint32_t key)
{
    return std::hash<uint32_t>{}(~key);
}

class CuckooHashMap
{
private:
#pragma pack(push, 1)
    struct Entry
    {
        uint32_t key;   // 4 bytes
        uint64_t value; // 8 bytes

        Entry() : key(0), value(0) {}
        Entry(uint32_t k, uint64_t v) : key(k), value(v) {}
    };
#pragma pack(pop)

    std::vector<Entry> table1;
    std::vector<Entry> table2;
    size_t capacity;
    size_t size;
    size_t maxIterations;

public:
    CuckooHashMap(size_t initialCapacity = 20000000, size_t maxLoop = 500)
        : capacity(initialCapacity), size(0), maxIterations(maxLoop)
    {
        table1.resize(capacity);
        table2.resize(capacity);
    }

    void insert(uint32_t key, uint64_t value)
    {
        if (key == 0)
            throw std::invalid_argument("Key cannot be zero.");

        // Verificar si la clave ya existe en table1
        size_t h1 = hashFunction1(key) % capacity;
        if (table1[h1].key == key)
        {
            table1[h1].value = value;
            return;
        }

        // Verificar si la clave ya existe en table2
        size_t h2 = hashFunction2(key) % capacity;
        if (table2[h2].key == key)
        {
            table2[h2].value = value;
            return;
        }

        // Si la clave no existe, proceder con la inserción
        if (size >= capacity * 0.9)
        {
            rehash();
        }

        uint32_t k = key;
        uint64_t v = value;

        for (size_t i = 0; i < maxIterations; ++i)
        {
            size_t h1 = hashFunction1(k) % capacity;
            if (table1[h1].key == 0)
            {
                table1[h1] = Entry(k, v);
                ++size;
                return;
            }
            std::swap(k, table1[h1].key);
            std::swap(v, table1[h1].value);

            size_t h2 = hashFunction2(k) % capacity;
            if (table2[h2].key == 0)
            {
                table2[h2] = Entry(k, v);
                ++size;
                return;
            }
            std::swap(k, table2[h2].key);
            std::swap(v, table2[h2].value);
        }

        // Si falla después de maxIterations, rehash y reintentar
        rehash();
        insert(k, v);
    }

    uint64_t find(uint32_t key)
    {
        size_t h1 = hashFunction1(key) % capacity;
        if (table1[h1].key == key)
        {
            return table1[h1].value;
        }
        size_t h2 = hashFunction2(key) % capacity;
        if (table2[h2].key == key)
        {
            return table2[h2].value;
        }
        return static_cast<uint64_t>(-1); // Clave no encontrada
    }

    void remove(uint32_t key)
    {
        size_t h1 = hashFunction1(key) % capacity;
        if (table1[h1].key == key)
        {
            table1[h1].key = 0;
            table1[h1].value = 0;
            --size;
            return;
        }
        size_t h2 = hashFunction2(key) % capacity;
        if (table2[h2].key == key)
        {
            table2[h2].key = 0;
            table2[h2].value = 0;
            --size;
            return;
        }
        // Clave no encontrada
    }

    void clear()
    {
        std::fill(table1.begin(), table1.end(), Entry());
        std::fill(table2.begin(), table2.end(), Entry());
        size = 0;
    }

    void rehash()
    {
        capacity *= 2;
        size = 0;
        std::vector<Entry> oldTable1 = std::move(table1);
        std::vector<Entry> oldTable2 = std::move(table2);
        table1.resize(capacity);
        table2.resize(capacity);
        std::fill(table1.begin(), table1.end(), Entry());
        std::fill(table2.begin(), table2.end(), Entry());

        for (const auto &entry : oldTable1)
        {
            if (entry.key != 0)
            {
                insert(entry.key, entry.value);
            }
        }
        for (const auto &entry : oldTable2)
        {
            if (entry.key != 0)
            {
                insert(entry.key, entry.value);
            }
        }
    }

    void update(uint32_t key, uint64_t value)
    {
        size_t h1 = hashFunction1(key) % capacity;
        if (table1[h1].key == key)
        {
            table1[h1].value = value;
            return;
        }

        size_t h2 = hashFunction2(key) % capacity;
        if (table2[h2].key == key)
        {
            table2[h2].value = value;
            return;
        }

        // Si la clave no existe, insertar normalmente
        insert(key, value);
    }
};

class Record
{
public:
    uint32_t dni;
    unsigned short edad;
    string nombre, apellido, fechaNacimiento, direccion, nacionalidad, lugarNacimiento, telefonoReferencia, correoElectronico, estadoCivil, departamento, provincia, ciudad, distrito, ubicacion;
    char genero, status;

    Record() : status('A'), dni(0), nombre(""), apellido(""), genero('M'), fechaNacimiento(""), edad(0), direccion(""), nacionalidad(""), lugarNacimiento(""), telefonoReferencia(""), correoElectronico(""), estadoCivil("") {}

    Record(int dni, const string &nombre, const string &apellido, char genero, const string &fechaNacimiento, int edad, const string &direccion, const string &nacionalidad, const string &lugarNacimiento, const string &telefonoReferencia, const string &correoElectronico, const string &estadoCivil, const string &departamento, const string &provincia, const string &ciudad, const string &distrito, const string &ubicacion)
        : status('A'), dni(dni), nombre(nombre), apellido(apellido), genero(genero), fechaNacimiento(fechaNacimiento), edad(edad), direccion(direccion), nacionalidad(nacionalidad), lugarNacimiento(lugarNacimiento), telefonoReferencia(telefonoReferencia), correoElectronico(correoElectronico), estadoCivil(estadoCivil), departamento(departamento), provincia(provincia), ciudad(ciudad), distrito(distrito), ubicacion(ubicacion) {}

    string fixedWidth(const string &text, int width) const
    {
        if (text.length() < width)
        {
            return text + string(width - text.length(), ' '); // Rellena con espacios
        }
        else
        {
            return text.substr(0, width); // Trunca si es necesario
        }
    }

    string serialize() const
    {
        ostringstream oss;
        oss << "  " << status << "    | " << fixedWidth(to_string(dni), 8) << " | " << fixedWidth(nombre, 13) << " | " << fixedWidth(apellido, 15) << " | " << "   " << genero << "   | " << fixedWidth(fechaNacimiento, 10) << " | " << " " << fixedWidth(to_string(edad), 2) << "  | " << fixedWidth(departamento, 10) << "   | " << fixedWidth(provincia, 13) << " | " << fixedWidth(ciudad, 15) << " | " << fixedWidth(distrito, 17) << " | " << fixedWidth(ubicacion, 30) << " | " << fixedWidth(nacionalidad, 12) << " | " << fixedWidth(lugarNacimiento, 16) << " | " << fixedWidth(telefonoReferencia, 10) << " | " << fixedWidth(correoElectronico, 31) << " | " << estadoCivil << '\n';
        return oss.str();
    }

    static Record deserialize(const string &data)
    {
        Record record;
        istringstream iss(data);
        string token;

        getline(iss, token, '|');
        record.status = token[2];
        getline(iss, token, '|');
        record.dni = stoi(token);
        getline(iss, record.nombre, '|');
        getline(iss, record.apellido, '|');
        getline(iss, token, '|');
        record.genero = token[4];
        getline(iss, record.fechaNacimiento, '|');
        getline(iss, token, '|');
        record.edad = stoi(token);
        getline(iss, record.departamento, '|');
        getline(iss, record.provincia, '|');
        getline(iss, record.ciudad, '|');
        getline(iss, record.distrito, '|');
        getline(iss, record.ubicacion, '|');
        getline(iss, record.nacionalidad, '|');
        getline(iss, record.lugarNacimiento, '|');
        getline(iss, record.telefonoReferencia, '|');
        getline(iss, record.correoElectronico, '|');
        getline(iss, record.estadoCivil);

        record.direccion = record.departamento + ", " + record.provincia + ", " + record.ciudad + ", " + record.distrito + ", " + record.ubicacion;

        return record;
    }
};

class Database
{
private:
    CuckooHashMap indexMap; // Mapa Cuckoo Hash para el índice
    // Genera DNI Unicos para cada persona
    vector<int> generateUniqueDni(int count)
    {
        vector<int> dniList;
        for (int i = 10000000; i < 10000000 + count; ++i)
        {
            dniList.push_back(i);
        }

        random_device rd;
        mt19937 g(rd());
        shuffle(dniList.begin(), dniList.end(), g); // shuffle es utilizado para que los DNI esten distribuidos de una manera no secuencial

        return dniList;
    }

public:
    void loadIndex()
    {
        ifstream indexFile(HASH_INDEX_FILE, ios::binary);
        if (!indexFile.is_open())
        {
            throw runtime_error("Error abriendo el archivo de índice hash.");
        }

        uint32_t dniInFile;
        uint64_t pos;

        indexMap.clear();

        while (indexFile.read(reinterpret_cast<char *>(&dniInFile), sizeof(uint32_t)))
        {
            indexFile.read(reinterpret_cast<char *>(&pos), sizeof(uint64_t));
            indexMap.insert(dniInFile, pos);
        }

        indexFile.close();
    }

    // Busca un DNI en el índice en memoria y devuelve su posición en el archivo de datos
    uint64_t findPositionInIndex(uint32_t dni)
    {
        uint64_t pos = indexMap.find(dni);
        return pos;
    }

    uint64_t permute(uint64_t x)
    {
        x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
        x = x ^ (x >> 31);
        return x;
    }

    uint64_t generateUniquePhoneNumber(uint64_t index)
    {
        uint64_t phoneNumber = permute(index) % 100000000; // Rango de 0 a 99,999,999
        return 900000000 + phoneNumber;                    // Asegurar que el numero comienza con '9' y tiene 9 digitos
    }

    void generateData()
    {

        const string nombres[] = {
            "Juan", "Maria", "Carlos", "Ana", "Pedro", "Luis", "Sofia", "Laura", "David", "Jose", "Elena", "Ricardo", "Daniel", "Marta", "Jorge", "Sandra", "Santiago", "Patricia", "Roberto", "Carmen", "Fernando", "Lucia", "Francisco", "Isabel", "Alejandro", "Natalia", "Miguel", "Teresa", "Raul", "Monica", "Gabriel", "Ana Maria", "Enrique", "Esther", "Javier", "Susana", "Pablo", "Andrea", "Manuel", "Silvia", "Ivan", "Beatriz", "Christian", "Raquel", "Oscar", "Virginia", "Alberto", "Sara", "Diego", "Eva", "Claudia", "Cristina", "Veronica", "Marcos", "Joaquin", "Bruno", "Diana", "Rodrigo", "Nicolas", "Renato", "Sebastian", "Emilia", "Victoria", "Matias", "Paula", "Adriana", "Sergio", "Marcela", "Benjamin", "Elisa", "Martin", "Renata", "Federico", "Daniela", "Guillermo", "Luz", "Antonio", "Juliana", "Mariano", "Agustin", "Esteban", "Valeria", "Belen", "Cecilia", "Gonzalo", "Ignacio", "Carolina", "Florencia", "Emilio", "Fabiana", "Pilar", "Valentina", "Andres", "Celeste", "Mariela", "Luciano", "Romina", "Mauro", "Maribel", "Francisca"};
        const int nombresSize = sizeof(nombres) / sizeof(nombres[0]);

        const string apellidos[] = {
            "Gomez", "Perez", "Rodriguez", "Sanchez", "Garcia", "Martinez", "Lopez", "Diaz", "Fernandez", "Torres", "Ortiz", "Ramirez", "Cruz", "Flores", "Hernandez", "Jimenez", "Moreno", "Munoz", "Alvarez", "Romero", "Navarro", "Campos", "Vazquez", "Castillo", "Gil", "Ruiz", "Morales", "Ortega", "Delgado", "Castro", "Vargas", "Guerrero", "Iglesias", "Gutierrez", "Santos", "Lara", "Marquez", "Nieto", "Vega", "Leon", "Herrera", "Reyes", "Aguilar", "Dominguez", "Lorenzo", "Soto", "Serrano", "Cabrera", "Ramos", "Molina", "Paredes", "Chavez", "Silva", "Palacios", "Rojas", "Guzman", "Cordero", "Estrada", "Ponce", "Bustos", "Salazar", "Mendoza", "Figueroa", "Carrillo", "Villanueva", "Bermudez", "Vargas", "Guerra", "Mejia", "Montes", "Pacheco", "Huaman", "Rincon", "Rios", "Benitez", "Aguirre", "Arce", "Luna", "Valdez", "Maldonado", "Trujillo", "Espinoza", "Mora", "Caceres", "Llanos", "Barrientos", "Peralta", "Cordova", "Zambrano", "Loayza", "Morante", "Zarate", "Tamayo", "Venegas", "Bellido", "Valencia", "Cruzado", "Leiva", "Salas", "Renteria"};
        const int apellidosSize = sizeof(apellidos) / sizeof(apellidos[0]);

        const string fechasNacimiento[] = {
            "01-05", "03-12", "07-23", "10-31", "05-18", "08-20", "12-14", "02-10", "04-25", "09-09", "11-17", "06-03", "01-20", "03-29", "08-05", "10-12", "05-27", "09-15", "12-01", "02-28", "04-14", "07-08", "11-05", "06-24", "01-13", "03-17", "07-30", "10-18", "05-22", "08-17", "12-09", "02-22", "04-02", "09-01", "11-30", "06-11", "01-29", "03-04", "07-15", "10-27", "05-09", "08-23", "12-21", "02-07", "04-28", "09-12", "11-19", "06-19", "01-08", "03-21", "07-11", "10-05", "05-13", "08-08", "12-04", "02-15", "04-06", "09-26", "11-01", "06-28", "01-26", "03-09", "07-25", "10-14", "05-30", "08-14", "12-19", "02-03", "04-17", "09-07", "11-22", "06-16", "01-03", "03-26", "07-03", "10-23", "05-05", "08-26", "12-29", "02-18", "04-09", "09-19", "11-14", "06-06", "01-17", "03-31", "07-21", "10-07", "05-19", "08-02", "12-16", "02-27", "04-13", "09-22", "11-03", "06-30", "01-11", "03-07", "07-27", "10-25", "05-02", "08-25", "12-07", "02-05", "04-21", "09-05", "11-09", "06-13", "01-24", "03-14", "07-02", "10-29", "05-08", "08-13", "12-25", "02-23", "04-04", "09-11", "11-25", "06-09", "01-16", "03-19", "07-17", "10-09", "05-25", "08-03", "12-03", "02-12", "04-19", "09-30", "11-07", "06-20", "01-31", "03-02", "07-14", "10-15", "05-11", "08-30", "12-23", "02-01", "04-07", "09-14", "11-20", "06-25", "01-09", "03-24", "07-10", "10-06", "05-16", "08-07", "12-12", "02-19", "04-15", "09-04", "11-28", "06-08", "01-28", "03-16", "07-29", "10-21", "05-04", "08-10", "12-06", "02-26", "04-24", "09-21", "11-11", "06-27", "01-19", "03-06", "07-19", "10-17", "05-20", "08-15", "12-20", "02-14", "04-11", "09-25", "11-02", "06-10", "01-23", "03-30", "07-05", "10-02", "05-14", "08-22", "12-31", "02-24", "04-18", "09-17", "11-26", "06-15", "01-12", "03-13", "07-07", "10-20", "05-21", "08-04", "12-11", "02-09"};
        const int fechasNacimientoSize = sizeof(fechasNacimiento) / sizeof(fechasNacimiento[0]);

        unordered_map<string, unordered_map<string, unordered_map<string, pair<string, vector<string>>>>> peru_geografia = {
            {"Lima", {{"Lima", {{"San Isidro", {"Lima", {"Av. Javier Prado 1234", "Av. Camino Real 5678"}}}, {"Miraflores", {"Lima", {"Av. Larco 1234", "Av. Pardo 5678"}}}, {"San Borja", {"Lima", {"Av. San Borja Norte 1357", "Av. De Las Artes 2468"}}}, {"La Molina", {"Lima", {"Av. La Molina 6785", "Av. Raul Ferrero 8642"}}}}}, {"Huaral", {{"Huaral", {"Huaral", {"Av. Solar 1111", "Av. Peral 2222"}}}, {"Chancay", {"Chancay", {"Av. Chancay 9876", "Av. Libertad 5432"}}}, {"Aucallama", {"Aucallama", {"Av. Grau 8231", "Av. Tupac Amaru 1742"}}}}}, {"Canete", {{"San Vicente", {"Canete", {"Av. Benavides 6543", "Av. 28 de Julio 3210"}}}, {"Imperial", {"Canete", {"Av. La Mar 8765", "Av. Dos de Mayo 9087"}}}}}, {"Barranca", {{"Pativilca", {"Barranca", {"Av. Independencia 3456", "Av. Progreso 8769"}}}, {"Supe", {"Barranca", {"Av. Lima 1234", "Av. Panamericana Norte 5678"}}}}}}},
            {"Cusco", {{"Urubamba", {{"Ollantaytambo", {"Urubamba", {"Calle Principal Ollantaytambo 123", "Camino Inca Ollantaytambo"}}}, {"Machu Picchu", {"Aguas Calientes", {"Av. Machu Picchu 456", "Calle Intipunku 789"}}}}}, {"La Convencion", {{"Quillabamba", {"Quillabamba", {"Av. de la Cultura 123", "Calle Comercio 456"}}}, {"Echarate", {"Echarate", {"Av. Echarate 789", "Calle Central 101"}}}}}, {"Calca", {{"Calca", {"Calca", {"Av. Tupac Amaru 564", "Calle Sacsayhuaman 678"}}}, {"Lamay", {"Calca", {"Av. Los Incas 4321", "Calle Inti Raymi 234"}}}}}, {"Paucartambo", {{"Paucartambo", {"Paucartambo", {"Av. Libertad 123", "Calle Qosqo 456"}}}}}}},
            {"Arequipa", {{"Arequipa", {{"Cercado", {"Arequipa", {"Av. Independencia 654", "Av. Goyeneche 890"}}}, {"Yanahuara", {"Arequipa", {"Av. Ejercito 4321", "Av. Salaverry 5678"}}}, {"Cayma", {"Arequipa", {"Av. Cayma 8765", "Av. Panamericana 567"}}}}}, {"Caylloma", {{"Chivay", {"Caylloma", {"Calle Principal Chivay 345", "Calle Plaza de Armas 567"}}}}}, {"Islay", {{"Mollendo", {"Islay", {"Av. Maritima 456", "Calle El Sol 789"}}}, {"Mejia", {"Islay", {"Av. La Costanera 123", "Calle Los Pinos 567"}}}}}}},
            {"Piura", {{"Piura", {{"Castilla", {"Piura", {"Av. Grau 1234", "Calle Los Cocos 5678"}}}, {"Catacaos", {"Piura", {"Calle Comercio 4321", "Av. 28 de Julio 8765"}}}}}, {"Sullana", {{"Sullana", {"Sullana", {"Av. Sanchez Cerro 123", "Av. Transversal 456"}}}}}, {"Talara", {{"Talara", {"Talara", {"Av. Industrial 654", "Av. Grau 876"}}}, {"Lobitos", {"Talara", {"Calle Mar 345", "Calle Sol 678"}}}}}}},
            {"La Libertad", {{"Trujillo", {{"Trujillo", {"Trujillo", {"Av. Espana 9876", "Av. Mansiche 1234"}}}, {"Huanchaco", {"Trujillo", {"Av. Victor Larco 5678", "Calle Grau 987"}}}}}, {"Pacasmayo", {{"San Pedro de Lloc", {"Pacasmayo", {"Calle Comercio 345", "Calle Progreso 876"}}}, {"Guadalupe", {"Pacasmayo", {"Av. Principal 432", "Calle Las Palmas 654"}}}}}, {"Otuzco", {{"Otuzco", {"Otuzco", {"Av. Los Andes 876", "Calle 28 de Julio 321"}}}}}}},
            {"Junin", {{"Huancayo", {{"El Tambo", {"Huancayo", {"Av. Huancavelica 567", "Calle Real 123"}}}, {"Chilca", {"Huancayo", {"Av. 9 de Diciembre 234", "Av. Tupac Amaru 654"}}}}}, {"Jauja", {{"Jauja", {"Jauja", {"Calle Comercio 432", "Calle Real 876"}}}}}, {"Tarma", {{"Tarma", {"Tarma", {"Av. Aviacion 345", "Calle Los Heroes 654"}}}}}}},
            {"Puno", {{"Puno", {{"Puno", {"Puno", {"Av. Titicaca 654", "Calle Arequipa 987"}}}, {"Juliaca", {"Puno", {"Av. Ferrocarril 123", "Calle Huancavelica 567"}}}}}, {"Yunguyo", {{"Yunguyo", {"Yunguyo", {"Calle Comercio 345", "Av. Principal 654"}}}}}}}};

        const string nacionalidades[] = {
            "Peruana", "Argentina", "Chilena", "Colombiana", "Venezolana", "Ecuatoriana", "Boliviana", "Brasilena", "Uruguaya", "Paraguaya", "Mexicana", "Guatemalteca", "Cubana", "Hondurena", "Salvadorena", "Costarricense", "Panamena", "Nicaraguense", "Dominicana", "Puertorriquena", "Espanola", "Italiana", "Francesa", "Alemana", "Inglesa", "Portuguesa", "Belga", "Holandesa", "Suiza", "Austriaca", "Sueca", "Noruega", "Danesa", "Finlandesa", "Rusa", "Polaca", "Griega", "Turca", "China", "Japonesa", "Coreana", "India", "Egipcia", "Sudafricana", "Australiana", "Neozelandesa", "Canadiense", "Estadounidense", "Mexicana", "Venezolana", "Colombiana", "Brasilena", "Peruana", "Argentina", "Chilena", "Boliviana", "Ecuatoriana", "Uruguaya", "Paraguaya", "Cubana", "Dominicana", "Salvadorena", "Costarricense", "Panamena", "Hondurena", "Nicaraguense", "Guatemalteca", "Belicena", "Jamaicana", "Haitiana", "Trinitense", "Bahamena", "Barbadense", "Granadina", "Surinamesa", "Guyanes", "Venezolana", "Colombiana", "Peruana", "Brasilena", "Argentina", "Chilena", "Boliviana", "Paraguaya", "Uruguaya", "Ecuatoriana", "Mexicana", "Guatemalteca", "Salvadorena", "Hondurena", "Nicaraguense", "Costarricense", "Panamena", "Cana", "Estadounidense", "Canadiense", "Espanola", "Francesa", "Italiana", "Alemana", "Inglesa", "Irlandesa", "Escocesa", "Galesa", "Portuguesa", "Belga", "Holandesa", "Suiza", "Austriaca", "Sueca", "Noruega", "Danesa", "Finlandesa", "Rusa", "Polaca", "Checa", "Eslovaca", "Hungara", "Griega", "Turca", "Bulgara", "Rumana", "Croata", "Serbia", "Eslovena", "Bosnia", "Macedonia", "Albanesa", "Ucraniana", "Bielorrusa", "Lituana", "Letona", "Estonia", "Moldava", "Kazaja", "Uzbeca", "Tayika", "Kirguisa", "Turkmena", "Azerbaiyana", "Armenia", "Georgiana", "China", "Japonesa", "Coreana", "India", "Pakistani", "Bangladesi", "Nepali", "Butanesa", "Camboyana", "Vietnamita", "Laosiana", "Tailandesa", "Malaya", "Indonesia", "Filipina", "Singapurense", "Bruneana", "Australiana", "Neozelandesa", "Papu", "Egipcia", "Marroqui", "Argelina", "Tunecina", "Libia", "Sudanesa", "Etiope", "Somali", "Keniata", "Tanzana", "Ugandesa", "Ruandesa", "Congolena", "Sudafricana", "Nigeriana", "Ghanesa", "Senegalesa", "Mali", "Chadiana", "Angolena", "Zimbabuense", "Mozambiquena", "Madagascarense", "Namibia", "Botswana", "Zambiana", "Malaui", "Lesoto", "Suazi", "Gambiana", "Mauritana", "Liberiana", "Sierra Leona", "Guineana", "Ecuatoguineana", "Gabonesa", "Saharaui", "Israeli", "Palestina", "Jordana", "Libanesa", "Siria", "Iraqui", "Irani", "Saudi", "Emirati", "Qatari", "Kuwaiti", "Yemeni", "Omani", "Bahraini"};
        const int nacionalidadesSize = sizeof(nacionalidades) / sizeof(nacionalidades[0]);

        const string lugaresNacimiento[] = {
            "Lima", "Cusco", "Arequipa", "Trujillo", "Chiclayo", "Piura", "Iquitos", "Tacna", "Puno", "Huancayo", "Ayacucho", "Huaraz", "Pucallpa", "Chimbote", "Moquegua", "Tarapoto", "Juliaca", "Tumbes", "Ica", "Mollendo", "Nazca", "Buenos Aires", "Cordoba", "Rosario", "Mendoza", "Mar del Plata", "Santiago", "Valparaiso", "Vina del Mar", "Concepcion", "La Serena", "Bogota", "Medellin", "Cali", "Barranquilla", "Cartagena", "Caracas", "Maracaibo", "Valencia", "Barquisimeto", "Merida", "Quito", "Guayaquil", "Cuenca", "Ambato", "Loja", "La Paz", "Santa Cruz", "Cochabamba", "Oruro", "Sucre", "Rio de Janeiro", "Sao Paulo", "Brasilia", "Salvador", "Fortaleza", "Montevideo", "Punta del Este", "Salto", "Rivera", "Maldonado", "Asuncion", "Ciudad del Este", "Encarnacion", "Pedro Juan Caballero", "San Lorenzo", "Mexico City", "Guadalajara", "Monterrey", "Cancun", "Puebla", "Guatemala City", "Antigua", "Quetzaltenango", "Escuintla", "Coban", "Havana", "Santiago de Cuba", "Camaguey", "Holguin", "Santa Clara", "Madrid", "Barcelona", "Seville", "Valencia", "Bilbao", "Rome", "Milan", "Florence", "Naples", "Venice", "Paris", "Marseille", "Lyon", "Toulouse", "Nice", "Berlin", "Munich", "Frankfurt", "Hamburg", "Cologne", "London", "Manchester", "Birmingham", "Liverpool", "Edinburgh", "Lisbon", "Porto", "Coimbra", "Braga", "Faro", "Brussels", "Antwerp", "Ghent", "Bruges", "Leuven", "Amsterdam", "Rotterdam", "The Hague", "Utrecht", "Eindhoven", "Zurich", "Geneva", "Bern", "Basel", "Lausanne", "Vienna", "Salzburg", "Innsbruck", "Graz", "Linz", "Stockholm", "Gothenburg", "Malmo", "Uppsala", "Vasteras", "Oslo", "Bergen", "Trondheim", "Stavanger", "Drammen", "Copenhagen", "Aarhus", "Odense", "Aalborg", "Esbjerg", "Helsinki", "Espoo", "Tampere", "Vantaa", "Oulu", "Moscow", "Saint Petersburg", "Novosibirsk", "Yekaterinburg", "Nizhny Novgorod", "Warsaw", "Krakow", "Lodz", "Wroclaw", "Poznan", "Athens", "Thessaloniki", "Patras", "Heraklion", "Larissa", "Istanbul", "Ankara", "Izmir", "Bursa", "Antalya", "Beijing", "Shanghai", "Guangzhou", "Shenzhen", "Chengdu", "Tokyo", "Osaka", "Kyoto", "Yokohama", "Nagoya", "Seoul", "Busan", "Incheon", "Daegu", "Daejeon", "New Delhi", "Mumbai", "Bangalore", "Kolkata", "Chennai", "Cairo", "Alexandria", "Giza", "Luxor", "Aswan", "Cape Town", "Johannesburg", "Durban", "Pretoria", "Port Elizabeth", "Sydney", "Melbourne", "Brisbane", "Perth", "Adelaide", "Auckland", "Wellington", "Christchurch", "Hamilton", "Dunedin", "Toronto", "Vancouver", "Montreal", "Calgary", "Ottawa", "New York", "Los Angeles", "Chicago", "Houston", "Philadelphia", "Buenos Aires", "Cordoba", "Rosario", "Mendoza", "La Plata"};
        const int lugaresNacimientoSize = sizeof(lugaresNacimiento) / sizeof(lugaresNacimiento[0]);

        const string estadosCiviles[] = {
            "Soltero", "Casado", "Divorciado", "Viudo", "Soltera", "Casada", "Divorciada", "Viuda"};
        const int estadosCivilesSize = sizeof(estadosCiviles) / sizeof(estadosCiviles[0]);

        ofstream dataFile(DATA_FILE, ios::binary);
        if (!dataFile.is_open())
            throw runtime_error("Error abriendo el archivo de datos.");

        ofstream indexFile(HASH_INDEX_FILE, ios::binary | ios::trunc); // Crear el archivo de indice de hash
        if (!indexFile.is_open())
            throw runtime_error("Error abriendo el archivo de indice hash.");

        auto startTime = chrono::high_resolution_clock::now();

        const string emailDomains[] = {"@gmail.com", "@yahoo.com", "@hotmail.com", "@outlook.com", "@live.com"};
        const int emailDomainsSize = sizeof(emailDomains) / sizeof(emailDomains[0]);

        // Contador para garantizar unicidad si es necesario

        vector<int> dniList = generateUniqueDni(RECORD_COUNT);

        // Agregar encabezado de columnas al inicio del archivo usando el delimitador con espacios ' | '
        dataFile << "Status |    DNI   |     Nombre    |     Apellido    | Genero |  FechaNac  | Edad | Departamento |   Provincia   |     Ciudad      |     Distrito      |           Ubicacion            | Nacionalidad |     LugarNac     |   TelRef   |             Correo              | EstadoCivil\n";

        const size_t BUFFER_SIZE = 100000; // Numero de registros a bufferizar
        string dataBuffer;
        vector<pair<uint32_t, uint64_t>> indexEntries;

        for (int i = 0; i < RECORD_COUNT; ++i)
        {
            int dni = dniList[i];
            string nombre = nombres[rand() % nombresSize];
            string apellido = apellidos[rand() % apellidosSize] + ' ' + apellidos[rand() % apellidosSize];
            string apellidoc = apellidos[rand() % apellidosSize];
            int edad = rand() % 100 + 1;

            string emailLocalPart = nombre + "." + apellidoc + to_string(dni % 1000);

            // Verificar si ya existe y agregar un numero si es necesario
            transform(emailLocalPart.begin(), emailLocalPart.end(), emailLocalPart.begin(), ::tolower);

            string emailDomain = emailDomains[rand() % emailDomainsSize];
            string correoElectronico = emailLocalPart + emailDomain;

            char genero = (rand() % 2 == 0) ? 'M' : 'F';
            int randomIndex = rand() % fechasNacimientoSize;
            int birthYear = 2024 - edad;
            string fechaNacimiento = fechasNacimiento[randomIndex] + "-" + to_string(birthYear);

            // 1. Seleccionar departamento
            auto departamento_it = peru_geografia.begin();
            advance(departamento_it, rand() % peru_geografia.size());
            string departamento = departamento_it->first;

            // 2. Seleccionar provincia
            auto provincia_it = departamento_it->second.begin();
            advance(provincia_it, rand() % departamento_it->second.size());
            string provincia = provincia_it->first;

            // 3. Seleccionar distrito
            auto distrito_it = provincia_it->second.begin();
            advance(distrito_it, rand() % provincia_it->second.size());
            string distrito = distrito_it->first;

            // Obtener la ciudad asociada al distrito
            string ciudad = distrito_it->second.first;

            // 4. Seleccionar ubicacion segun el distrito
            vector<string> &ubicaciones = distrito_it->second.second;
            string ubicacion = ubicaciones[rand() % ubicaciones.size()];

            // Construir la direccion completa
            string direccion = departamento + ", " + provincia + ", " + distrito + ", " + ubicacion;

            string nacionalidad = nacionalidades[rand() % nacionalidadesSize];
            string lugarNacimiento = lugaresNacimiento[rand() % lugaresNacimientoSize];
            string telefonoReferencia = to_string(generateUniquePhoneNumber(i));
            string estadoCivil = estadosCiviles[rand() % estadosCivilesSize];

            uint64_t pos = static_cast<uint64_t>(dataFile.tellp()) + dataBuffer.size();

            Record record(dni, nombre, apellido, genero, fechaNacimiento, edad, direccion, nacionalidad, lugarNacimiento, telefonoReferencia, correoElectronico, estadoCivil, departamento, provincia, ciudad, distrito, ubicacion);

            string recordStr = record.serialize();
            dataBuffer += recordStr;
            indexEntries.emplace_back(dni, pos);

            if ((i + 1) % BUFFER_SIZE == 0)
            {
                dataFile.write(dataBuffer.c_str(), dataBuffer.size());
                dataBuffer.clear();

                for (const auto &entry : indexEntries)
                {
                    indexFile.write(reinterpret_cast<const char *>(&entry.first), sizeof(int));
                    indexFile.write(reinterpret_cast<const char *>(&entry.second), sizeof(uint64_t));
                }
                indexEntries.clear();

                cout << (i + 1) << " registros generados..." << endl;
            }
        }

        if (!dataBuffer.empty())
        {
            dataFile.write(dataBuffer.c_str(), dataBuffer.size());
        }

        for (const auto &entry : indexEntries)
        {
            indexFile.write(reinterpret_cast<const char *>(&entry.first), sizeof(uint32_t));
            indexFile.write(reinterpret_cast<const char *>(&entry.second), sizeof(uint64_t));
        }

        dataFile.close();
        indexFile.close();
        auto endTime = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsedSeconds = endTime - startTime;
        cout << "Generacion de datos completada en " << elapsedSeconds.count() << " segundos." << endl;
        cout << "Cargando hash en memoria..." << endl;
        loadIndex();
    }

    // Funcion que permite ingresar al usuario un nuevo registro. Con validaciones incluidas
    void insertRecord()
    {
        uint32_t dni;
        unsigned short edad;
        string nombre, apellido, direccion, nacionalidad, lugarNacimiento, telefonoReferencia, correoElectronico, estadoCivil, departamento, provincia, ciudad, distrito, ubicacion;
        char genero;

        // Funcion que verifica si el input introducido es una letra
        auto esAlfabetico = [](const string &str) -> bool
        {
            return !str.empty() && all_of(str.begin(), str.end(), [](char c)
                                          { return isalpha(c) || isspace(c); });
        };

        auto esUbicacionValida = [](const string &str) -> bool
        {
            return !str.empty() &&
                   all_of(str.begin(), str.end(), [](char c)
                          { return isalnum(c) || c == ' ' || c == '.' || c == '-' || c == ','; }) &&
                   any_of(str.begin(), str.end(), [](char c)
                          { return isalnum(c); });
        };

        auto trim = [](const string &str) -> string
        {
            size_t first = str.find_first_not_of(' ');
            if (first == string::npos)
                return "";
            size_t last = str.find_last_not_of(' ');
            return str.substr(first, (last - first + 1));
        };

        // Ingreso de DNI
        cout << "\nIngrese DNI (8 digitos): ";
        while (!(cin >> dni) || dni < 10000000 || dni > 99999999)
        {
            cout << "DNI invalido. Debe ser un numero de 8 digitos. Intente nuevamente: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Verificar si el DNI ya existe
        uint64_t pos = findPositionInIndex(dni);
        if (pos != static_cast<uint64_t>(-1))
        {
            // Leer el estado del registro
            ifstream dataFile(DATA_FILE, ios::binary);
            if (!dataFile.is_open())
            {
                cout << "\nError abriendo el archivo de datos.\n";
                systemPauseWn();
                return;
            }

            dataFile.seekg(pos);
            char status;
            dataFile.get(status);
            dataFile.close();

            if (status == 'D')
            {
                cout << "\nDNI ya existe pero está eliminado. No se puede agregar el registro.\n";
            }
            else
            {
                cout << "\nDNI ya existe. No se puede agregar el registro.\n";
            }
            systemPauseWn();
            return;
        }

        // Ingreso y validacion del nombre (solo letras y espacios)
        do
        {
            cout << "Ingrese nombre: ";
            getline(cin, nombre);
            nombre = trim(nombre);
            if (nombre.length() > NAME_LENGTH)
            {
                nombre = nombre.substr(0, NAME_LENGTH);
            }
            if (!esAlfabetico(nombre))
            {
                cout << "Nombre invalido. Solo se permiten letras y espacios.\n";
            }
        } while (!esAlfabetico(nombre));

        // Ingreso y validacion del apellido (solo letras y espacios)
        do
        {
            cout << "Ingrese apellido: ";
            getline(cin, apellido);
            apellido = trim(apellido);
            if (apellido.length() > SURNAME_LENGTH)
            {
                apellido = apellido.substr(0, SURNAME_LENGTH);
            }
            if (!esAlfabetico(apellido))
            {
                cout << "Apellido invalido. Solo se permiten letras y espacios.\n";
            }
        } while (!esAlfabetico(apellido));

        // Validacion del genero
        cout << "Ingrese genero (M/F): ";
        while (!(cin >> genero) || (toupper(genero) != 'M' && toupper(genero) != 'F'))
        {
            cout << "Genero invalido. Intente nuevamente (M/F): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        genero = toupper(genero);

        // Validacion de la edad (solo numeros)
        cout << "Ingrese edad: ";
        while (true)
        {
            if (cin >> edad && edad >= 1 && edad <= 120)
            {
                // Verificar si hay caracteres adicionales en el buffer
                if (cin.peek() == '\n')
                {
                    cin.get(); // Consumir el salto de linea
                    break;     // Entrada valida, salir del bucle
                }
                else
                {
                    cout << "Edad invalida. No se permiten caracteres adicionales. Intente nuevamente: ";
                    cin.clear();                                         // Limpiar el estado de error de cin
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar el resto de la linea
                }
            }
            else
            {
                cout << "Edad invalida. Debe ser un numero entre 1 y 120. Intente nuevamente: ";
                cin.clear();                                         // Limpiar el estado de error de cin
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar el resto de la linea
            }
        }

        // Calcular el ano de nacimiento basado en la edad
        int currentYear = 2024;
        int birthYear = currentYear - edad;
        string fechaNacimiento = "01-01-" + to_string(birthYear);

        // Ingreso y validacion de la direccion detallada
        do
        {
            cout << "Ingrese departamento: ";
            getline(cin, departamento);
            departamento = trim(departamento);
            if (departamento.length() > 20)
            {
                departamento = departamento.substr(0, 20);
            }
            if (!esAlfabetico(departamento))
            {
                cout << "Departamento invalido. Solo se permiten letras y espacios.\n";
            }
        } while (!esAlfabetico(departamento));

        do
        {
            cout << "Ingrese provincia: ";
            getline(cin, provincia);
            provincia = trim(provincia);
            if (provincia.length() > 20)
            {
                provincia = provincia.substr(0, 20);
            }
            if (!esAlfabetico(provincia))
            {
                cout << "Provincia invalida. Solo se permiten letras y espacios.\n";
            }
        } while (!esAlfabetico(provincia));

        do
        {
            cout << "Ingrese ciudad: ";
            getline(cin, ciudad);
            ciudad = trim(ciudad);
            if (ciudad.length() > 25)
            {
                ciudad = ciudad.substr(0, 25);
            }
            if (!esAlfabetico(ciudad))
            {
                cout << "Ciudad invalida. Solo se permiten letras y espacios.\n";
            }
        } while (!esAlfabetico(ciudad));

        do
        {
            cout << "Ingrese distrito: ";
            getline(cin, distrito);
            distrito = trim(distrito);
            if (distrito.length() > 25)
            {
                distrito = distrito.substr(0, 25);
            }
            if (!esAlfabetico(distrito))
            {
                cout << "Distrito invalido. Solo se permiten letras y espacions. \n";
            }
        } while (!esAlfabetico(distrito));

        do
        {
            cout << "Ingrese ubicacion (ej. Av. Principal 1234): ";
            getline(cin, ubicacion);
            ubicacion = trim(ubicacion.substr(0, 35)); // Recortar a 35 caracteres y eliminar espacios

            if (!esUbicacionValida(ubicacion))
            {
                cout << "Ubicacion invalida.";
            }
        } while (!esUbicacionValida(ubicacion));

        // Construir la direccion completa
        direccion = departamento + ", " + provincia + ", " + ciudad + ", " + distrito + ", " + ubicacion;

        // Ingreso y validacion de nacionalidad (solo letras y espacios)
        do
        {
            cout << "Ingrese nacionalidad: ";
            getline(cin, nacionalidad);
            nacionalidad = trim(nacionalidad);
            if (nacionalidad.length() > NATIONALITY_LENGTH)
            {
                nacionalidad = nacionalidad.substr(0, NATIONALITY_LENGTH);
            }
            if (!esAlfabetico(nacionalidad))
            {
                cout << "Nacionalidad invalida. Solo se permiten letras y espacios.\n";
            }
        } while (!esAlfabetico(nacionalidad));

        // Ingreso y validacion del lugar de nacimiento (solo letras y espacios)
        do
        {
            cout << "Ingrese lugar de nacimiento: ";
            getline(cin, lugarNacimiento);
            lugarNacimiento = trim(lugarNacimiento);
            if (lugarNacimiento.length() > BIRTHPLACE_LENGTH)
            {
                lugarNacimiento = lugarNacimiento.substr(0, BIRTHPLACE_LENGTH);
            }
            if (!esAlfabetico(lugarNacimiento))
            {
                cout << "Lugar de nacimiento invalido. Solo se permiten letras y espacios.\n";
            }
        } while (!esAlfabetico(lugarNacimiento));

        // Validacion de telefono de referencia (9 digitos, comienza con '9')
        do
        {
            cout << "Ingrese telefono de referencia (9 digitos, comienza con '9'): ";
            getline(cin, telefonoReferencia);
            if (telefonoReferencia.length() != 9 || !all_of(telefonoReferencia.begin(), telefonoReferencia.end(), ::isdigit) || telefonoReferencia[0] != '9')
            {
                cout << "Telefono no valido. Debe contener exactamente 9 digitos y comenzar con '9'.\n";
            }
        } while (telefonoReferencia.length() != 9 || !all_of(telefonoReferencia.begin(), telefonoReferencia.end(), ::isdigit) || telefonoReferencia[0] != '9');

        // Validacion de correo electronico utilizando expresion regular
        do
        {
            cout << "Ingrese correo electronico: ";
            getline(cin, correoElectronico);
            correoElectronico = trim(correoElectronico);
            regex emailRegex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
            if (!regex_match(correoElectronico, emailRegex))
            {
                cout << "Correo electronico no valido. Ingrese un correo electronico valido.\n";
            }
        } while (!regex_match(correoElectronico, regex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)")));

        // Validacion de estado civil (solo Casad@, Solter@, Divorciad@, Viud@)
        do
        {
            cout << "Ingrese estado civil (Casad@/Solter@/Divorciad@/Viud@): ";
            getline(cin, estadoCivil);
            estadoCivil = trim(estadoCivil);
            transform(estadoCivil.begin(), estadoCivil.end(), estadoCivil.begin(), ::tolower);
            if (estadoCivil != "casado" && estadoCivil != "casada" && estadoCivil != "soltero" && estadoCivil != "soltera" && estadoCivil != "divorciado" && estadoCivil != "divorciada" && estadoCivil != "viudo" && estadoCivil != "viuda")
            {
                cout << "Estado civil invalido. Solo se permiten Casad@, Solter@, Divorciad@ o Viud@.\n";
            }
        } while (estadoCivil != "casado" && estadoCivil != "casada" && estadoCivil != "soltero" && estadoCivil != "soltera" && estadoCivil != "divorciado" && estadoCivil != "divorciada" && estadoCivil != "viudo" && estadoCivil != "viuda");

        // Guardar el registro (el codigo de guardado permanece igual)
        fstream dataFile(DATA_FILE, ios::in | ios::out | ios::binary);
        ofstream indexFile(HASH_INDEX_FILE, ios::binary | ios::app);

        if (!dataFile.is_open() || !indexFile.is_open())
        {
            cout << "\nError abriendo el archivo de datos o indice.\n";
            return;
        }

        dataFile.seekp(0, ios::end);
        uint64_t posNueva = static_cast<uint64_t>(dataFile.tellp());

        // Crear el objeto Record
        Record record(dni, nombre, apellido, genero, fechaNacimiento, edad, direccion, nacionalidad, lugarNacimiento, telefonoReferencia, correoElectronico, estadoCivil, departamento, provincia, ciudad, distrito, ubicacion);

        string recordStr = record.serialize();
        dataFile << recordStr;

        // Actualizar el índice en memoria
        indexMap.insert(dni, posNueva);

        // Escribir la nueva entrada al final del archivo de índice
        indexFile.seekp(0, ios::end);
        indexFile.write(reinterpret_cast<const char *>(&dni), sizeof(uint32_t));
        indexFile.write(reinterpret_cast<const char *>(&posNueva), sizeof(uint64_t));

        dataFile.close();
        indexFile.close();

        cout << "\nRegistro insertado correctamente.\n";
        systemPauseWn();
    }

    void MenuWorkFlow()
    {
        cout << system("pause");
        cout << "\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    void systemPauseWn()
    {
        system("pause");
        cout << "\n";
    }

    void menu()
    {
        while (true)
        {
            cout << "\n======================================\n";
            cout << "|         GESTION DE REGISTROS       |\n";
            cout << "======================================\n";
            cout << "|  1. Insertar registro              |\n";
            cout << "|  2. Buscar por DNI                 |\n";
            cout << "|  3. Eliminar por DNI               |\n";
            cout << "|  4. Salir                          |\n";
            cout << "======================================\n";
            cout << "Seleccione una opcion: ";
            unsigned short option;

            if (!(cin >> option))
            {
                cout << "\nOpcion no valida. Intentelo de nuevo.\n"
                     << endl;
                MenuWorkFlow();
                continue;
            }
            switch (option)
            {
            case 1:
                insertRecord();
                break;
            case 2:
            {
                int dni;
                cout << "\nIngrese el DNI a buscar: ";
                if (!(cin >> dni))
                {
                    cout << "\nEntrada invalida.\n"
                         << endl;
                    MenuWorkFlow();
                    continue;
                }
                searchByDni(dni);
                break;
            }
            case 3:
            {
                int dni;
                cout << "\nIngrese el DNI a eliminar: ";
                if (!(cin >> dni))
                {
                    cout << "\nEntrada invalida.\n"
                         << endl;
                    MenuWorkFlow();
                    continue;
                }
                deleteByDni(dni);
                break;
            }
            case 4:
                return; // Salir del bucle y la función
            default:
                cout << "\nOpcion no valida. Intentelo de nuevo.\n"
                     << endl;
                systemPauseWn();
            }
        }
    }

    void searchByDni(uint32_t dni)
    {
        uint64_t pos = findPositionInIndex(dni);
        if (pos == static_cast<uint64_t>(-1))
        {
            cout << "\nRegistro no encontrado.\n";
            systemPauseWn();
            return;
        }

        ifstream dataFile(DATA_FILE, ios::binary);
        if (!dataFile.is_open())
        {
            cout << "\nError abriendo el archivo de datos.\n";
            systemPauseWn();
            return;
        }

        dataFile.seekg(pos);
        string recordStr;
        getline(dataFile, recordStr); // Leer la línea completa del registro

        dataFile.close();

        if (recordStr.empty())
        {
            cout << "\nRegistro no encontrado en la posición especificada.\n";
            systemPauseWn();
            return;
        }

        Record record = Record::deserialize(recordStr);

        if (record.status == 'D')
        {
            cout << "\nEl registro ya ha sido eliminado.\n";
            systemPauseWn();
            return;
        }

        cout << "\nRegistro encontrado:\n";
        cout << left;
        cout << setw(20) << "Status: " << record.status << "\n";
        cout << setw(20) << "DNI: " << record.dni << "\n";
        cout << setw(20) << "Nombre: " << record.nombre << "\n";
        cout << setw(20) << "Apellido: " << record.apellido << "\n";
        cout << setw(20) << "Genero: " << record.genero << "\n";
        cout << setw(20) << "Fecha de Nacimiento: " << record.fechaNacimiento << "\n";
        cout << setw(20) << "Edad: " << record.edad << "\n";
        cout << setw(20) << "Departamento: " << record.departamento << "\n";
        cout << setw(20) << "Provincia: " << record.provincia << "\n";
        cout << setw(20) << "Ciudad: " << record.ciudad << "\n";
        cout << setw(20) << "Distrito: " << record.distrito << "\n";
        cout << setw(20) << "Ubicacion: " << record.ubicacion << "\n";
        cout << setw(20) << "Nacionalidad: " << record.nacionalidad << "\n";
        cout << setw(20) << "Lugar de Nacimiento: " << record.lugarNacimiento << "\n";
        cout << setw(20) << "Telefono: " << record.telefonoReferencia << "\n";
        cout << setw(20) << "Correo: " << record.correoElectronico << "\n";
        cout << setw(20) << "Estado Civil: " << record.estadoCivil << "\n";
        systemPauseWn();
    }

    // Elimina un registro mediante su DNI, Escribe un D de (delete) en el resgitro
    void deleteByDni(uint32_t dni)
    {
        uint64_t pos = findPositionInIndex(dni);
        if (pos == static_cast<uint64_t>(-1))
        {
            cout << "\nRegistro no encontrado.\n";
            systemPauseWn();
            return;
        }

        fstream dataFile(DATA_FILE, ios::in | ios::out | ios::binary);
        if (!dataFile.is_open())
        {
            cout << "Error abriendo el archivo de datos." << endl;
            return;
        }

        // Moverse a la posición del registro
        dataFile.seekg(pos);

        // Leer el primer carácter para verificar el estado
        char status;
        dataFile.get(status);
        if (status == 'D')
        {
            cout << "\nEl registro ya ha sido eliminado.\n";
            dataFile.close();
            systemPauseWn();
            return;
        }

        // Marcar el registro como eliminado
        dataFile.seekp(pos + 2); // Desplazarse a la posición correcta del 'status'
        dataFile.write("D", 1);
        dataFile.close();

        cout << "\nRegistro eliminado correctamente.\n";
        systemPauseWn();
    }
};

int main()
{
    try
    {
        Database db;
        ifstream dataFileCheck(DATA_FILE, ios::binary);
        ifstream indexFileCheck(HASH_INDEX_FILE, ios::binary);

        // Verificar si ambos archivos están disponibles y no están vacios
        if (!dataFileCheck.is_open() || dataFileCheck.peek() == ifstream::traits_type::eof() ||
            !indexFileCheck.is_open() || indexFileCheck.peek() == ifstream::traits_type::eof())
        {
            cout << "Generando datos..." << endl;
            db.generateData();
        }
        else
        {
            cout << "Archivo de datos y archivo de indice hash cargados correctamente." << endl;
        }
        dataFileCheck.close();
        indexFileCheck.close();

        db.loadIndex();
        db.menu();
    }
    catch (const exception &ex)
    {
        cerr << "Error: " << ex.what() << endl;
    }

    return 0;
}
