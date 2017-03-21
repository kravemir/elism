#include <stdio.h>
#include <iostream>

static void lex(const char *YYCURSOR)
{
    const char *tok;
    const char *YYMARKER;
    for(;;) {
        tok = YYCURSOR;
        /*!re2c
            re2c:define:YYCTYPE = char;
            re2c:yyfill:enable = 0;

            end = "\x00";
            bin = '0b' [01]+;
            oct = "0" [0-7]*;
            dec = [1-9][0-9]*;
            hex = '0x' [0-9a-fA-F]+;
            WS = " "|"\n";

            WS { continue; }
            bin { printf ("       bin: %.*s\n", (int)(YYCURSOR - tok), tok); continue; }
            dec { printf ("       dec: %.*s\n", (int)(YYCURSOR - tok), tok); continue; }
            oct { printf ("       oct: %.*s\n", (int)(YYCURSOR - tok), tok); continue; }
            hex { printf ("       hex: %.*s\n", (int)(YYCURSOR - tok), tok); continue; }

            "fn"        { printf ("   keyword: fn\n"); continue; }
            "return"    { printf ("   keyword: return\n"); continue; }

            [a-zA-Z]+ { printf ("identifier: %.*s\n", (int)(YYCURSOR - tok), tok); continue; }

            "("  { printf ("    symbol: (\n"); continue; }
            ")"  { printf ("    symbol: )\n"); continue; }
            "{"  { printf ("    symbol: {\n"); continue; }
            "}"  { printf ("    symbol: }\n"); continue; }
            ";"  { printf ("    symbol: ;\n"); continue; }

            end  { printf ("       end: success\n"); return; }
            [^]  { printf ("       end: error\n"); return; }
        */
    }
}

int main(int argc, char **argv)
{
    int length;
    char *buffer;

    // read whole stdin
    std::cin.seekg(0, std::ios::end);
    length = std::cin.tellg();
    std::cin.seekg(0, std::ios::beg);
    buffer = new char[length+1];
    std::cin.read(buffer, length);
    buffer[length] = 0;

    // perform lexical analysis
    lex(buffer);

    // free resources
    delete[] buffer;

    return 0;
}
