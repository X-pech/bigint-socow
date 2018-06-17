#include <iostream>
#include <memory>
#include <cstdlib>

#ifndef BIGINT_SOCOW_VECTOR_H
#define BIGINT_SOCOW_VECTOR_H


struct socow_vector {
    socow_vector();

    ~socow_vector();

    explicit socow_vector(size_t s);

    socow_vector(const socow_vector &other);

    socow_vector(const std::initializer_list<unsigned int> &list);

    void reserve(size_t capacity);

    void resize(size_t size);

    size_t size() const;

    bool empty() const;

    unsigned int &operator[](size_t index);

    const unsigned int &operator[](size_t index) const;

    socow_vector &operator=(const socow_vector &other);

    void pop_back();

    void push_back(unsigned int value);

    unsigned int back() const;

    unsigned int *begin();

    unsigned int *end();

    friend bool operator==(const socow_vector &a, const socow_vector &b);

    friend unsigned int *begin(const socow_vector &x);

    friend unsigned int *end(const socow_vector &x);

private:
    static const size_t SMALL_N = 4;
    static const size_t EXPAND = 2;

    size_t __size;
    bool __big;

    struct big_obj {
        std::shared_ptr<unsigned int> ptr;
        size_t capacity;

        big_obj()
                : big_obj(static_cast<unsigned int*>(nullptr), 0) {}

        big_obj(unsigned int *p, size_t c)
                : ptr(p, std::default_delete<unsigned int[]>()), capacity(c) {}

        big_obj(const big_obj &other) = default;
    };

    union gc {
        big_obj big;
        unsigned int small[SMALL_N]{};

        gc() {};

        ~gc() {};
    } data;

    unsigned int *current_cell;

    size_t get_capacity() { return __big ? data.big.capacity : SMALL_N; };

    void cowchk();
};


#endif //BIGINT_SOCOW_VECTOR_H
