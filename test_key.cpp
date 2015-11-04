


#include <iostream>
#include <vector>
#include <ctime>
#include <type_traits>


using namespace std;





const int MAX_DIG = 90;
const int TOP_DIG = MAX_DIG + 1;



// actually a number of previous characters, not a digit simbol
inline
bool is_digit(char c) {
    return 0 <= c && c <= MAX_DIG;
}


// key's format: <letter>[<num>]
//    "aa3b12de"
//    "aaaaa" or "a5"
template<typename It>
int cmp_key(It beg1, It end1, It beg2, It end2) {
    size_t s1 = end1 - beg1;
    size_t s2 = end2 - beg2;

    if (!s1 && !s2)
        return 0;
    else if (!s1)
        return -1;
    else if (!s2)
        return 1;


    char c1 = *beg1;
    char c2 = *beg2;

    It tmp1 = beg1; ++tmp1;
    It tmp2 = beg2; ++tmp2;
        
    char n1 = tmp1 != end1 ? (is_digit(*tmp1) ? *(++beg1) : 1) : 1;
    char n2 = tmp2 != end2 ? (is_digit(*tmp2) ? *(++beg2) : 1) : 1;


    while (beg1 != end1 && beg2 != end2) {

        if (c1 > c2) 
            return 1;
        else if (c1 < c2)
            return -1;

        if (n1 < n2) {
            n2 -= n1;
            n1 = 0;
        }
        else {
            n1 -= n2;
            n2 = 0;
        }

        if (!n1) {
            ++beg1;
            if (beg1 != end1) {
                c1 = *beg1;
                tmp1 = beg1; ++tmp1;
                n1 = tmp1 != end1 ? (is_digit(*tmp1) ? *(++beg1) : 1) : 1;
            }
        }
        if (!n2) {
            ++beg2;
            if (beg2 != end2) {
                c2 = *beg2; 
                tmp2 = beg2; ++tmp2;
                n2 = tmp2 != end2 ? (is_digit(*tmp2) ? *(++beg2) : 1) : 1;
            }
        }

    }

    if (!n1 && !n2)
        return 0;
    else if (!n1)
        return -1;
    return 1;
}


class key_t {
public:
    typedef unsigned char CHAR;
    typedef vector<CHAR>::iterator iterator;

private:
    vector<CHAR> storage;

public:
    
    iterator begin() { return storage.begin(); }
    iterator end() { return storage.end(); }

    size_t size() const {return storage.size();}

    void push_back(CHAR c) { 
        size_t s = size();


        if (is_digit(c)) {
            if (is_digit(storage[s-1])) {
                if (TOP_DIG > int(storage[s-1]) + c - 1) {
                    storage[s-1] += c - 1;
                }
                else {
                    storage.push_back(storage[s-2]);
                    char v = int(storage[s-1]) + c - 1 - MAX_DIG;
                    storage[s-1] = MAX_DIG;
                    if (1 < v)
                        storage.push_back(v);
                }
            }
            else {
                storage.push_back(c);
            }
        }
        else if (2 <= s) {
            if (is_digit(storage[s-1])) {
                if (storage[s-2] == c) {
                    if (TOP_DIG > int(storage[s-1]) + 1) {
                        ++storage[s-1];
                    }
                    else {
                        //storage[s-1] = MAX_DIG;
                        storage.push_back(storage[s-2]);
                    }
                }
                else 
                    storage.push_back(c);
            }
            else if (storage[s-1] == c) {
                storage.push_back((char)2);
            }
            else
                storage.push_back(c);
        }
	else if (1 == s && storage[s-1] == c) {
            storage.push_back((char)2);
        }
        else {
            storage.push_back(c);
        } 
    }

    template<typename T>
    void append(T&& other) {
        size_t new_size = storage.size() + other.size();
        if (new_size > storage.capacity())
            storage.reserve(new_size * 2);

        storage.insert(storage.end(), other.begin(), other.end());
    }

    template<typename T>
    void append_push(T&& other) {
        size_t new_size = storage.size() + other.size();
        if (new_size > storage.capacity())
            storage.reserve(new_size * 2);

        if (2 <= other.size()) {
            typename remove_reference<T>::type::iterator it = other.begin();
            push_back(*(it++));
            push_back(*(it++));

            storage.insert(storage.end(), it, other.end());
        }
        else {
            for (typename remove_reference<T>::type::iterator it = other.begin(); it != other.end(); ++it)
                push_back(*it);
        }
    }
        

    friend key_t make_key(const char* s); 


    ostream& print(ostream& os) const {
        for (int i = 0; i < storage.size(); ++i)
            os << (char)(is_digit(storage[i]) ? storage[i] + '0' : storage[i]);
        return os;
    }
};

ostream& operator<< (ostream& os, const key_t& key) {
    return key.print(os);
}


key_t make_key(const char* s) {
    key_t key;
    unsigned char dig = 0;

    while (*s) {
        if ('0' <= *s && *s <= '9')
            dig = dig * 10 + *s - '0';
            //key.storage.push_back(*s - '0');
        else {
            if (dig != 0) {
                key.storage.push_back(dig);
                dig = 0;
            }
            key.storage.push_back(*s);
        }
        ++s;
    }

    if (dig != 0) 
        key.storage.push_back(dig);

    return key;
}


template<typename T>
void test(T&& k1, T&& k2, int res, const char* name) {
    int r = cmp_key(k1.begin(), k1.end(), k2.begin(), k2.end());
    if (r == res)
        cout << name << " OK" << endl;
    else
        cout << name << " FAILED, result " << r << " expected " << res << endl;

}



void test1() {
    {
        key_t k1 = make_key("aa2");
        key_t k2 = make_key("a3");

        cout << k1 << " vs " << k2 << " : " << cmp_key(k1.begin(), k1.end(), k2.begin(), k2.end()) << endl;
    }

    {
        key_t k1 = make_key("a1a1a1");
        key_t k2 = make_key("aaa");
        cout << k1 << " vs " << k2 << " : " << cmp_key(k1.begin(), k1.end(), k2.begin(), k2.end());
    }

    cout << endl;
}

void test_append() {
    key_t k1 = make_key("maxim alekseykin");
    key_t k2 = make_key(" and ");
    k1.append(k2);
    k1.append(make_key("eleonora alexeykina"));

    key_t check = make_key("maxim alekseykin and eleonora alexeykina");

    test(k1, check, 0, "test_append");
}



void test_push_back() {
    key_t key;

    key.push_back((char)'m');
    key.push_back((char)'m');
    key.push_back((char)'a');
    key.push_back((char)'a');
    key.push_back((char)'a');
    key.push_back((char)'x');
    key.push_back((char)'i');
    key.push_back((char)'i');
    key.push_back((char)'i');
    key.push_back((char)'i');
    key.push_back((char)'m');

    string sk;
    for (key_t::iterator it = key.begin(); it != key.end(); ++it) 
        sk += is_digit(*it) ? *it + '0' : *it;

    string check("m2a3xi4m");

    if (check == sk)
        cout << "test_push_back OK" << endl;
    else
        cout << "test_push_back FAILED, '" << sk << "' expected '" << check << "'" << endl;
}


void test_append2() {
    cout << "test_append2" << endl;

    key_t to_append(make_key("a2bc3d4ef5g3igk9lm2nopqr4stu5vwxyz6a8"));
   

    key_t k1;
    clock_t start = clock();
    for (int i = 0; i < 200000; ++i)
        k1.append(to_append);
    cout << "    creation: " << (float(clock() - start) / CLOCKS_PER_SEC) << " sec" << endl;

    key_t k2;
    start = clock();
    for (int i = 0; i < 200000; ++i)
        k2.append_push(to_append);
    cout << "    creation: " << (float(clock() - start) / CLOCKS_PER_SEC) << " sec" << endl;
   
    start = clock();
    for (int i = 0; i < 10; ++i)
       cmp_key(k1.begin(), k1.end(), k1.begin(), k1.end());
    cout << "    cmp: " << (float(clock() - start) / CLOCKS_PER_SEC) << " sec" << endl;
 
    start = clock();
    for (int i = 0; i < 10; ++i)
       cmp_key(k2.begin(), k2.end(), k2.begin(), k2.end());
    cout << "    cmp: " << (float(clock() - start) / CLOCKS_PER_SEC) << " sec" << endl;
 
    test(k1, k2, 0, "   cmp");

//    cout << k1 << endl;
//    cout << k2 << endl;
    
}


void test_big_nums() {
    key_t k;
    k.append_push(make_key("a50sd18baca80"));
    k.append_push(make_key("a50sd18baca80"));

    vector<char> tmp;
    for (int i = 0; i < 50; ++i) 
        tmp.push_back('a');
    tmp.push_back('s');
    for (int i = 0; i < 18; ++i)
        tmp.push_back('d');
    tmp.push_back('b');
    tmp.push_back('a');
    tmp.push_back('c');
    for (int i = 0; i < 80; ++i)
        tmp.push_back('a');

    key_t check;
    check.append(tmp);
    check.append(tmp);
    

    test(k, check, 0, "test big nums");


}



int main(int argc, const char* argv[]) {

    test1();

    test(make_key("abc"),  make_key("bcd"), -1, "test2");
    test(make_key("aaaa"), make_key(""),     1, "test3");
    test(make_key("aaa4"), make_key("a6"),   0, "test4");
    test(make_key("aaa4"), make_key("a6b"), -1, "test5");
    test(string("maxim"),  string("elya"),   1, "test6");


    test(make_key(""), make_key(""), 0, "test");


    test_append();
    test_push_back();
    test_append2();
    test_big_nums();



    return 0;
}





