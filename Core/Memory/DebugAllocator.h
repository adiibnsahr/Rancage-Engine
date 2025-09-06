// Core/Memory/DebugAllocator.h
#pragma once
#include <unordered_map>
#include <mutex>
#include <cstddef>
#include <cstdlib>
#include <iostream>

/**
 * @class DebugAllocator
 * @brief Tracks heap allocations to detect memory leaks and mismatched delete/delete[] calls.
 *
 * DebugAllocator menyimpan informasi setiap alokasi memori (ukuran, lokasi file, baris kode, dan tipe alokasi),
 * dan memungkinkan pendeteksian:
 * - Memory leaks (alokasi yang tidak pernah dibebaskan)
 * - Mismatch antara `new`/`delete` dan `new[]`/`delete[]`
 *
 * Disarankan hanya digunakan pada build DEBUG, karena memiliki overhead.
 *
 * @note Gunakan macro `RG_NEW` untuk menyertakan info file dan baris saat menggunakan `new`.
 *       Misalnya: `MyClass* obj = RG_NEW MyClass();`
 */
class DebugAllocator {
public:
    /**
     * @struct AllocationInfo
     * @brief Menyimpan metadata untuk setiap alokasi.
     */
    struct AllocationInfo {
        size_t size;       ///< Ukuran alokasi dalam byte
        const char* file;  ///< Nama file sumber saat alokasi terjadi
        int line;          ///< Nomor baris dalam file
        bool isArray;      ///< True jika menggunakan new[], false jika new
    };

    /**
     * @brief Mengalokasikan blok memori dan menyimpan informasi alokasi.
     *
     * @param size     Ukuran memori yang ingin dialokasikan (dalam byte).
     * @param file     Nama file sumber yang melakukan alokasi.
     * @param line     Baris dalam file sumber.
     * @param isArray  Apakah alokasi dilakukan dengan new[].
     * @return Pointer ke memori yang dialokasikan, atau nullptr jika gagal.
     */
    void* Allocate(size_t size, const char* file, int line, bool isArray) {
        void* ptr = std::malloc(size);
        if (ptr) {
            std::lock_guard<std::mutex> lock(_mutex);
            _allocs[ptr] = { size, file, line, isArray };
            _total += size;
            if (_peak < _total) _peak = _total;
        }
        return ptr;
    }

    /**
     * @brief Membebaskan memori dan memverifikasi bahwa alokasi cocok (new vs new[]).
     *
     * @param ptr      Pointer yang ingin dibebaskan.
     * @param isArray  Apakah pembebasan dilakukan dengan delete[].
     */
    void Free(void* ptr, bool isArray) {
        if (!ptr) return;

        std::lock_guard<std::mutex> lock(_mutex);
        auto it = _allocs.find(ptr);
        if (it != _allocs.end()) {
            if (it->second.isArray != isArray) {
                std::cerr << " Mismatched delete at "
                    << it->second.file << ":" << it->second.line
                    << " — used " << (it->second.isArray ? "new[]" : "new")
                    << ", but deleted with " << (isArray ? "delete[]" : "delete") << std::endl;
            }
            _total -= it->second.size;
            _allocs.erase(it);
        }
        else {
            std::cerr << " Freeing unknown pointer: " << ptr << std::endl;
        }

        std::free(ptr);
    }

    /**
     * @brief Melaporkan semua alokasi yang belum dibebaskan (memory leaks).
     *
     * Digunakan untuk mendeteksi memory leaks pada akhir eksekusi program.
     */
    void ReportLeaks() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!_allocs.empty()) {
            std::cerr << " Memory leaks detected: " << _allocs.size() << " allocations still active!" << std::endl;
            for (const auto& kv : _allocs) {
                const AllocationInfo& a = kv.second;
                std::cerr << "  Leak at " << kv.first << " | size=" << a.size
                    << " bytes | location: " << a.file << ":" << a.line
                    << (a.isArray ? " (new[])" : " (new)") << std::endl;
            }
        }
        else {
            std::cerr << " No memory leaks detected." << std::endl;
        }

        std::cerr << "Peak memory usage: " << _peak << " bytes" << std::endl;
    }

private:
    std::unordered_map<void*, AllocationInfo> _allocs; ///< Map untuk tracking semua alokasi aktif.
    std::mutex _mutex;                                 ///< Mutex untuk menjaga thread-safety.
    size_t _total = 0;                                 ///< Total alokasi saat ini (dalam byte).
    size_t _peak = 0;                                  ///< Peak (tertinggi) penggunaan memori.
};

/// Hanya aktifkan operator override jika dalam build DEBUG
#ifdef _DEBUG

/// Global instance DebugAllocator (inline untuk linkage yang aman)
inline DebugAllocator gDebugAllocator;

/**
 * @brief Overload operator new untuk menyertakan file dan line.
 * @param size Ukuran memori yang ingin dialokasikan.
 * @param file Nama file tempat alokasi terjadi.
 * @param line Baris dalam file tempat alokasi terjadi.
 * @return Pointer ke blok memori yang dialokasikan.
 */
inline void* operator new(size_t size, const char* file, int line) {
    return gDebugAllocator.Allocate(size, file, line, false);
}

/**
 * @brief Overload operator new[] untuk menyertakan file dan line.
 * @param size Ukuran memori yang ingin dialokasikan.
 * @param file Nama file tempat alokasi terjadi.
 * @param line Baris dalam file tempat alokasi terjadi.
 * @return Pointer ke blok memori yang dialokasikan.
 */
inline void* operator new[](size_t size, const char* file, int line) {
    return gDebugAllocator.Allocate(size, file, line, true);
}

/**
 * @brief Overload operator delete (tanpa info file/line).
 * @param ptr Pointer yang ingin dibebaskan.
 */
inline void operator delete(void* ptr) noexcept {
    gDebugAllocator.Free(ptr, false);
}

/**
 * @brief Overload operator delete[] (tanpa info file/line).
 * @param ptr Pointer yang ingin dibebaskan.
 */
inline void operator delete[](void* ptr) noexcept {
    gDebugAllocator.Free(ptr, true);
}

/**
 * @def RG_NEW
 * @brief Macro untuk mengganti keyword `new` dengan versi yang menyertakan file dan line.
 *
 * Gunakan seperti:
 * @code
 * MyClass* obj = RG_NEW MyClass();
 * @endcode
 */
#define RG_NEW new(__FILE__, __LINE__)

#else
/// Jika bukan build debug, RG_NEW menjadi alias biasa untuk new
#define RG_NEW new
#endif
