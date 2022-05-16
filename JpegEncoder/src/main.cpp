#include <cstdio>
#include <cstdlib>
#include <string>

#include <iostream>
#include <fstream>

#include "psnr.hpp"
#include "ctpl_stl.h"  // ThreadPool
#include "jpeg_decoder.hpp"
#include "jpeg_encoder.hpp"

#include "ppm.hpp"
#include <filesystem>
#include <mutex>

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

    ppm ppm_data = read_ppm(task->input_file);
    std::ofstream out(task->output_file, std::ios::binary);
    auto myCallback = [&](uint8_t oneByte) {
        out.put(oneByte);
    };
    task->success = JpegEncoder::writeJpeg(myCallback, (void *)ppm_data.pixels, ppm_data.width, ppm_data.height,
            true /*always RGB*/, task->quality, task->chroma_subsampling);
    out.close();
    if (!task->success) {
        std::cerr << "Failed to compress " << task->input_file << std::endl;
        return;
    }

    task->uncompressed_filesize = std::filesystem::file_size(task->input_file);
    task->jpeg_filesize = std::filesystem::file_size(task->output_file);
}

std::string kTestImages[] = {
    // test_*.ppm
    "test_bw.ppm",
    "test_circle.ppm",
    "test_colorlines.ppm",
    "test_freq.ppm",
    "test_noise_bin.ppm",
    "test_noise.ppm",
    "test_star.ppm",

    // one of big_*.ppm
    "big_building.ppm",

    // three of the kodim_*.ppm
    "kodim01.ppm",
    "kodim05.ppm",
    "kodim11.ppm",

    "artificial.ppm",

    "flower_foveon.ppm",
    "spider_web.ppm",

    // *_iso_*.ppm
    "leaves_iso_200.ppm",
    "leaves_iso_1600.ppm",
    "nightshot_iso_100.ppm",
    "nightshot_iso_1600.ppm"
};

void opgave2() {
    std::string input_dir = "../../afbeeldingen";
    std::string output_dir = "output";

    ctpl::thread_pool pool(std::thread::hardware_concurrency());

    std::mutex thread_lock;

    for (auto &input_file : kTestImages) {
        for (int quality = 100; quality >= 5; quality -= 5) {
            for (int chroma_subsampling = 0; chroma_subsampling <= 1; chroma_subsampling++) {
                compression_task task;

                task.input_file = input_dir + "/" + input_file;
                task.output_file = output_dir + "/" + input_file + "_" + std::to_string(quality) + "_" +
                                    std::to_string(chroma_subsampling) + ".jpg";

                task.quality = quality;
                task.chroma_subsampling = !!chroma_subsampling;

                std::cout << "Compressing " << task.input_file << " with quality " << task.quality << " and "
                            << (!!chroma_subsampling ? "" : "no ") << "chroma subsampling" << std::endl;

                thread_lock.lock();
                pool.push([&](int thread_id) {
                    //compression_task task_thread = task;

                    compress(&task);
                    thread_lock.unlock();
                    std::cout << "Done compressing " << task.output_file
                              << ", result: " << (task.success ? "success" : "failure")
                              << ", size: " << task.jpeg_filesize << std::endl;
                });
            }
        }
    }
    pool.stop(true);
}

void opgave3() {
}

int main() {

    opgave2();
    return 0;
}

