
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
// Double Hashing algorithm 
class DoubleHashing{
    int bucket;
    vector <Student>table;
    int insert_steps = 0; 
    int find_steps = 0;
    public:
    int insert_count() {
        return insert_steps;
    }
    int find_count() {
        return find_steps;
    }
    // check if x is a prime number
    bool checkPrime(int x){
        if (x <=2){
            return false;
        }
        for( int i = 2; i < x/2; i++){
            if ( x % i == 0 ) { return false;}
        }
        return true;
    }
    /**
     * @brief Get the Prime number smaller than a specified number
     * 
     * @param n the specified number that the prime number is smaller than 
     * @param count the steps included to get a prime number (this function is o(n))
     * @return int the prime number
     */
    int getPrime(int n, int &count){
        if( n % 2 == 0){n++;} // ensure odd number
        while(checkPrime(n) == false && n < bucket){
            n-= 2; 
            count++;
        }
        return n;
    }
    // this function get a larger than n prime number. 
    // the return num will be the table size
    // this ensures that all data is able to find a place (avoid primary and secondary clustering)
    int getPrime_size(int n){
        if (n%2 == 0) {n++;}
        while(checkPrime(n) == false){
            n+=2;
        }
        return n;
    }
    DoubleHashing (int value){
        value = getPrime_size(value);
        this->bucket = value;
        table.resize(bucket);
    }
    int hashFunction (int x){
        return (x%bucket);
    }
    int hashFunction_sub(int x, int &insert_steps){
        int q = getPrime(x, insert_steps);
        return (q - x % q);
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
                // in hashFunction_sub has a call to a function with while loop o(n) 
                tempo = hashFunction(key + probe * hashFunction_sub(key, insert_steps));
                //for this while loop
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
                tempo = hashFunction(searchval + probe * hashFunction_sub(searchval, find_steps));
                find_steps++;
            }
        if (table[tempo] == search) return true;
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
    DoubleHashing hashDouble(HASH_SIZE);
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
        hashDouble.insert(s);
    }
    for (auto x: searchIdx) {
        Student s = studentList[x];
        hashDouble.find(s);
    }

    Dinsert += hashDouble.insert_count() ;
    Dfind += hashDouble.insert_count();
    }

    cout << "Input Size: " << INPUT_SIZE;
    cout << "\n\tDouble Hasing \n"<<"Insert steps: " << Dinsert/SIMUL_SIZE << "\t Find steps " << Dfind/SIMUL_SIZE << "\t Total steps "<< Dinsert/SIMUL_SIZE + Dfind/SIMUL_SIZE ;
    return 0;
}
