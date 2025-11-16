#pragma once
#include <memory>
#include <cstddef>
#include <vector>

template<typename T, std::size_t BlockSize = 10>
class custom_allocator {
private:
    struct memory_block {
        char* data;
        std::size_t used;
        std::size_t size;
        
        memory_block(std::size_t n) : data(new char[sizeof(T) * n]), used(0), size(n) {}
        ~memory_block() { delete[] data; }
        
        bool can_allocate(std::size_t n) const { return used + n <= size; }
        T* allocate(std::size_t n) {
            if (!can_allocate(n)) return nullptr;
            T* ptr = reinterpret_cast<T*>(data + used * sizeof(T));
            used += n;
            return ptr;
        }
    };
    
    std::vector<memory_block*> blocks;
    std::size_t current_block;
    
    void expand_memory(std::size_t n) {
        std::size_t new_size = (n > BlockSize) ? n : BlockSize;
        blocks.push_back(new memory_block(new_size));
        current_block = blocks.size() - 1;
    }
    
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    
    template<typename U>
    struct rebind {
        using other = custom_allocator<U, BlockSize>;
    };
    
    custom_allocator() : current_block(0) {
        expand_memory(BlockSize);
    }
    
    ~custom_allocator() {
        for (auto block : blocks) {
            delete block;
        }
    }
    
    pointer allocate(size_type n) {
        if (n == 0) return nullptr;
        
        if (!blocks[current_block]->can_allocate(n)) {
            expand_memory(n);
        }
        
        return blocks[current_block]->allocate(n);
    }
    
    void deallocate(pointer p, size_type n) {
        // Поэлементное освобождение - в нашей реализации память не возвращается
        // до полного уничтожения аллокатора
    }
    
    void deallocate_all() {
        for (auto block : blocks) {
            delete block;
        }
        blocks.clear();
        expand_memory(BlockSize);
    }
    
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        new(p) U(std::forward<Args>(args)...);
    }
    
    template<typename U>
    void destroy(U* p) {
        p->~U();
    }
    
    size_type max_size() const {
        return BlockSize * blocks.size();
    }
};
