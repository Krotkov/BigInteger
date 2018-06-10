// Created by user on 02.06.18.
//

#ifndef BIGINT_MY_VECTOR_H
#define BIGINT_MY_VECTOR_H

#include <vector>
#include <memory>
#include <cassert>
#include <cstring>
#include <cmath>

struct my_vector {
private:
    static const unsigned int SMALL_SIZE = 5;
    union {
        unsigned int small[SMALL_SIZE];
        std::shared_ptr<std::vector <unsigned int> > big;
    };
    unsigned int _size;
    bool is_small;

    void make_big();
    void check_count();

public:
    my_vector();
    ~my_vector();
    my_vector(my_vector const &other);
    my_vector &operator=(my_vector const &other);


    unsigned int size() const;
    bool empty();

    void push_back(unsigned int x);

    void pop_back();

    unsigned int &operator[](unsigned int ind);

    unsigned int const &operator[](unsigned int ind) const;

    void resize(unsigned int len, unsigned int z = 0);

    unsigned int back() const;

    void clear();

};

#endif //BIGINT_MY_VECTOR_H
