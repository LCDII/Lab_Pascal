#include <iostream>
#include <cctype>
#include <algorithm>
#include"Programm.h"


int main() {
    try {
        string source =
            "a = 12;\n"
            "i = 0;\n"
            "z = 1;\n"
            "while (i < a) do\n"
            "  if (i < 5) then\n"
            "    i = i + 1;\n"
            "    z = z * z;\n"
            "  else\n"
            "    i = i + 2;\n"
            "    z = z + 1;\n"
            "  end;\n"
            "end;\n";

        Programm prog(source);
        cout << "=== AST (binary with ';' as sequence root) ===\n";
        prog.print();
        cout << endl;
        prog.stepDebug();
        cout << "\n=== Execution ===\n";
        prog.execute();
        cout << "Execution finished." << endl;
    }
    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}