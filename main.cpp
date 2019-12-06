#include <iostream>

using namespace std;

void SHR(bool *LFSR, int ukuran, bool t)
{
    int n = ukuran - 1;
    
    for(int i = n; i > 0; --i)
        LFSR[i] = LFSR[i - 1];

    LFSR[0] = t;
}

void clocking(bool *LFSR_1, bool *LFSR_2, bool *LFSR_3, bool *k, int ukuran)
{
    for(int i = 0; i < ukuran; ++i)
    {   
        bool temp = LFSR_1[18] ^ LFSR_1[17] ^ LFSR_1[16] ^ LFSR_1[13] ^ k[i];
        SHR(LFSR_1, 19, temp);

        temp = LFSR_2[21] ^ LFSR_2[20] ^ k[i];
        SHR(LFSR_2, 22, temp);

        temp = LFSR_3[22] ^ LFSR_3[21] ^ LFSR_3[20] ^ LFSR_3[7] ^ k[i]; 
        SHR(LFSR_3, 23, temp);
    }
}

void majorityClocking(bool *LFSR_1, bool *LFSR_2, bool *LFSR_3)
{
    for(int i = 0; i < 100; ++i)
    {   
        bool m = LFSR_1[8] ^ LFSR_2[10] ^ LFSR_3[10]; 
        bool temp;

        if(LFSR_1[8] == m)
        {
            temp = LFSR_1[18] ^ LFSR_1[17] ^ LFSR_1[16] ^ LFSR_1[13];
            SHR(LFSR_1, 19, temp);
        }

        if(LFSR_2[10] == m)
        {
            temp = LFSR_2[21] ^ LFSR_2[20];
            SHR(LFSR_2, 22, temp);
        }
        
        if(LFSR_3[10] == m)
        {
            temp = LFSR_3[22] ^ LFSR_3[21] ^ LFSR_3[20] ^ LFSR_3[7]; 
            SHR(LFSR_3, 23, temp);
        }
    }
}

void pembangkitKunciAlir(bool *kunci, bool *LFSR_1, bool *LFSR_2, bool *LFSR_3)
{
    for(int i = 0; i < 228; ++i)
    {   
        kunci[i] = LFSR_1[18] ^ LFSR_2[21] ^ LFSR_3[22];

        bool m = LFSR_1[8] ^ LFSR_2[10] ^ LFSR_3[10]; 
        bool temp;

        if(LFSR_1[8] == m)
        {
            temp = LFSR_1[18] ^ LFSR_1[17] ^ LFSR_1[16] ^ LFSR_1[13];
            SHR(LFSR_1, 19, temp);
        }

        if(LFSR_2[10] == m)
        {
            temp = LFSR_2[21] ^ LFSR_2[20];
            SHR(LFSR_2, 22, temp);
        }
        
        if(LFSR_3[10] == m)
        {
            temp = LFSR_3[22] ^ LFSR_3[21] ^ LFSR_3[20] ^ LFSR_3[7]; 
            SHR(LFSR_3, 23, temp);
        }
    }
}

int main()
{   
    bool session_key[64] = {0, 1, 0, 1, 1, 1, 0, 1,
                            1, 1, 1, 1, 1, 0, 1, 0,
                            0, 0, 1, 0, 0, 1, 1, 0,
                            1, 0, 1, 0, 1, 0, 0, 0,
                            1, 1, 1, 1, 0, 1, 0, 0,
                            1, 1, 0, 1, 0, 0, 1, 0,
                            1, 1, 0, 1, 0, 1, 1, 1,
                            1, 0, 0, 1, 1, 0, 1, 0};
    bool frame[22] = {1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 
                      0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1};
    bool LFSR_1[19] = {0};
    bool LFSR_2[22] = {0};
    bool LFSR_3[23] = {0};

    clocking(LFSR_1, LFSR_2, LFSR_3, session_key, 64);
    clocking(LFSR_1, LFSR_2, LFSR_3, frame, 22);
    majorityClocking(LFSR_1, LFSR_2, LFSR_3);

    bool kunci[228];

    pembangkitKunciAlir(kunci, LFSR_1, LFSR_2, LFSR_3);

    int pilihan, n;
    string pesan, cipher, biner;

    cout << "Enkripsi dan Dekripsi Menggunakan A5/1\n";
    cout << "1. Enkripsi\n";
    cout << "2. Dekripsi\n";
    cout << "Pilihan: ";
    cin >> pilihan;

    while(cin.get() != '\n');

    switch(pilihan)
    {
        case 1:
            cout << "\nInput pesan yang akan dienkripsi:\n";
            getline(cin, pesan);

            biner = "";
            biner = [](string pesan) -> string {
                        string b = "";
                        int p_pesan = pesan.length();

                        for(int i = 0; i < p_pesan; ++i)
                        {
                            char karakter = pesan.at(i);

                            for(int j = 7; j >= 0; --j)
                                b += (((karakter >> j) & 1) + '0');
                        }
                        
                        // padding
                        while(b.length() % 228) b += '0';

                        return b;
                    }(pesan);
            
            n = biner.length();
            cipher = "";

            for(int i = 0; i < n; ++i)
            {
                cipher += ((biner.at(i) - '0') ^ kunci[i % 228]) + '0';
            }

            cout << "\nHasil enkripsi:\n";
            cout << cipher << "\n";

            break;
        case 2:
            cout << "\nMasukkan teks cipher yang akan didekripsi DALAM BINER:\n";
            cin >> cipher;

            n = cipher.length();
            biner = "";

            for(int i = 0; i < n; ++i)
            {
                biner += ((cipher.at(i) - '0') ^ kunci[(i % 228)]) + '0';
            }

            pesan = [](string biner) -> string {
                        string a = "";
                        int p_biner = biner.length();

                        for(int i = 0, c = 0; i < p_biner; c = 0)
                        {
                            for(int j = 7; j >= 0; --j, ++i)
                                c = c | ((biner.at(i) - '0') << j);

                            if(c == 0) break;
                            
                            a += static_cast<char>(c);
                        }

                        return a;
                    }(biner);

            cout << "\nHasil dekripsi: " << pesan;

            break;
        default:
            cout << "Pilihan tidak tersedia.";
    }

    return 0;
}