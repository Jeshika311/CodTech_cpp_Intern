// create a cpp application to read , write and append data to text files

#include<iostream>
#include<fstream>
#include<string>
using namespace std;

int main()
{
    string self_intro = "intro.txt";
    string data;

    // Write to file
    
    ofstream outfile(self_intro);    //open file for writing
    if (outfile.is_open()) {
        outfile << "Hello, My name is Jeshika.\n";
        outfile << "I am from kurukshetra.\n";
        outfile.close();
        cout << "Data written to file.\n";
    }
    else{
        cout<<"Unable to open file for writing.\n";
    }

    // Append to file

    ofstream appendfile(self_intro, ios::app); // open for appending
    if (appendfile.is_open()) {
        appendfile << "I am 18 years old.\n";
        appendfile.close();
        cout << "Data appended to file.\n";
    } else {
        cout << "Unable to open file for appending.\n";
    }

    // Read from file

    ifstream infile(self_intro); // open for reading
    if (infile.is_open()) {
        cout << "\nReading data from file:\n";
        while (getline(infile, data)) {
            cout << data << endl;
        }
        infile.close();
    } else {
        cout << "Unable to open file for reading.\n";
    }

    return 0;

}