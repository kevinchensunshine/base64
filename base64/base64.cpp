

#include <iostream>
#include<string>
#include <vector>
#include <fstream>
using namespace std;
char base64_alphabet[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
    };

int reverse_base64_alphabet[128] = {
    -1,//0
    -1,//1
    -1,//2
    -1,//3
    -1,//4
    -1,//5
    -1,//6
    -1,//7
    -1,//8
    -1,//9
    -1,//10
    -1,//11
    -1,//12
    -1,//13
    -1,//14
    -1,//15
    -1,//16
    -1,//17
    -1,//18
    -1,//19
    -1,//20
    -1,//21
    -1,//22
    -1,//23
    -1,//24
    -1,//25
    -1,//26
    -1,//27
    -1,//28
    -1,//29
    -1,//30
    -1,//31
    -1,//32
    -1,//33
    -1,//34
    -1,//35
    -1,//36
    -1,//37
    -1,//38
    -1,//39
    -1,//40
    -1,//41
    -1,//42
    62,//43
    -1,//44
    -1,//45
    -1,//46
    63,//47
    52,//48
    53,//49
    54,//50
    55,//51
    56,//52
    57,//53
    58,//54
    59,//55
    60,//56
    61,//57
    -1,//58
    -1,//59
    -1,//60
    -1,//61
    -1,//62
    -1,//63
    -1,//64
    0,//65
    1,//66
    2,//67
    3,//68
    4,//69
    5,//70
    6,//71
    7,//72
    8,//73
    9,//74
    10,//75
    11,//76
    12,//77
    13,//78
    14,//79
    15,//80
    16,//81
    17,//82
    18,//83
    19,//84
    20,//85
    21,//86
    22,//87
    23,//88
    24,//89
    25,//90
    -1,//91
    -1,//92
    -1,//93
    -1,//94
    -1,//95
    -1,//96
    26,//97
    27,//98
    28,//99
    29,//100
    30,//101
    31,//102
    32,//103
    33,//104
    34,//105
    35,//106
    36,//107
    37,//108
    38,//109
    39,//110
    40,//111
    41,//112
    42,//113
    43,//114
    44,//115
    45,//116
    46,//117
    47,//118
    48,//119
    49,//120
    50,//121
    51,//122
    -1,//123
    -1,//124
    -1,//125
    -1,//126
    -1,//127
};

union base64_input_group {
    uint8_t octets[4];
    struct {
        uint32_t sextet_three : 6;
        uint32_t sextet_two : 6;
        uint32_t sextet_one : 6;
        uint32_t sextet_zero : 6;
        uint32_t unused : 8;
    } s ;
};

bool encode_group(const uint8_t group[3], size_t input_size, char output_characters[4]) {
    if (input_size > 3 || input_size < 1) {
        return false;
    }
    base64_input_group input_group;
    input_group.octets[2] = group[0];
    if (input_size > 1) {
        input_group.octets[1] = group[1];
    }
    else {
        input_group.octets[1] = 0;
    }
    if (input_size > 2) {
        input_group.octets[0] = group[2];;
    }
    else {
        input_group.octets[0] = 0;
    }
 

    output_characters[0] = base64_alphabet[input_group.s.sextet_zero];
    output_characters[1] = base64_alphabet[input_group.s.sextet_one];
    if (input_size > 1) {
        output_characters[2] = base64_alphabet[input_group.s.sextet_two];
    }
    else {
        output_characters[2] = '=';
    }
    if (input_size > 2) {
        output_characters[3] = base64_alphabet[input_group.s.sextet_three];
    }
    else {
        output_characters[3] = '=';
    }
    return true;
}

bool encoder(const vector<uint8_t>& s, vector<char>& output_string) {
    size_t number_of_groups = (s.size() + 2) / 3;
    output_string.resize(number_of_groups * 4);
    for (size_t i = 0, j = 0; i < s.size(); i += 3, j += 4) {
        encode_group(s.data() + i,
            (s.size() - i >= 3) ? 3 : (s.size() - i),
            output_string.data() + j);
    }
    return true;
}


bool decode_group(const char input_characters[4], size_t input_size, uint8_t output_group[3]) {
    base64_input_group input_group;
    for (size_t i = 0; i < input_size; i++) {
        if (input_characters[i] < 0) {
            return false;
        } 
        int base64_character = reverse_base64_alphabet[input_characters[i]];
        if (base64_character < 0) {
            return false;
        }
    }
    input_group.s.sextet_zero = reverse_base64_alphabet[input_characters[0]];
    input_group.s.sextet_one = reverse_base64_alphabet[input_characters[1]];
    if (input_size > 2) {
        input_group.s.sextet_two = reverse_base64_alphabet[input_characters[2]];
    }
    if (input_size > 3) {
        input_group.s.sextet_three = reverse_base64_alphabet[input_characters[3]];
    }
    output_group[0] = input_group.octets[2];
    if (input_size > 2) {
        output_group[1] = input_group.octets[1];
    }
    if (input_size > 3) {
        output_group[2] = input_group.octets[0];
    }
    return true;
}
bool decoder(const vector<char>& encoded, vector<uint8_t>& decoded) {
    if ((encoded.size() % 4) != 0) {
        return false;
    }
    size_t l = encoded.size();
    if (encoded[l - 1] == '=') {
        l--;
    }
    if (encoded[l - 1] == '=') {
        l--;
    }
    size_t number_of_full_groups = l / 4;
    size_t remainder = l % 4;
    size_t output_size = number_of_full_groups * 3;
    if (remainder == 2) {
        output_size++;
    }
    else if (remainder == 3) {
        output_size += 2;
    }
    decoded.resize(output_size);
    for (size_t i = 0, j = 0; i < l; i += 4, j += 3) {
        if (!decode_group(encoded.data() + i, (l - i >= 4) ? 4 : (l - i), decoded.data() + j)) {
            return false;
        }
    }
    return true;
}


/*
Decoder:
1. Check if input is a multiple of 4
2. Check if last 2 chars are ==. If true, consider padding of 2 bytes
3. Check is last char is =. If true, consider padding of a byte
4. Check if every character is valid base64 character with reverse lookup table of numbers/smaller than 0 and add special characters for values over 64
Elements of table are signed
*/
int main(int argc, char** argv)
{
    if (argc < 3) {
        cout << "Usage:base64.exe <-e|-d> <filename>\n";
        return -1;
    }
    if (string(argv[1]) == "-e") {
        ifstream fs(argv[2], ios::binary);
        if (!fs) {
            cout << "Unable to open file\n";
            return -1;
        }
        ofstream output_fs(string(argv[2]) + ".b64");
        if (!output_fs) {
            cout << "Unable to create file\n";
        }
        vector<uint8_t> buffer;
        vector<char> encoded;

        while (fs.good()) {
            buffer.resize(75);
            fs.read((char*)buffer.data(), 75);
            buffer.resize(fs.gcount());
            encoder(buffer, encoded);
            output_fs.write(encoded.data(), encoded.size());
            output_fs << "\n";
        }
    } 
    else if (string(argv[1]) == "-d") {
        ifstream fs(argv[2]);
        if (!fs) {
            cout << "Unable to open file\n";
            return -1;
        }
        ofstream output_fs(string(argv[2]) + ".decoded", ios::binary);

        if (!output_fs) {
            cout << "Unable to create file\n";
        }
        string line;
        vector<uint8_t> output;
        while (fs.good()) {
            getline(fs, line);
            if (!line.empty()) {
                vector<char> input(line.begin(), line.end());
                if (!decoder(input, output)) {
                    cout << "Error: failed to decode\n";
                    break;
                }
                output_fs.write((char*) output.data(), output.size());
            }
        }
    }
    else {
        cout << "Usage:base64.exe <-e|-d> <filename>\n";
    }
    return 0;
 
    
    

    /*const char* s = "light wor";
    vector<uint8_t> sample_string(s, s + strlen(s));
    auto r = encoder(sample_string);
    string rs(r.begin(), r.end());
    cout << rs << "\n";
    vector<uint8_t> decoded;
    if (!decoder(r, decoded))
    {
        cout << "Decoding failed\n";
    }
    else
    {
        string decoded_string(decoded.begin(), decoded.end());
        cout << decoded_string << "\n";
    }*/
}


