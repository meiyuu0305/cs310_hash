
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
// create a structure, such that it stores Student object and a pointer to the next structure
struct Node {
    Student data; 
    Node* next;
    public: 
    Node(){
        next = nullptr;
    }
};
// create a linkedList class, with heads to connect the node structures together
class LinkedList {
    private:
        Node *head;
    public:
        LinkedList(){head = nullptr;}
        int count;
        Node * createNewNode(Student value){
            Node *node = new Node;
            node->data = value;
            return node;
        }
        void push_front(Student newStudent){
        Node * node = createNewNode (newStudent);
        count++;
        if (head == nullptr){
            head = node;
            head->next = nullptr;
        }
        else {
            node->next = head;
            head = node;
        }
    }
    bool isEmpty(){
        if (this->head == nullptr) {return true;}
        return false;
    }
    bool pop_front (){ 
            if(count==0 || head == nullptr)
                return false;
            if(head->next== nullptr){ // if this is the only node
                delete head;
                head = nullptr;
                count =0;
                return true;}
            Node * prevHead = head;
            // head should point to the next node
            head = head->next;
            // the current head would be deleted
            delete prevHead;
            count--;
            return true;
        }
        ~LinkedList() {
            while(pop_front());
        } // destructor 
    void display(){
    Node * ptr = head;
    while(ptr != nullptr){
                Student tempo = ptr->data;
                cout << "-->"; tempo.to_string();
                ptr = ptr->next;
            }
    }
    /**
     * @brief find the student object
     * @param search the object that needs to be search for
     * @param count how many steps are in a find function
     * @return true when successfully found the object 
     * @return false when unsuccessful, searched from head to the end of linked list 
     */
    bool find(Student search, int &count){
        Node * ptr = head;
        while (ptr != nullptr){
            count++;
            if (ptr->data == search){return true;}
        ptr = ptr->next;
        }
        return false;
    }
};
// create a list class, has a pointer to point to an "array" of linked list - hash table
class List {
    public:
    int count;
    LinkedList* array;
    LinkedList& operator[](int i){return array[i];} 
    List(int i){ // to initiate a table 
        array = new LinkedList[i-1];
        count = i-1;
    }
    /**
     * @brief put an object in the front of linked list at specified index 
     * @param k index of the linked list
     * @param newStudent the inserting Student object
     */
    void insert (int k, Student newStudent){
        array[k].push_front(newStudent);
        // cout << "inserted "; newStudent.to_string();
        // cout << " at " << k << endl;
    }
    void display() {
        for(int i = 0; i < count; i++){
        cout << i;
        array[i].display();  
        cout << endl;
        }
    }
    /**
     * @brief find the search for object in the specified linked list 
     * 
     * @param search the Student object searching for
     * @param index the index of the specified linked list 
     * @param find_steps the steps to find this object
     * @return true - when the index is found, and object is found at index linked list 
     * @return false - when the Linked list at the index is empty, or unable to find object
     */
    bool find (Student search, int index, int &find_steps){
        if (array[index].isEmpty() != true){
        if(array[index].find(search, find_steps) == true) return true;
        }
        return false;
    }
    ~List(){
        array->pop_front();
    }
   
};

// Chaining algorithm 
class Chaining{
    int bucket;
    List *table;
    int insert_steps; 
    int find_steps;
    public:
    int insert_count() {
        return this->insert_steps;
    }
    int find_count() {
        return this->find_steps;
    }
    Chaining (int value){
        this->bucket = value;
        table = new List(bucket);
    }
    int hashFunction (int x){
        return (x%bucket);
    }
    void insert (Student newStudent){
        int key = newStudent();
        key = hashFunction(key);
        table->insert(key, newStudent);
        //since both hashFunction and insert functions are both O(1). no insert steps were added
    }
    void display(){
        table->display();
    }
    ~Chaining(){ table->~List();}
    bool find (Student search){
        int searchval = search();
        searchval = hashFunction(searchval);
        // hashFunction does not have loops - constant time O(1)
        // find function has a function call to a function with loops
        return table->find(search, searchval, find_steps);
    }
};
// Linear Probing algorithm 
class LinearProbing{
    int bucket;
    vector <Student>table;
    int insert_steps = 0; 
    int find_steps = 0;
    public:
    int insert_count() {
        return insert_steps;
    }
    int find_count(){
        return find_steps;
    }
    LinearProbing (int value){
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
        int probe = 0;
        int tempo = key;
        if((table[key]).isEmpty() == true) // when there is no collision 
        {
            table[key] = newStudent;
        }
        else {
            while((table[tempo].isEmpty() != true) && probe < bucket){
                probe++;
                tempo = hashFunction(key+probe);
                insert_steps++;
                //while loop adds to insert_steps
            }
            key = tempo;
            table[key] = newStudent; 
        }
        // cout << "inserted ";
        // newStudent.to_string();
        // cout << " at " << key << endl;
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
        while((!(table[tempo] == search)) && probe < bucket){
                probe++;
                tempo = hashFunction(searchval+probe);
                find_steps++;
            }
        if(table[tempo] == search) return true;
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
    Chaining hashChain(HASH_SIZE);
    LinearProbing linearP(HASH_SIZE);
    QuadraticProbing quadraP(HASH_SIZE);
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
        hashChain.insert(s);
        linearP.insert(s);
        quadraP.insert(s);
        hashDouble.insert(s);
    }
    for (auto x: searchIdx) {
        Student s = studentList[x];
        hashChain.find(s);
        linearP.find(s);
        quadraP.find(s);
        hashDouble.find(s);
    }

    Cinsert += hashChain.insert_count(), Linsert +=linearP.insert_count(), Qinsert += quadraP.insert_count(), Dinsert += hashDouble.insert_count() ;
    Cfind += hashChain.find_count(), Lfind += linearP.find_count(), Qfind += quadraP.find_count(), Dfind += hashDouble.insert_count();
    hashChain.~Chaining();
    }

    cout << "Input Size: " << INPUT_SIZE;
    cout << "\n\tHash Chain \n"<<"Insert steps: " << Cinsert/SIMUL_SIZE << "\t Find steps " << Cfind/SIMUL_SIZE << "\t Total steps "<< Cinsert/SIMUL_SIZE + Cfind/SIMUL_SIZE ;
    cout << "\n\tLinear Probing \n"<<"Insert steps: " << Linsert/SIMUL_SIZE << "\t Find steps " << Lfind/SIMUL_SIZE << "\t Total steps "<< Linsert/SIMUL_SIZE + Lfind/SIMUL_SIZE ;
    cout << "\n\tQuadratic Probing \n"<<"Insert steps: " << Qinsert/SIMUL_SIZE << "\t Find steps " << Qfind/SIMUL_SIZE<< "\t Total steps "<< Qinsert/SIMUL_SIZE + Qfind/SIMUL_SIZE;
    cout << "\n\tDouble Hasing \n"<<"Insert steps: " << Dinsert/SIMUL_SIZE << "\t Find steps " << Dfind/SIMUL_SIZE << "\t Total steps "<< Dinsert/SIMUL_SIZE + Dfind/SIMUL_SIZE ;
    return 0;
}
