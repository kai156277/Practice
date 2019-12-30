#include "EnumNames.h"
#include "EnumType.h"

#include <iostream>

using namespace std;

int main()
{
    system("chcp 65001");
    cout << "Hello World!" << endl;
    {
        keyInfo key = findByKey(One::Two::Test1);
        cout << "find by key Test1:" << key.name << "-" << key.value << "-" << key.describe << endl;
    }
    {
        keyInfo key = findByKey(One::Two::Test16);
        cout << "find by key Test16:" << key.name << "-" << key.value << "-" << key.describe << endl;
    }
    {
        keyInfo key = findByName("One::Two::Test1");
        cout << "find by name Test1:" << key.name << "-" << key.value << "-" << key.describe << endl;
    }
    {
        keyInfo key = findByName("One::Two::Test16");
        cout << "find by name Test1:" << key.name << "-" << key.value << "-" << key.describe << endl;
    }
    {
        keyInfo key = findByKey((One::Two::Test) 255);
        cout << "find by key 255:" << key.name << "-" << key.value << "-" << key.describe << endl;
    }
    {
        keyInfo key = findByName("One::Two::Test17");
        cout << "find by name Test17:" << key.name << "-" << key.value << "-" << key.describe << endl;
    }
    return 0;
}
