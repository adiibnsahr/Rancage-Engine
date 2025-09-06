// Core/Memory/FrameAllocator.h
#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdlib>

/**
 * @class FrameAllocator
 * @brief Double-buffered linear allocator for per-frame transient allocations.
 *
 * FrameAllocator adalah allocator sederhana dan sangat cepat yang digunakan
 * untuk mengalokasikan memori sementara dalam siklus hidup satu frame.
 * Ini menggunakan dua buffer (double buffering) untuk memungkinkan penggunaan
 * ulang memori antar frame tanpa perlu dealokasi manual.
 *
 * Penggunaan umum:
 * - Call `BeginFrame()` di awal setiap frame untuk mengganti buffer aktif.
 * - Call `Allocate()` untuk alokasi memori sementara yang cepat.
 *
 * @note Memori yang dialokasikan dari allocator ini bersifat sementara
 *       dan akan dianggap hangus pada frame berikutnya setelah `BeginFrame()`.
 *
 * @warning Bukan thread-safe.
 */
class FrameAllocator {
public:
    /**
     * @brief Konstruktor FrameAllocator.
     *
     * Menginisialisasi dua buffer memori untuk digunakan secara bergantian.
     *
     * @param bufferSize Ukuran setiap buffer (dalam byte). Default: 1MB.
     */
    explicit FrameAllocator(size_t bufferSize = 1 << 20)
        : _bufferSize(bufferSize) {
        _buffers[0] = reinterpret_cast<uint8_t*>(std::malloc(_bufferSize));
        _buffers[1] = reinterpret_cast<uint8_t*>(std::malloc(_bufferSize));
        _offsets[0] = 0;
        _offsets[1] = 0;
    }

    /**
     * @brief Destruktor.
     *
     * Menghapus alokasi dua buffer internal.
     */
    ~FrameAllocator() {
        std::free(_buffers[0]);
        std::free(_buffers[1]);
    }

    /**
     * @brief Memulai frame baru.
     *
     * Menukar buffer aktif dan mereset offset alokasi untuk buffer aktif.
     *
     * @note Harus dipanggil **sekali** di awal setiap frame.
     */
    void BeginFrame() {
        _current = 1 - _current;         ///< Switch antara buffer 0 dan 1
        _offsets[_current] = 0;          ///< Reset offset buffer aktif
    }

    /**
     * @brief Mengalokasikan blok memori dari buffer aktif.
     *
     * Melakukan alokasi linear dari buffer aktif dengan alignment tertentu.
     * Tidak melakukan dealokasi individual; semua alokasi akan direset
     * saat `BeginFrame()` berikutnya.
     *
     * @param size Jumlah byte yang ingin dialokasikan.
     * @param alignment Alignment memori yang dibutuhkan (default: 16 byte).
     * @return Pointer ke memori yang dialokasikan, atau `nullptr` jika buffer penuh.
     */
    void* Allocate(size_t size, size_t alignment = 16) {
        uintptr_t base = reinterpret_cast<uintptr_t>(_buffers[_current]);
        uintptr_t ptr = base + _offsets[_current];
        uintptr_t aligned = (ptr + (alignment - 1)) & ~(alignment - 1);
        size_t pad = aligned - ptr;

        // Cek apakah cukup ruang tersisa di buffer
        if (_offsets[_current] + pad + size > _bufferSize) {
            // Alokasi gagal: buffer tidak cukup besar
            return nullptr;
        }

        _offsets[_current] += static_cast<uint32_t>(pad + size);
        return reinterpret_cast<void*>(aligned);
    }

private:
    /**
     * @brief Pointer ke dua buffer memori internal.
     *
     * Buffer digunakan secara bergantian setiap frame (double buffering).
     */
    uint8_t* _buffers[2] = { nullptr, nullptr };

    /**
     * @brief Offset alokasi saat ini di masing-masing buffer.
     *
     * Digunakan untuk tracking posisi alokasi linear dalam setiap buffer.
     */
    uint32_t _offsets[2] = { 0, 0 };

    /**
     * @brief Indeks buffer aktif saat ini (0 atau 1).
     *
     * Diubah setiap kali `BeginFrame()` dipanggil.
     */
    int _current = 0;

    /**
     * @brief Ukuran setiap buffer dalam byte.
     */
    size_t _bufferSize;
};
