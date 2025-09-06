// Core/Memory/PoolAllocator.h
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <mutex>
#include <cstdlib>

/**
 * @class PoolAllocator
 * @brief Thread-safe memory pool allocator for fixed-size objects using a free-list.
 *
 * Cocok digunakan untuk objek kecil yang sering dialokasikan dan dibebaskan,
 * seperti komponen game, node AST, atau objek partikel.
 *
 * Keunggulan:
 * - Sangat cepat (O(1) alloc/dealloc)
 * - Menghindari fragmentasi heap
 * - Otomatis menumbuhkan chunk baru saat free list habis
 *
 * @note Bersifat thread-safe (menggunakan mutex).
 * @note Tidak mendukung ukuran objek yang bervariasi.
 * @note Tidak memanggil konstruktor/destruktor — hanya mengelola raw memory.
 */
class PoolAllocator {
public:
    /**
     * @brief Konstruktor PoolAllocator.
     *
     * Mengalokasikan chunk pertama dan menyiapkan free-list.
     *
     * @param elementSize Ukuran setiap elemen (dalam byte).
     * @param chunkCount  Jumlah elemen per chunk (default: 1024).
     */
    PoolAllocator(size_t elementSize, size_t chunkCount = 1024)
        : _elemSize(Align(elementSize, sizeof(void*))),
        _chunkCount(chunkCount) {
        AllocateChunk();
    }

    /**
     * @brief Destruktor.
     *
     * Membebaskan semua blok memori yang telah dialokasikan.
     */
    ~PoolAllocator() {
        for (void* mem : _chunks)
            std::free(mem);
    }

    /**
     * @brief Mengalokasikan blok memori untuk satu objek.
     *
     * Jika free-list kosong, maka chunk baru akan dialokasikan.
     *
     * @return Pointer ke blok memori yang dialokasikan.
     */
    void* Allocate() {
        std::lock_guard<std::mutex> lock(_mutex);

        // Jika free list kosong, alokasikan chunk baru
        if (!_freeList)
            AllocateChunk();

        // Ambil node dari free list
        void* p = _freeList;
        _freeList = *reinterpret_cast<void**>(_freeList);
        return p;
    }

    /**
     * @brief Mengembalikan blok memori ke allocator.
     *
     * Memasukkan kembali pointer ke dalam free list.
     *
     * @param ptr Pointer ke blok memori yang akan dikembalikan.
     */
    void Deallocate(void* ptr) {
        std::lock_guard<std::mutex> lock(_mutex);

        *reinterpret_cast<void**>(ptr) = _freeList;
        _freeList = ptr;
    }

private:
    size_t _elemSize;                     ///< Ukuran elemen yang telah disejajarkan
    size_t _chunkCount;                   ///< Jumlah elemen dalam setiap chunk
    std::vector<void*> _chunks;           ///< Semua blok memori yang pernah dialokasikan
    void* _freeList = nullptr;            ///< Kepala dari free-list (linked list pointer)
    std::mutex _mutex;                    ///< Mutex untuk thread safety

    /**
     * @brief Menyelaraskan nilai ke alignment tertentu (power-of-two).
     *
     * @param v Nilai yang ingin disejajarkan.
     * @param a Nilai alignment (harus power-of-two).
     * @return Nilai yang telah disejajarkan.
     */
    static size_t Align(size_t v, size_t a) {
        return (v + (a - 1)) & ~(a - 1);
    }

    /**
     * @brief Mengalokasikan satu chunk baru dan memperluas free-list.
     *
     * Chunk baru ditambahkan ke daftar `_chunks` dan tiap slot dihubungkan
     * ke dalam free list sebagai node linked list.
     */
    void AllocateChunk() {
        size_t bytes = _elemSize * _chunkCount;
        void* block = std::malloc(bytes);

        // Simpan chunk untuk cleanup saat destruksi
        _chunks.push_back(block);

        uint8_t* p = reinterpret_cast<uint8_t*>(block);
        for (size_t i = 0; i < _chunkCount; ++i) {
            void* node = p + i * _elemSize;
            *reinterpret_cast<void**>(node) = _freeList;
            _freeList = node;
        }
    }
};
