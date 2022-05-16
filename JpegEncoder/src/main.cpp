#include <cstdio>
#include <cstdlib>
#include <string>

#include "psnr.hpp"
#include "ctpl_stl.h"  // ThreadPool
#include "jpeg_decoder.hpp"
#include "jpeg_encoder.hpp"

struct compression_task {
    // Task description
    std::string input_file; ///< PPM file
    int quality{0};
    bool chroma_subsampling{false};

    // Compresion Result
    bool success{false};
    std::string output_file; ///< JPEG file
    uint64_t uncompressed_filesize{0};
    uint64_t jpeg_filesize{0};
    double psnr{0.0}; ///< PSNR in dB
    double rate{0.0}; ///< Bits per pixels (bpp)
};

void compress(compression_task *task) {
    // Do something here
    // Fill in all the result fields of the task struct.
}

void opgave2() {
    ctpl::thread_pool pool(std::thread::hardware_concurrency());

    pool.push([](int thread_id) {
        compression_task task;
        task.input_file = "afbeeldingen/test_star.ppm";
        task.output_file = "out.jpg";
        task.quality = 70;
        compress(&task);
    });
    pool.stop(true);
}

void opgave3() {
}

int main() {
    opgave2();

    return 0;
}

