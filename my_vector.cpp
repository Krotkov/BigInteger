    //
// Created by user on 02.06.18.
//
#include <iostream>
#include "my_vector.h"
#include <cassert>

my_vector::my_vector() : _size(0), is_small(true) {
    for (unsigned int &i : small) {
        i = 0;
    }
}

my_vector::~my_vector() {
    if (!is_small) big.reset();
}

my_vector::my_vector(my_vector const &other) {
    _size = other._size;
    is_small = other.is_small;
    if (is_small) {
        for (unsigned int i = 0; i < SMALL_SIZE; i++) {
            small[i] = other.small[i];
        }
    } else {
        new(&big) std::shared_ptr<std::vector<unsigned int>>(other.big);
    }
}

my_vector &my_vector::operator=(my_vector const &other) {
    _size = other._size;
    if (!is_small) big.reset();
    is_small = other.is_small;
    if (is_small) {
        for (unsigned int i = 0; i < SMALL_SIZE; i++) {
            small[i] = other.small[i];
        }
    } else {
        new(&big) std::shared_ptr<std::vector<unsigned int>>(other.big);
    }
    return *this;
}

void my_vector::make_big() {
    if (!is_small || _size < SMALL_SIZE) return;
    std::vector<unsigned int>* copy = new std::vector<unsigned int>(_size, 0);
    for (unsigned int i = 0; i < SMALL_SIZE; i++) {
        (*copy)[i] = small[i];
    }
    new(&big) std::shared_ptr<std::vector<unsigned int>>((copy));
    is_small = false;
}

unsigned int my_vector::size() const {
    return _size;
}

bool my_vector::empty() {
    return (_size == 0);
}

void my_vector::push_back(unsigned int x) {
    if (is_small && _size < SMALL_SIZE) {
        small[_size] = x;
        _size++;
        return;
    }
    make_big();
    _size++;
    check_count();
    (*big).push_back(x);
}

void my_vector::pop_back() {
    assert(_size != 0);
    _size--;
    if (!is_small) {
        check_count();
        (*big).pop_back();
    }
}

unsigned int &my_vector::operator[](unsigned int ind) {
    assert(ind < _size);
    if (is_small) return small[ind];
    else {
        check_count();
        return (*big)[ind];
    }
}

unsigned int const &my_vector::operator[](unsigned int ind) const {
    assert(ind < _size);
    if (is_small) return small[ind];
    else return (*big)[ind];
}

void my_vector::resize(unsigned int len, unsigned int z) {
    _size = len;
    make_big();
    for (unsigned int i = 0; i < _size; i++) {
        if (is_small) small[i] = z;
        else (*big)[i] = z;
    }
}

unsigned int my_vector::back() const {
    assert(_size != 0);
    if (is_small) {
        return small[_size - 1];
    } else {
        return (*big)[_size - 1];
    }
}

void my_vector::clear() {
    _size = 0;
    if (!is_small) big.reset();
    is_small = true;
    for (unsigned int &i : small) {
        i = 0;
    }
}

void my_vector::check_count() {
    if (!big.unique()) {
        big = std::make_shared<std::vector<unsigned int>>(*big);
    }
}
