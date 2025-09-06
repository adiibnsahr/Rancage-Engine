// Core/Memory/ArenaAllocator.h
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <algorithm>

/**
 * @class ArenaAllocator
 * @brief Fast linear (bump pointer) allocator with optional dynamic growth.
 *
 * ArenaAllocator mengalokasikan memori secara linear dari blok buffer,
 * dengan biaya alokasi yang sangat murah (O(1)) dan reset total juga O(1).
 * Tidak mendukung deallocasi per objek.
 *
 * Ideal digunakan untuk:
 * - Per-frame allocation (game engine)
 * - Short-lived allocations (parser, scratch memory)
 * - Subsystem-specific memory pools
 *
 * @note Arena ini tidak thread-safe.
 * @note Memori bisa dimiliki internal (malloc) atau eksternal (user-supplied).
 */
class ArenaAllocator {
public:
    /**
     * @brief Membuat ArenaAllocator dengan kapasitas awal tertentu.
     *
     * Arena akan mengalokasikan buffer internal menggunakan `malloc`,
     * dan akan tumbuh secara otomatis jika diperlukan (realloc).
     *
     * @param initialCapacity Kapasitas awal dalam byte (default: 1 MB).
     */
    explicit ArenaAllocator(size_t initialCapacity = 1 << 20) // 1 MB
        : _head(nullptr), _tail(nullptr), _capacity(0), _owned(true) {
        Reserve(initialCapacity);
    }

    /**
     * @brief Membuat ArenaAllocator dari blok memori eksternal.
     *
     * Dalam mode ini, Arena **tidak** akan tumbuh secara otomatis dan
     * tidak akan membebaskan memori di destruktor.
     *
     * @param memory Pointer ke blok memori yang diberikan user.
     * @param size   Ukuran blok memori dalam byte.
     */
    ArenaAllocator(void* memory, size_t size)
        : _head(reinterpret_cast<uint8_t*>(memory)),
        _tail(_head),
        _capacity(size),
        _owned(false) {
    }

    /**
     * @brief Destruktor.
     *
     * Jika arena memiliki buffer sendiri (internal), maka akan dibebaskan.
     */
    ~ArenaAllocator() {
        if (_owned) std::free(_buffer());
    }

    /**
     * @brief Mengalokasikan memori dari arena.
     *
     * Melakukan alokasi linear (bump-pointer) dengan alignment.
     * Jika kapasitas tidak cukup dan arena dimiliki sendiri, maka akan mencoba tumbuh.
     *
     * @param size      Jumlah byte yang ingin dialokasikan.
     * @param alignment Alignment (harus power-of-two), default 8 byte.
     * @return Pointer ke memori, atau nullptr jika gagal alokasi.
     */
    void* Allocate(size_t size, size_t alignment = 8) {
        uintptr_t current = reinterpret_cast<uintptr_t>(_tail);
        uintptr_t aligned = (current + (alignment - 1)) & ~(alignment - 1);
        size_t padding = aligned - current;

        // Jika tidak cukup ruang, coba grow
        if (Size() + padding + size > _capacity) {
            if (!Grow(std::max(_capacity * 2, padding + size))) return nullptr;

            // Recalculate setelah grow
            current = reinterpret_cast<uintptr_t>(_tail);
            aligned = (current + (alignment - 1)) & ~(alignment - 1);
            padding = aligned - current;
        }

        _tail = reinterpret_cast<uint8_t*>(aligned + size);
        return reinterpret_cast<void*>(aligned);
    }

    /**
     * @brief Mereset arena ke posisi awal.
     *
     * Tidak membebaskan memori, hanya menyetel ulang tail pointer.
     * Alokasi sebelumnya akan dianggap hangus.
     */
    void Reset() {
        _tail = _head;
    }

    /**
     * @brief Mendapatkan jumlah byte yang telah dialokasikan.
     * @return Byte yang digunakan.
     */
    size_t Size() const {
        return static_cast<size_t>(_tail - _head);
    }

    /**
     * @brief Mendapatkan kapasitas total arena (dalam byte).
     * @return Total kapasitas buffer.
     */
    size_t Capacity() const {
        return _capacity;
    }

private:
    uint8_t* _head;     ///< Awal buffer
    uint8_t* _tail;     ///< Posisi alokasi saat ini (berjalan maju)
    size_t _capacity;   ///< Kapasitas total buffer
    bool _owned;        ///< Apakah arena memiliki buffer sendiri

    /**
     * @brief Mengembalikan pointer ke buffer dasar (head).
     * @return Pointer ke awal buffer.
     */
    uint8_t* _buffer() const {
        return _head;
    }

    /**
     * @brief Mengalokasikan buffer awal (internal malloc).
     * @param bytes Ukuran buffer yang diminta.
     */
    void Reserve(size_t bytes) {
        _head = reinterpret_cast<uint8_t*>(std::malloc(bytes));
        _tail = _head;
        _capacity = _head ? bytes : 0;
    }

    /**
     * @brief Mencoba menumbuhkan arena jika kapasitas tidak cukup.
     *
     * Hanya diperbolehkan jika arena dimiliki (`_owned == true`).
     *
     * @param add Jumlah byte tambahan.
     * @return True jika berhasil, false jika gagal.
     */
    bool Grow(size_t add) {
        if (!_owned) return false;

        size_t used = Size();
        size_t newCap = _capacity + add;
        uint8_t* newBuf = reinterpret_cast<uint8_t*>(std::realloc(_buffer(), newCap));

        if (!newBuf) return false;

        _head = newBuf;
        _tail = newBuf + used;
        _capacity = newCap;
        return true;
    }
};
