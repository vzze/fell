#ifndef FELL_VAR_HELPERS_OBJECT_HH
#define FELL_VAR_HELPERS_OBJECT_HH

#include <unordered_map>
#include <utility>
#include <cstddef>
#include <memory>

namespace fell {
    template<typename T>
    struct object {
        private:
            inline static std::unordered_map<T*, std::size_t> owners;
            T * stored;

            void release();
            void acquire();
        public:
            object();
            explicit object(T * ptr);

            object(const object<T> & rhs);
            object(object<T> && rhs) noexcept;

            object<T> & operator = (const object<T> & rhs);
            object<T> & operator = (object<T> && rhs) noexcept;

            T & operator * () const;
            T * operator -> () const;

            T * get() const;

            ~object();
    };

    template<typename T, typename ... Args>
    object<T> make_object(Args && ... args);
}

template<typename T>
void fell::object<T>::object::release() {
    if(stored == nullptr)
        return;

    auto & owner = owners[stored];

    --owner;

    if(owner == 0)
        std::default_delete<T>{}(stored);
}

template<typename T>
void fell::object<T>::object::acquire() {
    if(stored == nullptr)
        return;

    ++owners[stored];
}

template<typename T>
fell::object<T>::object::object() : stored(nullptr) {}

template<typename T>
fell::object<T>::object::object(T * ptr) : stored(ptr) {}

template<typename T>
fell::object<T>::object::object(const object<T> & rhs) : stored(rhs.stored) {
    acquire();
}

template<typename T>
fell::object<T>::object::object(object<T> && rhs) noexcept : stored(nullptr) {
    std::swap(stored, rhs.stored);
}

template<typename T>
fell::object<T> & fell::object<T>::object::operator = (const object<T> & rhs) {
    if(this == &rhs) return *this;

    stored = rhs.stored;

    acquire();

    return *this;
}

template<typename T>
fell::object<T> & fell::object<T>::object::operator = (object<T> && rhs) noexcept {
    std::swap(stored, rhs.stored);
    return *this;
}

template<typename T>
T * fell::object<T>::object::operator -> () const {
    return stored;
}

template<typename T>
T & fell::object<T>::object::operator * () const {
    return *stored;
}

template<typename T>
T * fell::object<T>::object::get() const {
    return stored;
}

template<typename T>
fell::object<T>::object::~object() { release(); }

template<typename T, typename ... Args>
fell::object<T> fell::make_object(Args && ... args) {
    return object<T>(new T(std::forward<Args>(args)...));
}

#endif
