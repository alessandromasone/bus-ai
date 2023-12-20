//
//  funzione.h
//  Bus
//
//  Created by Alessandro masone on 21/04/22.
//

#ifndef funzione_h
#define funzione_h

namespace funzioni {
class funzioni {
public:
    void debug(bool attivo, string s) {
        if (attivo) {
            cout << s << endl;
        }
    }
    int random(int min, int max) {
        random_device dev;
        mt19937 rng(dev());
        uniform_int_distribution<mt19937::result_type> dist1(min, max);
        int value = dist1(rng);
        return value;
    }
    int input(vector<int> &vettore) {
        char s[65000] = {};
        FILE *fp;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        int init_size = 0;
        char *ptr;
        int i = 0;
        fp = fopen("dati.txt", "r");
        while ((read = getline(&line, &len, fp)) != -1) {
            init_size = (int)strlen(line);
            ptr = strtok(line, ":");
            while (ptr != NULL) {
                if (i %2 != 0) {
                    strcpy(s, ptr);
                    s[strlen(s)] = '\0';
                    vettore.push_back(atoi(s));
                }
                ptr = strtok(NULL, ":");
                i++;
            }
        }
        fclose(fp);
        if (vettore.at(0) < 1) {
            return 1;
        }
        if (vettore.at(1) < 1 || vettore.at(1) > 100) {
            return 2;
        }
        if (vettore.at(2) < 1 || vettore.at(2) > 139) {
            return 3;
        }
        if (vettore.at(3) < 0) {
            return 4;
        }
        return 0;
        
        
    }
};
funzioni funzione;
}

using namespace funzioni;

template<typename T>
auto empty_swap(std::vector<T>& vec) {
   std::vector<T>().swap(vec);
   return vec.capacity();
}

#endif /* funzione_h */
