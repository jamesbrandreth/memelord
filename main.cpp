#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <filesystem>
#include <regex>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <opencv4/opencv2/opencv.hpp>

namespace fs = std::filesystem;
using namespace cv;
using namespace std;

void index(string root_filepath, string output_filepath) {

    tesseract::TessBaseAPI *ocr;
    ocr = new tesseract::TessBaseAPI();
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetPageSegMode(tesseract::PSM_AUTO);

    regex whitespace("\\s+");

    string index_filepath = root_filepath + "/.memes_index";
    // Check for existing index file
    if (FILE *file = fopen(index_filepath.c_str(), "r")) {
        fclose(file);
        cout << "Index already exists" << endl;
        fs::remove(index_filepath.c_str());
        cout << "Index removed." << endl;
    } else {
        cout << "There be no index" << endl;
    }

    ofstream index_file(index_filepath);

    cout << "INDEXING..." << endl;

    // Get list of files
    string filepath;
    for (const auto &entry : fs::directory_iterator(root_filepath)) {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
            filepath = entry.path().string();
            cout << filepath << std::endl;

            // Process the image, extract tokens
            Mat im;
            im = imread(filepath, 1);
            ocr->SetImage(im.data, im.cols, im.rows, 3, im.step);
            string text = string(ocr->GetUTF8Text());

            regex_replace(text, whitespace, " ");

            stringstream textstream(text);
            string token;
            vector<string> tokens;
            while (getline(textstream, token, ' ')) {
                tokens.push_back(token);
            }

            vector<string> clean_tokens;
            for (auto & t : tokens) {
                if (t.size() > 3) {
                    clean_tokens.push_back(t);
                    cout << t << endl;
                }
            }

            index_file << filepath;
            for (auto it = clean_tokens.begin(); it != clean_tokens.end(); it++ ) {
                index_file << "," << *it;
            }
            index_file << endl;
        }
    }
    index_file.close();

    ocr->End();
}

vector<string> search(string term) {
    vector<string> filenames;

    return filenames;
}


int main(int argc, char *argv[]) {

    string banner = " __  __ ___ __  __ ___ _    ___  ___ ___\n"
                    "|  \\/  | __|  \\/  | __| |  / _ \\| _ \\   \\ \n"
                    "| |\\/| | _|| |\\/| | _|| |_| (_) |   / |) |\n"
                    "|_|  |_|___|_|  |_|___|____\\___/|_|_\\___/\n";
    std::cout << banner << std::endl;

    index("./data", "./");

    return 0;
}
