
#include <iostream> 
#include <fstream>
#include <vector>
using namespace std;

/*
 * Deliverable 1
 * create a class Student and overload ==, () operator
*/
class Student {
    string phone; 
    string name; 
    string email; 
    string course;
    public:
    Student (){
        phone = "NULL";
        name = "NULL";
        email = "NULL";
        course = "NULL";
    }
    //Constructor
    Student (string p, string n, string e, string c){
        this->phone = p;
        this->name = n;
        this->course = c;
        this->email = e;
    }
    /* Overloading == for comparing two Student objects */
    bool operator ==(const Student &obj){
        int count = 0;
        if (this->phone == obj.phone) count++;
        if (this->name == obj.name) count++;
        if (this->email == obj.email) count++;
        if (this->course == obj.course) count++;
        return (count == 4);
    }
    /* Overloading operator () to return a hash code*/
    int operator()(){
    string obj = "";
    obj = this->phone + this->name + this->email + this->course;
    int hash = 0;
        for (int i = 0;i < obj.length(); i++){
            hash = hash +(i + 1) * int(obj[i]);
        }
    return hash;
    }
    /* function prints out object values to check correct input*/
    void to_string (){
        cout << this->phone << " " << this->name ;
    }
    bool isEmpty(){
        if (this->name == "NULL") {return true;}
        return false;
    }
};
/*
Quadratic probing avoid primary clustering, but not secondary clustering 
That is when values have the same hash key after probing 
*/

// Quadratic Probing algorithm 
class QuadraticProbing{
    int bucket;
    vector <Student>table;
    int insert_steps = 0;
    int find_steps = 0;
    public:
    int insert_count (){
        return insert_steps;
    }
    int find_count(){
        return find_steps;
    }
    QuadraticProbing (int value){
        this->bucket = value;
        table.resize(bucket);
    }
    int hashFunction (int x){
        return (x%bucket);
    }
    Student& operator[](int i){return table[i];} 
    void insert (Student newStudent){
        int key = newStudent();
        key = hashFunction(key);
        if((table[key]).isEmpty() == true) // when there is no collision 
        {
            table[key] = newStudent;
        }
        else {
            int probe = 0;
            int tempo = key;
            while((table[tempo].isEmpty() != true) && probe <= bucket){
                probe++;
                tempo = hashFunction(key+pow(probe, 2));
                insert_steps++;
            }
            key = tempo;
            table[key] = newStudent; 
        }
    }
    void display(){
        for (int i = 0; i < bucket; i++){
            cout << i << "\t";
            (table[i]).to_string();
            cout << endl;
        }
    }
    bool find (Student search){
        int searchval = search();
        searchval = hashFunction(searchval);
        int probe = 0;
        int tempo = searchval;
        while((!(table[tempo] == search)) && probe <= bucket){
                probe++;
                tempo = hashFunction(searchval+pow(probe, 2));
                find_steps++;
            }
        if(table[tempo] == search) {return true;}
        return false;  
    }
};
vector<int> random_generate(int sizeVector, int input_size){
    vector<int> indexes;
    srand(time(0)); // in order to generate random numbers
    for(int i = 0; i < input_size; i++){
        indexes.push_back(rand() % sizeVector);
    }
    return indexes;
};

int main() {
    ifstream data; 
    vector <Student> studentList;
    data.open("data_100.csv");
    string phone;
    /* Deliverable 5
    * changing hash table size and input size
    */
    const int SIMUL_SIZE = 10;
    const int HASH_SIZE = 1100;
    const int INPUT_SIZE = 1000;
    /* Deliverable 2
    * read the file content into a vector of Students
    */
    while(getline(data,phone, ',')){
        string name; getline(data,name, ',');
        string email; getline(data,email, ',');
        string course; getline(data,course);
        Student s(phone, name, email, course);
        studentList.push_back(s);
   }
   int Cinsert = 0, Linsert =0, Qinsert = 0, Dinsert = 0;
   int Cfind = 0, Lfind = 0, Qfind = 0, Dfind = 0;

    for (int i = 0; i < SIMUL_SIZE; i++ ){
    QuadraticProbing quadraP(HASH_SIZE);
    // int i = 0;
    // for (auto x : studentList){
    //     hashChain.insert(x);
    // }
    // hashChain.display(); // display full chaining table. 
    vector<int> listIdx = random_generate(studentList.size()/2, INPUT_SIZE); // many collisions 
    vector<int> searchIdx = random_generate(studentList.size(), INPUT_SIZE); // many indexes will not be found (success & unsucessful cases)

    /* Deliverable 3
     * store the Student objects in hash
    */
    for (auto x: listIdx){
        Student s = studentList[x];
        quadraP.insert(s);    }
    for (auto x: searchIdx) {
        Student s = studentList[x];
        quadraP.find(s);
    }

    Qinsert += quadraP.insert_count() ;
    Qfind += quadraP.find_count();
    }

    cout << "Input Size: " << INPUT_SIZE;
    cout << "\n\tQuadratic Probing \n"<<"Insert steps: " << Qinsert/SIMUL_SIZE << "\t Find steps " << Qfind/SIMUL_SIZE<< "\t Total steps "<< Qinsert/SIMUL_SIZE + Qfind/SIMUL_SIZE;
    return 0;
}
