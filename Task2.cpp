#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>

using namespace std;

mutex mtx; // For thread-safe output

// Run-Length Encoding for a substring
string compress_chunk(const string& data) {
    string compressed;
    int n = data.length();

    for (int i = 0; i < n; ) {
        char current = data[i];
        int count = 1;
        while (i + count < n && data[i + count] == current) count++;
        compressed += current + to_string(count);
        i += count;
    }

    return compressed;
}

// Thread worker to compress a chunk and store it in result vector
void compress_worker(const string& chunk, vector<string>& results, int index) {
    string result = compress_chunk(chunk);
    lock_guard<mutex> lock(mtx);
    results[index] = result;
}

// Split data and compress using multiple threads
void compress_file(const string& inputFile, const string& outputFile, int threadCount) {
    ifstream in(inputFile);
    ofstream out(outputFile);
    
    if (!in || !out) {
        cerr << "Error opening file.\n";
        return;
    }

    string data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    int len = data.length();
    int chunkSize = len / threadCount;

    vector<thread> threads;
    vector<string> results(threadCount);

    for (int i = 0; i < threadCount; ++i) {
        int start = i * chunkSize;
        int end = (i == threadCount - 1) ? len : (i + 1) * chunkSize;
        string chunk = data.substr(start, end - start);
        threads.emplace_back(compress_worker, chunk, ref(results), i);
    }

    for (auto& th : threads)
        th.join();

    for (const auto& part : results)
        out << part;

    cout << "Compression complete. Output saved to " << outputFile << endl;
}

string decompress(const string& data) {
    string decompressed;
    int i = 0;
    while (i < data.size()) {
        char ch = data[i++];
        string countStr;
        while (isdigit(data[i])) countStr += data[i++];
        int count = stoi(countStr);
        decompressed.append(count, ch);
    }
    return decompressed;
}

void decompress_file(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile);
    ofstream out(outputFile);

    if (!in || !out) {
        cerr << "Error opening file.\n";
        return;
    }

    string data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    string result = decompress(data);
    out << result;

    cout << "Decompression complete. Output saved to " << outputFile << endl;
}

int main() {
    string inputFile = "input.txt";
    string compressedFile = "compressed.txt";
    string decompressedFile = "decompressed.txt";

    int threads = 4; // Change for testing performance

    compress_file(inputFile, compressedFile, threads);
    decompress_file(compressedFile, decompressedFile);

    return 0;
}