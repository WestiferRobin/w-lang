#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<string> test_files{
        "algro.w",
        "calc.w",
        "stress_test.w",
        "test_space.w",
        "test.w"
    };
    vector<string> failed_files;
    int error_count = 0;

    for (auto it=test_files.begin(); it != test_files.end(); it++)
    {
        string command = "./wlang test/" + *it;
        cout << "NOW RUNNING: " << command << endl;
        int result = system(command.c_str());
        cout << "ENDING " << *it << " WITH A " << to_string(result) << endl;
        if (result != 0) 
        {
            error_count++;
            failed_files.push_back(*it);
        }
    }

    if (error_count != 0)
    {
        for (auto it=failed_files.begin(); it != failed_files.end(); it++)
        {
            cout << "Please Check file: " << *it << endl;
        }
    }
    else 
    {
        cout << "FLAWLESS CODE!!!!" << endl;
    }

    return error_count;
}