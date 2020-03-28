#include <iostream>
#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <filesystem>
#include <regex>
#include <algorithm>
#include <opencv4/opencv2/opencv.hpp>

namespace fs = std::filesystem;
using namespace cv;
using namespace std;

void index(const string& root_filepath) {

    tesseract::TessBaseAPI *ocr;
    ocr = new tesseract::TessBaseAPI();
    ocr->Init(nullptr, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetPageSegMode(tesseract::PSM_AUTO);

    regex commas(",");
    regex whitespace("(\\s+)");
    regex nonword("([^a-zA-Z0-9 ])");

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

            transform(text.begin(), text.end(), text.begin(), ::tolower);
            text = regex_replace(text, whitespace, " ");
            text = regex_replace(text, nonword, "");
            text = regex_replace(text, commas, "");

            stringstream text_stream(text);
            string token;
            vector<string> tokens;
            while (getline(text_stream, token, ' ')) {
                tokens.push_back(token);
            }

            vector<string> clean_tokens;
            for (auto & t : tokens) {
                if (t.length() > 3) {
                    clean_tokens.push_back(t);
                    cout << t << endl;
                }
            }

            index_file << filepath;
            for (auto & clean_token : clean_tokens) {
                index_file << "," << clean_token;
            }
            index_file << endl;
        }
    }
    index_file.close();

    ocr->End();
}

set<string> search(const string& root_filepath, const string& term) {
    set<string> filenames;

    string index_filepath = root_filepath + "/.memes_index";
    // Check for existing index file
    if (FILE *file = fopen(index_filepath.c_str(), "r")) {

        fclose(file);
        cout << "Index found." << endl;
        ifstream index_file(index_filepath);

        // Load index
        vector<vector<string>> index_entries;
        string line;
        vector<string> index_entry;
        while(getline(index_file, line)) {
            stringstream line_stream(line);
            string token;
            index_entry = {};
            while(getline(line_stream,token,',')) {
                index_entry.push_back(token);
            }
            index_entries.push_back(index_entry);
        }

        // Search index

        string filename;
        for (auto & entry : index_entries) {
            filename = entry[0];
            for (auto & tag : entry) {
                if (tag == term) {
                    filenames.insert(filename);
                }
            }
        }

    } else {
        cout << "Index not found" << endl;
    }


    // Look for term

    return filenames;
}


int main(int argc, char *argv[]) {

    string banner = " __  __ ___ __  __ ___ _    ___  ___ ___\n"
                    "|  \\/  | __|  \\/  | __| |  / _ \\| _ \\   \\ \n"
                    "| |\\/| | _|| |\\/| | _|| |_| (_) |   / |) |\n"
                    "|_|  |_|___|_|  |_|___|____\\___/|_|_\\___/\n";
    std::cout << banner << std::endl;

    if ((string)argv[1] == "index") {
        index((string)argv[2]);
    } else if ((string)argv[1] == "search") {
        auto results = search((string) argv[2], (string) argv[3]);

        for (auto & file : results) {
            cout << file << endl;
        }

    }


    return 0;
}
