
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
   int Cinsert = 0;
   int Cfind = 0;

    for (int i = 0; i < SIMUL_SIZE; i++ ){
    Chaining hashChain(HASH_SIZE);
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
    }
    for (auto x: searchIdx) {
        Student s = studentList[x];
        hashChain.find(s);
    }

    Cinsert += hashChain.insert_count() ;
    Cfind += hashChain.find_count();
    hashChain.~Chaining();
    }

    cout << "Input Size: " << INPUT_SIZE;
    cout << "\n\tHash Chain \n"<<"Insert steps: " << Cinsert/SIMUL_SIZE << "\t Find steps " << Cfind/SIMUL_SIZE << "\t Total steps "<< Cinsert/SIMUL_SIZE + Cfind/SIMUL_SIZE ;
    return 0;
}
