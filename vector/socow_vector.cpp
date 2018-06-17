#include "socow_vector.h"

typedef unsigned int ui;

socow_vector::socow_vector()
        : __size(0), __big(false), current_cell(data.small) {}

socow_vector::socow_vector(size_t size) : socow_vector() {
    reserve(size);
    std::fill(current_cell, current_cell + size, 0);
    __size = size;
}

socow_vector::socow_vector(const socow_vector &other) : socow_vector() {
    if (other.__big) {
        __big = true;
        new(&data.big)big_obj(other.data.big);
        current_cell = data.big.ptr.get();
    } else {
        __big = false;
        std::copy(other.data.small, other.data.small + other.size(), data.small);
        current_cell = data.small;
    }
    __size = other.size();
}

socow_vector::socow_vector(const std::initializer_list<unsigned int> &list) : socow_vector() {
    reserve(list.size());
    std::copy(list.begin(), list.end(), current_cell);
    __size = list.size();
}

socow_vector::~socow_vector() {
    if (__big) {
        data.big.~big_obj();
    }
}

void socow_vector::reserve(size_t capacity) {

    if (capacity < get_capacity()) {
        return;
    }

    if (capacity > get_capacity()) {
        capacity = std::max(capacity, get_capacity() * 2);
    }

    if (capacity > SMALL_N) {

        if (__big) {
            std::shared_ptr<ui> cpy = data.big.ptr;
            data.big.ptr.reset(new ui[capacity], std::default_delete<ui[]>());
            std::copy(cpy.get(), cpy.get() + __size, data.big.ptr.get());
        } else {
            __big = true;
            auto copy = new ui[capacity];
            std::copy(current_cell, current_cell + __size, copy);
            new(&data.big)big_obj(copy, capacity);
        }


        data.big.capacity = capacity;
        current_cell = data.big.ptr.get();
    }
}

void socow_vector::resize(size_t size) {
    reserve(size);
    if (size > __size) {
        std::fill(current_cell + __size, current_cell + size, 0);
    }
    __size = size;
}

size_t socow_vector::size() const { return __size; }

bool socow_vector::empty() const { return __size == 0; }

void socow_vector::cowchk() {
    if (__big && !data.big.ptr.unique()) {
        data.big.ptr.reset(new ui[std::min(data.big.capacity, __size * EXPAND)], std::default_delete<ui[]>());
        std::copy(current_cell, current_cell + __size, data.big.ptr.get());
        current_cell = data.big.ptr.get();
    }
}

ui &socow_vector::operator[](size_t index) {
    cowchk();
    return current_cell[index];
}

const ui &socow_vector::operator[](size_t index) const {
    return current_cell[index];
}

void socow_vector::pop_back() { __size--; }

ui socow_vector::back() const { return current_cell[__size - 1]; }

void socow_vector::push_back(ui value) {
    cowchk();
    if (get_capacity() < __size + 1) {
        reserve(get_capacity() * EXPAND);
    }
    current_cell[__size] = value;
    __size++;
}

bool operator==(const socow_vector &a, const socow_vector &b) {
    if (a.__size != b.__size)
        return false;
    return std::equal(a.current_cell, a.current_cell + a.__size, b.current_cell);
}

socow_vector &socow_vector::operator=(const socow_vector &other) {
    __size = other.__size;
    if (other.__big) {
        if (__big) {
            data.big = big_obj(other.data.big);
        } else {
            __big = true;
            new(&data.big)big_obj(other.data.big);
        }
        current_cell = data.big.ptr.get();
    } else {
        if (__big) {
            __big = false;
            data.big.~big_obj();
        }
        std::copy(other.data.small, other.data.small + other.__size, data.small);
        current_cell = const_cast<ui *>(data.small);
    }
    return *this;
}

ui *socow_vector::begin() { return current_cell; }

ui *socow_vector::end() { return current_cell + __size; }

ui *begin(const socow_vector &x) { return x.current_cell; }

ui *end(const socow_vector &x) { return x.current_cell + x.__size; }





